/**
  ******************************************************************************
  * @file           : esp8266.c
  * @brief          : the esp8266's init&com program body.
  * @author         : GEEK209 ADT ___ÕÅ¶«.
  * @date           £º4/8/2018.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 GEEK209 AGV Development Team
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "esp8266.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
/* Definition part ---------------------------------------------------------*/
#define ESP_UART &huart2

/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  WiFi pattern string sending program.
  *
  * @retval None
  */
void wifi_transmit(uint8_t * str)
{
	HAL_UART_Transmit_IT(ESP_UART,str,sizeof(str));
}
/**
  * @brief  The initialization of the WiFi module.
  *
  * @retval None
  */
void esp8266_init(void)
{
	wifi_transmit((uint8_t *)"AT+CWMODE=2\r\n");
	HAL_Delay(200);
	wifi_transmit((uint8_t *)"AT+RST\r\n");
	HAL_Delay(1000);
	wifi_transmit((uint8_t *)"AT+CWSAP=\"LinYueChen\",\"20030717\",11,0\r\n");
	HAL_Delay(200);
	wifi_transmit((uint8_t *)"AT+CIPMUX=1\r\n");
	HAL_Delay(200);
	wifi_transmit((uint8_t *)"AT+CIPSERVER=1,8080\r\n");
	HAL_Delay(200);
	/*USART 2 IT_RX ENABLE,IT_IDLE DISABLE*/
	//__HAL_UART_ENABLE_IT(&huart2,UART_IT_RXNE);
	//__HAL_UART_DISABLE_IT(&huart2,UART_IT_IDLE);
}


/************************ (C) GEEK209 AGV Development Team  *****END OF FILE****/
