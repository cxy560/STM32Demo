#ifndef USART2_H
#define USART2_H
#include "stdio.h"	

#include "bmd.h"
#include "ac_api.h"
#include <ac_common.h>
#include <ac_protocol_interface.h>

#define USART2_REC_LEN  			100  	//�����������ֽ��� 100
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������2����

/*AC*/
#define RCTRL_MSG_FLAG		       0x02030405
#define RCTRL_MSG_PREFIX		"\2\3\4\5"
#define AT_CMD_PREFIX			"AT#"
#define ARM_ZIGBEE_MSG_FLAG		"ARM#"
#define PRINT_FLAG                      "\1\2\3\4"

#define NUM_SSI_DATA 3
#define FLASH_SN_ADDR ((u32)&(g_u64_air_machine_sn))

//how many packets can be buffered in rxring ,(each packet size need < UARTRX_RING_LEN - 1)
#define    NUM_DESCS           30

//buffer length for uart rx buffer whose data is moved from uart UART HW RX FIFO
#define    USART2RX_RING_LEN    1024   
#define    AC_PAYLOADLENOFFSET 9

//#define    UART_BAUTRATE       115200
//#define    UART0_INTPRIO       6
//#define    UART1_INTPRIO       6



extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART2_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         		//����״̬���

//����봮���жϽ��գ��벻Ҫע�����º궨��
extern void usart2_init(u32 bound);											//��������2��ʼ������
extern void Usart2_Send_Data(u16 Data);									//��������2����16�������ݺ���
extern void Usart2_Send_Ch(u8 ch);											//��������2�����ֽں���
extern void Usart2_Send_Str(u8 *str);										//��������2�����ַ�������
extern void Usart2_Send_Array(u8 send_array[],u8 num);	//��������2�������麯��

typedef enum
{
    OPT_TRANSPORT = 0
}RCTRL_OPT_TYPE;


typedef enum
{
    ARM_ZIGB_QUERY_MAINNODE_ADDR_REQ = 0,
    ARM_ZIGB_SET_NET_ACCESS_REQ = 1,
    ARM_ZIGB_SET_CLIENT_LIST_REQ = 2,
    ZIGB_ARM_QUERY_MAINNODE_ADDR_RSP = 0x1000,
}ARM_ZIGBEE_MSG_TYPE;

typedef enum {
    PKT_UNKNOWN,
    PKT_ATCMD,
    PKT_PUREDATA,
    PKT_ZADATA,
    PKT_PRINTCMD,
    PKT_HR01DATA
} PKT_TYPE;

typedef struct tag_RCTRL_STRU_MSGHEAD
{
    u32 MsgFlag;         //��Ϣ��ʾ byte0:0X76, byte1:0x81, byte2:0x43, byte3:0x50 0x76814350
    AC_MessageHead  struMessageHead;
}RCTRL_STRU_MSGHEAD;    //��Ϣͷ����

//typedef struct tag_ARM_ZIGB_MSG_HEAD
//{
//    u16 MsgType;        //��Ϣ����
//    u16 MsgLen;
//}ARM_ZIGB_MSG_HEAD;    //��Ϣͷ����

//typedef struct tag_ARM_ZIGB_MSG_HEADER
//{
//    s8 MsgFlag[4];         //��Ϣ��ʾ byte0:0X76, byte1:0x81, byte2:0x43, byte3:0x50 0x76814350
//    ARM_ZIGB_MSG_HEAD MsgHead;
//}ARM_ZIGB_MSG_HEADER;    //��Ϣͷ����

//typedef struct tag_ZIGB_ARM_QUERY_MSG_RSP
//{
//   ARM_ZIGB_MSG_HEAD MsgHead;
//    u8  MachineType;        //���ӻ�״̬0:�ӻ�,1:����
//    u8 sn[8];             //sn
//}ZIGB_ARM_QUERY_MSG_RSP;    //��Ϣͷ����

//typedef struct tag_SET_NET_ACCESS_REQ
//{        //��Ϣ��ʾ byte0:0X76, byte1:0x81, byte2:0x43, byte3:0x50 0x76814350
//    ARM_ZIGB_MSG_HEADER MsgHead;
//    u8 u8AccessTime; //��λ:s 0,�ر�,ff��,�м�ֵ��ʱ��
//}ARM_ZIGB_SET_ACCESS_TIMER_REQ;    //��Ϣͷ����

//typedef struct tag_SET_CLIENT_LIST
//{
//    ARM_ZIGB_MSG_HEADER MsgHeader;
//    u8 u8BlackClientNum;
//    u8 u8WhiteClientNum;    
//    u8 u8Pad[2];
//    u8 DeviceId[0];     //�û�ID,����AC_HS_DEVICE_ID_LEN(8�ֽ�),��������
//}ARM_ZIGB_CLIENT_LIST;    //��Ϣͷ����




//typedef struct tag_STRU_SERVER_CLIENT_SET_REPROT_PERIOD_REQ
//{
//    u32      u32Period;        // ��λ:��
//}STRU_SERVER_CLIENT_SET_REPROT_PERIOD_REQ;

//typedef struct tag_STRU_SERVER_CLIENT_SET_TH_REQ
//{	
//    u32      u32Type;          //�ϱ���������
//    u32      u32Threshold;    //�ϱ���ֵ
//} STRU_SERVER_CLIENT_SET_TH_REQ;


typedef struct {
    PKT_TYPE pkt_type;
    u16   pkt_len;
}PKT_FIFO;//packet infor is in sequence with index[0,num_desc-1] which mapping the sequence in rx


/*
* 2014/05/13,terrence
* struct to descrypt the packet in ring buffer
* we just need to know the packet number and their lengh in sequence
*/
typedef struct {
    PKT_TYPE  cur_type;              //receiving packet:which packet type is receiving current? 
    u8     cur_num;               //receiving packet:current index of receiving packet
    u8     pkt_num;               //completely packets:packet number in ring buffer
    PKT_FIFO  infor[NUM_DESCS];      //completely packets:FIFO,packet infor in ring
} PKT_DESC; 


typedef struct
{
    BUFFER_INFO                    Rx_Buffer;  //receive buffer
    PKT_DESC                       Rx_desc;    //description       
    
    BUFFER_INFO                    Tx_Buffer;  //transmit buffer
    
} UARTStruct;

// ================================================================
//				Golbal	Variable Definition
// ================================================================

extern u8 g_u8_wifi_powstatus;

extern u8 g_u8_machineType;
//char Zigbee_Arm_Flag[] = ZIGBEE_ARM_MSG_FLAG;
//*****************************************************************************
//
//! fuction
//
//*****************************************************************************

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//extern void UartSend(u8  *Buffer, u16  NByte);
//extern void UartInit();
//extern void SendMsg2Zigbee( u8  *Buffer, u16  NByte);
//extern void SendMsg2Wifi( u8  *Buffer, u16  NByte);
//extern u32 get_register_status();
extern void ProcessWifiMsg(void);
//extern void ProcessZigbeeMsg();
//extern void Send2ServerCommonMsg(u32 Base,u8 Version,u8 MsgCode,u8 MsgId);
//extern u8 get_cloud_connect_status();
//extern void SendDevStatus2Server(u32 Base, u8 Version,u8 MsgId);
#endif
