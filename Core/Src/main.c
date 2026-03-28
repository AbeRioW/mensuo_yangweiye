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
#include "AS608.h"
#include "stdio.h"
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
// 蓝牙状态标志位
volatile uint8_t ble_connected = 0;
volatile uint8_t ble_disconnected = 0;
volatile uint8_t add_nfc_flag = 0;
volatile uint8_t del_nfc_flag = 0;
volatile uint8_t add_finger_flag = 0;
volatile uint8_t del_finger_flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void ReadNFCCardsFromFlash(void);
uint8_t IsNFCCardRegistered(uint8_t *cardID);
void SaveNFCCardToFlash(uint8_t *cardID);
void AddNFCCardMode(void);
void DelNFCCardMode(void);
void DelNFCCardFromFlash(uint8_t *cardID);
void AddFingerMode(void);
void DelFingerMode(void);
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  OLED_ShowString(0,0,(uint8_t*)"Initializing...",8,1);
  OLED_Refresh();
  HAL_Delay(1000);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET); // LAY1连接到PB13
	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); // LAY1连接到PB13
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
    
    // 进入主页面
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)"Wait for NFC card", 8, 1);
    OLED_ShowString(0, 8, (uint8_t*)"Wait for fingerprint", 8, 1);
    OLED_Refresh();
  }
  else
  {
    // 握手失败
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)"AS608 Handshake", 8, 1);
    OLED_ShowString(0, 8, (uint8_t*)"Failed!", 8, 1);
    OLED_Refresh();
    HAL_Delay(2000);
		while(1);
  }
  
  // 读取已保存的NFC卡数据
  ReadNFCCardsFromFlash();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // 处理蓝牙连接标志
    if (ble_connected)
    {
      // 显示蓝牙连接成功
      OLED_Clear();
      OLED_ShowString(0, 0, (uint8_t*)"Wait for NFC card", 8, 1);
      OLED_ShowString(0, 8, (uint8_t*)"Wait for fingerprint", 8, 1);
      OLED_ShowString(0, 16, (uint8_t*)"BLE Connected", 8, 1);
      OLED_Refresh();
      HAL_Delay(2000);
      
      // 清除标志位
      ble_connected = 0;
    }
    
    // 处理蓝牙断开标志
    if (ble_disconnected)
    {
      // 显示蓝牙断开
      OLED_Clear();
      OLED_ShowString(0, 0, (uint8_t*)"Wait for NFC card", 8, 1);
      OLED_ShowString(0, 8, (uint8_t*)"Wait for fingerprint", 8, 1);
      OLED_ShowString(0, 16, (uint8_t*)"BLE Disconnected", 8, 1);
      OLED_Refresh();
      HAL_Delay(2000);
      
      // 清除标志位
      ble_disconnected = 0;
    }
    
    // 处理添加NFC卡标志
    if (add_nfc_flag)
    {
      // 显示添加NFC卡界面
      OLED_Clear();
      OLED_ShowString(0, 0, (uint8_t*)"Add NFC Card", 8, 1);
      OLED_ShowString(0, 8, (uint8_t*)"Please approach card", 8, 1);
      OLED_Refresh();
      
      // 清除标志位
      add_nfc_flag = 0;
      
      // 进入添加NFC卡模式
      AddNFCCardMode();
    }
    
    // 处理删除NFC卡标志
    if (del_nfc_flag)
    {
      // 显示删除NFC卡界面
      OLED_Clear();
      OLED_ShowString(0, 0, (uint8_t*)"Delete NFC Card", 8, 1);
      OLED_ShowString(0, 8, (uint8_t*)"Please approach card", 8, 1);
      OLED_Refresh();
      
      // 清除标志位
      del_nfc_flag = 0;
      
      // 进入删除NFC卡模式
      DelNFCCardMode();
    }
    
    // 处理添加指纹标志
    if (add_finger_flag)
    {
      // 显示添加指纹界面
      OLED_Clear();
      OLED_ShowString(0, 0, (uint8_t*)"Add Fingerprint", 8, 1);
      OLED_ShowString(0, 8, (uint8_t*)"Please place finger", 8, 1);
      OLED_Refresh();
      
      // 清除标志位
      add_finger_flag = 0;
      
      // 进入添加指纹模式
      AddFingerMode();
    }
    
    // 处理删除指纹标志
    if (del_finger_flag)
    {
      // 显示删除指纹界面
      OLED_Clear();
      OLED_ShowString(0, 0, (uint8_t*)"Delete Fingerprint", 8, 1);
      OLED_ShowString(0, 8, (uint8_t*)"Please place finger", 8, 1);
      OLED_Refresh();
      
      // 清除标志位
      del_finger_flag = 0;
      
      // 进入删除指纹模式
      DelFingerMode();
    }
    
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
        
        // 检查是否已注册
        if (IsNFCCardRegistered(cardID))
        {
          OLED_ShowString(0, 16, (uint8_t*)"Access Granted", 8, 1);
          
          // 拉高LAY1使得继电器工作
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); // LAY1连接到PB13
          
          OLED_Refresh();
          HAL_Delay(3000); // 3秒后关闭
          
          // 关闭继电器
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
        }
        else
        {
          OLED_ShowString(0, 16, (uint8_t*)"Access Denied", 8, 1);
          
          // 拉低beep工作
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET); // beep连接到PB15
          
          OLED_Refresh();
          HAL_Delay(3000); // 3秒后关闭
          
          // 关闭蜂鸣器
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
        }
        
        // 显示主页面
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"Wait for NFC card", 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)"Wait for fingerprint", 8, 1);
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
        
        // 检查指纹是否已保存
        // 这里假设AS608_VerifyFinger返回ACK_OK表示指纹已保存
        OLED_ShowString(0, 16, (uint8_t*)"Access Granted", 8, 1);
        
        // 拉高LAY1使得继电器工作
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); // LAY1连接到PB13
        
        OLED_Refresh();
        HAL_Delay(3000); // 3秒后关闭
        
        // 关闭继电器
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
        
        // 显示主页面
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"Wait for NFC card", 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)"Wait for fingerprint", 8, 1);
        OLED_Refresh();
      }
      else if (fingerResult == AS608_ACK_NO_FINGER)
      {
        // 没有检测到指纹，不做处理
      }
      else
      {
        // 指纹未保存
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"Finger Not Found", 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)"Access Denied", 8, 1);
        
        // 拉低beep工作
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET); // beep连接到PB15
        
        OLED_Refresh();
        HAL_Delay(3000); // 3秒后关闭
        
        // 关闭蜂鸣器
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
        
        // 显示主页面
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"Wait for NFC card", 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)"Wait for fingerprint", 8, 1);
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

// NFC卡ID存储相关定义
#define MAX_NFC_CARDS 10
#define NFC_CARD_SIZE 4 // 每个NFC卡ID为4字节

// 存储NFC卡ID的数组
uint8_t nfcCards[MAX_NFC_CARDS][NFC_CARD_SIZE];
uint8_t nfcCardCount = 0;

// 从Flash读取NFC卡数据
void ReadNFCCardsFromFlash(void)
{
    // 这里需要实现从Flash读取数据的逻辑
    // 暂时使用默认值
    nfcCardCount = 0;
}

// 检查NFC卡是否已注册
uint8_t IsNFCCardRegistered(uint8_t *cardID)
{
    for (uint8_t i = 0; i < nfcCardCount; i++)
    {
        if (memcmp(nfcCards[i], cardID, NFC_CARD_SIZE) == 0)
        {
            return 1; // 卡已注册
        }
    }
    return 0; // 卡未注册
}

// 保存NFC卡ID到Flash
void SaveNFCCardToFlash(uint8_t *cardID)
{
    if (nfcCardCount < MAX_NFC_CARDS)
    {
        // 复制卡ID到数组
        memcpy(nfcCards[nfcCardCount], cardID, NFC_CARD_SIZE);
        nfcCardCount++;
        
        // 这里需要实现将数据写入Flash的逻辑
    }
}

// 从Flash删除NFC卡ID
void DelNFCCardFromFlash(uint8_t *cardID)
{
    for (uint8_t i = 0; i < nfcCardCount; i++)
    {
        if (memcmp(nfcCards[i], cardID, NFC_CARD_SIZE) == 0)
        {
            // 找到要删除的卡，将后面的卡向前移动
            for (uint8_t j = i; j < nfcCardCount - 1; j++)
            {
                memcpy(nfcCards[j], nfcCards[j+1], NFC_CARD_SIZE);
            }
            nfcCardCount--;
            
            // 这里需要实现将数据写入Flash的逻辑
            break;
        }
    }
}

// 添加NFC卡模式
void AddNFCCardMode(void)
{
    uint8_t status;
    uint8_t cardType;
    uint8_t cardID[4];
    char idString[16];
    
    // 等待用户靠近NFC卡
    while (1)
    {
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
                
                // 检查卡是否已注册
                if (IsNFCCardRegistered(cardID))
                {
                    // 显示已注册
                    OLED_Clear();
                    OLED_ShowString(0, 0, (uint8_t*)"Card Already Registered", 8, 1);
                    OLED_ShowString(0, 8, (uint8_t*)idString, 8, 1);
                    OLED_Refresh();
                    HAL_Delay(2000);
                }
                else
                {
                    // 保存卡ID到Flash
                    SaveNFCCardToFlash(cardID);
                    
                    // 显示注册成功
                    OLED_Clear();
                    OLED_ShowString(0, 0, (uint8_t*)"Card Registered", 8, 1);
                    OLED_ShowString(0, 8, (uint8_t*)idString, 8, 1);
                    OLED_Refresh();
                    HAL_Delay(2000);
                }
                
                // 显示主页面
                OLED_Clear();
                OLED_ShowString(0, 0, (uint8_t*)"Wait for NFC card", 8, 1);
                OLED_ShowString(0, 8, (uint8_t*)"Wait for fingerprint", 8, 1);
                OLED_Refresh();
                break;
            }
        }
        
        HAL_Delay(100);
    }
}

// 删除NFC卡模式
void DelNFCCardMode(void)
{
    uint8_t status;
    uint8_t cardType;
    uint8_t cardID[4];
    char idString[16];
    
    // 等待用户靠近NFC卡
    while (1)
    {
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
                
                // 检查卡是否已注册
                if (IsNFCCardRegistered(cardID))
                {
                    // 从Flash删除卡ID
                    DelNFCCardFromFlash(cardID);
                    
                    // 显示删除成功
                    OLED_Clear();
                    OLED_ShowString(0, 0, (uint8_t*)"Card Deleted", 8, 1);
                    OLED_ShowString(0, 8, (uint8_t*)idString, 8, 1);
                    OLED_Refresh();
                    HAL_Delay(2000);
                }
                else
                {
                    // 显示卡片未注册
                    OLED_Clear();
                    OLED_ShowString(0, 0, (uint8_t*)"Card Not Registered", 8, 1);
                    OLED_ShowString(0, 8, (uint8_t*)idString, 8, 1);
                    OLED_Refresh();
                    HAL_Delay(2000);
                }
                
                // 显示主页面
                OLED_Clear();
                OLED_ShowString(0, 0, (uint8_t*)"Wait for NFC card", 8, 1);
                OLED_ShowString(0, 8, (uint8_t*)"Wait for fingerprint", 8, 1);
                OLED_Refresh();
                break;
            }
        }
        
        HAL_Delay(100);
    }
}

// 添加指纹模式
void AddFingerMode(void)
{
    uint16_t id = 0;
    uint8_t status;
    uint16_t pageID, score;
    
    // 查找可用的指纹ID
    for (id = 0; id < 200; id++)
    {
        // 尝试搜索当前ID，如果没有找到则认为该ID可用
        status = AS608_Search(1, id, 1, &pageID, &score);
        if (status != AS608_ACK_OK)
        {
            break; // 找到可用的ID
        }
    }
    
    if (id >= 200)
    {
        // 没有可用的ID
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"No space", 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)"for new fingerprint", 8, 1);
        OLED_Refresh();
        HAL_Delay(2000);
        return;
    }
    
    // 第一步：采集指纹
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)"Place finger", 8, 1);
    OLED_ShowString(0, 8, (uint8_t*)"First time", 8, 1);
    OLED_Refresh();
    
    while (1)
    {
        status = AS608_GetImage();
        if (status == AS608_ACK_OK)
        {
            break; // 采集成功
        }
        HAL_Delay(500);
    }
    
    // 提取特征
    status = AS608_GenChar(1); // 1表示第一缓冲区
    if (status != AS608_ACK_OK)
    {
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"Feature extract", 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)"failed", 8, 1);
        OLED_Refresh();
        HAL_Delay(2000);
        return;
    }
    
    // 第二步：再次采集指纹
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)"Place finger", 8, 1);
    OLED_ShowString(0, 8, (uint8_t*)"Second time", 8, 1);
    OLED_Refresh();
    
    while (1)
    {
        status = AS608_GetImage();
        if (status == AS608_ACK_OK)
        {
            break; // 采集成功
        }
        HAL_Delay(500);
    }
    
    // 提取特征
    status = AS608_GenChar(2); // 2表示第二缓冲区
    if (status != AS608_ACK_OK)
    {
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"Feature extract", 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)"failed", 8, 1);
        OLED_Refresh();
        HAL_Delay(2000);
        return;
    }
    
    // 合并特征
    status = AS608_RegModel();
    if (status != AS608_ACK_OK)
    {
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"Model merge", 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)"failed", 8, 1);
        OLED_Refresh();
        HAL_Delay(2000);
        return;
    }
    
    // 保存指纹
    status = AS608_StoreChar(1, id);
    if (status == AS608_ACK_OK)
    {
        // 注册成功
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"Fingerprint", 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)"registered", 8, 1);
        OLED_Refresh();
        HAL_Delay(2000);
    }
    else
    {
        // 注册失败
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"Registration", 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)"failed", 8, 1);
        OLED_Refresh();
        HAL_Delay(2000);
    }
    
    // 显示主页面
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)"Wait for NFC card", 8, 1);
    OLED_ShowString(0, 8, (uint8_t*)"Wait for fingerprint", 8, 1);
    OLED_Refresh();
}

// 删除指纹模式
void DelFingerMode(void)
{
    uint8_t status;
    uint16_t pageID, score;
    
    // 采集指纹图像
    while (1)
    {
        status = AS608_GetImage();
        if (status == AS608_ACK_OK)
        {
            break; // 采集成功
        }
        HAL_Delay(500);
    }
    
    // 提取特征
    status = AS608_GenChar(1); // 1表示第一缓冲区
    if (status != AS608_ACK_OK)
    {
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"Feature extract", 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)"failed", 8, 1);
        OLED_Refresh();
        HAL_Delay(2000);
        return;
    }
    
    // 搜索指纹
    status = AS608_Search(1, 0, 200, &pageID, &score);
    if (status == AS608_ACK_OK)
    {
        // 找到指纹，删除它
        status = AS608_DeleteChar(pageID, 1);
        if (status == AS608_ACK_OK)
        {
            // 删除成功
            OLED_Clear();
            OLED_ShowString(0, 0, (uint8_t*)"Fingerprint", 8, 1);
            OLED_ShowString(0, 8, (uint8_t*)"deleted", 8, 1);
            OLED_Refresh();
            HAL_Delay(2000);
        }
        else
        {
            // 删除失败
            OLED_Clear();
            OLED_ShowString(0, 0, (uint8_t*)"Delete", 8, 1);
            OLED_ShowString(0, 8, (uint8_t*)"failed", 8, 1);
            OLED_Refresh();
            HAL_Delay(2000);
        }
    }
    else
    {
        // 没有找到指纹
        OLED_Clear();
        OLED_ShowString(0, 0, (uint8_t*)"Fingerprint", 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)"not found", 8, 1);
        OLED_Refresh();
        HAL_Delay(2000);
    }
    
    // 显示主页面
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)"Wait for NFC card", 8, 1);
    OLED_ShowString(0, 8, (uint8_t*)"Wait for fingerprint", 8, 1);
    OLED_Refresh();
}

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
