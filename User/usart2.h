#ifndef USART2_H
#define USART2_H
#include "stdio.h"	

#include "bmd.h"
#include "ac_api.h"
#include <ac_common.h>
#include <ac_protocol_interface.h>

#define USART2_REC_LEN  			100  	//定义最大接收字节数 100
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口2接收

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



extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART2_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记

//如果想串口中断接收，请不要注释以下宏定义
extern void usart2_init(u32 bound);											//声明串口2初始化函数
extern void Usart2_Send_Data(u16 Data);									//声明串口2发送16进制数据函数
extern void Usart2_Send_Ch(u8 ch);											//声明串口2发送字节函数
extern void Usart2_Send_Str(u8 *str);										//声明串口2发送字符串函数
extern void Usart2_Send_Array(u8 send_array[],u8 num);	//声明串口2发送数组函数

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
    u32 MsgFlag;         //消息标示 byte0:0X76, byte1:0x81, byte2:0x43, byte3:0x50 0x76814350
    AC_MessageHead  struMessageHead;
}RCTRL_STRU_MSGHEAD;    //消息头定义

//typedef struct tag_ARM_ZIGB_MSG_HEAD
//{
//    u16 MsgType;        //消息类型
//    u16 MsgLen;
//}ARM_ZIGB_MSG_HEAD;    //消息头定义

//typedef struct tag_ARM_ZIGB_MSG_HEADER
//{
//    s8 MsgFlag[4];         //消息标示 byte0:0X76, byte1:0x81, byte2:0x43, byte3:0x50 0x76814350
//    ARM_ZIGB_MSG_HEAD MsgHead;
//}ARM_ZIGB_MSG_HEADER;    //消息头定义

//typedef struct tag_ZIGB_ARM_QUERY_MSG_RSP
//{
//   ARM_ZIGB_MSG_HEAD MsgHead;
//    u8  MachineType;        //主子机状态0:子机,1:主机
//    u8 sn[8];             //sn
//}ZIGB_ARM_QUERY_MSG_RSP;    //消息头定义

//typedef struct tag_SET_NET_ACCESS_REQ
//{        //消息标示 byte0:0X76, byte1:0x81, byte2:0x43, byte3:0x50 0x76814350
//    ARM_ZIGB_MSG_HEADER MsgHead;
//    u8 u8AccessTime; //单位:s 0,关闭,ff打开,中间值是时间
//}ARM_ZIGB_SET_ACCESS_TIMER_REQ;    //消息头定义

//typedef struct tag_SET_CLIENT_LIST
//{
//    ARM_ZIGB_MSG_HEADER MsgHeader;
//    u8 u8BlackClientNum;
//    u8 u8WhiteClientNum;    
//    u8 u8Pad[2];
//    u8 DeviceId[0];     //用户ID,定长AC_HS_DEVICE_ID_LEN(8字节),依次下排
//}ARM_ZIGB_CLIENT_LIST;    //消息头定义




//typedef struct tag_STRU_SERVER_CLIENT_SET_REPROT_PERIOD_REQ
//{
//    u32      u32Period;        // 单位:秒
//}STRU_SERVER_CLIENT_SET_REPROT_PERIOD_REQ;

//typedef struct tag_STRU_SERVER_CLIENT_SET_TH_REQ
//{	
//    u32      u32Type;          //上报数据类型
//    u32      u32Threshold;    //上报阀值
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
