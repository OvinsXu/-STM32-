#ifndef  __TEST_H
#define	 __TEST_H



#include "stm32f10x.h"



/********************************** 用户需要设置的参数**********************************/
#define      macUser_ESP8266_ApSsid                       "iPhone"                //要连接的热点的名称
#define      macUser_ESP8266_ApPwd                        "xwt123456"           //要连接的热点的密钥

#define      macUser_ESP8266_TcpServer_IP                 "192.168.43.37"      //要连接的服务器的 IP
#define      macUser_ESP8266_TcpServer_Port               "3000"               //要连接的服务器的端口



/********************************** 外部全局变量 ***************************************/
extern volatile uint8_t ucTcpClosedFlag;



/********************************** 测试函数声明 ***************************************/
void                     ESP8266_StaTcpClient_UnvarnishTest  ( void );



#endif

