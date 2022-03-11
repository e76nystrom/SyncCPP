#define WD_ENA 1

#define PIN_DISPLAY 0
#define ENCODER_TEST 0
#define USEC_MIN (60L * 1000000L)

#define DBGPORT USART1
//#define REMPORT USART3

//#define remoteISR(x) USART3_IRQHandler(x)
//#define REMOTE_IRQn USART3_IRQn

#define encoderISR(x) EXTI15_10_IRQHandler(x)

#define SPIn SPI1
#define spiISR(x) SPI1_IRQHandler(x)
#define SPI_IRQn SPI1_IRQn

#define SPI_ISR
#define REM_ISR 1		/* remote port using isr */

#include "timers.h"
#include "pinDef.h"
