/**
******************************************************************************
* @file     ac_app_main.c
* @authors  cxy
* @version  V1.0.0
* @date     10-Sep-2014
* @brief   
******************************************************************************
*/

#include <ac_common.h>
#include <ac_protocol_interface.h>
#include <ac_api.h>
#include <ac_hal.h>
#include <ac_cfg.h>
//#include "utils/uartstdio.h"

#include "usart2.h"	  
#include "led.h"	  
#include "stm32f10x.h"
#include <stdio.h>

u8  g_u8EqVersion[AC_EQVERSION_LEN]={0,0,0,0};      
u8  g_u8ModuleKey[AC_MODULE_KEY_LEN] = DEFAULT_IOT_PRIVATE_KEY;
u64  g_u64Domain = ((((u64)MAJOR_DOMAIN_ID)<<16) + (u64)SUB_DOMAIN_ID) ;
u8  g_u8DeviceId[AC_HS_DEVICE_ID_LEN];
u32 g_u32CloudStatus = CLOUDDISCONNECT;
typedef struct tag_STRU_LED_ONOFF
{		
    u8	     u8LedOnOff ; // 0:关，1：开，2：获取当前开关状态
    u8	     u8Pad[3];		 
}STRU_LED_ONOFF;

u32 g_u32WifiPowerStatus = WIFIPOWEROFF;

extern u8 g_u8MsgBuildBuffer[AC_MESSAGE_MAX_LEN];


/*************************************************
* Function: AC_SendDevStatus2Server
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void AC_SendLedStatus2Server(void)
{
    STRU_LED_ONOFF struRsp;
    u16 u16DataLen;
    struRsp.u8LedOnOff = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);
//    struRsp.u8LedOnOff = struRsp.u8LedOnOff>>2;
    AC_BuildMessage(MSG_SERVER_CLIENT_GET_LED_STATUS_RSP,0,
                    (u8*)&struRsp, sizeof(STRU_LED_ONOFF),
                    NULL, 
                    g_u8MsgBuildBuffer, &u16DataLen);
    AC_SendMessage(g_u8MsgBuildBuffer, u16DataLen);
}
/*************************************************
* Function: AC_Init
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void AC_Init(void)
{
    u8 u8DevceIdLen = sizeof(DEVICE_ID) -1;
    AC_SendDeviceStart(NULL);
    memset(g_u8DeviceId,0x30,sizeof(g_u8DeviceId));
    memcpy(g_u8DeviceId+(AC_HS_DEVICE_ID_LEN -u8DevceIdLen),DEVICE_ID,u8DevceIdLen);
}

/*************************************************
* Function: AC_ConfigWifi
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void AC_ConfigWifi(void)
{
    AC_Configuration struConfig;
    u8 u8CloudKey[AC_CLOUD_KEY_MAX_LEN] = DEFAULT_IOT_CLOUD_KEY;
    struConfig.u32TraceSwitch = AC_HTONL(0);     //Trace data switch, 1:open, 0:close,default 0
    struConfig.u32SecSwitch = AC_HTONL(0);       //Sec data switch, 1:open, 0:close, 2:close RSA, default 1
    struConfig.u32WifiConfig =  AC_HTONL(0);      //Use Config SSID,password,1:open, 0:close, default 0
    struConfig.u32TestAddrConfig = AC_HTONL(0);      //connect with test url,1:open, 0:close, default 0
    memcpy(struConfig.u8Ssid, "HW_test", 7);
    memcpy(struConfig.u8Password, "87654321", 8);
    struConfig.u32IpAddr = AC_HTONL(0xc0a8c772);  //local test ip
    struConfig.u16Port = AC_HTONS(9100); 
    //struConfig.u16Port = AC_HTONS(4000); 
    memcpy(struConfig.u8CloudAddr, "test.ablecloud.cn", AC_CLOUD_ADDR_MAX_LEN);
    memcpy(struConfig.u8CloudKey, u8CloudKey, AC_CLOUD_KEY_MAX_LEN);

    AC_SendDeviceConfig(NULL, &struConfig);
}
/*************************************************
* Function: AC_SendMessage
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void AC_SendMessage(u8 *pu8Msg, u16 u16DataLen)
{
    u8 u8MagicHead[4] = {0x02,0x03,0x04,0x05};
//    IntMasterDisable();
    Usart2_Send_Array ( u8MagicHead,  sizeof(u8MagicHead));
    Usart2_Send_Array ( pu8Msg,  u16DataLen);
//    IntMasterEnable();
}

/*************************************************
* Function: AC_DealNotifyMessage
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void AC_DealNotifyMessage(AC_MessageHead *pstruMsg, AC_OptList *pstruOptList, u8 *pu8Playload)
{
    
    //处理wifi模块的通知类消息
    switch(pstruMsg->MsgCode)
    {
        case AC_CODE_EQ_DONE://wifi模块启动通知
        AC_StoreStatus(WIFIPOWERSTATUS , WIFIPOWERON);
        AC_ConfigWifi();
        Usart2_Send_Str("Wifi Power On!\n");
        break;
        case AC_CODE_WIFI_CONNECTED://wifi连接成功通知
        //AC_ConfigWifi();
        AC_SendDeviceRegsiter(NULL, g_u8EqVersion,g_u8ModuleKey,g_u64Domain,g_u8DeviceId);
        Usart2_Send_Str("Wifi Connect!\n");
        break;
        case AC_CODE_CLOUD_CONNECTED://云端连接通知
        AC_StoreStatus(CLOUDSTATUS,CLOUDCONNECT);
        Usart2_Send_Str("Cloud Connect!\n");
        break;
        case AC_CODE_CLOUD_DISCONNECTED://云端断链通知
        AC_StoreStatus(CLOUDSTATUS,CLOUDDISCONNECT);
        Usart2_Send_Str("Cloud DisConnect!\n");
        break;
    }
}

/*************************************************
* Function: AC_DealOtaMessage
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void AC_DealOtaMessage(AC_MessageHead *pstruMsg, AC_OptList *pstruOptList, u8 *pu8Playload)
{
    //处理wifi模块的OTA类消息，本Demo没有实现该功能
    AC_SendAckMsg(NULL, pstruMsg->MsgId);
}

/*************************************************
* Function: AC_DealEvent
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void AC_DealEvent(AC_MessageHead *pstruMsg, AC_OptList *pstruOptList, u8 *pu8Playload)
{   
    //处理设备自定义控制消息
    u16 u16DataLen;
    u8 test[] = "hello";
    switch(pstruMsg->MsgCode)
    {
        case MSG_SERVER_CLIENT_SET_LED_ONOFF_REQ:
        {
            AC_DealLed(pstruMsg, pstruOptList, pu8Playload);
        }
        break;
    }
        //AC_SendAckMsg(pstruOptList, pstruMsg->MsgId);
    AC_BuildMessage(CLIENT_SERVER_OK,pstruMsg->MsgId,
                    (u8*)test, sizeof(test),
                    pstruOptList, 
                    g_u8MsgBuildBuffer, &u16DataLen);
    AC_SendMessage(g_u8MsgBuildBuffer, u16DataLen);
}

/*************************************************
* Function: AC_DealEvent
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void AC_StoreStatus(u32 u32Type , u32 u32Data)
{
    
    switch(u32Type)
    {
        case CLOUDSTATUS:
        g_u32CloudStatus = u32Data;
        break;
        case WIFIPOWERSTATUS:
        g_u32WifiPowerStatus = u32Data;
        break;
    }
}
/*************************************************
* Function: AC_BlinkLed
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void AC_BlinkLed(unsigned char blink)
{
	if (blink)
	{
	GPIO_ResetBits(GPIOA,GPIO_Pin_8); //LED0输出低
	}
	else
		GPIO_SetBits(GPIOA,GPIO_Pin_8);//LED0输出高
	
//		LED1_ON(); //LED1输出低
}
/*************************************************
* Function: AC_DealLed
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
void AC_DealLed(AC_MessageHead *pstruMsg, AC_OptList *pstruOptList, u8 *pu8Playload)
{

    STRU_LED_ONOFF struRsp;
    u16 u16DataLen;


    switch (((STRU_LED_ONOFF *)pu8Playload)->u8LedOnOff)
    {
        case 0://处理开关消息
        case 1:        
            AC_BlinkLed(((STRU_LED_ONOFF *)pu8Playload)->u8LedOnOff);
            break;
        case 2://获取灯状态
        {
            AC_BuildMessage(MSG_SERVER_CLIENT_GET_LED_STATUS_RSP,pstruMsg->MsgId,
                (u8*)&struRsp, sizeof(STRU_LED_ONOFF),
                pstruOptList, 
                g_u8MsgBuildBuffer, &u16DataLen);
            AC_SendMessage(g_u8MsgBuildBuffer, u16DataLen);
            break;
        }
        
    }
}
/*************************************************
* Function: AC_DealEvent
* Description: 
* Author: cxy 
* Returns: 
* Parameter: 
* History:
*************************************************/
u32 AC_GetStoreStatus(u32 u32Type)
{
	u32	mid_redata;
	switch(u32Type)
	{
			case CLOUDSTATUS:
			{
				mid_redata = g_u32CloudStatus;
			}
			break;
			case WIFIPOWERSTATUS:
			{
				mid_redata = g_u32WifiPowerStatus;
			}
			break;
			default:
				mid_redata = AC_RET_ERROR;
			break;
	}
	return mid_redata;
}

