/**
  ******************************************************************************
  * @file    VDDriver.c
  * @author  张东
  * @version V1.0.0
  * @date    2019-9-4
  * @brief   真空荧光表示器 驱动程式
  ******************************************************************************
  */
#include "stm32f1xx_hal.h"
#include "VFDDriver.h"
#include "main.h"

#define NT_DATA(a) GPIOB->BSRR = DAT_Pin << 16 * !a
#define NT_CLK(a)  GPIOB->BSRR = CLK_Pin << 16 * !a
#define NT_OUT(a)  GPIOB->BSRR = RCK_Pin << 16 * !a

//#define NT_DATA(a) HAL_GPIO_WritePin(GPIOB,DAT_Pin,a)
//#define NT_CLK(a)  HAL_GPIO_WritePin(GPIOB,CLK_Pin,a)
//#define NT_OUT(a)  HAL_GPIO_WritePin(GPIOB,RCK_Pin,a)

uint8_t VFDFrameBuffer[10][16];//VFD表示器显示缓冲区
uint8_t VFDLedSateBuffer[6];//控制面板LED状态缓冲区


//VFD 显示段全控
void VFDAllSegment(uint8_t cmd)
{
		for(int i = 0;i < 10;i++)
	{
		for(int j = 0;j < 16;j++)
		{
			VFDFrameBuffer[i][j] = (cmd == 0);
		}
	}
	for(int i = 0;i < 6;i++)
	{
		VFDLedSateBuffer[i] = !(cmd == 0);
	}
}


//显示缓冲区初始化 
void VFDDriverInit(void)
{
	VFDAllSegment(0);
}

//电平输出
void VFDOutputCtrl(uint8_t * num)
{
	uint16_t i;
	for(i = 0;i < 32;i++)
	{
		//data level out
		if(num[31 - i] > 0)
		{
			NT_DATA(1);
		}
		else
		{
			NT_DATA(0);
		}
		//clock out
		NT_CLK(1);
		NT_CLK(0);
	}
	//out put
	NT_OUT(1);
	NT_OUT(0);
}
//VFD 栅极与基底控制
void VFDGateSegmentCtrl(uint8_t gate,uint8_t * segment,uint8_t * led)
{
	uint8_t dat[32] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,0,0, 0,0,0,0, };
	//栅极控制
	dat[gate + 16] = 0;
	//段显示控制
	for(int i = 0;i < 16;i++)
	{
		dat[i] = segment[i];
	}
	//LED状态控制
	for(int i = 26;i < 32;i++)
	{
		dat[i] = led[i - 26];
	}
	//输出显示
	VFDOutputCtrl(dat);
}

//VFD显示刷新
void VFDDsaplayUpdate(void)
{
	static uint8_t gate = 0;
	VFDGateSegmentCtrl(gate,VFDFrameBuffer[gate],VFDLedSateBuffer);
	gate++;
	if(gate > 9)
	{
		gate = 0;
	}
}

//VFD数码管控制
void VFDNumSegment(uint8_t * num)
{
	//VFD 数码管数值与相应段使能对应表
	const uint8_t VFDNUMSEG[24]={ 
		//"0" "1" "2" "3" "4" "5" "6" "7" "8" "9" "A" "B" "C" 
		0x88,0xDD,0xA4,0x94,0xD1,0x92,0x82,0xDC,0x80,0x90,0xC0,0x83,
		//"D" "E" "F" "H" "L" "n" "u" "P" "o" "-" 熄灭
		0xAA,0x85,0xA2,0xE2,0xC1,0xAB,0xC8,0x89,0xE0,0x87,0xF7,0xFF,  
	};
	//七位数码管段控制
	for(int i = 0;i < 7;i++)
	{
		for(int j = 0;j < 7;j++)
		{
			if((VFDNUMSEG[num[i]] & (0x01 << j)) != 0)
			{
				VFDFrameBuffer[i + 2][j + 1] = 1;
			}
			else
			{
				VFDFrameBuffer[i + 2][j + 1] = 0;
			}
		}
	}
}

//驱动板 led组控制
void LEDGroupCtrl(uint8_t num,uint8_t cmd)
{
	//参数合法性
	if(num < 6 && (cmd == ENABLE || cmd == DISABLE))
	{
		//使能或者失能组内 某一个LED
		VFDLedSateBuffer[num] = cmd;
	}
}

//VFD下方数字组 控制  1-20
void VFDDownNumCtrl(uint8_t num,uint8_t cmd)
{
	const uint8_t VFDNGATE[20] = {1,1,1,2,2,3,4,4,4,5,6,6,7,8,8,8,9,9,9,9,};
	const uint8_t VFDSDATA[20] = {8,9,10,8,9,8,8,9,10,8,8,9,8,8,9,10,4,8,9,10,};
	//参数合法性
	if((0 < num && num < 21) && (cmd == ENABLE || cmd == DISABLE))
	{
		VFDFrameBuffer[VFDNGATE[num - 1]][VFDSDATA[num - 1]] = (cmd == 0);
	}
}
//vfd 右侧显示数字和数字边框的控制 seg 0 1 2 num （） ↑
void RightNumCtrl(uint8_t num,uint8_t seg,uint8_t cmd)
{
	//1 2 3 4 5 6  num ( ) ↑
	const uint8_t RNUMGATE[18] = {8,8,8, 9,9,9, 9,9,9, 8,8,8, 9,9,9, 9,9,9};
	const uint8_t NUMSEG[18] ={0,13,14, 7,5,3, 0,13,14, 11,12,15, 1,2,6, 11,12,15,};
	
	//参数合法性
	if((0 < num && num < 7 && seg < 3) && (cmd == ENABLE || cmd == DISABLE))
	{
		uint8_t pos = (num - 1) * 3 + seg;
		
		VFDFrameBuffer[RNUMGATE[pos]][NUMSEG[pos]] = (cmd == 0);
	}
}

//vfd 数字间隔符号控制
void NumSymbolCtrl(uint8_t num,uint8_t cmd)
{
	if(num == 0)
	{
		VFDFrameBuffer[4][11] = (cmd == 0);//-
	}
	else if(num == 1)
	{
		VFDFrameBuffer[6][11] = (cmd == 0);//:
	}
}

//vfd 关键字高亮显示控制
void WordDisCtrl(VFDWordTypedef num,uint8_t cmd)
{
	const uint8_t VFDWORDSEG[26] = {
		1,2,6,7,9,10,15,12,0,5,3,4,11, 
		15,0,12,1,13,2, 15, 15,0, 15,0, 15, 15
	};
	
	
	if(num <= 12)//gate 0
	{
		VFDFrameBuffer[0][VFDWORDSEG[num]] = (cmd == 0);
	}
	else if(num <= 18)//gate 1
	{
		VFDFrameBuffer[1][VFDWORDSEG[num]] = (cmd == 0);
	}
	else if(num <= 19)//gate 2
	{
		VFDFrameBuffer[2][VFDWORDSEG[num]] = (cmd == 0);
	}
	else if(num <= 21)//gate 3
	{
		VFDFrameBuffer[3][VFDWORDSEG[num]] = (cmd == 0);
	}
	else if(num <= 23)//gate 4
	{
		VFDFrameBuffer[4][VFDWORDSEG[num]] = (cmd == 0);
	}
	else if(num == 24)//gate 6
	{
		VFDFrameBuffer[6][VFDWORDSEG[num]] = (cmd == 0);
	}
	else if(num == 25)//gate 7
	{
		VFDFrameBuffer[7][VFDWORDSEG[num]] = (cmd == 0);
	}
}





/*test function -----------------------------------------------*/
void Test(void)
{
	static uint8_t num = 0;
	uint8_t str[7] = {0,0,0,0,0,0,0};
	
	WordDisCtrl(num,1);
	
	str[5] = num /10;
	str[6] = num %10;
	VFDNumSegment(str);
	
	num ++;
	
	HAL_Delay(1000);
	
	if(num > 25)
	{
		num = 0;
		for(int i = 0;i < 26;i++)
		{
			WordDisCtrl(i,0); 
		}

	}
}

void RightNumTest(void)
{
	static uint8_t num = 0;
	RightNumCtrl(num / 3,num % 3,1);
	num++;

	HAL_Delay(300);
	if(num > 20)
	{
		num = 1;
		for(int i = 0;i < 21;i++)
		{
			RightNumCtrl(i / 3,i % 3,0);
		}
	}
}

void DownNumTest(void)
{
	static uint8_t num = 2;
	
	VFDDownNumCtrl(num++,0);
	VFDDownNumCtrl(num,1);
	if(num > 20)
	{
		num = 0;
	}
	
	HAL_Delay(300);
}


void NumTest(void)
{
	static uint32_t dat = 0;
	uint8_t num[7];
	num[0] = dat/1000000%10;
	num[1] = dat/100000%10;
	num[2] = dat/10000%10;
	num[3] = dat/1000%10;
	num[4] = dat/100%10;
	num[5] = dat/10%10;
	num[6] = dat%10;
	
	if(dat > 9999999)
	{
		dat = 0;
	}
	dat++;
	VFDNumSegment(num);
	//VFDFrameBuffer[2][3] = 0;
}


void SegTest(void)
{
	uint8_t seg[16] = {
		1,0,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 
	};
	
	uint8_t led[6] = {
		1,1,1,1,1,0
	};
	
	VFDGateSegmentCtrl(3,seg,led);
}

//VFD 显示测试
void DisTest(void)
{
	static int num = 0;
	uint8_t dat[32];
	
	for(int i = 0;i< 32;i++)
	{
		dat[i] = 1;
	}
	
	for(int i = 16;i < 26;i++)
	{
		dat[i] = 0;
	}
	dat[num++] = 0;
	if(num > 15)num = 0;
	VFDOutputCtrl(dat);
	HAL_Delay(100);
}



