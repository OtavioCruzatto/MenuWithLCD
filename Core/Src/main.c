/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcdController.h"
#include "pushButton.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim9;

/* USER CODE BEGIN PV */
LcdController lcd;

PushButton upPushButton;
PushButton downPushButton;
PushButton enterPushButton;
PushButton escPushButton;

PressState pushButtonState = UNPRESSED;

uint8_t screen_0[17] = "++++++++++++++++";
uint8_t screen_1[17] = "----------------";
uint8_t screen_2[17] = "****************";
uint8_t screen_3[17] = "////////////////";
uint8_t screen_4[17] = "================";
uint8_t currentScreen = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM9_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
 * Timer interrupt handling (10us)
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim9)
	{
		if(lcdControllerGetDelayStatus(&lcd) == TRUE)
		{
			lcdControllerIncrementDelayCounter(&lcd);
		}

		if(lcdControllerGetEnableDelayStatus(&lcd) == TRUE)
		{
			lcdControllerIncrementEnableDelayCounter(&lcd);
		}
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM9_Init();
  /* USER CODE BEGIN 2 */

  pushButtonInit(&upPushButton, BTN_UP_GPIO_Port, BTN_UP_Pin, NORMALLY_LOW, 400, 3000);
  pushButtonInit(&downPushButton, BTN_DOWN_GPIO_Port, BTN_DOWN_Pin, NORMALLY_LOW, 400, 3000);
  pushButtonInit(&enterPushButton, BTN_ENTER_GPIO_Port, BTN_ENTER_Pin, NORMALLY_LOW, 3000, 15000);
  pushButtonInit(&escPushButton, BTN_ESC_GPIO_Port, BTN_ESC_Pin, NORMALLY_LOW, 3000, 15000);

  lcdControllerInitStruct(&lcd,
		  LCD_RS_GPIO_Port, LCD_RS_Pin,
		  LCD_RW_GPIO_Port, LCD_RW_Pin,
		  LCD_EN_GPIO_Port, LCD_EN_Pin,
		  LCD_DB4_GPIO_Port, LCD_DB4_Pin,
		  LCD_DB5_GPIO_Port, LCD_DB5_Pin,
		  LCD_DB6_GPIO_Port, LCD_DB6_Pin,
		  LCD_DB7_GPIO_Port, LCD_DB7_Pin,
		  LCD_ON_OFF_GPIO_Port, LCD_ON_OFF_Pin,
		  &htim2, TIM_CHANNEL_1);

  HAL_Delay(250);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(&htim9);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if(lcdControllerIsInitialized(&lcd) == FALSE)
	  {
		  lcdControllerInit(&lcd);
		  continue;
	  }

	  pushButtonState = pushButtonReadSingleAndContinuousPress(&upPushButton);
	  if(pushButtonState == PRESSED)
	  {
		  currentScreen += 1;
		  if(currentScreen > 4)
		  {
			  currentScreen = 4;
		  }
	  }

	  pushButtonState = pushButtonReadSingleAndContinuousPress(&downPushButton);
	  if(pushButtonState == PRESSED)
	  {
		  currentScreen -= 1;
		  if(currentScreen < 0)
		  {
			  currentScreen = 0;
		  }
	  }

	  switch (currentScreen)
	  {
		  case 0:
			  lcdControllerSetString(&lcd, ROW_0, screen_0);
			  lcdControllerSetString(&lcd, ROW_1, screen_0);
			  lcdControllerSetString(&lcd, ROW_2, screen_0);
			  lcdControllerSetString(&lcd, ROW_3, screen_0);
			  break;

		  case 1:
			  lcdControllerSetString(&lcd, ROW_0, screen_1);
			  lcdControllerSetString(&lcd, ROW_1, screen_1);
			  lcdControllerSetString(&lcd, ROW_2, screen_1);
			  lcdControllerSetString(&lcd, ROW_3, screen_1);
			  break;

		  case 2:
			  lcdControllerSetString(&lcd, ROW_0, screen_2);
			  lcdControllerSetString(&lcd, ROW_1, screen_2);
			  lcdControllerSetString(&lcd, ROW_2, screen_2);
			  lcdControllerSetString(&lcd, ROW_3, screen_2);
			  break;

		  case 3:
			  lcdControllerSetString(&lcd, ROW_0, screen_3);
			  lcdControllerSetString(&lcd, ROW_1, screen_3);
			  lcdControllerSetString(&lcd, ROW_2, screen_3);
			  lcdControllerSetString(&lcd, ROW_3, screen_3);
			  break;

		  case 4:
			  lcdControllerSetString(&lcd, ROW_0, screen_4);
			  lcdControllerSetString(&lcd, ROW_1, screen_4);
			  lcdControllerSetString(&lcd, ROW_2, screen_4);
			  lcdControllerSetString(&lcd, ROW_3, screen_4);
			  break;

		  default:
			  currentScreen = 0;
			  break;
	  }

	  lcdControllerUpdate(&lcd);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 40-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 50;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 5-1;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 80;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD_ON_OFF_Pin|LCD_RW_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_DB5_Pin|LCD_DB6_Pin|LCD_DB7_Pin|LCD_RS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_DB4_Pin|LCD_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_ON_OFF_Pin LCD_RW_Pin */
  GPIO_InitStruct.Pin = LCD_ON_OFF_Pin|LCD_RW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN_ENTER_Pin BTN_ESC_Pin */
  GPIO_InitStruct.Pin = BTN_ENTER_Pin|BTN_ESC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_DB5_Pin LCD_DB6_Pin LCD_DB7_Pin LCD_RS_Pin */
  GPIO_InitStruct.Pin = LCD_DB5_Pin|LCD_DB6_Pin|LCD_DB7_Pin|LCD_RS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_DB4_Pin LCD_EN_Pin */
  GPIO_InitStruct.Pin = LCD_DB4_Pin|LCD_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN_UP_Pin BTN_DOWN_Pin */
  GPIO_InitStruct.Pin = BTN_UP_Pin|BTN_DOWN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
