/*
 * EE.C         Expression Evaluator
 *				I got this from SIMTEL...
 *				http://www.simtel.net/pub/simtelnet/msdos/c/c-eval.zip
 *				Modified for use in monitor.
 *
 * AUTHOR:      Mark Morley
 * COPYRIGHT:   (c) 1992 by Mark Morley
 * DATE:        December 1991
 * HISTORY:     Jan 1992 - Made it squash all command line arguments
 *                         into one big long string.
 *                       - It now can set/get VMS symbols as if they
 *                         were variables.
 *                       - Changed max variable name length from 5 to 15
 *              Jun 1992 - Updated comments and docs
 *              May 1995 - ELS additions:
 *				* changed global naming a bit.
 *				* added hex (0x) number type to accept hex input.
 *				* added hex() function to print result in hex.
 *				* added '>' '<' '|' and '&' to EE_level2() to 
 *			      support shifts and masking. 
 *				* additional arg to Evaluate so that it can 
 *			      return the string equivalent of the result.
 *
 * You are free to incorporate this code into your own works, even if it
 * is a commercial application.  However, you may not charge anyone else
 * for the use of this code!  If you intend to distribute your code,
 * I'd appreciate it if you left this message intact.  I'd like to
 * receive credit wherever it is appropriate.  Thanks!
 *
 * I don't promise that this code does what you think it does...
 *
 * Please mail any bug reports/fixes/enhancments to me at:
 *      morley@camosun.bc.ca
 * or
 *      Mark Morley
 *      3889 Mildred Street
 *      Victoria, BC  Canada
 *      V8Z 7G1
 *      (604) 479-7861
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
#if INCLUDE_EE
#include "stddefs.h"
#include "ee.h"
#include "genlib.h"
#include "cli.h"

#define TOKLEN          30              /* Max token length */

#define VAR             1
#define DEL             2
#define NUM             3
#define HEXNUM          4

/* Codes returned from the evaluator */
#define E_OK           0        /* Successful evaluation */
#define E_SYNTAX       1        /* Syntax error */
#define E_UNBALAN      2        /* Unbalanced parenthesis */
#define E_DIVZERO      3        /* Attempted division by zero */
#define E_UNKNOWN      4        /* Reference to unknown variable */
#define E_MAXVARS      5        /* Maximum variables exceeded */
#define E_BADFUNC      6        /* Unrecognised function */
#define E_NUMARGS      7        /* Wrong number of arguments to funtion */
#define E_NOARG        8        /* Missing argument to function */
#define E_EMPTY        9        /* Empty expression */
#define E_OPNOSUPPORT  10       /* Operation not supported */

static void	EE_error(int);
static int	EE_level1(), EE_level2(), EE_level3(), EE_level4();
static int	EE_level5(), EE_level6(), EE_power();

static int		EE_printmode;
static char		*EE_expression;			/* Pointer to user's expression */
static char		EE_token[TOKLEN + 1];   /* Holds the current token */
static int		EE_type;				/* Type of the current token */
static int 		EE_errno;

void
EEInit()
{
	EE_printmode = 0;
	EE_expression = (char *)0;
	memset(EE_token,0,TOKLEN+1);
	EE_type = 0;
}

TYPE
EE_hex(TYPE v)
{
	EE_printmode = HEX_PRINT;
	return(v);
}

TYPE
EE_hex0(TYPE v)
{
	EE_printmode = HEX0_PRINT;
	return(v);
}

TYPE
EE_hex2(TYPE v)
{
	EE_printmode = HEX2_PRINT;
	return(v);
}

TYPE
EE_hex4(TYPE v)
{
	EE_printmode = HEX4_PRINT;
	return(v);
}

TYPE
EE_hex8(TYPE v)
{
	EE_printmode = HEX8_PRINT;
	return(v);
}

#if EERAND
TYPE
EE_rand(void)
{
	static int firsttime = 1;

	if (firsttime) {
		srand((int)time(0));
		firsttime = 0;
	}

	return(rand());
}
#endif

/* Add any math functions that you wish to recognise here...  The first
 *	field is the name of the function as it would appear in an expression.
 *	The second field tells how many arguments to expect.  The third is
 *	a pointer to the actual function to use.
 */
FUNCTION EE_funcs[] =
{
	{ "hex",     1,    EE_hex },
	{ "hex0",    1,    EE_hex0 },
	{ "hex2",    1,    EE_hex2 },
	{ "hex4",    1,    EE_hex4 },
	{ "hex8",    1,    EE_hex8 },
#if EERAND
	{ "rand",    0,    EE_rand },
#endif
	{ 0,0,0 }
};

/* EE_parse():
 *	This function is used to grab the next token from the expression that
 *	is being evaluated.
 */
static void
EE_parse(void)
{
	char* t;

	EE_type = 0;
	t = (char *)EE_token;
	while( iswhite( *EE_expression ) )
		EE_expression++;
	if(isdelim(*EE_expression)) {
		EE_type = DEL;
		*t++ = *EE_expression++;
	}
	else if(!strncmp(EE_expression,"0x",2)) {
		EE_type = HEXNUM;
		EE_expression+=2;
		while( ishexnum( *EE_expression ) )
			*t++ = *EE_expression++;
	}
	else if(isnumer(*EE_expression)) {
		EE_type = NUM;
		while( isnumer( *EE_expression ) )
			*t++ = *EE_expression++;
	}
	else if(isalpha(*EE_expression)) {
		EE_type = VAR;
		while( isalnum( *EE_expression ) )
			*t++ = *EE_expression++;
		EE_token[VARLEN] = 0;
	}
	else if(*EE_expression) {
		*t++ = *EE_expression++;
		*t = 0;
		EE_error( E_SYNTAX );
	}
	*t = 0;
	while( iswhite( *EE_expression ) )
		EE_expression++;
}


/* EE_level1( TYPE* r )
 *	This function handles any variable assignment operations.
 *	It returns a value of 1 if it is a top-level assignment operation,
 *	otherwise it returns 0.
 */
static int
EE_level1(TYPE *r)
{
	char t[VARLEN + 1];

	if (EE_errno != E_OK)
		return(-1);

	if( EE_type == VAR ) {
		if( *EE_expression == '=' )
		{
			strcpy( t, EE_token );
			EE_parse();
			EE_parse();
			if( !*EE_token )
			{
				setenv(t,"");
				return(1);
			}
			if (EE_level2( r ) < 0)
				return(-1);
			shell_sprintf(t,"%u",*r);
			return( 1 );
		}
	}
	return(EE_level2( r ));
}


/* EE_level2( TYPE* r )
 *   This function handles any addition and subtraction operations.
 */
static int
EE_level2(TYPE *r)
{
	TYPE t = 0;
	char o;

	if (EE_errno != E_OK)
		return(-1);

	if (EE_level3( r ) < 0)
		return(-1);
	while((o = *EE_token) == '+' || o == '-' ||
			o == '&' || o == '|'  ||
			o == '>' || o == '<' )
	{
		EE_parse();
		if (EE_level3( &t ) < 0)
			return(-1);
		if( o == '+' )
			*r = *r + t;
		else if( o == '-' )
			*r -= t;
		else if( o == '&' )
			*r &= t;
		else if( o == '|' )
			*r |= t;
		else if( o == '>' )
			*r >>= t;
		else if( o == '<' )
			*r <<= t;
	}
	return(0);
}

/* EE_level3( TYPE* r )
 *	This function handles any multiplication, division, or modulo.
 */

static int
EE_level3(TYPE *r)
{
	TYPE t;
	char o;

	if (EE_errno != E_OK)
		return(-1);

	if (EE_level4( r ) < 0)
		return(-1);
	while( (o = *EE_token) == '*' || o == '/' || o == '%' )
	{
		EE_parse();
		if (EE_level4( &t ) < 0)
			return(-1);
		if( o == '*' ) {
			*r = *r * t;
		}
		else if( o == '/' )
		{
			if( t == 0 )
				EE_error( E_DIVZERO );
#ifdef DIVIDE_NOT_SUPPORTED
			EE_error(E_OPNOSUPPORT);
#else
			*r = *r / t;
#endif
		}
		else if( o == '%' )
		{
			if( t == 0 )
				EE_error( E_DIVZERO );
			*r = *r % t;
		}
	}
	return(0);
}

/* EE_level4( TYPE* r )
 *	This function handles any "to the power of" operations.
 */

static int
EE_level4(TYPE *r)
{
	TYPE t;

	if (EE_errno != E_OK)
		return(-1);
	if (EE_level5( r ) < 0)
		return(-1);
	if( *EE_token == '^' )
	{
		EE_parse();
		if (EE_level5( &t ) < 0)
			return(-1);
		*r = EE_power( *r, t );
	}
	return(0);
}

static int
EE_power(TYPE base,TYPE power)
{
	int i, val;

	val = 1;
	for(i=0;i<(long)power;i++)
		val *= base;

	return(val);
}


/* EE_level5(TYPE* r)
 *	This function handles any unary + or - signs.
 */

static int
EE_level5(TYPE *r)
{
	char o = 0;

	if (EE_errno != E_OK)
		return(0);
	if( *EE_token == '+' || *EE_token == '-' )
	{
		o = *EE_token;
		EE_parse();
	}
	if (EE_level6( r ) < 0)
		return(-1);
	if( o == '-' )
		*r = -(long)*r;
	return(0);
}


/* EE_level6(TYPE* r)
 *	This function handles any literal numbers, variables, or functions.
 */
static int
EE_level6(TYPE *r)
{
	char	*cp;
	int  i;
	int  n;
	TYPE a[3];

	if (EE_errno != E_OK)
		return(-1);

	if( *EE_token == '(' )
	{
		EE_parse();
		if( *EE_token == ')' ) {
			EE_error( E_NOARG );
		}
		EE_level1( r );
		if( *EE_token != ')' )
			EE_error( E_UNBALAN );
		EE_parse();
	}
	else
	{
		if( EE_type == NUM )
		{
			*r = (TYPE) atoi( EE_token );
			EE_parse();
		}
		else if( EE_type == HEXNUM )
		{
			*r = (TYPE) strtoul(EE_token,(char **)0,16);
			EE_parse();
		}
		else if( EE_type == VAR )
		{
			if( *EE_expression == '(' )
			{
				for( i = 0; EE_funcs[i].name; i++ ) {
					if( ! strcmp( EE_token, EE_funcs[i].name ) )
					{
						EE_parse();
						n = 0;
						do
						{
							EE_parse();
							if( *EE_token == ',') {
								EE_error( E_NOARG );
							}
							else if( *EE_token == ')' ) {
								break;
							}
							a[n] = 0;
							if (EE_level1( &a[n] ) < 0)
								return(-1);
							n++;
						} while( n < 4 && *EE_token == ',' );
						EE_parse();
						if( n != EE_funcs[i].args )
						{
							strcpy( EE_token, EE_funcs[i].name );
							EE_error( E_NUMARGS );
						}
						*r = EE_funcs[i].func( a[0], a[1], a[2] );
						return(0);
					}
				}
				if( ! EE_funcs[i].name )
					EE_error( E_BADFUNC );
			}
			else {
				cp = getenv(EE_token);
				if (cp)
					*r = strtol(cp,(char **)0,0);
				else
					EE_error( E_UNKNOWN );
			}
			EE_parse();
		}
		else
			EE_error( E_SYNTAX );
	}
	return(0);
}


/* Evaluate(char* e, TYPE* result, int* a ) 
 *	This function is called to evaluate the expression E and return the
 *	answer in RESULT.  If the expression was a top-level assignment, a
 *	value of 1 will be returned in A, otherwise it will contain 0.
 *	Returns E_OK if the expression is valid, or an error code. 
*/

int
Evaluate(char *e, char *sresult)
{
	TYPE result;

	EE_errno = E_OK;
	EE_printmode = DEC_PRINT;
	EE_expression = e;
	strtolower( EE_expression );
	result = 0;
	EE_parse();
	if( ! *EE_token )
		EE_error( E_EMPTY );
	EE_level1( &result );
	switch(EE_printmode) {
		case DEC_PRINT:
			sprintf(sresult,"%d",result);
			break;
		case HEX_PRINT:
			sprintf(sresult,"0x%x",result);
			break;
		case HEX0_PRINT:
			sprintf(sresult,"%x",result);
			break;
		case HEX2_PRINT:
			sprintf(sresult,"%02x",result);
			break;
		case HEX4_PRINT:
			sprintf(sresult,"%04x",result);
			break;
		case HEX8_PRINT:
			sprintf(sresult,"%08x",result);
			break;
	}

	return((int)result);
}

void
EE_error(int erno)
{
	char *err;

	EE_errno = erno;
	switch(erno) {
		case E_SYNTAX:
			err = "Syntax error";
			break;
		case E_UNBALAN:
			err = "Unbalanced parenthesis";
			break;
		case E_DIVZERO:
			err = "Attempted division by zero";
			break;
		case E_UNKNOWN:
			err = "Reference to unknown variable";
			break;
		case E_MAXVARS:
			err = "Maximum variables exceeded";
			break;
		case E_BADFUNC:
			err = "Unrecognised function";
			break;
		case E_NUMARGS:
			err = "Wrong number of arguments to funtion";
			break;
		case E_NOARG:
			err = "No args specified for function";
			break;
		case E_EMPTY:
			err = "Empty expression";
			break;
		case E_OPNOSUPPORT:
			err = "Operation not supported";
			break;
		default:
			err = "???";
			break;
	}

	printf("Expression Evaluator Error","%s",err);
}

/* setEE():
 * Called by 'set' command (as of uMon1.0, the 'let' command is removed.
 */
int
setEE(char *string)
{
	int len;
	char srslt[16];
	char *eqsign, vcopy[24];

	eqsign = strchr(string,'=');
	if (!eqsign)
		return(0);

	len = eqsign - string;
	if (len >= (sizeof(vcopy))) {
		printf("Varname too long\n");
		return(-1);
	}
	memcpy(vcopy,string,len);
	vcopy[len] = 0;

	Evaluate(eqsign+1,srslt);
	setenv(vcopy,srslt);
	return(1);
}

#endif
