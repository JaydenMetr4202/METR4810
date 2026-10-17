/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "tim.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for goForwardTask */
osThreadId_t goForwardTaskHandle;
uint32_t goForwardTaskBuffer[ 128 ];
osStaticThreadDef_t goForwardTaskControlBlock;
const osThreadAttr_t goForwardTask_attributes = {
  .name = "goForwardTask",
  .stack_mem = &goForwardTaskBuffer[0],
  .stack_size = sizeof(goForwardTaskBuffer),
  .cb_mem = &goForwardTaskControlBlock,
  .cb_size = sizeof(goForwardTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for goBackwardTask */
osThreadId_t goBackwardTaskHandle;
uint32_t goBackwardTaskBuffer[ 128 ];
osStaticThreadDef_t goBackwardTaskControlBlock;
const osThreadAttr_t goBackwardTask_attributes = {
  .name = "goBackwardTask",
  .stack_mem = &goBackwardTaskBuffer[0],
  .stack_size = sizeof(goBackwardTaskBuffer),
  .cb_mem = &goBackwardTaskControlBlock,
  .cb_size = sizeof(goBackwardTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for goForwardSemaphore */
osSemaphoreId_t goForwardSemaphoreHandle;
osStaticSemaphoreDef_t goForwardSemaphoreControlBlock;
const osSemaphoreAttr_t goForwardSemaphore_attributes = {
  .name = "goForwardSemaphore",
  .cb_mem = &goForwardSemaphoreControlBlock,
  .cb_size = sizeof(goForwardSemaphoreControlBlock),
};
/* Definitions for goBackwardSemaphore */
osSemaphoreId_t goBackwardSemaphoreHandle;
osStaticSemaphoreDef_t goBackwardSemaphoreControlBlock;
const osSemaphoreAttr_t goBackwardSemaphore_attributes = {
  .name = "goBackwardSemaphore",
  .cb_mem = &goBackwardSemaphoreControlBlock,
  .cb_size = sizeof(goBackwardSemaphoreControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartGoForwardTask(void *argument);
void StartGoBackwardTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of goForwardSemaphore */
  goForwardSemaphoreHandle = osSemaphoreNew(1, 1, &goForwardSemaphore_attributes);

  /* creation of goBackwardSemaphore */
  goBackwardSemaphoreHandle = osSemaphoreNew(1, 1, &goBackwardSemaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of goForwardTask */
  goForwardTaskHandle = osThreadNew(StartGoForwardTask, NULL, &goForwardTask_attributes);

  /* creation of goBackwardTask */
  goBackwardTaskHandle = osThreadNew(StartGoBackwardTask, NULL, &goBackwardTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartGoForwardTask */
/**
  * @brief  Function implementing the goForwardTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartGoForwardTask */
void StartGoForwardTask(void *argument)
{
  /* USER CODE BEGIN StartGoForwardTask */

  /* Infinite loop */
  for(;;)
  {
    //xSemaphoreTake(goForwardSemaphoreHandle, portMAX_DELAY);

    //HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
    //HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);

    osDelay(1);
  }
  /* USER CODE END StartGoForwardTask */
}

/* USER CODE BEGIN Header_StartGoBackwardTask */
/**
* @brief Function implementing the goBackwardTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGoBackwardTask */
void StartGoBackwardTask(void *argument)
{
  /* USER CODE BEGIN StartGoBackwardTask */
  /* Infinite loop */
  for(;;)
  {
    //xSemaphoreTake(goBackwardSemaphoreHandle, portMAX_DELAY);
    osDelay(1);
  }
  /* USER CODE END StartGoBackwardTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

