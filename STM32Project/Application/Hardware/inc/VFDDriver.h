#ifndef __VFDDRIVER_H
#define __VFDDRIVER_H

#include "stm32f1xx_hal.h"


typedef enum
{
	//GATE 0							SEG
	PROGRAM 	= 0,		// 1
	KARAOKE 	= 1,		// 2
	PBC 			= 2,		// 6
	CCRS 			= 3,		// 7
	PLAY 			= 4,		// 9
	PAUSE 		= 5,		// 10
	RANDOM 		= 6,		// 15
	ONE 			= 7,		// 12
	ALL 			= 8,		// 0
	EDIT 			= 9,		// 5
	A 				= 10,		// 3
	B 				= 11,		// 4
	VIDEOCD 	= 12,		// 11
	
	//GATE 1
	REPEAT 		= 13,		// 15
	SPACE 		= 14,		// 0
	PC 				= 15,		// 12
	SEARCH 		= 16,		// 1
	L 				= 17,		// 13
	R 				= 18,		// 2
	
	//GATE 2
	DISCNO 		= 19,		// 15
	
	//GATE 3
	ENTRY 		= 20,		// 15
	TRACK 		= 21,		// 0
	
	//GATE 4
	SCENE 		= 22,		// 15
	NO 				= 23,		// 0
	
	//GATE 6
	SINGLE 		= 24,		// 15
	
	//GATE 7
	TOTAL 		= 25,		// 15
	
}VFDWordTypedef;



//VFD 驱动初始化
void VFDDriverInit(void);
//VFD显示刷新
void VFDDsaplayUpdate(void);
//VFD数码管控制
void VFDNumSegment(uint8_t * num);
//VFD 显示段全控
void VFDAllSegment(uint8_t cmd);
//驱动板 led组控制
void LEDGroupCtrl(uint8_t num,uint8_t cmd);
//VFD下方数字组 控制  1-20
void VFDDownNumCtrl(uint8_t num,uint8_t cmd);
//vfd 右侧显示数字和数字边框的控制 seg 0 1 2 num （） ↑
void RightNumCtrl(uint8_t num,uint8_t seg,uint8_t cmd);
//vfd 数字间隔符号控制
void NumSymbolCtrl(uint8_t num,uint8_t cmd);
//vfd 关键字高亮显示控制
void WordDisCtrl(VFDWordTypedef num,uint8_t cmd);

void Test(void) ;


#endif /*__VFDDRIVER_H*/


