#ifdef __STM32F1xx_HAL_H
#if !defined(__TIMERS_H)
#define __TIMERS_H

/*
board     sync
proc      STM32F103
encTmr    2
intTmr    3
intTmrPwm 0
cmpTmr    4
encTmrIsr TIM2
intTmrIsr TIM3
cmpTmrIsr TIM4
*/

#define ENC_TMR2
#define INT_TMR3
#define INT_TMR_PWM0
#define CMP_TMR4

/* encTmr timer 2 */

#define ENC_TIMER 2
#define ENC_TMR TIM2

#define encTmrISR(x) TIM2_IRQHandler(x)

inline void encTmrInit() { \
	__HAL_RCC_TIM2_CLK_ENABLE(); \
	TIM2->CR1 |= TIM_CR1_DIR; \
	TIM2->CR1 &= ~TIM_CR1_CEN;}

inline void     encTmrClrIE()         {TIM2->DIER &= ~TIM_IT_UPDATE;}
inline void     encTmrSetIE()         {TIM2->DIER |= TIM_IT_UPDATE;}
inline uint16_t encTmrTstIE()         \
	{return((TIM2->DIER & TIM_IT_UPDATE) != 0);}
inline uint16_t encTmrIF()            \
	{return((TIM2->SR & TIM_FLAG_UPDATE) != 0);}
inline void     encTmrClrIF()         {TIM2->SR = ~TIM_FLAG_UPDATE;}
inline void     encTmrStart()         {TIM2->CR1 |= TIM_CR1_CEN;}
inline void     encTmrPulse()         {TIM2->CR1 |= (TIM_CR1_OPM | TIM_CR1_CEN);}
inline void     encTmrStop()          \
	{TIM2->CR1 &= ~(TIM_CR1_OPM | TIM_CR1_CEN);}
inline void     encTmrScl(uint16_t y) {TIM2->PSC = (y);}
inline uint16_t encTmrRead()          {return(TIM2->CNT);}
inline void     encTmrCntClr()        {TIM2->CNT = 0;}
inline void     encTmrCnt(uint16_t x) {TIM2->CNT = (x);}
inline void     encTmrMax(uint16_t x) {TIM2->ARR = ((x) - 1);}
inline void     encTmrSet(uint16_t x) {TIM2->ARR = (x);}
inline uint16_t encTmrMaxRead()       {return(TIM2->ARR);}

/* intTmr timer 3 */

#define INT_TIMER 3
#define INT_TMR TIM3

#define intTmrISR(x) TIM3_IRQHandler(x)

inline void intTmrInit() { \
	__HAL_RCC_TIM3_CLK_ENABLE(); \
	TIM3->CR1 |= TIM_CR1_DIR; \
	TIM3->CR1 &= ~TIM_CR1_CEN;}

inline void     intTmrClrIE()         {TIM3->DIER &= ~TIM_IT_UPDATE;}
inline void     intTmrSetIE()         {TIM3->DIER |= TIM_IT_UPDATE;}
inline uint16_t intTmrTstIE()         \
	{return((TIM3->DIER & TIM_IT_UPDATE) != 0);}
inline uint16_t intTmrIF()            \
	{return((TIM3->SR & TIM_FLAG_UPDATE) != 0);}
inline void     intTmrClrIF()         {TIM3->SR = ~TIM_FLAG_UPDATE;}
inline void     intTmrStart()         {TIM3->CR1 |= TIM_CR1_CEN;}
inline void     intTmrPulse()         {TIM3->CR1 |= (TIM_CR1_OPM | TIM_CR1_CEN);}
inline void     intTmrStop()          \
	{TIM3->CR1 &= ~(TIM_CR1_OPM | TIM_CR1_CEN);}
inline void     intTmrScl(uint16_t y) {TIM3->PSC = (y);}
inline uint16_t intTmrRead()          {return(TIM3->CNT);}
inline void     intTmrCntClr()        {TIM3->CNT = 0;}
inline void     intTmrCnt(uint16_t x) {TIM3->CNT = (x);}
inline void     intTmrMax(uint16_t x) {TIM3->ARR = ((x) - 1);}
inline void     intTmrSet(uint16_t x) {TIM3->ARR = (x);}
inline uint16_t intTmrMaxRead()       {return(TIM3->ARR);}

/* cmpTmr timer 4 */

#define CMP_TIMER 4
#define CMP_TMR TIM4

#define cmpTmrISR(x) TIM4_IRQHandler(x)

inline void cmpTmrInit() { \
	__HAL_RCC_TIM4_CLK_ENABLE(); \
	TIM4->CR1 |= TIM_CR1_DIR; \
	TIM4->CR1 &= ~TIM_CR1_CEN;}

inline void     cmpTmrClrIE()         {TIM4->DIER &= ~TIM_IT_UPDATE;}
inline void     cmpTmrSetIE()         {TIM4->DIER |= TIM_IT_UPDATE;}
inline uint16_t cmpTmrTstIE()         \
	{return((TIM4->DIER & TIM_IT_UPDATE) != 0);}
inline uint16_t cmpTmrIF()            \
	{return((TIM4->SR & TIM_FLAG_UPDATE) != 0);}
inline void     cmpTmrClrIF()         {TIM4->SR = ~TIM_FLAG_UPDATE;}
inline void     cmpTmrStart()         {TIM4->CR1 |= TIM_CR1_CEN;}
inline void     cmpTmrPulse()         {TIM4->CR1 |= (TIM_CR1_OPM | TIM_CR1_CEN);}
inline void     cmpTmrStop()          \
	{TIM4->CR1 &= ~(TIM_CR1_OPM | TIM_CR1_CEN);}
inline void     cmpTmrScl(uint16_t y) {TIM4->PSC = (y);}
inline uint16_t cmpTmrRead()          {return(TIM4->CNT);}
inline void     cmpTmrCntClr()        {TIM4->CNT = 0;}
inline void     cmpTmrCnt(uint16_t x) {TIM4->CNT = (x);}
inline void     cmpTmrMax(uint16_t x) {TIM4->ARR = ((x) - 1);}
inline void     cmpTmrSet(uint16_t x) {TIM4->ARR = (x);}
inline uint16_t cmpTmrMaxRead()       {return(TIM4->ARR);}

inline void     cmpTmrCap1EnaSet() {TIM4->CCER |= TIM_CCER_CC1E;}
inline void     cmpTmrCap1SetIE()  {TIM4->DIER |= TIM_DIER_CC1IE;}
inline void     cmpTmrCap1ClrIE()  {TIM4->DIER &= ~TIM_DIER_CC1IE;}
inline uint16_t cmpTmrCap1IF()     {return((TIM4->SR & TIM_SR_CC1IF) != 0);}
inline void     cmpTmrCap1ClrIF()  {TIM4->SR &= ~TIM_SR_CC1IF;}
inline uint16_t cmpTmrCap1()       {return(TIM4->CCR1);}
inline void     cmpTmrCap2EnaSet() {TIM4->CCER |= TIM_CCER_CC2E;}
inline void     cmpTmrCap2SetIE()  {TIM4->DIER |= TIM_DIER_CC2IE;}
inline void     cmpTmrCap2ClrIE()  {TIM4->DIER &= ~TIM_DIER_CC2IE;}
inline uint16_t cmpTmrCap2IF()     {return((TIM4->SR & TIM_SR_CC1IF) != 0);}
inline void     cmpTmrCap2ClrIF()  {TIM4->SR &= ~TIM_SR_CC1IF;}
inline uint16_t cmpTmrCap2()       {return(TIM4->CCR2);}
inline void     cmpTmrOCP1Clr()    {TIM4->SR &= ~TIM_SR_CC1OF;}
inline void     cmpTmrOCP2Clr()    {TIM4->SR &= ~TIM_SR_CC2OF;}

#endif /* __TIMERS_H */
#endif /* __STM32F4xx_HAL_H */
