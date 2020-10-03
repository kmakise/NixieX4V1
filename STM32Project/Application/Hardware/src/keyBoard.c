/**
  ******************************************************************************
  * @file    keyBoard.c
  * @author  张东
  * @version V1.0.0
  * @date    2019-7-7
  * @brief   真空荧光表示器键盘驱动程序
  ******************************************************************************
  */
#include "stm32f1xx_hal.h"
#include "keyBoard.h"



//获得按键键值
uint8_t getKey_Ctrl(void)
{
	//按键编号 2左 3右 4上 5下 6cfm 
	//GPIO		B0	A7	A6 	A5	A4
	//返回值		3		4		1		2		5
	
	if((GPIOB->IDR & GPIO_PIN_0) == 0)//左
	{
		return 3;
	}
	if((GPIOA->IDR & GPIO_PIN_7) == 0)//右
	{
		return 4;
	}
	if((GPIOA->IDR & GPIO_PIN_6) == 0)//上
	{
		return 1;
	}
	if((GPIOA->IDR & GPIO_PIN_5) == 0)//下
	{
		return 2;
	}
	if((GPIOA->IDR & GPIO_PIN_4) == 0)//cfm
	{
		return 5;
	}
	return 0;
}