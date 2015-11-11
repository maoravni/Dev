#ifndef __SNTP_H__
#define __SNTP_H__

#ifdef __cplusplus
extern "C"
{
#endif

extern u32_t sntp_server_address;

void sntp_init(u32_t server);

#ifdef __cplusplus
}
#endif

#endif /* __SNTP_H__ */
