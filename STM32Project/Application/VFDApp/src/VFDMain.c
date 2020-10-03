/**
  ******************************************************************************
  * @file    VFDMain.h.c
  * @author  张东
  * @version V1.0.0
  * @date    2019-9-4
	           2020-9-30
  * @brief   四位辉光钟主程序
  ******************************************************************************
  */
#include "main.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "usart.h"

#include "VFDApp.h"

//hardware
#include "oled.h"
#include "DS3231.h"
//#include "VFDDriver.h"

#include "esp8266.h"


/*setup loop and interrupt function*/
void VFDMainSetup(void)
{
	//MCU config
	__HAL_AFIO_REMAP_SWJ_NONJTRST();

//	__HAL_UART_ENABLE_IT(&huart2,UART_IT_RXNE);
//	__HAL_UART_DISABLE_IT(&huart2,UART_IT_IDLE);
	
	
	//hardware init
	OLED_Init();
	OLED_Clear();
	
	//VFDDriverInit();
	
	//esp8266_init();
	
	VFDApp_Setup();
}

void VFDMainLoop(void)
{
	//Test();
	VFDApp_Loop();
}

void VFDMainInterrupt(void)
{
	VFDApp_Interrupt();
}

