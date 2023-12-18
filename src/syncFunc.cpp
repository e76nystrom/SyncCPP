#include <stdint.h>
#define NO_REM_MACROS
#include "syncParm.h"
#include "syncStruct.h"

T_SYNC_VAR sVar;

#define FLT (0x80)
#define SIZE_MASK (0x7)

unsigned char syncSize[] =
{
 sizeof(sVar.syncCycle),                /* 0x00 sync cycle length */
 sizeof(sVar.syncOutput),               /* 0x01 sync outputs per cycle */
 sizeof(sVar.syncPrescaler),            /* 0x02 sync prescaler */
 sizeof(sVar.syncEncoder),              /* 0x03 sync encoder pulses */
 sizeof(sVar.syncMaxParm),              /* 0x04 sync maximum parameter */
};

void setSyncVar(const int parm, const T_DATA_UNION val)
{
 switch(parm)
 {
 default:
  break;

 case SYNC_CYCLE:                /*  0 0x00 sync cycle length */
  sVar.syncCycle = val.t_uint16_t;
  break;

 case SYNC_OUTPUT:               /*  1 0x01 sync outputs per cycle */
  sVar.syncOutput = val.t_uint16_t;
  break;

 case SYNC_PRESCALER:            /*  2 0x02 sync prescaler */
  sVar.syncPrescaler = val.t_uint16_t;
  break;

 case SYNC_ENCODER:              /*  3 0x03 sync encoder pulses */
  sVar.syncEncoder = val.t_uint16_t;
  break;

 case SYNC_MAX_PARM:             /*  4 0x04 sync maximum parameter */
  sVar.syncMaxParm = val.t_int16_t;
  break;

 }
}

void getSyncVar(const int parm, const P_DATA_UNION val)
{
 switch(parm)
 {
 default:
  break;

 case SYNC_CYCLE:                /*  0 0x00 sync cycle length */
  val->t_uint16_t = sVar.syncCycle;
  break;

 case SYNC_OUTPUT:               /*  1 0x01 sync outputs per cycle */
  val->t_uint16_t = sVar.syncOutput;
  break;

 case SYNC_PRESCALER:            /*  2 0x02 sync prescaler */
  val->t_uint16_t = sVar.syncPrescaler;
  break;

 case SYNC_ENCODER:              /*  3 0x03 sync encoder pulses */
  val->t_uint16_t = sVar.syncEncoder;
  break;

 case SYNC_MAX_PARM:             /*  4 0x04 sync maximum parameter */
  val->t_int16_t = sVar.syncMaxParm;
  break;

 }
}
