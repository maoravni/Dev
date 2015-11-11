/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dds_c_string_h
#define dds_c_string_h



#ifndef dds_c_dll_h
#include "dds_c/dds_c_dll.h"
#endif
#ifndef dds_c_common_h
#include "dds_c/dds_c_common.h"
#endif
#ifndef dds_c_sequence_h
#include "dds_c/dds_c_sequence.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define DDS_StringSeq REDA_StringSeq
#define DDS_StringSeq_initialize REDA_StringSeq_initialize
#define DDS_StringSeq_finalize REDA_StringSeq_finalize
#define DDS_StringSeq_get_maximum REDA_StringSeq_get_maximum
#define DDS_StringSeq_set_maximum REDA_StringSeq_set_maximum
#define DDS_StringSeq_set_length REDA_StringSeq_set_length
#define DDS_StringSeq_get_length REDA_StringSeq_get_length
#define DDS_StringSeq_get_reference REDA_StringSeq_get_reference
#define DDS_StringSeq_copy REDA_StringSeq_copy
#define DDS_StringSeq_is_equal REDA_StringSeq_is_equal
#define DDS_StringSeq_loan_contiguous REDA_StringSeq_loan_contiguous
#define DDS_StringSeq_loan_discontiguous REDA_StringSeq_loan_discontiguous
#define DDS_StringSeq_unloan REDA_StringSeq_unloan
#define DDS_StringSeq_has_ownership REDA_StringSeq_has_ownership
#define DDS_StringSeq_get_buffer REDA_StringSeq_get_buffer
#define DDS_StringSeq_set_buffer REDA_StringSeq_set_buffer
#define DDS_StringSeq_has_discontiguous_buffer REDA_StringSeq_has_discontiougous_buffer

/* ----------------------------------------------------------------- */
/*e @ingroup DDSStringClass
 * 
 * @brief Create a new empty string that can hold up to \p length
 *        characters.
 * 
 *        A string created by this \method must be deleted using 
 *        DDS_String_free().
 *
 *        This function will allocate enough memory to hold a string of 
 *        \p length characters, \b plus one additional byte to hold the NULL 
 *        terminating character.
 * 
 * @param length \st_in Capacity of the string. 
 * 
 * @return A newly created non-NULL string upon success or
 *         NULL upon failure.
 */
DDSCDllExport char*
DDS_String_alloc(DDS_UnsignedLong length);

#define DDS_String_alloc(l_) REDA_String_alloc(l_)

/* ----------------------------------------------------------------- */
/*e @ingroup DDSStringClass
 * 
 * @brief Clone a string. Creates a new string that duplicates 
 *        the value of \p string. 
 * 
 *        A string created by this \method must be deleted using 
 *        DDS_String_free()
 * 
 * @param str \st_in The string to duplicate.
 * 
 * @return If \p string == NULL, this \method always returns NULL.
 *         Otherwise, upon success it returns a newly created string
 *         whose value is \p string; upon failure it returns NULL.
 */
DDSCDllExport char*
DDS_String_dup(const char *str);

#define DDS_String_dup(str_) REDA_String_dup(str_)

/* ----------------------------------------------------------------- */
/*i @ingroup DDSStringClass
 * 
 * @brief Assign a string to a new value. Replaces the string 
 *        pointed to by \p string_ptr, with a string whose value 
 *        is \p new_value.
 * 
 *        A string created by this \method must be deleted using 
 *        DDS_String_free().       
 *  
 * @pre \p string_ptr be a non-NULL pointer. *string_ptr must be 
 *      either NULL, or a string created using DDS_String_alloc() 
 *      or DDS_String_dup(), or DDS_String_replace().
 * 
 * @param string_ptr \st_inout Pointer to the string to replace.
 * 
 * @param new_value \st_in The value of the replacement string.
 * 
 * @return If \p new_value = NULL, this \method always returns NULL.
 *         Otherwise, upon success it returns *string_ptr whose value 
 *         is \p new_value; upon failure it returns NULL. 
 *
 * @post If \p new_value = NULL, *string_ptr == NULL.
 *       Otherwise, upon success \p string_ptr contains a pointer 
 *       to a string whose value is \p new_value; upon failure, 
 *       \p string_ptr is left unchanged.
 */
DDSCDllExport char*
DDS_String_replace(char **string_ptr, const char *new_value);

#define DDS_String_replace(str_,val_) REDA_String_replace(str_,val_)

/* ----------------------------------------------------------------- */
/*e @ingroup DDSStringClass
 * 
 * @brief Delete a string.
 * 
 * @pre \p string must be either NULL, or must have been created 
 *       using DDS_String_alloc(), DDS_String_dup()
 * 
 * @param str \st_in The string to delete.
 * @internal String could also have been created with DDS_String_replace()
 */
DDSCDllExport void
DDS_String_free(char *str);

#define DDS_String_free(str_) REDA_String_free(str_)

/* ----------------------------------------------------------------- */
/*e @ingroup DDSStringClass
 * 
 * @brief Compare two strings.
 * 
 * @pre \p s1 and s2 can be NULL or non-NULL.
 * 
 * @param s1 \st_in String 1 to compare.
 * @param s2 \st_in String 2 to compare.
 *
 * @return 0 if s1 == s2, > 0 if s1 > s2 and < 0 if s1 < s2
 */
DDSCDllExport int
DDS_String_cmp(const char *s1, const char *s2);

#define DDS_String_cmp(str1_,str2_) REDA_String_compare(str1_,str2_)

/* ----------------------------------------------------------------- */
/*e @ingroup DDSStringClass
 * 
 * @brief Return the length of a string
 * 
 * @pre \p string must be non-NULL
 * 
 * @param string \st_in String to return length of
 *
 * @return length of string. Does not include the null-termination character
 */
DDSCDllExport int
DDS_String_length(const char *string);

#define DDS_String_length(str1_) REDA_String_length(str1_)

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* dds_c_string_h */
