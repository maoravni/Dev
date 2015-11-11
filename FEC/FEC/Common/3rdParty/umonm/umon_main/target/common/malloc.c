/* malloc():
 *	A simple memory allocator useful for embedded systems to provide 
 *	some level of debug, plus the ability to increase heap space into
 *	additional memory that is not contiguous with the intial statically
 *	allocated array of memory.  The reason for supporting the ability to
 *	allocate from 2 distinct blocks of memory is so that the monitor can
 *	be built with some reasonable amount of heap space allocated to it; then
 *	if the application is going to also use this malloc, it can do so
 *	by simply extending the heap.  The monitor would not have to be
 *	specifically built to support the large heap allocation.
 *	
 *	The allocator data structures are part of the memory space used for
 *	the allocation.  To test for memory overruns (using memory after the
 *	end of the allocated space) or underruns (using memory prior to the
 *	beginning of the allocated space), the data structure starts and ends
 *	with a fixed tag that is always checked upon entry into malloc()
 *	or free().
 *	When the memory is freed, the next and previous block is checked to
 *	determine if this newly freed block can be combined with a neighboring
 *	block.  This provides some level of defragmentation.  Note, at
 *	this point, that the blocks are only combined if they are found to be
 *	contiguous.  This correctly implies that neighboring free blocks need
 *	not be within contiguous memory space.
 *
 *	NOTE THAT THERE IS ABSOLUTELY NO CONCERN FOR SPEED IN THIS MEMORY
 *	ALLOCATOR, IT IS SLOW!  Every call to malloc/free does a sanity check
 *	on all allocation structures, so it is fairly good at detecting illegal
 *	use	of the allocated memory.
 *	
 *	General notice:
 *	This code is part of a boot-monitor package developed as a generic base
 *	platform for embedded system designs.  As such, it is likely to be
 *	distributed to various projects beyond the control of the original
 *	author.  Please notify the author of any enhancements made or bugs found
 *	so that all may benefit from the changes.  In addition, notification back
 *	to the author will allow the new user to pick up changes that may have
 *	been made by other users after this version of the code was distributed.
 *
 *	Note1: the majority of this code was edited with 4-space tabs.
 *	Note2: as more and more contributions are accepted, the term "author"
 *		   is becoming a mis-representation of credit.
 *
 *	Original author:	Ed Sutter
 *	Email:				esutter@lucent.com
 *	Phone:				908-582-2351
 */
#include "config.h"
#if INCLUDE_MALLOC
#include "genlib.h"
#include "stddefs.h"
#include "cli.h"

#define FNAMESIZE			32

#define PRETAG				0xDEAD
#define POSTTAG				0xBEEF
#define MHDRSIZE			sizeof(struct mhdr)

extern	char *GetMemory(int);
extern	int GetMemoryLeft(void);
extern	int extendHeap(char *,int);
extern	void unExtendHeap(void);
extern	char *getExtHeapBase(void);


/* mhdr:
   The control structure used by the memory allocator.
*/
struct	mhdr {
	ushort	pretag;			/* Fixed value used as an overrun sanity
							 * check for the previous memory block.
							 */
	int	size;				/* Size of useable memory block.  Size is
							 * positive if block is free and negative if
							 * block is not free.
							 */
	struct mhdr *next;		/* Points to next mhdr structure (not
							 * necessarily in contiguous memory space).
							 */
	struct mhdr *prev;		/* Points to previous mhdr structure (not
							 * necessarily in contiguous memory space).
							 */
	ushort	posttag;		/* Fixed value used as an underrun sanity
							 * check for this memory block.
							 */
};

/* mcalls, fcalls & mfails:
 *	Used to keep track of malloc and free calls.  Plus keep track
 *	of the number of times malloc is called, but it returns 0.
 */
static int mcalls, rcalls, fcalls, mfails;

/* mtot, ftot & highwater:
 *	Keep track of total amount of memory allocated.
 */
static int mtot, ftot, highwater;

/* mquiet:
 * If set (by heap -q), then the MALLOC ERROR messages are not
 * printed at runtime when an error is detected.
 */
static char mquiet;

/* mtrace:
 * If set (by heap -V), then each call to malloc or free will include
 * a printed message.
 */
static char mtrace;

/* heapbase:
 *	Initially zero, this pointer is set to the base of the heap on
 *	the first call to malloc().
 */
static struct mhdr	*heapbase;
static long allocbuf[ALLOCSIZE/sizeof(long)];

static void
heapinit(void)
{
	mcalls = rcalls = fcalls = mfails = 0;
	mtot = ftot = highwater = 0;
	heapbase = (struct mhdr *)allocbuf;
	heapbase->pretag = PRETAG;
	heapbase->posttag = POSTTAG;
	heapbase->size = ALLOCSIZE - MHDRSIZE;
	if (heapbase->size < 0)
		printf("heapinit(): ALLOCSIZE too small!\n");
	heapbase->next = (struct mhdr *)0;
	heapbase->prev = (struct mhdr *)0;
}

/* heapcheck():
 *	Called with an mhdr pointer (or NULL).  This function just steps through
 *	the heap control structures to make sure there is some level of sanity.
 *	If the incoming mhdr pointer is non-zero, then it will also verify that
 *	the pointer is a valid control pointer in the heap.
 */
int
heapcheck(struct mhdr *mp,char *msg)
{
	int	i, mpok;
	register struct	mhdr *mptr;

	mpok = 0;
	if (!heapbase)
		heapinit();

	mptr = heapbase;
	for(i=0;mptr;i++,mptr=mptr->next) {
		if (mp == mptr)
			mpok = 1;
		if ((mptr->pretag != PRETAG) || (mptr->posttag != POSTTAG)) {
			if (!mquiet) {
				printf("\007MALLOC ERROR: heap corrupted at entry %d",i);
				if (msg)
					printf(" (%s)",msg);
				printf("\n");
			}
			return(-1);
		}
	}
	if ((mp) && (!mpok)) {
		if (!mquiet) {
			printf("\007MALLOC ERROR: 0x%lx (mem @ 0x%lx) invalid heap pointer",
		   	 (ulong)mp,(ulong)(mp+1));
			if (msg)
				printf(" (%s)",msg);
			printf("\n");
		}
		return(-1);
	}
	return(0);
}

char *
malloc(int size)
{
	register struct	mhdr *mptr, *mptr1;

	if (mtrace)
		printf("malloc(%d) = ",size);

	if (size <= 0) {
		if (mtrace)
			printf("0\n");
		return(0);
	}

	/* Start by checking sanity of heap. */
	if (heapcheck(0,0) < 0) {
		if (mtrace)
			printf("00\n");
		return((char *)0);
	}

	/* Keep track of number of calls to malloc for debug. */
	mcalls++;

	/* Make size divisible by 4: */
	if (size & 3) {
		size += 4;
		size &= 0xfffffffc;
	}

	mptr = (struct mhdr *)heapbase;
	while(1) {
		/* If request size is equal to the free block size or
		 * if the free block size is only slightly larger than the
		 * request size, then just use that free block as is.
		 * If the request size is at least "MHDRSIZE * 2"
		 * bytes smaller than free block size, then break
		 * that block up into 2 smaller chunks with one of the chunks
		 * being the size of the request and the size of the other chunk
		 * being whatever is left over.
		 */
		if (mptr->size >= size) {
			if (mptr->size <= (int)(size + (MHDRSIZE * 2))) {
				mtot += mptr->size;
				if ((mtot - ftot) > highwater)
					highwater = (mtot - ftot);
				mptr->size = -mptr->size;
			}
			else {
				mptr1 = (struct mhdr *)((char *)(mptr+1) + size);
				mptr1->pretag  = PRETAG;
				mptr1->posttag = POSTTAG;
				mptr1->next = mptr->next;
				mptr->next = mptr1;
				if (mptr1->next)
					mptr1->next->prev = mptr1;
				mptr1->prev = mptr;
				mptr1->size = (mptr->size - size) - MHDRSIZE;
				mptr->size = -size;
				mtot += size;
				if ((mtot - ftot) > highwater)
					highwater = (mtot - ftot);
			}
			if (mtrace)
				printf("0x%lx\n",(long)(mptr+1));
			return((char *)(mptr+1));
		}
		if (mptr->next == (struct mhdr *)0) {
			mfails++;
			if (!mquiet)
				printf("\007MALLOC ERROR: no more memory\n");
			if (mtrace)
				printf("000\n");
			return((char *)0);
		}
		else
			mptr = mptr->next;
	}
}

void
free(void *vp)
{
	char	*cp	 = vp;
	struct	mhdr	*mptr;

	if (mtrace)
		printf("free(0x%lx)\n",(long)cp);

	/* Keep track of number of calls to free for debug. */
	fcalls++;

	mptr = (struct mhdr *)cp - 1;

	/* Start by checking sanity of heap and make sure that the incoming
	 * pointer corresponds to a valid entry in the heap.
	 */
	if (heapcheck(mptr,0) < 0)
		return;

	/* The first thing to do to free the block is to make the size
	 * positive.
	 */
	mptr->size = abs(mptr->size);

	/* Keep track of how much memory is freed for debug. */
	ftot += mptr->size;

	/* To defragment the memory, see if previous and/or
	 * next block is free; if yes, then join them into one larger
	 * block. Note that the blocks will only be joined if they are in
	 * contiguous memory space.
	 */
	if (mptr->next) {
		if ((mptr->next->size > 0)  &&
		    (mptr->next == (struct mhdr *)
		    ((char *)mptr + mptr->size + MHDRSIZE))) {
			mptr->size += mptr->next->size + MHDRSIZE;
			mptr->next = mptr->next->next;
			if (mptr->next)
				mptr->next->prev = mptr;
		}
	}
	if (mptr->prev) {
		if ((mptr->prev->size > 0) &&
		    (mptr->prev == (struct mhdr *)
		    ((char *)mptr-mptr->prev->size - MHDRSIZE))) {
			if (mptr->next)
				mptr->next->prev = mptr->prev;
			mptr->prev->next = mptr->next;
			mptr->prev->size += mptr->size + MHDRSIZE;
		}
	}
}

/* calloc():
 *	Allocate space for an array of nelem elements of size elsize.
 *	Initialize the space to zero.
 */
char *
calloc(int nelem, int elsize)
{
	register char	*cp, *end;
	char	*base;
	int		size;

	size = nelem * elsize;
	base = malloc(size);
	if (base) {
		cp = base;
		end = base+size;
		while(cp<end)
			*cp++ = 0;
	}
	return(base);
}

char *
realloc(char *cp,int newsize)
{
	char			*new;
	int				asize, delta;
	struct	mhdr	*mptr, tmphdr;

	rcalls++;

	/* If incoming pointer is NULL, then do a regular malloc. */
	if (!cp)
		return(malloc(newsize));

	/* If newsize is zero and pointer is not null, then do a free. */
	if (!newsize) {
		free(cp);
		return((char *)0);
	}

	/* Set the mhdr pointer to the header attached to the incoming 
	 * char pointer.  We assume here that the incoming pointer is the
	 * base address of the block of memory that is being reallocated.
	 */
	mptr = (struct mhdr *)cp - 1;

	/* Start by checking sanity of heap and make sure that the incoming
	 * pointer corresponds to a valid entry in the heap.
	 */
	if (heapcheck(mptr,0) < 0)
		return((char *)0);

	/* Recall that mptr->size is negative since the block is not free, so
	 * use the absolute value of mptr->size...
	 */
	asize = abs(mptr->size);

	/* Make requested size divisible by 4:
	 */
	if (newsize & 3) {
		newsize += 4;
		newsize &= 0xfffffffc;
	}

	/* If newsize is less than or equal to current size, just return with
	 * the same pointer.  At some point, this should be improved so that
	 * the memory made made available by this reallocation is put back in
	 * the pool.
	 */
	if (newsize <= asize) 
		return(cp);

	/* Now we do the actual reallocation...
	 * If there is a fragment after this one (next != NULL) AND it is
	 * available (size > 0) AND the combined size of the next fragment
	 * along with the current fragment exceeds the request, then we can
	 * reallocate quickly.
	 * Otherwise, we have to just malloc a whole new block and copy the
	 * old buffer to the new larger space.
	 */
	if ((mptr->next) && (mptr->next->size > 0) &&
		((asize + mptr->next->size + MHDRSIZE) > newsize)) {

		/* At this point we know we have the space to reallocate without
		 * the malloc/free step.  Now we need to add the necessary space
		 * to the current fragment, and take that much away from the next
		 * fragment...
		 */
		delta = newsize - asize;
		/* next line used to be: tmphdr = *mptr->next... */
		memcpy((char *)&tmphdr,(char *)mptr->next, sizeof(struct mhdr));
		mptr->size = -newsize;
		mptr->next = (struct mhdr *)(delta + (int)(mptr->next));
		mptr->next->size = (abs(tmphdr.size) - delta);
		mptr->next->pretag  = PRETAG;
		mptr->next->posttag = POSTTAG;
		mptr->next->next = tmphdr.next;
		mptr->next->prev = tmphdr.prev;

		/* Keep track of totals and highwater:
		 */
		mtot += (newsize - asize);
		if ((mtot - ftot) > highwater)
			highwater = (mtot - ftot);
		return(cp);
	}

	/* If the next fragment is not large enough, then malloc new space,
	 * copy the existing data to that block, free the old space and return
	 * a pointer to the new block.
	 */
	new = malloc(newsize);
	if (!new)
		return((char *)0);

	memcpy(new,cp,asize);
	free(cp);
	return(new);
}

#else
char *
malloc(int size)
{
	return(0);
}

void
free(char *buf)
{
}

char *
realloc(char *buf, int newsize, char *fname, int fline)
{
	return(0);
}

#endif
