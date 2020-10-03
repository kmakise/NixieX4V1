/**
  ******************************************************************************
  * @file    VDDriver.c
  * @author  张东
  * @version V1.0.0
  * @date    2019-9-30
  * @brief   四位辉光管 驱动程式
  ******************************************************************************
  */

#include "nixie.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"


#define NT_DATA(a) GPIOB->BSRR = DAT_Pin << 16 * !a
#define NT_CLK(a)  GPIOB->BSRR = CLK_Pin << 16 * !a
#define NT_OUT(a)  GPIOB->BSRR = RCK_Pin << 16 * !a

void NixieTubeDrive(uint8_t * num)
{
	uint16_t i;
	for(i = 0;i < 48;i++)
	{
		//data level out
		NT_DATA(num[i]);
		//clock out
		NT_CLK(1);
		NT_CLK(0);
	}
	//out put
	NT_OUT(1);
	NT_OUT(0);
}
const uint8_t TUBE[4][12] = 
{ 
//  0  1  2  3  4  5  6  7  8  9 . NA
	{12,13,14, 5, 6, 4, 7, 8,10,11, 9,50,},//0
	{23,24,25,16,17,15,18,19,21,22,20,50,},//1   
	{34,35,36,27,28,26,29,30,32,33,31,50,},//2
	{45,46,47,38,39,37,40,41,43,44,42,50,},//3
};

uint8_t NixieTubeRunning = 1;

void NixieTubeDisplay(uint8_t * num)
{
	uint8_t buf[51] = { 
		
	//0 1 2 3 4 5 6 7
		0,0,0,0,0,0,0,0,//7
		0,0,0,0,0,0,0,0,//15
		0,0,0,0,0,0,0,0,//23
		0,0,0,0,0,0,0,0,//31
		0,0,0,0,0,0,0,0,//39
		0,0,0,0,0,0,0,0,//47
		0,0,
	};
	uint8_t i = 0;
	if(NixieTubeRunning)
	{
		for(i = 0;i < 4;i++)
		{
			buf[TUBE[i][num[i]]] = 1;
		}
	}
	NixieTubeDrive(buf);
}

void NixieTubeControl(uint8_t cmd)
{
	if(cmd == ENABLE)
	{
		NixieTubeRunning = 1;
	}
	else if(cmd == DISABLE)
	{
		NixieTubeRunning = 0;
	}
}

void NixieTubeTest(uint8_t delay)
{
	uint8_t num[8] = {11,11,11,11,11,11,11,11};
	for(int i = 0;i < 8;i++)
	{
		num[i] = 10;
		NixieTubeDisplay(num);
		HAL_Delay(delay);
	}
	for(int i = 0;i < 10;i++)
	{
		for(int j = 0;j < 8;j++)
		{
			num[j] = i;
		}
		NixieTubeDisplay(num);
		HAL_Delay(delay*2);
	}
	for(int i = 0;i < 8;i++)
	{
		num[i] = 11;
		NixieTubeDisplay(num);
		HAL_Delay(delay);
	}
}


void NixieTubeFlash(uint8_t delay)
{
	uint8_t num[8];
	
	for(int i = 0;i < 30;i++)
	{
		for(int j = 0;j < 8;j++)
		{
			num[j] = rand()%10;
		}
		NixieTubeDisplay(num);
		HAL_Delay(delay);
	}
}

