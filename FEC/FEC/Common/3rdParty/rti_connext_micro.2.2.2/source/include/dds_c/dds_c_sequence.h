/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dds_c_sequence_h
#define dds_c_sequence_h



#ifndef dds_c_dll_h
#include "dds_c/dds_c_dll.h"
#endif
#ifndef dds_c_common_h
#include "dds_c/dds_c_common.h"
#endif
#ifndef reda_sequence_h
#include "reda/reda_sequence.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* Make sure that NDDSUSERDllExport expands to nothing by default */
#ifndef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

/*ci @ingroup DDSSequenceModule
  @brief    Empty Sequence dll export variable

  Since for dds c sequence structure, there are no methods. Therefore, there is
  no need to export anything.
*/
#define DDS_SEQUENCE_DLL_EXPORT

#define DDS_SEQUENCE_METHODS_IN_C REDA_DEFINE_SEQUENCE_IN_C


/*i @ingroup DDSSequenceModule

  @brief Declares an abstract \st_interface \p TSeq, for IDL \p
  sequence\_T of type \p T.
 
   @uses     T      base type
   @defines  TSeq   name of the sequence type to be defined

   Using this macro introduces a class declaration TSeq in the
   enclosing name space.

   <STRONG>Incarnation:</STRONG>

   \em Incarnation is the process of realizing a concrete type
   from the \em generic skeleton defined by TSeq.  Given a user type
   \p Foo, a type \p FooSeq representing the IDL type \p
   sequence\_Foo is incarnated as follows.

   <EM>\em Value-type sequences</EM>

   When the base type \p T is a value-type, \p sequence\_T is declared
   follows.  
   \code
      DDS_SEQUENCE(FooSeq, Foo);
   \endcode

   <EM>\em Reference-type sequences</EM>

   When the base type \p Foo is a reference-type, \p sequence\_Foo is
   declared as follows.  

   \code
      typedef  Foo*  Foo_ptr;

      DDS_SEQUENCE(FooSeq, Foo_ptr);

   \endcode      

   We use a two step process, because in C++
   \code 
       const Foo_ptr  =   Foo *const  
                 !=   const Foo*
   \endcode
   and we want the \p Foo_ptr semantics for the method:
   \code
       const T& TSeq::operator[]() const
   \endcode

  \hideinitializer
*/

#define DDS_SEQUENCE REDA_DEFINE_SEQUENCE

/*ce \dref_FooSeq_SEQUENCE_INITIALIZER
 */
#define DDS_SEQUENCE_INITIALIZER(_t) REDA_DEFINE_SEQUENCE_INITIALIZER(_t)


/* ================================================================= */
#ifdef DOXYGEN_DOCUMENTATION_ONLY

/*ce \dref_FooSeq
 */
struct FooSeq
{
    /*e @brief Pointer to array of contiguous or discontiguous data
     *  
     */
    Foo *_contiguous_buffer;


    /*e @brief Maximum size of the sequence
     * 
     * The allocated length of this sequence. It applies to whichever of
     * the above buffers is non-NULL, if any. If both a NULL, its value
     * must be 0.
     * 
     * If _maximum == 0, _owned == true.
     */
    DDS_UnsignedLong _maximum;

    /*e @brief Actual length of the sequence that contains data
     * 
     * The current logical length of this seqeunce, i.e. the number of valid
     * elements it contains. It applies to whichever of the above
     * buffers is non-null, if any. If both are NULL, its value must be 0.
     */
    DDS_UnsignedLong _length;

    /*e @brief Size of data element in the sequence
     *  
     * Each element in the sequence has this size. 
     */
    DDS_Long _element_size;

    /*e @brief Size of data element in the sequence
     *  
     * Each element in the sequence has this size. 
     */
    DDS_Long _element_size;

    /*e @brief Internal-use flags
     *  
     * Reserved for internal-use. 
     */
    DDS_Char _flags;
};

/*ce \dref_FooSeq_initialize
 */
DDS_Boolean FooSeq_initialize(struct FooSeq *self);

/*ce \dref_FooSeq_finalize
 */
DDS_Boolean FooSeq_finalize(struct FooSeq *self);

/*ce \dref_FooSeq_get_maximum
 */
DDS_Long FooSeq_get_maximum(const struct FooSeq *self);

/*ce \dref_FooSeq_set_maximum
 */
DDS_Boolean FooSeq_set_maximum(struct FooSeq *self, DDS_Long new_max);

/*ce \dref_FooSeq_get_length
 */
DDS_Long FooSeq_get_length(const struct FooSeq *self);

/*ce \dref_FooSeq_set_length
 */
DDS_Boolean FooSeq_set_length(struct FooSeq *self, DDS_Long new_length);

/*ce \dref_FooSeq_get_reference
 */
Foo *FooSeq_get_reference(const struct FooSeq *self, DDS_Long i);

/*ce \dref_FooSeq_copy
 */
struct FooSeq *FooSeq_copy(struct FooSeq *self, const struct FooSeq *src_seq);

/*ce \dref_FooSeq_copy
 */
DDS_Boolean FooSeq_is_equal(struct FooSeq* self, void *buffer,   
                            RTI_INT32 new_length, RTI_INT32 new_max);

/*ce \dref_FooSeq_loan_contiguous
 */
DDS_Boolean FooSeq_loan_contiguous(struct FooSeq *self, void *buffer, 
                                   DDS_Long new_length, DDS_Long new_max);

/*ce \dref_FooSeq_loan_discontiguous
 */
DDS_Boolean FooSeq_loan_discontiguous(struct FooSeq *self, void *buffer, 
                                   DDS_Long new_length, DDS_Long new_max);

/*ce \dref_FooSeq_unloan
 */
DDS_Boolean FooSeq_unloan(struct FooSeq *self);

/*ce \dref_FooSeq_has_ownership
 */
DDS_Boolean FooSeq_has_ownership(const struct FooSeq *self);

/*ce \dref_FooSeq_get_buffer
 */
Foo* FooSeq_get_buffer(struct FooSeq *self);

/*ce \dref_FooSeq_set_buffer
 */
DDS_Boolean FooSeq_set_buffer(struct FooSeq *self, Foo* buffer);

/*ce \dref_FooSeq_has_discontiguous_buffer
 */
DDS_Boolean FooSeq_has_discontiguous_buffer(const struct FooSeq *self);


#endif /* DOXYGEN_DOCUMENTATION_ONLY */

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* dds_c_sequence_h */
