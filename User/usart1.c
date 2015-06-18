/***********************串口1.2.3发送接收数据*************************/

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "usart1.h"	  

u8 USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART1_REC_LEN个字节.
u16 USART1_RX_STA=0;       //接收状态标记	  

/******串口1初始化设置,以及数据接收发送设置******/
	/*串口1初始化设置*/
void usart1_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//使能复用IO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//使能USART1时钟
	USART_DeInit(USART1);  																	//复位串口1
	
	/*GPIO端口设置*/
	/*USART1_TX   PA.9*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 					//PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); 							//初始化PA9
   
	/*USART1_RX	  PA.10*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 						//PA.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  							//初始化PA10

	/*Usart1 NVIC 配置*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				//选择串口1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;				//亚优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化VIC寄存器
  
	/*USART1 初始化设置*/
	USART_InitStructure.USART_BaudRate = bound;																			//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;													//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;															//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//使能收发模式

	USART_Init(USART1, &USART_InitStructure); 			//初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//开启接收中断
	USART_Cmd(USART1, ENABLE);                    	//使能串口1
}

/*串口1发送一个16进制数据*/
void Usart1_Send_Data(u16 Data)
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// 查看发送数据寄存器是否为空
	USART_SendData(USART1,Data);																		// 发送数据
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);   // 等待数据发送完毕,检查USART_FLAG_TC是否置1
}

/*串口1发送一个字节*/
void Usart1_Send_Ch(u8 ch)
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// 查看发送数据寄存器是否为空
	USART_SendData(USART1,ch);																		// 发送数据
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);   // 等待数据发送完毕,检查USART_FLAG_TC是否置1
}

	/*串口1发送一个字符串*/
void Usart1_Send_Str(u8 *str)
{
	u8 i=0;
	while(str[i]!='\0')
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// 查看发送数据寄存器是否为空
		USART_SendData(USART1,str[i]);																// 发送数据
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);   // 等待数据发送完毕,检查USART_FLAG_TC是否置1
		i++;
	}
}

	/*串口1发送数组数据*/
void Usart1_Send_Array(u8 send_array[],u8 num) //send_array[]代表要发送的数组的数组数据,//num代表要发送的数据个数，即就是发送该数组的0-num元素
{
	u8 i=0;
	while(i<num)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// 查看发送数据寄存器是否为空
		USART_SendData(USART1,send_array[i]);													// 发送数据
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);   // 等待数据发送完毕,检查USART_FLAG_TC是否置1
		i++;
	}
}

#if EN_USART1_RX	//如果使能了接收
	/*串口1数据中断接收方式*/
void USART1_IRQHandler(void)                								//串口1中断服务程序
{
	u8 USART1_Rx;
	void Usart1_Send_Byte(u8 byte);														//发送字节
	while(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);					//清除中断标志
		USART1_Rx=USART_ReceiveData(USART1);										//(RxData=USART1->DR);读取接收到的数据

		USART1_RX_BUF[USART1_RX_STA&0X3FFF]=USART1_Rx ;
		USART1_RX_STA++;

	}
} 
#endif
