/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef netio_address_h
#define netio_address_h


/*e \file
  \brief NETIO Route API
*/

#ifndef netio_dll_h
#include "netio/netio_dll.h"
#endif
#ifndef osapi_config_h
#include "osapi/osapi_config.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef reda_sequence_h
#include "reda/reda_sequence.h"
#endif
#ifndef db_db_h
#include "db/db_api.h"
#endif
#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define NETIO_ADDRESS_PORT_INVALID   (0)
#define NETIO_ADDRESS_KIND_INVALID   (-1)
#define NETIO_ADDRESS_KIND_RESERVED  0
#define NETIO_ADDRESS_KIND_UDPv4     1
#define NETIO_ADDRESS_KIND_UDPv6     2
#define NETIO_ADDRESS_KIND_SHMEM     3
#define NETIO_ADDRESS_KIND_GUID      4


struct NETIO_GuidPrefix
{
    RTI_UINT8 value[12];
};

struct NETIO_GuidEntity
{
    RTI_UINT8 value[4];
};


struct NETIO_AddressRtps {
    RTI_UINT32 host_id;
    RTI_UINT32 app_id;
    RTI_UINT32 instance_id;
    RTI_UINT32 object_id;
};

struct NETIO_Guid {
    struct NETIO_GuidPrefix prefix;
    struct NETIO_GuidEntity entity;
};

struct NETIO_AddressGuidPrefix {
    RTI_UINT8 prefix[12];
    RTI_UINT32 entity;
};

struct NETIO_AddressIp4 {
    RTI_UINT32 address;
    RTI_UINT8 _unused[12];
};

struct NETIO_AddressIpv6 {
    RTI_UINT8 octet[16];
};

struct NETIO_AddressShem {
    RTI_UINT32 key;
    RTI_UINT8 guid[12];
};

struct NETIO_AddressInit {
    RTI_UINT32 val0;
    RTI_UINT32 val1;
    RTI_UINT32 val2;
    RTI_UINT32 val3;
};

struct NETIO_AddressInt32 {
    RTI_INT32 value[4];
};

union NETIO_AddressValue
{
    struct NETIO_AddressRtps rtps_guid;
    struct NETIO_Guid guid;
    struct NETIO_AddressGuidPrefix guid_prefix;
    struct NETIO_AddressIp4 ipv4;
    struct NETIO_AddressIpv6 ipv6;
    struct NETIO_AddressShem shmem;
    struct NETIO_AddressInit init;
    struct NETIO_AddressInt32 as_int32;
};
/* NOTE: All address kinds _must be exactly_ 16 bytes in size */
struct NETIO_Address
{
    /*e Address discriminator
     */
    RTI_INT32 kind;

    RTI_UINT32 port;

    union NETIO_AddressValue value;
};

#define NETIO_NETMASK_MASK_LENGTH        (4)
#define NETIO_NETMASK_MASK_SIZE          ((RTI_INT32)(sizeof(RTI_INT32)))
#define NETIO_NETMASK_MASK_BITS_PER_UNIT ((RTI_INT32)(sizeof(RTI_INT32) * 8))
#define NETIO_NETMASK_MASK_MAX_BITS      (NETIO_NETMASK_MAX_LENGTH * NETIO_NETMASK_ELEMENT_SIZE*8)

struct NETIO_Netmask
{
    RTI_INT32 bits;
    RTI_INT32 mask[NETIO_NETMASK_MASK_LENGTH];
};

REDA_DEFINE_SEQUENCE(NETIO_AddressSeq, struct NETIO_Address)
REDA_DEFINE_SEQUENCE(NETIO_NetmaskSeq, struct NETIO_Netmask)

#define NETIO_AddressSeq_INITIALIZER REDA_DEFINE_SEQUENCE_INITIALIZER(struct NETIO_Address)
#define NETIO_NetmaskSeq_INITIALIZER REDA_DEFINE_SEQUENCE_INITIALIZER(struct NETIO_Netmask)

#define NETIO_ADDRESS_INVALID                      0x0
#define NETIO_ADDRESS_LOCALHOST                    0x7F000001
#define NETIO_ADDRESS_GUID_UNKNOWN {{{0,0,0,0,0,0,0,0,0,0,0,0}},{{0,0,0,0}}}

NETIODllExport RTI_INT32
NETIO_Address_compare(const struct NETIO_Address *left_addr,
                      const struct NETIO_Address *right_addr);

#define NETIO_Address_INITIALIZER \
{\
    NETIO_ADDRESS_KIND_RESERVED, /* kind */ \
    0, /* port */ \
    {{0,0,0,0}} /* value */ \
}

#define NETIO_Address_init(addr_,kind_) \
{\
    (addr_)->kind = kind_; (addr_)->port = 0; \
    (addr_)->value.init.val0 = (addr_)->value.init.val1 = 0;\
    (addr_)->value.init.val2 = (addr_)->value.init.val3 = 0;\
}

#define NETIO_Address_set_ipv4(addr_,port_,address_) \
{\
    (addr_)->kind = NETIO_ADDRESS_KIND_UDPv4; \
    (addr_)->port = port_; \
    (addr_)->value.ipv4.address = address_;\
}

#define NETIO_Address_set_guid(addr_,port_,guid_) \
{\
    (addr_)->kind = NETIO_ADDRESS_KIND_GUID; \
    (addr_)->port = port_; \
    OSAPI_Memory_copy(&((addr_)->value.guid),(guid_),16);\
}

#if RTI_ENDIAN_BIG
#define NETIO_Address_set_guid_from_key(addr_,port_,key_) \
{\
    (addr_)->kind = NETIO_ADDRESS_KIND_GUID; \
    (addr_)->port = port_; \
    OSAPI_Memory_copy(&((addr_)->value.guid),(key_),16);\
}

#else
#define NETIO_Address_set_guid_from_key(addr_,port_,key_) \
{\
    (addr_)->kind = NETIO_ADDRESS_KIND_GUID; \
    (addr_)->port = port_; \
    (addr_)->value.rtps_guid.host_id = NETIO_htonl((key_)->value[0]);\
    (addr_)->value.rtps_guid.app_id = NETIO_htonl((key_)->value[1]);\
    (addr_)->value.rtps_guid.instance_id = NETIO_htonl((key_)->value[2]);\
    (addr_)->value.rtps_guid.object_id = NETIO_htonl((key_)->value[3]);\
}
#endif

#if RTI_ENDIAN_BIG
#define NETIO_Address_set_guid_from_int32(addr_,port_,int0_,int1_,int2_,int3_) \
{\
    (addr_)->kind = NETIO_ADDRESS_KIND_GUID; \
    (addr_)->port = port_; \
    (addr_)->value.rtps_guid.host_id = int0_;\
    (addr_)->value.rtps_guid.app_id = int1_;\
    (addr_)->value.rtps_guid.instance_id = int2_;\
    (addr_)->value.rtps_guid.object_id = int3_;\
}

#else
#define NETIO_Address_set_guid_from_int32(addr_,port_,int0_,int1_,int2_,int3_) \
{\
    (addr_)->kind = NETIO_ADDRESS_KIND_GUID; \
    (addr_)->port = port_; \
    (addr_)->value.rtps_guid.host_id = NETIO_htonl(int0_);\
    (addr_)->value.rtps_guid.app_id = NETIO_htonl(int1_);\
    (addr_)->value.rtps_guid.instance_id = NETIO_htonl(int2_);\
    (addr_)->value.rtps_guid.object_id = NETIO_htonl(int3_);\
}
#endif

#define NETIO_Address_set_ipv6(addr_,port_,ipv6_) \
{\
    (addr_)->kind = NETIO_ADDRESS_KIND_GUID; \
    (addr_)->port = port_; \
    (addr_)->value.ipv6 = (ipv6_);\
}

#define NETIO_Address_isIpv4(address) ((address)->_field[0] == 0 && \
                                       (address)->_field[1] == 0 && \
                                       (address)->_field[2] == 0)

/* Is it in range [::224.0.0.0, ::239.255.255.255]? */
#define NETIO_Address_isIpv4Multicast(address) \
  (NETIO_Address_isIpv4(address) && \
   ((address)->netOrderedByte[12] & 0xF0) == 0xE0)

NETIODllExport RTI_BOOL
NETIO_Address_ipv4_string_to_address(struct NETIO_Address *address_out,
                                     const char *address_in);

#define NETIO_Address_compare(left_,right_) \
(OSAPI_Memory_compare(left_, right_, sizeof(struct NETIO_Address)))

NETIODllExport RTI_BOOL
NETIO_Address_parse(const char *name,
                    RTI_UINT32 *base_port,
                    RTI_INT32 *index,
                    RT_ComponentFactoryId_T *out_id,
                    char *address_string);

#ifdef __cplusplus
}                               /* extern "C" */
#endif

#include "netio/netio_common_impl.h"

#endif /* netio_address_h */
