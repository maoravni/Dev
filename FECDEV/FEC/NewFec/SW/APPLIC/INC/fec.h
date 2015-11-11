#ifndef _FEC_h_
#define _FEC_h_

#ifdef __cplusplus
 extern "C" {
#endif
   

typedef enum {
  SUBSYS_ERROR,  //000
  SUBSYS_DRYER, //001
  SUBSYS_IBD,  //010
  SUBSYS_ITH,  //011
  SUBSYS_BTC,  //100
  SUBSYS_CONDITIONER,  //101
  SUBSYS_NOT_USED,  //  110
  SUBSYS_ATP,  //111
  
  SUBSYS_LAST
} SUBSYS_ID;
  
extern SUBSYS_ID GlobalSubSysId;  //global value

#ifdef __cplusplus
}
#endif

#endif