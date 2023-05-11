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
#include <string.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FLASH_STORAGE_TWR 0x0807F800
#define FLASH_STORAGE_FREE_BYTES 0x0807F000
#define FLASH_STORAGE_TOTAL_BYTES 0x0807E800
#define page_size 0x800
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char rx_buffer[50];
int64_t convert_rx_buff, rx_buff;
int64_t rx_buffer1 = 0;
int64_t a;
bool twr;
char firstDigit[20], secondDigit[20], thirdDigit[20];
char *dig;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */





char *uint64_to_string(int64_t input)
{
static char result[21] = "";
// Clear result from any leftover digits from previous function call.
memset(&result[0], 0, sizeof(result));
// temp is used as a temporary result storage to prevent sprintf bugs.
char temp[21] = "";
char c;
uint8_t base = 10;

while (input)
{
int num = input % base;
input /= base;
c = '0' + num;

sprintf(temp, "%c%s", c, result);
strcpy(result, temp);
}
return result;
}









/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void process_data(uint64_t fl_storage, bool twr)
{
    while(!(USART2->SR & USART_SR_RXNE));
    HAL_UART_Receive(&huart2, rx_buffer, sizeof(rx_buffer), 1000);
    HAL_Delay(1000);
    uint8_t str1[20];
    char* end;


    //dig = strtok((char*)rx_buffer, " ");

    //firstDigit = strtok((char*)rx_buffer, " ");

    /*std::string s = (char*)rx_buffer;
    std::string delimiter = " ";
    std::string firstDigitStr = s.substr(0, s.find(delimiter)); // token is "scott"
    strcpy(firstDigit, firstDigitStr.c_str());*/

    HAL_Delay(1000);
    while(!(USART2->SR & USART_SR_TC));
    HAL_UART_Transmit(&huart2, (uint64_t*)firstDigit, sizeof(firstDigit), 1000);
    HAL_Delay(1000);

    char *space = strstr(rx_buffer, " ");
    int firstDigitLen = space - rx_buffer;

    strncpy(firstDigit, rx_buffer, firstDigitLen);
    firstDigit[firstDigitLen] = '\0';

    //char buffer2[50];
    //sprintf(buffer2, "%lu", rx_buffer );

    HAL_Delay(1000);
    while(!(USART2->SR & USART_SR_TC));
    HAL_UART_Transmit(&huart2, (uint64_t*)firstDigit, sizeof(firstDigit), 1000);
    HAL_Delay(1000);

    /*a = strtoull((char*)rx_buffer,&end,10);

    if (twr == true){
        convert_rx_buff += llabs(a - rx_buffer1);
        rx_buffer1 = a;
        strcpy((char*)str1,uint64_to_string(convert_rx_buff));
    }
    else{
        //rx_buff = llabs(a);
        strcpy((char*)str1,uint64_to_string(a));
    }
    /*convert_rx_buff += llabs(a - rx_buffer1);
//	  convert_rx_buff += 2345678901;
    rx_buffer1 = a;
*/

    memset(rx_buffer, 0, sizeof(rx_buffer));
    sprintf((char*)rx_buffer,"%s", (char*)str1);
    save_to_flash((uint8_t*)rx_buffer, fl_storage);
    memset(rx_buffer, 0, sizeof(rx_buffer));
    char read_data[20];
    memset(read_data, 0, sizeof(read_data));
    /*read_flash((uint8_t*)read_data, fl_storage);
    HAL_Delay(100);
    while(!(USART2->SR & USART_SR_TC));
    HAL_UART_Transmit(&huart2, (uint8_t*)read_data,20 ,300);
    HAL_Delay(100);*/
}



void save_to_flash(uint8_t *data, uint64_t fl_storage)
{
    volatile uint32_t data_to_FLASH[(strlen((char*)data)/4)	+ (int)((strlen((char*)data) % 4) != 0)];
    memset((uint8_t*)data_to_FLASH, 0, strlen((char*)data_to_FLASH));
    strcpy((char*)data_to_FLASH, (char*)data);

    volatile uint32_t data_length = (strlen((char*)data_to_FLASH) / 4)
                                    + (int)((strlen((char*)data_to_FLASH) % 4) != 0);
    volatile uint16_t pages = (strlen((char*)data)/page_size)
                                    + (int)((strlen((char*)data)%page_size) != 0);
      /* Unlock the Flash to enable the flash control register access *************/
      HAL_FLASH_Unlock();

      /* Allow Access to option bytes sector */
      HAL_FLASH_OB_Unlock();

      /* Fill EraseInit structure*/
      FLASH_EraseInitTypeDef EraseInitStruct;
      EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
      EraseInitStruct.PageAddress = fl_storage;
      EraseInitStruct.NbPages = pages;
      uint32_t PageError;

      volatile uint32_t write_cnt=0, index=0;

      volatile HAL_StatusTypeDef status;
      status = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
      while(index < data_length)
      {
          if (status == HAL_OK)
          {
              status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, fl_storage+write_cnt, data_to_FLASH[index]);
              if(status == HAL_OK)
              {
                  write_cnt += 4;
                  index++;
              }
          }
      }

      HAL_FLASH_OB_Lock();
      HAL_FLASH_Lock();
}

void read_flash(uint8_t* data, uint64_t fl_storage)
{
    volatile uint32_t read_data;
    //read_data = *(uint32_t*)(fl_storage);
    //data[0] = (uint8_t)read_data;
    volatile uint32_t read_cnt=0;
    do
    {
        read_data = *(uint32_t*)(fl_storage + read_cnt);
        if(read_data != *(uint32_t*)(fl_storage + 1000))
        {
            data[read_cnt] = (uint8_t)read_data;
            data[read_cnt + 1] = (uint8_t)(read_data >> 8);
            data[read_cnt + 2] = (uint8_t)(read_data >> 16);
            data[read_cnt + 3] = (uint8_t)(read_data >> 24);
            read_cnt += 4;
        }
    }while(read_data != *(uint32_t*)(fl_storage + 1000));
}






int64_t GetDifference(uint64_t first, uint64_t second) {
    uint64_t abs_diff = (first > second) ? (first - second): (second - first);
    assert(abs_diff<=INT64_MAX);
    return (first > second) ? (int64_t)abs_diff : -(int64_t)abs_diff;
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */


//  HAL_UART_Receive(&huart2, rx_buffer,20,300);
//  HAL_Delay(100);
//
//  char write_data[50];
//    memset(write_data, 0, sizeof(write_data));
//    strcpy(write_data, "Hello World veryooregkerergerigjeogjbife!!!");

//    save_to_flash((uint8_t*)rx_buffer);
////    save_to_flash((uint8_t*)write_data);
//
//    char read_data[50];
//    memset(read_data, 0, sizeof(read_data));
//
//    read_flash((uint8_t*)read_data);
//    HAL_Delay(100);
//    HAL_UART_Transmit(&huart2, (uint8_t*)read_data,50 ,1000);


//    float write_number = 235.756f;
//    float *pointer_write = &write_number;
//    save_to_flash((uint8_t*)pointer_write);
//
//    float read_number = 0.0f;
//    float *pointer_read = &read_number;
//    read_flash((uint8_t*)pointer_read);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    twr = true;
    process_data((uint64_t)FLASH_STORAGE_TWR, twr);
    //twr = false;
    //process_data((uint64_t)FLASH_STORAGE_FREE_BYTES, twr);
    //process_data((uint64_t)FLASH_STORAGE_TOTAL_BYTES, twr);
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
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

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
