/***********************����1.2.3���ͽ�������*************************/

#include "stm32f10x.h"
#include <stdio.h>
#include "usart3.h"	  

u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART3_REC_LEN���ֽ�.
u16 USART3_RX_STA=0;       //����״̬���	  

/******����3��ʼ������,�Լ����ݽ��շ�������******/
	/*����3��ʼ������*/
void usart3_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//ʹ�ܸ���IOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3ʱ��
	USART_DeInit(USART3);  																	//��λ����3
	
	/*GPIO�˿�����*/
	/*USART3_TX   PB.10*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 					//PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure); 							//��ʼ��PB10
   
	/*USART3_RX	  PB.11*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 						//PB.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);  							//��ʼ��PB11

	/*Usart3 NVIC ����*/
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;				//ѡ�񴮿�3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;				//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);													//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	/*USART3 ��ʼ������*/
	USART_InitStructure.USART_BaudRate = bound;																			//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;													//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;															//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//ʹ���շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); 			//��ʼ������3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//���������ж�
	USART_Cmd(USART3, ENABLE);                    	//ʹ�ܴ���3
}

	/*����3����һ���ֽ�*/
void Usart3_Send_Ch(u8 ch)
{
  while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	// �鿴�������ݼĴ����Ƿ�Ϊ��
	USART_SendData(USART3,ch);																		// ��������
  while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);   // �ȴ����ݷ������,���USART_FLAG_TC�Ƿ���1
}

	/*����3����һ���ַ���*/
void Usart3_Send_Str(u8 *str)
{
	u8 i=0;
	while(str[i]!='\0')
	{
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	// �鿴�������ݼĴ����Ƿ�Ϊ��
		USART_SendData(USART3,str[i]);																// ��������
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);   // �ȴ����ݷ������,���USART_FLAG_TC�Ƿ���1
		i++;
	}
}

	/*����3������������*/
void Usart3_Send_Array(u8 send_array[],u8 num) //send_array[]����Ҫ���͵��������������,//num����Ҫ���͵����ݸ����������Ƿ��͸������0-numԪ��
{
	u8 i=0;
	while(i<num)
	{
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	// �鿴�������ݼĴ����Ƿ�Ϊ��
		USART_SendData(USART3,send_array[i]);													// ��������
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);   // �ȴ����ݷ������,���USART_FLAG_TC�Ƿ���1
		i++;
	}
}
#if EN_USART3_RX	//���ʹ���˽���
	/*����3�����жϽ��շ�ʽ*/
void USART3_IRQHandler(void)                								//����1�жϷ������
{
	u8 USART3_Rx;
	void Usart3_Send_Ch(u8 byte);														//�����ֽ�
	while(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);					//����жϱ�־
		USART3_Rx=USART_ReceiveData(USART3);										//(RxData=USART3->DR);��ȡ���յ�������
		Usart3_Send_Ch(USART3_Rx);
	}
} 
#endif	
