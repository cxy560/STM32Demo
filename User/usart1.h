#ifndef USART1_H
#define USART1_H
#include "stdio.h"	

#define USART1_REC_LEN  			50  	//定义最大接收字节数 50
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART1_REC_LEN个字节.末字节为换行符 
extern u16 USART1_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
extern void usart1_init(u32 bound);											//声明串口1初始化函数
extern void Usart1_Send_Data(u16 Data);									//声明串口1发送16进制数据函数
extern void Usart1_Send_Ch(u8 ch);											//声明串口1发送字节函数
extern void Usart1_Send_Str(u8 *str);										//声明串口1发送字符串函数
extern void Usart1_Send_Array(u8 send_array[],u8 num);	//声明串口1发送数组函数
#endif
