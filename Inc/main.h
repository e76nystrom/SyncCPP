/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Led_Pin GPIO_PIN_13
#define Led_GPIO_Port GPIOC
#define Index_Pin GPIO_PIN_0
#define Index_GPIO_Port GPIOA
#define ExtI_Pin GPIO_PIN_1
#define ExtI_GPIO_Port GPIOA
#define ZFlag_Pin GPIO_PIN_2
#define ZFlag_GPIO_Port GPIOA
#define XFlag_Pin GPIO_PIN_3
#define XFlag_GPIO_Port GPIOA
#define Dbg5_Pin GPIO_PIN_0
#define Dbg5_GPIO_Port GPIOB
#define Dbg4_Pin GPIO_PIN_1
#define Dbg4_GPIO_Port GPIOB
#define Dbg3_Pin GPIO_PIN_12
#define Dbg3_GPIO_Port GPIOB
#define Dbg2_Pin GPIO_PIN_13
#define Dbg2_GPIO_Port GPIOB
#define A_Pin GPIO_PIN_14
#define A_GPIO_Port GPIOB
#define A_EXTI_IRQn EXTI15_10_IRQn
#define B_Pin GPIO_PIN_15
#define B_GPIO_Port GPIOB
#define B_EXTI_IRQn EXTI15_10_IRQn
#define Dbg1_Pin GPIO_PIN_8
#define Dbg1_GPIO_Port GPIOA
#define Dbg0_Pin GPIO_PIN_15
#define Dbg0_GPIO_Port GPIOA
#define SyncOut_Pin GPIO_PIN_4
#define SyncOut_GPIO_Port GPIOB
#define IndexTest_Pin GPIO_PIN_5
#define IndexTest_GPIO_Port GPIOB
#define Encoder_Pin GPIO_PIN_7
#define Encoder_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
