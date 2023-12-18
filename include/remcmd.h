#if !defined(REMCMD_INC)	// <-
#define REMCMD_INC

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

void remCmd(P_REM_ACTION);

void loadVal(P_REM_ACTION act);

char getNum(P_REM_ACTION act);
char getHex(P_REM_ACTION act);
void sndHex(void (*putCh)(char ch), unsigned char *p, int size);

#include "syncCmdList.h"
#include "syncParm.h"

#endif  /* REMCMD_INC */	// ->
