/**
 * @file bsp_http.c
 * @author ovins@outlook.com
 * @brief 模拟http协议，向服务器发送数据
 * @version 0.1
 * @date 2021-04-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <string.h>
#include "bsp_http.h"
#include "bsp_usart.h"
#include "bsp_dht11.h"
#include "bsp_http.h"
#include "bsp_esp8266.h"

/**
 * @brief 通过IP,端口和
 * 
 * @param query 
 * @param Host_IP 
 * @param Host_Port 
 * 
 */
void http_Get(char *query, char *Host_IP, char *Host_Port)
{
	ESP8266_SendString(ENABLE, "GET ", 0, Single_ID_0);
	ESP8266_SendString(ENABLE, query, 0, Single_ID_0);
	ESP8266_SendString(ENABLE, " HTTP/1.1\r\n", 0, Single_ID_0);

	ESP8266_SendString(ENABLE, "Host: ", 0, Single_ID_0);
	ESP8266_SendString(ENABLE, Host_IP, 0, Single_ID_0);
	ESP8266_SendString(ENABLE, ":", 0, Single_ID_0);
	ESP8266_SendString(ENABLE, Host_Port, 0, Single_ID_0);
	ESP8266_SendString(ENABLE, "\r\n\r\n", 0, Single_ID_0);
	
}
/**
 * @brief 
 * 
 * @param query 
 * @param HostName 
 */
void http_Get_ByHostName(char *query, char *HostName)
{
	ESP8266_SendString(ENABLE, "GET ", 0, Single_ID_0);
	ESP8266_SendString(ENABLE, query, 0, Single_ID_0);
	ESP8266_SendString(ENABLE, " HTTP/1.1\r\n", 0, Single_ID_0);

	ESP8266_SendString(ENABLE, "Host: ", 0, Single_ID_0);
	ESP8266_SendString(ENABLE, HostName, 0, Single_ID_0);
	ESP8266_SendString(ENABLE, "\r\n\r\n", 0, Single_ID_0);
	
	//printf("%s",ESP8266_ReceiveString(ENABLE));
}

