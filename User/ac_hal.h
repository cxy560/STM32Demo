/**
******************************************************************************
* @file     ac_hal.h
* @authors  cxy
* @version  V1.0.0
* @date     10-Sep-2014
* @brief    
******************************************************************************
*/

#ifndef  __AC_HAL_H__ 
#define  __AC_HAL_H__

#include <ac_common.h>
#include <ac_protocol_interface.h>
#include <ac_api.h>
//#include <Iot_Uart.h>
#define MSG_SERVER_CLIENT_SET_LED_ONOFF_REQ  (68)
#define MSG_CLIENT_SERVER_GET_DEV_STATUS_RSP (201)
#define MSG_SERVER_CLIENT_GET_LED_STATUS_RSP (203) 
#define CLIENT_SERVER_OK  (102)   
#define CLOUDSTATUS 0
#define WIFIPOWERSTATUS 1

#define CLOUDCONNECT 1 
#define CLOUDDISCONNECT 0

#define AC_Printf Usart2_Send_Str
#define WIFIPOWERON 1
#define WIFIPOWEROFF 0
#ifdef __cplusplus
extern "C" {
#endif
void AC_Init(void);
void AC_SendMessage(u8 *pu8Msg, u16 u16DataLen);
void AC_DealNotifyMessage(AC_MessageHead *pstruMsg, AC_OptList *pstruOptList, u8 *pu8Playload);
void AC_DealOtaMessage(AC_MessageHead *pstruMsg, AC_OptList *pstruOptList,  u8 *pu8Playload);
void AC_DealEvent(AC_MessageHead *pstruMsg, AC_OptList *pstruOptList,u8 *pu8Playload);
u32  AC_GetStoreStatus(u32 u32Type);
unsigned short crc16_ccitt(const unsigned char *buf, unsigned int len);	
void AC_StoreStatus(u32 u32Type , u32 u32Data);
void AC_SendDevStatus2Server(void);
void AC_SendLedStatus2Server(void);
void AC_DealLed(AC_MessageHead *pstruMsg, AC_OptList *pstruOptList, u8 *pu8Playload);
void AC_BlinkLed(unsigned char blink);
void AC_SendDeviceConfig(AC_OptList *pstruOptList, AC_Configuration *pstruConfig);
#ifdef __cplusplus
}
#endif
#endif
/******************************* FILE END ***********************************/
