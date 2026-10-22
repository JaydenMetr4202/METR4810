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
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "semphr.h"
#include <stdint.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define START_UP_TIME 100 // 0.1 seconds

#define DRILL_TIME 5000   // 5 seconds

#define CHANGE_DIRECTION_TIME 500   // 0.5 seconds

#define LS2 (1 << 1)

#define MIN_FREQUENCY 100
#define MAX_FREQUENCY 500
#define FREQUENCY_STEP 100

#define MAX_DIRECTION_CHANGES 1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

extern TIM_HandleTypeDef htim2;

uint16_t direction_changes = 0;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[ 128 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_mem = &defaultTaskBuffer[0],
  .stack_size = sizeof(defaultTaskBuffer),
  .cb_mem = &defaultTaskControlBlock,
  .cb_size = sizeof(defaultTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void start_motor(void);
void stop_motor(void);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */

  uint32_t msg = 0;

  osDelay(START_UP_TIME);

  // Set forward direction and enable stepper motor
  HAL_GPIO_WritePin(DIR1_GPIO_Port, DIR1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET);

  // Start drill
  HAL_GPIO_WritePin(DRILL_GPIO_Port, DRILL_Pin, GPIO_PIN_SET);

  start_motor();

  /* Infinite loop */
  for(;;)
  {
    // Wait for limit switches or 5 seconds to switch directions
    xTaskNotifyWait(0, 0xFFFFFFFF, &msg, DRILL_TIME);

    stop_motor();
    
    if (msg & LS2) {
      // Stop the motor, everything is done
      HAL_GPIO_WritePin(DRILL_GPIO_Port, DRILL_Pin, GPIO_PIN_RESET);
    } else {
      // Change direction
      HAL_GPIO_TogglePin(DIR1_GPIO_Port, DIR1_Pin);

      // Give the driver time to register the new direction
      osDelay(CHANGE_DIRECTION_TIME);

      start_motor();
    }
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void start_motor(void) {
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

  uint32_t frequency = MIN_FREQUENCY;

  while (frequency <= MAX_FREQUENCY) {
    uint32_t arr = (1000000 / frequency) - 1;

    __HAL_TIM_SET_AUTORELOAD(&htim2, arr);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (arr + 1) / 2);

    osDelay(100);

    frequency += FREQUENCY_STEP;
  }
}

void stop_motor(void) {
  uint32_t frequency = MAX_FREQUENCY;

  while (frequency >= MIN_FREQUENCY) {
    uint32_t arr = (1000000 / frequency) - 1;

    __HAL_TIM_SET_AUTORELOAD(&htim2, arr);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (arr + 1) / 2);

    osDelay(100);

    frequency -= FREQUENCY_STEP;
  }

  HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
}

/* USER CODE END Application */

