/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "UART_Helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "queue.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUM_OF_TELLERS 3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RNG_HandleTypeDef hrng;

UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 512 * 4
};
/* Definitions for Teller1 */
osThreadId_t Teller1Handle;
const osThreadAttr_t Teller1_attributes = {
  .name = "Teller1",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for Teller2 */
osThreadId_t Teller2Handle;
const osThreadAttr_t Teller2_attributes = {
  .name = "Teller2",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for Teller3 */
osThreadId_t Teller3Handle;
const osThreadAttr_t Teller3_attributes = {
  .name = "Teller3",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for Heartbeat */
osTimerId_t HeartbeatHandle;
const osTimerAttr_t Heartbeat_attributes = {
  .name = "Heartbeat"
};
/* Definitions for m_teller1 */
osMutexId_t m_teller1Handle;
const osMutexAttr_t m_teller1_attributes = {
  .name = "m_teller1"
};
/* Definitions for m_teller2 */
osMutexId_t m_teller2Handle;
const osMutexAttr_t m_teller2_attributes = {
  .name = "m_teller2"
};
/* Definitions for m_teller3 */
osMutexId_t m_teller3Handle;
const osMutexAttr_t m_teller3_attributes = {
  .name = "m_teller3"
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RNG_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void *argument);
void StartTeller(void *argument);
void StartTeller2(void *argument);
void StartTeller3(void *argument);
void HeartbeatTimerCB(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Global Variables
int display_timer = 0;
int total_customers = 0;
Teller tellers[NUM_OF_TELLERS];
osMutexId_t mutexes[NUM_OF_TELLERS];
uint32_t randvar = 0;
int reducer = 10;
uint8_t Rx_data[1];
queue q;
Customer clist[30] = { };
int clist_id[30] = { };
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
//	int *a;
//	a = malloc(sizeof(int));
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
  MX_RNG_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart2, Rx_data, 1);
	int id = 1;
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of m_teller1 */
  m_teller1Handle = osMutexNew(&m_teller1_attributes);

  /* creation of m_teller2 */
  m_teller2Handle = osMutexNew(&m_teller2_attributes);

  /* creation of m_teller3 */
  m_teller3Handle = osMutexNew(&m_teller3_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
//  Clock_SHandle
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of Heartbeat */
  HeartbeatHandle = osTimerNew(HeartbeatTimerCB, osTimerPeriodic, NULL, &Heartbeat_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
//  osTimerStart(HeartbeatHandle, 1000);
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Teller1 */
  Teller1Handle = osThreadNew(StartTeller, NULL, &Teller1_attributes);

  /* creation of Teller2 */
  Teller2Handle = osThreadNew(StartTeller2, NULL, &Teller2_attributes);

  /* creation of Teller3 */
  Teller3Handle = osThreadNew(StartTeller3, NULL, &Teller3_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
 
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_RNG;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.RngClockSelection = RCC_RNGCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

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
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
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

}

/* USER CODE BEGIN 4 */
void print_info(queue *q, int my_time) {
	HAL_UART_Transmit(&huart2, "Time: ", strlen("Time: "), HAL_MAX_DELAY);
	UART_print(my_time);
	HAL_UART_Transmit(&huart2, "\r\nQueue Size: ", strlen("\r\nQueue Size: "),
	HAL_MAX_DELAY);
	UART_print(get_size(q));
	HAL_UART_Transmit(&huart2, "\r\nTeller 1: ", strlen("\r\nTeller 1: "),
	HAL_MAX_DELAY);
	UART_prints(tellers[0].state ? "Busy" : "Idle");
	HAL_UART_Transmit(&huart2, "\r\nTeller 2: ", strlen("\r\nTeller 2: "),
	HAL_MAX_DELAY);
	UART_prints(tellers[1].state ? "Busy" : "Idle");
	HAL_UART_Transmit(&huart2, "\r\nTeller 3: ", strlen("\r\nTeller 3: "),
	HAL_MAX_DELAY);
	UART_prints(tellers[2].state ? "Busy" : "Idle");
	HAL_UART_Transmit(&huart2, "\r\n-----------------------------\r\n",
			strlen("\r\n-----------------------------\r\n"), HAL_MAX_DELAY);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	HAL_UART_Receive_IT(&huart2, Rx_data, 1);
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	// Create Queue
//	queue *q;
//	q = malloc(sizeof(queue));
	initialize(&q);

	// Start Timer
//	osTimerStart(HeartbeatHandle, 1000);

	int my_time = 0;
	HAL_RNG_GenerateRandomNumber(&hrng, &randvar);
	int spawn_time_left = randvar % 4 + 1; // 1->4
	int total_queue_time = 0;
	int max_queue_time = 0;
	int open_for_business = 1;

	for (;;) {

		// Timer
		while (my_time < display_timer) {
			print_info(&q, my_time);
			if (!isEmpty(&q)) {
				// Add wait time to all customers
				Customer *n = q.front;
				do {
					n->queue_wait_time++;
					n = n->next;
				} while (n != NULL);
			}

			// New Customer Arrived
			if (!spawn_time_left && open_for_business) { // = 0
//				Customer *c;
//				c = (Customer*) malloc(sizeof(Customer));
				int available = 1;
				int customer_id = 0;
				int tries = 5;
				while (available == 1) {
					if (tries-- == 0)
						goto qskip;
					HAL_RNG_GenerateRandomNumber(&hrng, &randvar);
					customer_id = (randvar % 30); // 1->4
					available = clist_id[customer_id];
				}
				clist_id[customer_id] = 1; // Taken
				Customer *c = &clist[customer_id];

				init_customer(c);
				enqueue(&q, c);
				HAL_RNG_GenerateRandomNumber(&hrng, &randvar);
				spawn_time_left = randvar % 4 + 1; // 1->4
				total_customers++;
			}
qskip:
			// Check if Teller is empty and send customer to them
			for (int i = 0; i < NUM_OF_TELLERS; i++) {
				if (!isEmpty(&q)) {
					if (tellers[i].state == 0) {
						int q_wait_time = 0;
						HAL_RNG_GenerateRandomNumber(&hrng, &randvar);
						osMutexAcquire(mutexes[i], osWaitForever);
						tellers[i].c = dequeue(&q);
						tellers[i].c->teller_wait_time = randvar % 7 + 1; // 1 -> 8
						q_wait_time = tellers[i].c->queue_wait_time;
						tellers[i].customers_serverd++;
						if (tellers[i].wait_for_customers
								> tellers[i].max_wait_for_customers) {
							tellers[i].max_wait_for_customers =
									tellers[i].wait_for_customers;
							tellers[i].wait_for_customers = 0;
						}
						tellers[i].state = 1; // Busy
						osMutexRelease(mutexes[i]);

						// Metrics
						total_queue_time += q_wait_time;
						if (q_wait_time > max_queue_time) {
							max_queue_time = q_wait_time;
						}

					}
				}
			}

			spawn_time_left--;
			my_time++;
			if (my_time >= 420) {
				open_for_business = 0;
				// make sure all customers have left
				if (q.count == 0 && tellers[0].state == 0
						&& tellers[1].state == 0 && tellers[2].state == 0) {
					// We are done
					HAL_UART_Transmit(&huart2, "\r\n\n\nTotal Customers: ", strlen("\r\n\n\nTotal Customers: "), HAL_MAX_DELAY);
					UART_print(total_customers);
					HAL_UART_Transmit(&huart2, "\r\nTeller 1 Customers: ", strlen("\r\nTeller 1 Customers: "), HAL_MAX_DELAY);
					UART_print(tellers[0].customers_serverd);
					HAL_UART_Transmit(&huart2, "\r\nTeller 2 Customers: ", strlen("\r\nTeller 2 Customers: "), HAL_MAX_DELAY);
					UART_print(tellers[1].customers_serverd);
					HAL_UART_Transmit(&huart2, "\r\nTeller 3 Customers: ", strlen("\r\nTeller 3 Customers: "), HAL_MAX_DELAY);
					UART_print(tellers[2].customers_serverd);
					HAL_UART_Transmit(&huart2, "\r\nMax Customer Wait Time: ", strlen("\r\nMax Customer Wait Time: "), HAL_MAX_DELAY);
					UART_print(max_queue_time);
					HAL_UART_Transmit(&huart2, "\r\nMax Teller Wait Time: ", strlen("\r\nMax Teller Wait Time: "), HAL_MAX_DELAY);
					UART_print(max_queue_time);
					HAL_UART_Transmit(&huart2, "\r\nMax Transaction Time: ", strlen("\r\nMax Transaction Time: "), HAL_MAX_DELAY);
					UART_print(max_queue_time);
					HAL_UART_Transmit(&huart2, "\r\nLongest Queue: ", strlen("\r\nLongest Queue: "), HAL_MAX_DELAY);
					UART_print(max_queue_time);
					HAL_UART_Transmit(&huart2, "\r\nAvg Customer Wait Time: ", strlen("\r\nAvg Customer Wait Time: "), HAL_MAX_DELAY);
					UART_printlnf((double)(total_queue_time)/(double)(total_customers));
					HAL_UART_Transmit(&huart2, "\r\nAvg Transaction Time: ", strlen("\r\nAvg Transaction Time: "), HAL_MAX_DELAY);
					UART_print(max_queue_time);
					HAL_UART_Transmit(&huart2, "\r\nAvg Wait Time: ", strlen("\r\nAvg Wait Time: "), HAL_MAX_DELAY);
					UART_print(max_queue_time);


					while(1);
				}
//				osDelay(1);
			}

		}

	}
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_StartTeller */
/**
 * @brief Function implementing the Teller1 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTeller */
void StartTeller(void *argument)
{
  /* USER CODE BEGIN StartTeller */
	// Create New Teller
	int id = 0;
	Teller *t = &tellers[id];
	init_teller(t);

	/* Infinite loop */
	int my_time = 0;
	while (1) {
		//Generates Random Time for customer
		//Processes customer

		while (my_time < display_timer) {
			my_time++;
			// If no customer
			if (t->c == NULL) {
				osMutexAcquire(mutexes[id], osWaitForever);
				t->wait_for_customers++;
				osMutexRelease(mutexes[id]);
			} else if (t->c->teller_wait_time == 0) {
				// Customer is done
				int cid = t->c->id;
				clist_id[cid] = 0; // Free Customer
				t->state = 0; // Free Teller
			}

			else {
				t->c->teller_wait_time--;
			}
		}
//		osDelay(1);
	}
  /* USER CODE END StartTeller */
}

/* USER CODE BEGIN Header_StartTeller2 */
/**
 * @brief Function implementing the Teller2 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTeller2 */
void StartTeller2(void *argument)
{
  /* USER CODE BEGIN StartTeller2 */
	// Create New Teller
	int id = 1;
	Teller *t = &tellers[id];
	init_teller(t);

	/* Infinite loop */
	int my_time = 0;
	while (1) {
		//Generates Random Time for customer
		//Processes customer

		while (my_time < display_timer) {
			my_time++;
			// If no customer
			if (t->c == NULL) {
				osMutexAcquire(mutexes[id], osWaitForever);
				t->wait_for_customers++;
				osMutexRelease(mutexes[id]);
			} else if (t->c->teller_wait_time == 0) {
				// Customer is done
				int cid = t->c->id;
				clist_id[cid] = 0; // Free Customer
				t->state = 0; // Free Teller
			}

			else {
				t->c->teller_wait_time--;
			}
		}
//		osDelay(1);
	}
  /* USER CODE END StartTeller2 */
}

/* USER CODE BEGIN Header_StartTeller3 */
/**
 * @brief Function implementing the Teller3 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTeller3 */
void StartTeller3(void *argument)
{
  /* USER CODE BEGIN StartTeller3 */
	// Create New Teller
	int id = 2;
	Teller *t = &tellers[id];
	init_teller(t);

	/* Infinite loop */
	int my_time = 0;
	while (1) {
		//Generates Random Time for customer
		//Processes customer

		while (my_time < display_timer) {
			my_time++;
			// If no customer
			if (t->c == NULL) {
				osMutexAcquire(mutexes[id], osWaitForever);
				t->wait_for_customers++;
				osMutexRelease(mutexes[id]);
			} else if (t->c->teller_wait_time == 0) {
				// Customer is done
				int cid = t->c->id;
				clist_id[cid] = 0; // Free Customer
				t->state = 0; // Free Teller
			}

			else {
				t->c->teller_wait_time--;
			}
		}
//		osDelay(1);
	}
  /* USER CODE END StartTeller3 */
}

/* HeartbeatTimerCB function */
void HeartbeatTimerCB(void *argument)
{
  /* USER CODE BEGIN HeartbeatTimerCB */
	display_timer++;
	uint8_t buffer[13] = "Hello World\n\r";
	HAL_UART_Transmit(&huart2, buffer, 13, HAL_MAX_DELAY);
  /* USER CODE END HeartbeatTimerCB */
}

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
	if (htim->Instance == TIM6) {
//		if (reducer == 0) {
//			reducer = 10;
//			display_timer++;
//		}
//		reducer--;
		display_timer++;
	}
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
