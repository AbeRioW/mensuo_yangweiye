/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "AS608.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#include "oled.h"
#include "string.h"

// 串口2接收相关定义
#define USART2_MAX_RECV_LEN 256
#define USART2_MAX_SEND_LEN 256

uint8_t USART2_RX_BUF[USART2_MAX_RECV_LEN];
volatile uint16_t USART2_RX_STA = 0;

// 蓝牙状态标志位
extern volatile uint8_t ble_connected;
extern volatile uint8_t ble_disconnected;
extern volatile uint8_t add_nfc_flag;
extern volatile uint8_t del_nfc_flag;
extern volatile uint8_t add_finger_flag;
extern volatile uint8_t del_finger_flag;
extern volatile uint8_t beep_flag;
extern volatile uint8_t lay_flag;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles SPI1 global interrupt.
  */
void SPI1_IRQHandler(void)
{
  /* USER CODE BEGIN SPI1_IRQn 0 */

  /* USER CODE END SPI1_IRQn 0 */
  HAL_SPI_IRQHandler(&hspi1);
  /* USER CODE BEGIN SPI1_IRQn 1 */

  /* USER CODE END SPI1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/* USER CODE BEGIN 1 */
/**
  * @brief  UART������ɻص�����
  * @param  huart: UART���
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart3)
    {
        if ((USART3_RX_STA & 0x8000) == 0) // ����δ���
        {
            if (USART3_RX_STA < USART3_MAX_RECV_LEN) // ������δ��
            {
                USART3_RX_STA++; // ���ճ���+1
                // ����������һ���ֽ�
                HAL_UART_Receive_IT(&huart3, &USART3_RX_BUF[USART3_RX_STA], 1);
            }
            else
            {
                USART3_RX_STA |= 0x8000; // ��ǽ������
            }
        }
    }
    else if (huart == &huart2)
    {
        if ((USART2_RX_STA & 0x8000) == 0) // ����δ���
        {
            if (USART2_RX_STA < USART2_MAX_RECV_LEN) // ������δ��
            {
                USART2_RX_STA++; // ���ճ���+1
                
                // 添加结束符，确保strstr能正确工作
                USART2_RX_BUF[USART2_RX_STA] = '\0';
                
                // 检查是否接收到"CONNECT OK"
                if (strstr((char*)USART2_RX_BUF, "CONNECT OK"))
                {
                    ble_connected = 1;
                    // 清空接收缓冲区
                    memset(USART2_RX_BUF, 0, USART2_MAX_RECV_LEN);
                    // 重置接收状态
                    USART2_RX_STA = 0;
                }
                
                // 检查是否接收到"DISCONNECT"或"DISCONNECTED"
                else if (strstr((char*)USART2_RX_BUF, "DISCONNECT"))
                {
                    ble_disconnected = 1;
                    // 清空接收缓冲区
                    memset(USART2_RX_BUF, 0, USART2_MAX_RECV_LEN);
                    // 重置接收状态
                    USART2_RX_STA = 0;
                }
                
                // 检查是否接收到"add nfc"
                else if (strstr((char*)USART2_RX_BUF, "add nfc"))
                {
                    add_nfc_flag = 1;
                    // 清空接收缓冲区
                    memset(USART2_RX_BUF, 0, USART2_MAX_RECV_LEN);
                    // 重置接收状态
                    USART2_RX_STA = 0;
                }
                
                // 检查是否接收到"del nfc"
                else if (strstr((char*)USART2_RX_BUF, "del nfc"))
                {
                    del_nfc_flag = 1;
                    // 清空接收缓冲区
                    memset(USART2_RX_BUF, 0, USART2_MAX_RECV_LEN);
                    // 重置接收状态
                    USART2_RX_STA = 0;
                }
                
                // 检查是否接收到"add finger"
                else if (strstr((char*)USART2_RX_BUF, "add finger"))
                {
                    add_finger_flag = 1;
                    // 清空接收缓冲区
                    memset(USART2_RX_BUF, 0, USART2_MAX_RECV_LEN);
                    // 重置接收状态
                    USART2_RX_STA = 0;
                }
                
                // 检查是否接收到"del finger"
                else if (strstr((char*)USART2_RX_BUF, "del finger"))
                {
                    del_finger_flag = 1;
                    // 清空接收缓冲区
                    memset(USART2_RX_BUF, 0, USART2_MAX_RECV_LEN);
                    // 重置接收状态
                    USART2_RX_STA = 0;
                }
                
                // 检查是否接收到"beep"
                else if (strstr((char*)USART2_RX_BUF, "beep"))
                {
                    beep_flag = 1;
                    // 清空接收缓冲区
                    memset(USART2_RX_BUF, 0, USART2_MAX_RECV_LEN);
                    // 重置接收状态
                    USART2_RX_STA = 0;
                }
                
                // 检查是否接收到"lay"
                else if (strstr((char*)USART2_RX_BUF, "lay"))
                {
                    lay_flag = 1;
                    // 清空接收缓冲区
                    memset(USART2_RX_BUF, 0, USART2_MAX_RECV_LEN);
                    // 重置接收状态
                    USART2_RX_STA = 0;
                }
                
                // ����������һ���ֽ�
                if (USART2_RX_STA < USART2_MAX_RECV_LEN - 1) // 确保有空间接收下一个字符
                {
                    HAL_UART_Receive_IT(&huart2, &USART2_RX_BUF[USART2_RX_STA], 1);
                }
                else
                {
                    // 缓冲区已满，重置状态
                    USART2_RX_STA = 0;
                    memset(USART2_RX_BUF, 0, USART2_MAX_RECV_LEN);
                    HAL_UART_Receive_IT(&huart2, &USART2_RX_BUF[0], 1);
                }
            }
            else
            {
                USART2_RX_STA |= 0x8000; // ��ǽ������
            }
        }
    }
}
/* USER CODE END 1 */
