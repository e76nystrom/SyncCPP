#if !defined(INCLUDE)
#define __LCLCMD__
#include "stm32f1xx_hal.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define EXT extern
#include "sync.h"
#include "remvar.h"
#include "serialio.h"

#ifdef EXT
#undef EXT
#endif

#define EXT
#include "lclcmd.h"
#endif

#if defined(__LCLCMD_INC__)	// <-

#if !defined(EXT)
#define EXT extern
#endif

void lclcmd(int ch);

#endif	// ->
#ifdef __LCLCMD__

int lastFlags;

#if PIN_DISPLAY
void pinDisplay();
#endif

typedef struct
{
 char ch;
 GPIO_TypeDef *port;
} T_PORT_LIST, *P_PORT_LIST;

T_PORT_LIST portList[] =
{
 {'a', GPIOA},
 {'b', GPIOB},
 {'c', GPIOC},
 {'d', GPIOD},
 {'e', GPIOE},
};

void lclcmd(int ch)
{
 if (ch == 'i')			/* init character buffer */
 {
  initCharBuf();
 }
 else if (ch == 'b')
 {
  printf("\ncharOverflow %d\n", charOverflow);
 }
 else if (ch == 1)
 {
  cmpTmrCap1SetIE();
  cmpTmrCap2SetIE();
  cmpTmrStart();
 }
 else if (ch == 2)
 {
  cmpTmr.startInt = 1;
 }

 else if (ch == 'e')
 {
  newline();
  encoderStop();
  if (query(&getnum, "encoder cycle %d: ",  cmpTmr.encCycLen))
  {
   if (val != 0)
    cmpTmr.encCycLen = val;
  }
  if (query(&getnum, "internal cycle %d: ", cmpTmr.intCycLen))
  {
   if (val != 0)
    cmpTmr.intCycLen = val;
  }
  if (cmpTmr.preScale == 0)
   cmpTmr.preScale = 1;
  if (query(&getnum, "pre scaler %d: ", cmpTmr.preScale))
  {
   if (val != 0)
    cmpTmr.preScale = val;
  }
#if ENCODER_TEST
  if (testPulseMin != 0)
  {
   uint32_t tmrClkFreq = HAL_RCC_GetPCLK2Freq();
   uint64_t clksMin = (uint64_t) tmrClkFreq * 60;
   uint32_t clocksPulse = (uint32_t) (clksMin / testPulseMin);
   uint32_t clocksCycle = cmpTmr.encCycLen * clocksPulse;
   uint32_t clocksOutPulse = clocksCycle / cmpTmr.intCycLen;
   printf("\nclocksCycle %ld clocksPulse %ld clocksOutPulse %ld\n",
	 clocksCycle, clocksPulse, clocksOutPulse);
   printf("scaled clocksCycle %ld clocksPulse %ld clocksOutPulse %ld\n",
	  clocksCycle / cmpTmr.preScale, clocksPulse / cmpTmr.preScale,
	  clocksOutPulse / cmpTmr.preScale);
  }
#endif
  newline();
  encoderStart();
 }
 else if (ch == 'S')
 {
  printf("\nstop encoder\n");
  cmpTmr.stop = 1;
 }
#if DBG_COUNT
 else if (ch == 't')
 {
  printf("\nencCount %ld intCount %ld cycleCount %ld missedStart %d\n",
	 cmpTmr.encCount, cmpTmr.intCount, cmpTmr.cycleCount,
	 cmpTmr.missedStart);
 }
#endif
 else if (ch == '?')
 {
  newline();
// (clocksPerSec*60*ratioDenom)/(clocksCycle*encoderPulse)
  uint32_t tmrClkFreq = HAL_RCC_GetPCLK2Freq();
  uint64_t n = (uint64_t) tmrClkFreq * 60 * cmpTmr.encCycLen;
  uint64_t d = ((uint64_t) cmpTmr.cycleClocks * cmpTmr.preScale * syncEncoder);
  printf("n %lld d %lld\n", n, d);
  uint16_t rpm = (uint16_t) (n / d);
  printf("%d rpm\n", rpm);
 }
 
#if ENCODER_TEST

 else if (ch == 's')		/* start encoder */
 {
  newline();
  testEncoderStart();
 }
 else if (ch == 'r')		/* reverse encoder */
 {
  encRev ^= 1;
  printf("\nencoder reverse %d\n", encRev);
 }
 else if (ch == 'q')		/* stop encoder */
 {
  newline();
  testEncoderStop();
 }
 else if (ch == 'l')
 {
  if (query(&getnum, "encoder [%d] lines : ", encLines))
  {
   encLines = val;
   encPulse = encLines * 4;
   syncEncoder = encPulse;
  }
 }
 else if (ch == 'R')
 {
  newline();
  if (testPulseMin != 0)
  {
   printf("pulseMin %ld period %u usec encLines %d encPulse %d\n",
	  testPulseMin, (uint16_t) (USEC_MIN / testPulseMin),
	  encLines, encPulse);
   rpm = (unsigned int) (testPulseMin / encPulse);
  }
  if (query(&getnum, "rpm [%d]: ", rpm))
  {
   rpm = val;
  }
  /*
    usecPulse = (usec / min) / ((rev / min) * (pulse / rev))
    revMin = (usec / min) / ((usec / pulse) * (pulse / rev))
  */
  uint32_t pulseMin = (uint32_t) rpm * encPulse;
  if (testPulseMin != pulseMin)
  {
   uint16_t period = (uint16_t) (USEC_MIN / pulseMin);
   printf("\nrpm %d period %u usec encLines %d encPulse %d\n",
	  rpm, period, encLines, encPulse);

   uint32_t tmrClkFreq = HAL_RCC_GetPCLK2Freq();
   printf("tmrClkFreq %ld\n", tmrClkFreq);
   uint64_t clksMin = (uint64_t) tmrClkFreq * 60;
   uint32_t clocksPulse = (uint32_t) (clksMin / pulseMin);
   uint16_t preScaler = (uint16_t) (clocksPulse >> 16);

   if ((clocksPulse & 0xffff) != 0)
    preScaler += 1;
   uint16_t count = (uint16_t) (clocksPulse / preScaler);

   printf("clocksPulse %ld preScaler %d count %d\n",
	  clocksPulse, preScaler, count);

   preScaler -= 1;
   count -= 1;

   testPulseMin = pulseMin;
   testEncPreScaler = preScaler;
   testEncCount = count;

   testEncoderStop();
   testEncoderStart();
  }
 }

#endif

#if PIN_DISPLAY
 else if (ch == '>')
 {
  newline();
  pinDisplay();
 }
#endif

 else if (ch == '*')
 {
  unsigned int reg = 0;
  unsigned int mask = 0;
  unsigned int invert = 0;

  while (1)
  {
   if (query(&getnum, "\nreg "))
    reg = val;
   else
    break;

   if (query(&getnum, "mask "))
    mask = val;

   if (query(&getnum, "invert "))
    invert = val != 0;

   int set = (((reg & mask) != 0) ^ invert);
   int clr = (((reg & mask) == 0) ^ invert);

   printf("\nreg %02x mask %02x invert %02x set %d clr %d",
	  reg, mask, invert, set, clr);
  }
 }
 else if (ch == 'F')
 {
  if (query(&getnum, "IRQn: "))
  {
   HAL_NVIC_EnableIRQ((IRQn_Type) val);
  }
 }
 else if (ch == 'I')
 {
  ch = query("\nPort: ");
  unsigned int i;
  P_PORT_LIST p = portList;
  GPIO_TypeDef *port = 0;
  for (i = 0; i < sizeof(portList) / sizeof(T_PORT_LIST); i++)
  {
   if (ch == p->ch)
   {
    port = p->port;
    putBufChar(ch);
    break;
   }
  }
  if (port != 0)
  {
   if (query(&gethex, "\nmask: "))
   {
    port->ODR = val;
    printf("\n");
    gpioInfo(port);
   }
  }
 }

#if 1
 else if (ch == 'Q')		/* print peripheral info */
 {
  if (query(&getnum, " flag [0x%x]: ", lastFlags) == 0)
  {
   val = lastFlags;
  }
  else
  {
   lastFlags = val;
  }
  printf("\n");
  flushBuf();
  if (val & 0x01)
   tmrInfo(TIM1);
  if (val & 0x02)
   tmrInfo(TIM2);
  if (val & 0x04)
   tmrInfo(TIM3);
#ifdef TIM4
  if (val & 0x08)
   tmrInfo(TIM4);
#endif
#ifdef TIM5
  if (val & 0x10)
   tmrInfo(TIM5);
#endif
  if (val & 0x20)
  {
#ifdef TIM6
   tmrInfo(TIM6);
#endif
#ifdef TIM7
   tmrInfo(TIM7);
#endif
  }
#ifdef TIM8
  if (val & 0x40)
   tmrInfo(TIM8);
#endif
#ifdef TIM9
  if (val & 0x80)
   tmrInfo(TIM9);
#endif
#ifdef TIM10
  if (val & 0x100)
   tmrInfo(TIM10);
#endif
#ifdef TIM11
  if (val & 0x200)
   tmrInfo(TIM11);
#endif
#ifdef TIM12
  if (val & 0x400)
   tmrInfo(TIM12);
#endif
  if (val & 0x800)		/* exti */
   extiInfo();
  if (val & 0x01000)
   gpioInfo(GPIOA);
  if (val & 0x02000)
   gpioInfo(GPIOB);
  if (val & 0x04000)
   gpioInfo(GPIOC);
#ifdef GPIOD
  if (val & 0x08000)
   gpioInfo(GPIOD);
#endif
#ifdef GPIOE
  if (val & 0x10000)
   gpioInfo(GPIOE);
#endif
  if (val & 0x20000)
   i2cInfo(I2C1, "I2C1");
  if (val & 0x40000)
   usartInfo(DBGPORT, "DBG");
#if defined(REMPORT)
  if (val & 0x80000)
   usartInfo(REMPORT, "REM");
#endif  /* REMPORT */
  if (val & 0x100000)
   spiInfo(SPIn);
 }
#endif
 
#if 0 // DBGMSG
 if (ch == 'D')			/* dump dbg buffer */
 {
  newline();
  int empty = dbgemp;
  int i;
  for (i = 0; i < dbgcnt; i++)
  {
   P_DBGMSG p = &dbgdata[empty];
   float t = (float) p->time / 1000;
#if DBGMSG == 2
   printf("%8.3f %7s %6d\n", t, dMessageList[(int) p->dbg], (int) p->val);
#else
   printf("%8.3f %8s %6d\n", t, p->str, (int) p->val);
#endif
   empty++;
   if (empty >= MAXDBGMSG)
    empty = 0;
  }
  printf("z %d x %d\n", zLoc, xLoc);
 }
 else if (ch == 'E')		/* clear debug buffer */
 {
  memset(&dbgdata, 0, sizeof(dbgdata));
  dbgcnt = 0;
  dbgfil = 0;
  dbgemp = 0;
 }
#endif /* DBGMSG */

 else if (ch == 'd')		/* dump memory */
 {
  putBufChar(' ');
  if (gethex())
  {
   unsigned char *p;

   p = (unsigned char *) (int) val;
   if (gethex())
   {
    newline();
    prtbuf(p, val);
   }
  }
 }
 else if (ch == 'r')		/* read memory */
 {
  putBufChar(' ');
  if (gethex())
  {
   printf("%x", *(int16_t *) (int) val);
  }
 }
 else if (ch == 'w')
 {
  putBufChar(' ');
  if (gethex())
  {
   int16_t *p;
   p = (int16_t *) (int) val;
   printf(" %x ", *p);
   if (gethex())
   {
    *p = val;
   }
  }
 }
#if 0
 else if (ch == 'p')
 {
  if (query(&getnm, ' '))
  {
   print = val;
  }
 }
#endif

#if DBGTRK
 else if (ch == 'T')		/* print track buffer */
 {
  int16_t idx = trkidx;

  printf("\n");
  dbgTrk = false;
  if constexpr (DBGTRK1W)
  {
   int16_t i = sizeof(trkbuf) / sizeof(int16_t);
   while (--i >= 0)
   {
    int16_t tmp = (int16_t) (*(int16_t *) &trkbuf[idx]);
    printf("%4d %6d %4x\n", idx, tmp, (uint16_t) tmp);
    flushBuf();
    idx += 1;
    idx &= (TRKBUFSIZE - 1);
   }
  }
  if constexpr (DBGTRK1L)
  {
   int16_t i = sizeof(trkbuf) / sizeof(int);
   while (--i >= 0)
   {
    int *p = (int *) &trkbuf[idx];
    int tmp0 = *p;
    printf("%4d %10d\n", idx, tmp0);
    flushBuf();
    idx += 2;
    idx &= (TRKBUFSIZE - 1);
   }
  }
  if constexpr (DBGTRK2L)
  {
   int16_t i = sizeof(trkbuf) / (2 * sizeof(int));
   while (--i >= 0)
   {
    int *p = (int *) &trkbuf[idx];
    int tmp0 = *p;
    int tmp1 = *(p + 1);
    printf("%4d %10d %10d\n", idx, tmp0, tmp1);
    flushBuf();
    idx += 4;
    idx &= (TRKBUFSIZE - 1);
   }
  }
  if constexpr (DBGTRK2WL)
  {
   int16_t i = sizeof(trkbuf) / (2 * sizeof(int16_t) + sizeof(int));
   while (--i >= 0)
   {
    int16_t *p = (int16_t *) &trkbuf[idx];
    int16_t tmp0 = *p++;
    int16_t tmp1 = *p++;
    int tmp2 = *((int32_t *) p);
    printf("%4d %4d %5u %10d\n", idx, tmp0, (uint16_t) tmp1, tmp2);
    flushBuf();
    idx += 4;
    idx &= (TRKBUFSIZE - 1);
   }
  }
  dbgTrk = true;
 }
 else if (ch == 'B')		/* clear track buffer */
 {
  dbgTrk = false;
  memset(trkbuf, 0, sizeof(trkbuf)); /* clear track buffer */
  trkidx = 0;			/* reset index */
  dbgTrk = true;		/* start saving tracking info */
 }
#endif	/* DBGTRK */

 else if (ch == 'v')
 {
  tmrInfo(TIM3);
  tmrInfo(TIM4);
 }
}
#endif
