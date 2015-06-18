#ifndef USART3_H
#define USART3_H
#include "stdio.h"	

#define USART3_REC_LEN  			20  	//�����������ֽ��� 20
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������3����
	  	
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //���ջ���,���USART3_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART3_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
extern void usart3_init(u32 bound);											//��������3��ʼ������
extern void Usart3_Send_Ch(u8 ch);											//��������3�����ֽں���
extern void Usart3_Send_Str(u8 *str);										//��������3�����ַ�������
extern void Usart3_Send_Array(u8 send_array[],u8 num);	//��������3�������麯��
#endif
