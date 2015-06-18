#ifndef USART3_H
#define USART3_H
#include "stdio.h"	

#define USART3_REC_LEN  			20  	//定义最大接收字节数 20
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口3接收
	  	
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART3_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
extern void usart3_init(u32 bound);											//声明串口3初始化函数
extern void Usart3_Send_Ch(u8 ch);											//声明串口3发送字节函数
extern void Usart3_Send_Str(u8 *str);										//声明串口3发送字符串函数
extern void Usart3_Send_Array(u8 send_array[],u8 num);	//声明串口3发送数组函数
#endif
