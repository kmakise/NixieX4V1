#ifndef __VFDAPP_H
#define __VFDAPP_H

typedef enum
{
	NTIME = 0,		//常规时间显示
	NDATE = 1,		//常规日期显示
	TEMP  = 2,			//温度显示
	CLOSE = 3,		//关闭VFD显示 静默状态
	
}DisModTypedef;//显示模式状态类型

typedef enum
{
	NORMAL 				= 0,		//常规显示
	MAIN	 				= 1,		//主菜单 				顶级
		TIMEDATE		= 2,		//时间与日期 		二级
			TIME	 		= 3,		//时间修改 			三级
			DATE	 		= 4,		//日期修改 			三级
			DAY 		 	= 5,		//星期修改 			三级
		WLAN				= 6,		//无线局域网设置 二级
			SSID			= 7,		//SSID配置			三级
			PSWD			=	8,		//密码配置				三级
}CFGTypedef;



void VFDApp_Setup(void);
void VFDApp_Loop(void);
void VFDApp_Interrupt(void);

#endif /*__VFDAPP_H*/
