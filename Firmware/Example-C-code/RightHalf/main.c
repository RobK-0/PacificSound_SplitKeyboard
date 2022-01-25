/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_hid.h"
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
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */

extern USBD_HandleTypeDef hUsbDeviceFS;



// uint8_t kbUSB_data[8]={0x00};
uint8_t kbSPI_rx[8]={0x0};
uint8_t kbSPI_tx[8]={0x0};
uint16_t debounce = 50;

/* initialize a vector for the keyboard counting (counting cycles a key)
   is kept pressed */
uint8_t vect_ind = 0x0;
uint16_t kb_count_vect[22]={0x0};



void scan_cols(uint8_t row)
{

	// Row specified by rowInd is toggled. Scan through columns to check it any column is pulled low
	// diodes go from column to row. Rows are set low to toggle. Columns have a pull-up on to set them
	// high when there is no connection.

	//Check column 1
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)!=GPIO_PIN_SET){
		kb_count_vect[vect_ind]+=1;
	}
	else{
		kb_count_vect[vect_ind]=0;
	}
	vect_ind+=1;

	//Check column 2
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)!=GPIO_PIN_SET){
		kb_count_vect[vect_ind]+=1;
	}
	else{
		kb_count_vect[vect_ind]=0;
	}
	vect_ind+=1;

	//Check column 3
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)!=GPIO_PIN_SET){
		kb_count_vect[vect_ind]+=1;
	}
	else{
		kb_count_vect[vect_ind]=0;
	}
	vect_ind+=1;

	//Check column 4
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)!=GPIO_PIN_SET){
		kb_count_vect[vect_ind]+=1;
	}
	else{
		kb_count_vect[vect_ind]=0;
	}
	vect_ind+=1;

	// Row 3 only has 4 key spots so no need to check pin B12 or in B13 (Col 5 or Col 6)
	if(row<3){
		//Check column 5
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)!=GPIO_PIN_SET){
			kb_count_vect[vect_ind]+=1;
		}
		else{
			kb_count_vect[vect_ind]=0;
		}
		vect_ind+=1;

		//Check column 6
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)!=GPIO_PIN_SET){
			kb_count_vect[vect_ind]+=1;
		}
		else{
			kb_count_vect[vect_ind]=0;
		}
		vect_ind+=1;
	}


}


void scan_matrix(void)
{
	vect_ind = 0x0;
	// Key switch detection...
	// Trigger row 1 and scan columns
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,0);
	scan_cols(vect_ind);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,1);

	// Trigger row 2 and scan columns
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,0);
	scan_cols(1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,1);

	// Trigger row 3 and scan columns
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,0);
	scan_cols(2);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,1);

	// Trigger row 4 and scan columns
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,0);
	scan_cols(3);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,1);

}



void update_keys(void){

	// The indices of the vector are sent instead of the keycodes.
	// This will allow the half that is connected to the computer
	// to interpret the keys (use layers etc.)

	uint8_t kb_ind;
	uint8_t ii;

	for( kb_ind=0; kb_ind<8; ++kb_ind){
		kbSPI_tx[kb_ind] = 0x0;
	}

	kb_ind=0;

	for( ii = 0; ii < 22; ++ii ){
		if(kb_ind<8){
			if(kb_count_vect[ii]>debounce){
				kb_count_vect[ii]=debounce+1; //prevent overrun of uint16 values
				kbSPI_tx[kb_ind]=ii+1;
				++kb_ind;
			}
		}
	}

}



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
//static void MX_DMA_Init(void);
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
  MX_SPI1_Init();
  MX_TIM3_Init();
  //MX_DMA_Init();
  //MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  /* initialize all GPIO Output pins (rows) to 1 */
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,1);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,1);
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,1);
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,1);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    // On each loop scan the matrix of this half.
    // Then update the vector of keys that have
    // been pressed down (stored in kbSPI_tx).
    // Then send the vector of keys that are
    // held down to the other half using SPI_Transmit.
    // Since the other half will likely be slower the
    // transmit timeout should hold up the MCU on this
    // board.
    // The scanning frequency will likely be determined by
    // the other half.
    // Original intent was to use DMA to do SPI transmission
    // that's not working yet. This is a simple way to
    // get something functioning.

    scan_matrix();
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,0);
	update_keys();

	HAL_SPI_Transmit(&hspi1,kbSPI_tx,8,10000);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,1);




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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_SLAVE;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

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
   __HAL_RCC_GPIOH_CLK_ENABLE();
   __HAL_RCC_GPIOB_CLK_ENABLE();
   __HAL_RCC_GPIOA_CLK_ENABLE();


   /*Reset output pins */
   /*Configure GPIO pin Output Level */
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);
   /*Configure GPIO pin Output Level */
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_RESET);

   /* Set Output pins for GPIOA*/
   /*Configure GPIO pins : PA9 PA10 */
   GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

   /* Set Output pins for GPIOB*/
   /*Configure GPIO pins : PB1 PB2 */
   GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


   /* Set Output pins for GPIOC*/
   /*Configure GPIO pins : PC13 */
   GPIO_InitStruct.Pin = GPIO_PIN_13;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


   /* Set Input pins for GPIOA*/
   /*Configure GPIO pin : PA8 */
   GPIO_InitStruct.Pin = GPIO_PIN_8;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


   /* Set Input pins for GPIOB*/
   /*Configure GPIO pins : PB10 PB12 PB13 PB14
                            PB15 */
   GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                           |GPIO_PIN_15;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
