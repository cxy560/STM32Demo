#include "stm32f10x.h"
#include <stdio.h>
#include "usart1.h"	  
#include "usart2.h"	  
#include "usart3.h"	  
#include "led.h"	  
#include "key.h"
#include "delay_y.h"	

/*AbleCloud*/
#include "ac_common.h"
#include "ac_hal.h"

	/*设置中断优先级分组*/
void NVIC_Configuration(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	//设置NVIC中断分组2:2位抢占优先级，2位亚优先级
}

int main(void)
{

	NVIC_Configuration();															//设置中断优先级分组
	usart1_init(115200);															//串口1初始化,设置波特率为115200
	
	usart2_init(115200);															//串口2初始化,设置波特率为115200

	usart3_init(115200);															//串口3初始化,设置波特率为9600

	LED_Init();		  	//初始化与LED连接的硬件接口
	KEY_Init();
	while(WIFIPOWEROFF==AC_GetStoreStatus(WIFIPOWERSTATUS))
	{
		AC_Init();
		delay_s(1);//延时
		ProcessWifiMsg();
	}
	while(1)
	{
		ProcessWifiMsg();
	}
}
