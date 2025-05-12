/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
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
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "led.h"
#include	"stdio.h"
#include  "math.h"

uint8_t sendData[5]={0x1F,0x00,0x00,0x0F,0xA0};			//0x1FΪ�����֣����ݺ���4���ֽ�     0x1F�Զ���0x2F�ֶ�
//uint8_t com9[6]={9,9,9,9,9,9};
//uint8_t com8[6]={8,8,8,8,8,8};
//uint8_t com7[6]={7,7,7,7,7,7};
//uint8_t com6[6]={6,6,6,6,6,6};
//uint8_t com5[6]={5,5,5,5,5,5};
//uint8_t com4[6]={4,4,4,4,4,4};
//uint8_t com3[6]={3,3,3,3,3,3};
//uint8_t com2[6]={2,2,2,2,2,2};
//uint8_t com1[6]={1,1,1,1,1,1};
uint8_t com0[6]={12,12,12,12,12,12};			//����-
//����VSIN��VCOS��Angle����
const double VSIN ,VCOS;					//��λmV��
const double Angle;								//��λ

volatile uint8_t rx_count = 0;      // ��¼�ѽ��յ��ֽ���
// ������ջ������ͼ�����
#define BUFFER_SIZE 25
volatile uint8_t rx_buffer[BUFFER_SIZE];


// ����ȫ�ֱ���
uint32_t last_time = 0;


const uint32_t INTERVAL = 500; // 1����
// �����������4��������
volatile float data1=0.0f, data2=0.0f, data3=0.0f, data4=0.0f;
//���Ź�
//IWDG_HandleTypeDef hiwdg;

//void IWDG_Init(void)
//{
//    hiwdg.Instance = IWDG;
//    hiwdg.Init.Prescaler = IWDG_PRESCALER_64; // Ԥ��Ƶ64
//    hiwdg.Init.Reload = 4000;                 // ����ֵ����ʱʱ��6.4��
//    
//    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
//    {
//        Error_Handler();
//    }
//}

void SystemClock_Config(void);


int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 2);//huart1��Ҫ������������޸�
	return (ch);
}

// ���� arctan(Vsin/Vcos) �ĺ�����������ǻ��ȣ���Ҫ����180/��������ɽǶ�
double calculate_position(double Vsin, double Vcos) 
{
    if (Vcos == 0) 
		{
        // ����������
        if (Vsin > 0) 
				{
          return 1.57079632 ;				//��/2
        } 
					else if (Vsin < 0)
					{
            return -1.57079632;
					}
						else
						{
							return 0;
						}
    }
    return atan2(Vsin, Vcos);
}



int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
//	IWDG_Init();
//  MX_DMA_Init();
//  MX_SPI2_Init();
  MX_USART1_UART_Init();        //�ɼ���������Ϣ
  MX_USART2_UART_Init();				//������
	
  HAL_UART_Receive_IT(&huart1, (uint8_t *)rx_buffer, 1);// ʹ��USART1�����ж�

//  MX_TIM2_Init();
	
	// ��ʼ������TM1637
    TM1637_Init(GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1);
    TM1637_Init(GPIOA, GPIO_PIN_5, GPIOA, GPIO_PIN_6);
    TM1637_Init(GPIOB, GPIO_PIN_0, GPIOB, GPIO_PIN_1);
	

//		double VSIN=2.4563;				//�ӿڱ���
//		double VCOS=1.2345;				//�ӿڱ���
		TM1637_Display1(GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1,com0,6);

		TM1637_Display1(GPIOA, GPIO_PIN_5, GPIOA, GPIO_PIN_6,com0,6);

		TM1637_Display1(GPIOB, GPIO_PIN_0, GPIOB, GPIO_PIN_1,com0,6);

		send_packet(0x01,sendData,5);     //0x81ΪЭ��ͷ
		
		HAL_Delay(4000);									//�����ˣ�4s���ԣ�1.2.3�����У�ԭ�����ϵ��ģ������һЩ��Ч���ݣ����3s�����ݡ�
			
		uint32_t reset_flag = RCC->CSR; 	// ��ȡ��λ��־�Ĵ���
		 
		if (reset_flag & RCC_CSR_PORRSTF)
		{
				
				RCC->CSR |= RCC_CSR_RMVF; 		// �����λ��־����ѡ��
				NVIC_SystemReset(); 					// �Զ����������λ�������û�������
		}

  while (1)
  {
//		 if (HAL_IWDG_Refresh(&hiwdg) != HAL_OK)				//ι��
//    {
//        Error_Handler();
//    }
		
//				printf(" Vsin=%f\r\n",VSIN ); 							//���Գ���ʱ����ӡ��ʾ����
//				printf(" Vcos=%f\r\n",VCOS ); 
//				printf(" �����Ƕ�=%f \r\n",Angle ); 
//		    // �������ݴ��������ӡ������ֵ
//				printf("Sensor value: %f\n", sensorValue);

     
    if (HAL_GetTick() - last_time >= INTERVAL)				 // ��������ʱ
    {
        last_time = HAL_GetTick();
			
				double VSIN=(data1/1000);									//��mV�źŻ����V
				double VCOS=(data2/1000);									//��mV�źŻ����V
				double Angle=(calculate_position(VSIN, VCOS)/3.141592653589)*180*NUM;				//���Ȼ���ɽǶȣ�Ȼ���ٳ���NUMΪ�������õ������Ƕ�;
				if(Angle<0)
				{
					Angle +=360*NUM;

				}
				if((Angle>360)&&(Angle<=720))
				{
						Angle-=360;
				}
				if((Angle>720)&&(Angle<=1080))
				{
						Angle-=720;
					
				}
				if((Angle>1080)&&(Angle<=1440))
				{
						Angle-=1080;
				}
        
        // ִ������������
        display_float(GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1, VSIN);
        display_float(GPIOA, GPIO_PIN_5, GPIOA, GPIO_PIN_6, VCOS);
        display_angle(Angle);
	
    }

  }
}

// ��4�ֽ�ת��ΪFP32
float bytes_to_float(uint8_t *bytes)
{
    union 
		{
        uint32_t i;
        float f;
    } converter;
    
    // ����������С�˸�ʽ�����ֽ���ǰ��
    converter.i = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
    return converter.f;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{
			
        // ����һ���ֽڲ����Ӽ�����
        rx_count++;

        // ����Ƿ������25���ֽ�
        if (rx_count >= BUFFER_SIZE) 
				{
            // ������7-10���ֽ�Ϊdata1
            data1 = bytes_to_float(&rx_buffer[6]);
            
            // ������11-14���ֽ�Ϊdata2
            data2 = bytes_to_float(&rx_buffer[10]);
            
            // ������15-18���ֽ�Ϊdata3
            data3 = bytes_to_float(&rx_buffer[14]);
            
            // ������19-22���ֽ�Ϊdata4
            data4 = bytes_to_float(&rx_buffer[18]);
            
//            // ��ӡ�����������Ҫʵ��printf�ض���
//            printf("Data1: %f, Data2: %f, Data3: %f, Data4: %f\r\n", data1, data2, data3, data4);
            
            // ���ü�������׼����һ�ν���
            rx_count = 0;
					}

        // ����������һ���ֽ�
        HAL_UART_Receive_IT(&huart1, &rx_buffer[rx_count], 1);
  
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
