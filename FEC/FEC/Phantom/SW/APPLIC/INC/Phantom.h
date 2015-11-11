#ifndef _Phantom_h_
#define _Phantom_h_

#ifdef __cplusplus
 extern "C" {
#endif
   
#include "cfgDBUtil.h"


typedef enum {
  BOARD_TYPE_IBD,
  BOARD_TYPE_ITH,
  BOARD_TYPE_DRYER,
  BOARD_TYPE_PHANTOM=100
} BOARD_TYPE;



#ifdef __cplusplus
}
#endif

#endif
