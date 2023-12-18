#if !defined(SYNC_STRUCT)
#define SYNC_STRUCT
// c1File

#include <stdint.h>

#if !defined(__DATA_UNION__)
#define __DATA_UNION__

#define uint_t unsigned int

typedef union uDataUnion
{
 float    t_float;
 int      t_int;
 uint_t   t_uint_t;
 int32_t  t_int32_t;
 uint32_t t_uint32_t;
 int16_t  t_int16_t;
 uint16_t t_uint16_t;
 char     t_char;
} T_DATA_UNION, *P_DATA_UNION;

#endif  /* __DATA_UNION__ */

void setSyncVar(int parm, T_DATA_UNION val);
void getSyncVar(int parm, P_DATA_UNION val);

typedef struct sSyncVar
{
 uint16_t syncCycle;    /* 0x00 SYNC_CYCLE       sync cycle length */
 uint16_t syncOutput;   /* 0x01 SYNC_OUTPUT      sync outputs per cycle */
 uint16_t syncPrescaler;/* 0x02 SYNC_PRESCALER   sync prescaler */
 uint16_t syncEncoder;  /* 0x03 SYNC_ENCODER     sync encoder pulses */
 int16_t syncMaxParm;   /* 0x04 SYNC_MAX_PARM    sync maximum parameter */
} T_SYNC_VAR, *P_SYNC_VAR;

extern unsigned char syncSize[];
extern T_SYNC_VAR sVar;

#endif /* SYNC_STRUCT */
