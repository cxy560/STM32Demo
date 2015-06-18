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

	/*�����ж����ȼ�����*/
void NVIC_Configuration(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ�����ȼ�
}

int main(void)
{

	NVIC_Configuration();															//�����ж����ȼ�����
	usart1_init(115200);															//����1��ʼ��,���ò�����Ϊ115200
	
	usart2_init(115200);															//����2��ʼ��,���ò�����Ϊ115200

	usart3_init(115200);															//����3��ʼ��,���ò�����Ϊ9600

	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();
	while(WIFIPOWEROFF==AC_GetStoreStatus(WIFIPOWERSTATUS))
	{
		AC_Init();
		delay_s(1);//��ʱ
		ProcessWifiMsg();
	}
	while(1)
	{
		ProcessWifiMsg();
	}
}
