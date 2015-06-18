/***********************����1.2.3���ͽ�������*************************/

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "usart1.h"	  

u8 USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART1_REC_LEN���ֽ�.
u16 USART1_RX_STA=0;       //����״̬���	  

/******����1��ʼ������,�Լ����ݽ��շ�������******/
	/*����1��ʼ������*/
void usart1_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//ʹ�ܸ���IOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//ʹ��USART1ʱ��
	USART_DeInit(USART1);  																	//��λ����1
	
	/*GPIO�˿�����*/
	/*USART1_TX   PA.9*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 					//PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); 							//��ʼ��PA9
   
	/*USART1_RX	  PA.10*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 						//PA.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  							//��ʼ��PA10

	/*Usart1 NVIC ����*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				//ѡ�񴮿�1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;				//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);													//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	/*USART1 ��ʼ������*/
	USART_InitStructure.USART_BaudRate = bound;																			//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;													//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;															//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//ʹ���շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); 			//��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//���������ж�
	USART_Cmd(USART1, ENABLE);                    	//ʹ�ܴ���1
}

/*����1����һ��16��������*/
void Usart1_Send_Data(u16 Data)
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// �鿴�������ݼĴ����Ƿ�Ϊ��
	USART_SendData(USART1,Data);																		// ��������
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);   // �ȴ����ݷ������,���USART_FLAG_TC�Ƿ���1
}

/*����1����һ���ֽ�*/
void Usart1_Send_Ch(u8 ch)
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// �鿴�������ݼĴ����Ƿ�Ϊ��
	USART_SendData(USART1,ch);																		// ��������
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);   // �ȴ����ݷ������,���USART_FLAG_TC�Ƿ���1
}

	/*����1����һ���ַ���*/
void Usart1_Send_Str(u8 *str)
{
	u8 i=0;
	while(str[i]!='\0')
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// �鿴�������ݼĴ����Ƿ�Ϊ��
		USART_SendData(USART1,str[i]);																// ��������
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);   // �ȴ����ݷ������,���USART_FLAG_TC�Ƿ���1
		i++;
	}
}

	/*����1������������*/
void Usart1_Send_Array(u8 send_array[],u8 num) //send_array[]����Ҫ���͵��������������,//num����Ҫ���͵����ݸ����������Ƿ��͸������0-numԪ��
{
	u8 i=0;
	while(i<num)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// �鿴�������ݼĴ����Ƿ�Ϊ��
		USART_SendData(USART1,send_array[i]);													// ��������
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);   // �ȴ����ݷ������,���USART_FLAG_TC�Ƿ���1
		i++;
	}
}

#if EN_USART1_RX	//���ʹ���˽���
	/*����1�����жϽ��շ�ʽ*/
void USART1_IRQHandler(void)                								//����1�жϷ������
{
	u8 USART1_Rx;
	void Usart1_Send_Byte(u8 byte);														//�����ֽ�
	while(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);					//����жϱ�־
		USART1_Rx=USART_ReceiveData(USART1);										//(RxData=USART1->DR);��ȡ���յ�������

		USART1_RX_BUF[USART1_RX_STA&0X3FFF]=USART1_Rx ;
		USART1_RX_STA++;

	}
} 
#endif
