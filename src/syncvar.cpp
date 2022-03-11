#include <stdint.h>          /* For uint16_t definition */
#if !defined(EXT)
#define EXT
#endif

#include "syncstruct.h"

typedef struct sSyncParm
{
 char size;			/* parameter size */
} T_PARM, *P_PARM;

#define PARM(x) {sizeof(sVar.x)}
