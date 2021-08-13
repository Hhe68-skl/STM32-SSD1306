/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */
uint8_t OLED_Device_Address = 0x78;
uint8_t Frame[1024];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void OLED_WriteCmd(uint8_t);
void OLED_WriteData(uint8_t);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_SetPos(uint8_t, uint8_t);
void OLED_ON(void);
void OLED_OFF(void);
//Split Bottom/Top
void Put_Frame(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void OLED_WriteCmd(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&hi2c1, OLED_Device_Address, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 1000);
}

void OLED_WriteData(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1, OLED_Device_Address, 0x40, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
}

void OLED_Init(void)
{
	OLED_WriteCmd(0xAE);	//Disable screen
	OLED_WriteCmd(0xA4);	//Set normal display(using dot data)
	OLED_WriteCmd(0xA6);	//Set normal output
	OLED_WriteCmd(0x20);	//Set addressing mode		-\[0]
	OLED_WriteCmd(0x02);	//Set addressing mode  Page -\[1]
	OLED_WriteCmd(0x81);	//Set contrast		-\[0]
	OLED_WriteCmd(0x7F);	//Set contrast	7F  -/[1]
	OLED_WriteCmd(0xD3);	//Set offset(up)		-\[0]
	OLED_WriteCmd(0x00);	//Set offset(up)	0   -/[1]
	OLED_WriteCmd(0x2E);	//Stop roll
	OLED_WriteCmd(0xA8);	//Set MUX num(display row)	-\[0]
	OLED_WriteCmd(0x3F);	//Set MUX num(display row)	-/[1]
	OLED_WriteCmd(0xA1);	//Set Left-Right reverse off
	OLED_WriteCmd(0xC8);	//Set Up-Down reverse off
	OLED_WriteCmd(0xD5);	//Set clock		-\[0]
	OLED_WriteCmd(0x80);	//Set clock		-/[1]
	OLED_WriteCmd(0x8D);	//Start power	-\[0]
	OLED_WriteCmd(0x14);	//Start power	-/[1]
	OLED_WriteCmd(0xAF);	//Enable screen
	
	OLED_Clear();
}

void OLED_Clear(void)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		OLED_WriteCmd(0xb0+i);
		OLED_WriteCmd(0x00);
		OLED_WriteCmd(0x10);
		for(uint8_t j = 0; j < 128; j++)
		{
			OLED_WriteData(0);
		}
	}
}

void OLED_SetPos(uint8_t x, uint8_t y)
{
	OLED_WriteCmd(0xb0 + y);
	OLED_WriteCmd((x & 0xF0) >> 4 | 0x10);
	OLED_WriteCmd((x & 0x0F));
}

void OLED_ON(void)
{
	OLED_WriteCmd(0x8D);
	OLED_WriteCmd(0x14);
	OLED_WriteCmd(0xAF);
}

void OLED_OFF(void)
{
	OLED_WriteCmd(0x8D);
	OLED_WriteCmd(0x10);
	OLED_WriteCmd(0xAE);
}

void Put_Frame(void)
{
	OLED_WriteCmd(0x20);
	OLED_WriteCmd(0x00);
	
	OLED_WriteCmd(0x21);
	OLED_WriteCmd(0x00);
	OLED_WriteCmd(0x7F);

	OLED_WriteCmd(0x22);
	OLED_WriteCmd(0x00);
	OLED_WriteCmd(0x07);
	for(uint16_t i = 0; i < 1024; i++)
	{
		OLED_WriteData(Frame[i]);
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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  HAL_UART_Receive_IT(&huart1, Frame, 1024);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
