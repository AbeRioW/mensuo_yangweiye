/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "RC522.h"
#include "stdio.h"
#include "usart.h"
#include "AS608.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  OLED_ShowString(0,0,(uint8_t*)"Initializing...",8,1);
  OLED_Refresh();
  HAL_Delay(1000);
  
  PCD_Init(); // 初始化RC522
  AS608_Init(); // 初始化AS608
  
  // AS608握手
  OLED_Clear();
  OLED_ShowString(0,0,(uint8_t*)"Checking AS608...",8,1);
  OLED_Refresh();
  
  uint32_t as608_addr = 0xffffffff;
  uint8_t handshakeResult = AS608_HandShake(&as608_addr);
  
  if (handshakeResult == AS608_ACK_OK)
  {
    // 握手成功
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)"AS608 Handshake", 8, 1);
    OLED_ShowString(0, 8, (uint8_t*)"Success!", 8, 1);
    OLED_Refresh();
    HAL_Delay(2000);
  }
  else
  {
    // 握手失败
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)"AS608 Handshake", 8, 1);
    OLED_ShowString(0, 8, (uint8_t*)"Failed!", 8, 1);
    OLED_Refresh();
    HAL_Delay(2000);
  }
  
  OLED_Clear();
  OLED_ShowString(0, 0, (uint8_t*)"System Ready", 8, 1);
  OLED_ShowString(0, 8, (uint8_t*)"Scan NFC or Finger", 8, 1);
  OLED_Refresh();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // 保留RC522功能
    uint8_t status; // 状态变量
    uint8_t cardType; // 卡片类型
    uint8_t cardID[4]; // 卡片ID
    char idString[16]; // ID字符串
    
    // 寻找卡片
    status = PCD_Request(PICC_REQIDL, &cardType);
    if (status == PCD_OK)
    {
      // 防碰撞，获取卡片ID
      status = PCD_Anticoll(cardID);
      if (status == PCD_OK)
      {
        // 格式化ID字符串
        sprintf(idString, "ID: %02X %02X %02X %02X", cardID[0], cardID[1], cardID[2], cardID[3]);
        // 显示在OLED上
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"Card Detected", 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)idString, 8, 1);
        OLED_Refresh();
        HAL_Delay(1000);
        
        // 显示系统就绪
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"System Ready", 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)"Scan NFC or Finger", 8, 1);
        OLED_Refresh();
      }
    }
    
    // 指纹检测
    static uint32_t lastFingerCheckTime = 0;
    uint32_t currentTime = HAL_GetTick();
    
    if (currentTime - lastFingerCheckTime >= 500)
    {
      lastFingerCheckTime = currentTime;
      
      uint16_t fingerID = 0;
      uint16_t score = 0;
      uint8_t fingerResult = AS608_VerifyFinger(&fingerID, &score);
      
      if (fingerResult == AS608_ACK_OK)
      {
        // 指纹验证成功
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"Finger Verified", 8, 1);
        char idStr[16];
        sprintf(idStr, "ID: %d", fingerID);
        OLED_ShowString(0, 8, (uint8_t*)idStr, 8, 1);
        OLED_Refresh();
        HAL_Delay(1000);
        
        // 显示系统就绪
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"System Ready", 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)"Scan NFC or Finger", 8, 1);
        OLED_Refresh();
      }
    }
    
    HAL_Delay(100);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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

  /** Initializes the CPU, AHB and APB buses clocks
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
