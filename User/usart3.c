/***********************串口1.2.3发送接收数据*************************/

#include "stm32f10x.h"
#include <stdio.h>
#include "usart3.h"	  

u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART3_REC_LEN个字节.
u16 USART3_RX_STA=0;       //接收状态标记	  

/******串口3初始化设置,以及数据接收发送设置******/
	/*串口3初始化设置*/
void usart3_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//使能复用IO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3时钟
	USART_DeInit(USART3);  																	//复位串口3
	
	/*GPIO端口设置*/
	/*USART3_TX   PB.10*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 					//PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); 							//初始化PB10
   
	/*USART3_RX	  PB.11*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 						//PB.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);  							//初始化PB11

	/*Usart3 NVIC 配置*/
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;				//选择串口3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;				//亚优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化VIC寄存器
  
	/*USART3 初始化设置*/
	USART_InitStructure.USART_BaudRate = bound;																			//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;													//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;															//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//使能收发模式

	USART_Init(USART3, &USART_InitStructure); 			//初始化串口3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//开启接收中断
	USART_Cmd(USART3, ENABLE);                    	//使能串口3
}

	/*串口3发送一个字节*/
void Usart3_Send_Ch(u8 ch)
{
  while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	// 查看发送数据寄存器是否为空
	USART_SendData(USART3,ch);																		// 发送数据
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);   // 等待数据发送完毕,检查USART_FLAG_TC是否置1
}

	/*串口3发送一个字符串*/
void Usart3_Send_Str(u8 *str)
{
	u8 i=0;
	while(str[i]!='\0')
	{
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	// 查看发送数据寄存器是否为空
		USART_SendData(USART3,str[i]);																// 发送数据
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);   // 等待数据发送完毕,检查USART_FLAG_TC是否置1
		i++;
	}
}

	/*串口3发送数组数据*/
void Usart3_Send_Array(u8 send_array[],u8 num) //send_array[]代表要发送的数组的数组数据,//num代表要发送的数据个数，即就是发送该数组的0-num元素
{
	u8 i=0;
	while(i<num)
	{
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	// 查看发送数据寄存器是否为空
		USART_SendData(USART3,send_array[i]);													// 发送数据
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);   // 等待数据发送完毕,检查USART_FLAG_TC是否置1
		i++;
	}
}
#if EN_USART3_RX	//如果使能了接收
	/*串口3数据中断接收方式*/
void USART3_IRQHandler(void)                								//串口1中断服务程序
{
	u8 USART3_Rx;
	void Usart3_Send_Ch(u8 byte);														//发送字节
	while(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);					//清除中断标志
		USART3_Rx=USART_ReceiveData(USART3);										//(RxData=USART3->DR);读取接收到的数据
		Usart3_Send_Ch(USART3_Rx);
	}
} 
#endif	
