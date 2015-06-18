#ifndef USART1_H
#define USART1_H
#include "stdio.h"	

#define USART1_REC_LEN  			50  	//�����������ֽ��� 50
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART1_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
extern void usart1_init(u32 bound);											//��������1��ʼ������
extern void Usart1_Send_Data(u16 Data);									//��������1����16�������ݺ���
extern void Usart1_Send_Ch(u8 ch);											//��������1�����ֽں���
extern void Usart1_Send_Str(u8 *str);										//��������1�����ַ�������
extern void Usart1_Send_Array(u8 send_array[],u8 num);	//��������1�������麯��
#endif
