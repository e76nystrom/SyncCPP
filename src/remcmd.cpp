#define __REMCMD__
#include "stm32f1xx_hal.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "syncStruct.h"

#include "config.h"
#include "remvar.h"
#include "serialio.h"
#include "spix.h"
#include "sync.h"

#ifdef EXT
#undef EXT
#endif

#define EXT
#include "remcmd.h"

#if defined(__REMCMD_INC__)	// <-

#if !defined(EXT)
#define EXT extern
#endif

#define DBG_LOAD 1

#if defined(REMPORT)
#if REM_ISR
#define putrem putRem
#define putstrrem putstrRem
#define sndhexrem sndhexRem
#define getrem getRem
#define gethexrem gethexRem
#define getnumrem getnumRem
#define getstrrem getstrRem
#else
#define putrem putx1
#define putstrrem putstr1
#define sndhexrem sndhex1
#define getrem get1
#define gethexrem gethex1
#define getnumrem getnum1
#define getstrrem getstr1
#endif	/* REM_ISR */
#endif	/* REMPORT */

EXT int16_t tmpval;

typedef struct sRemAction
{
 int (*getCh)(void);
 void (*putCh)(char ch);
} T_REM_ACTION, *P_REM_ACTION;

#if defined(REMPORT)
extern T_REM_ACTION remAction;
#endif	/* REMPORT */

extern T_REM_ACTION spiAction;

void remcmd(P_REM_ACTION);

void loadVal(P_REM_ACTION act);

char getNum(P_REM_ACTION act);
char getHex(P_REM_ACTION act);
void sndHex(void (*putCh)(char ch), unsigned char *p, int size);

#include "syncCmdList.h"
#include "syncParmList.h"

#endif	/* __REMCMD_INC__*/ // ->
#ifdef __REMCMD__

#if defined(REMPORT)
T_REM_ACTION remAction = {&getRem, &putRem};
#endif	/* REMPORT */

T_REM_ACTION spiAction = {&getSPI, &putSPI};

#include "syncParm.h"

void remcmd(P_REM_ACTION act)
{
 //P_PARM valptr;
 int parm;

 remcmdUpdateTime = millis();
 remcmdTimeout = REMCMD_TIMEOUT;

 parm = act->getCh();
 act->putCh('-');
 getHex(act);		/* read parameter */
 parm = valRem;
 putBufChar(parm + '0');
 switch (parm)
 {
 case SYNC_SETUP:
  encoderSetup();
  break;

 case SYNC_START:
  encoderStart();
  break;

 case SYNC_STOP:
  cmpTmr.stop = 1;		/* set stop flag */
  break;
  
 case SYNC_LOADVAL:		/* load a local parameter */
  loadVal(act);
  break;

 case SYNC_LOADMULTI:		/* load multiple parameters */
 {
  getHex(act);
  int count = valRem;
  // printf("loadmulti count %d\n", count);
  while (--count >= 0)
   loadVal(act);
 }
 break;

 case SYNC_READVAL:		/* read a local parameter */
 {
  T_DATA_UNION parmVal;
  getHex(act);			/* save the parameter number */
  parm = valRem;		/* save it */
  if (parm < SYNC_MAX_PARM)	/* if in range */
  {
   parmVal.t_int = 0;
   getSyncVar(parm, &parmVal);
   int size = syncParm[parm];
#if DBG_LOAD
   printf("r p %2x s %d v %8x\n",
	  (unsigned int) parm, size, parmVal.t_unsigned_int);
#endif
   sndHex(act->putCh, (unsigned char *) &parmVal.t_char, size);
  }
 }
  break;

 case SYNC_POLL:
  act->putCh(' ');
  sndHex(act->putCh, (unsigned char *) &cmpTmr.encCount,
	 sizeof(cmpTmr.encCount));
  act->putCh(' ');
  sndHex(act->putCh, (unsigned char *) &cmpTmr.intCount,
	 sizeof(cmpTmr.intCount));
  break;

 default:
  break;
 }

#if 0
 while (1)
 {
  int tmp = getRem();
  if (tmp < 0)
  {
   printf("end of buffer\n");
   break;
  }
  if (tmp == '\r')
   break;
  printf("extra char %d\n", tmp);
  flushBuf();
 }
#endif

 while(1)
 {
  parm = act->getCh();
  if (parm < 0)
   break;
 }
 act->putCh('*');
}

void loadVal(P_REM_ACTION act)
{
 getHex(act);			/* read the parameter number */
 int parm = valRem;		/* save it */
 if (parm < SYNC_MAX_PARM)	/* if in range */
 {
  T_DATA_UNION parmVal;
  getHex(act);	/* get the value */
#if 0 && DBG_LOAD
  int size = syncParm[parm]; /* value size */
  printf("w parm %2x s %d val %8x\n", parm, size, (unsigned) valRem);
#endif
  parmVal.t_int = valRem;
  setSyncVar(parm, parmVal);
 }
}

char getHex(P_REM_ACTION act)
{
 char ch;
 int count;

 valRem = 0;
 count = 0;
 while (count <= 8)
 {
  int tmp = act->getCh();
  if (tmp > 0)
  {
   ch = (char) tmp;
   if ((ch >= '0')
   &&  (ch <= '9'))
   {
    act->putCh(ch);
    ch -= '0';
   }
   else if ((ch >= 'a')
   &&       (ch <= 'f'))
   {
    act->putCh(ch);
    ch -= 'a' - 10;
   }
   else if (ch == ' ')
   {
    act->putCh(ch);
    break;
   }
   else if (ch == '\r')
    break;
   else
    continue;

   count++;
   valRem <<= 4;
   valRem += ch;
  }
  else
   return(0);
 }
 return(count != 0);
}

void sndHex(void (*putCh)(char ch), unsigned char *p, int size)
{
 char tmp;
 char ch;
 int zeros = 0;

 p += size;
 while (size != 0)
 {
  --size;
  p--;
  tmp = *p;
  ch = tmp;
  ch >>= 4;
  ch &= 0xf;
  if ((ch != 0)
  ||  zeros)
  {
   zeros = 1;
   if (ch < 10)
    ch += '0';
   else
    ch += 'a' - 10;
   putCh(ch);
  }

  tmp &= 0xf;
  if ((tmp != 0)
  ||  zeros)
  {
   zeros = 1;
   if (tmp < 10)
    tmp += '0';
   else
    tmp += 'a' - 10;
   putCh(tmp);
  }
 }
 if (zeros == 0)
  putCh('0');
}

#endif
