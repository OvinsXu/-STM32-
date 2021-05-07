#include "test.h"
#include "bsp_esp8266.h"
#include "bsp_SysTick.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "bsp_dht11.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_http.h"

/**
  * @brief  获取网络调试助手和串口调试助手发来的信息
  * @param  无
  * @retval 无
  */
volatile uint8_t ucTcpClosedFlag = 0;

char cStr[1500] = {0}; //保存用于发送的数据

/**
  * @brief  ESP8266 （Sta Tcp Client）透传
  * @param  无
  * @retval 无
  */
void ESP8266_StaTcpClient_UnvarnishTest(void)
{
	uint8_t ucStatus;			   //保存连接状态信息
	DHT11_Data_TypeDef DHT11_Data; //获取温湿度信息

	printf("\r\n正在配置 ESP8266 ......\r\n");

	macESP8266_CH_ENABLE();

	ESP8266_AT_Test(); //测试AT指令

	ESP8266_Net_Mode_Choose(STA); //选择模式(wifi模式/热点模式)

	while (!ESP8266_JoinAP(macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd))
		; //连接WiFi

	ESP8266_Enable_MultipleId(DISABLE); //关闭多连接模式
	//连接服务器,参数:1.协议,2.目标IP,3.目标端口,4.单连接用Single_ID_0
	while (!ESP8266_Link_Server(enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0))
		;

	while (!ESP8266_UnvarnishSend())
		; //进入透传模式

	printf("\r\n配置 ESP8266 完毕\r\n");
	printf("\r\n开始透传......\r\n");

	while (1)
	{
		//读取 DHT11 温湿度信息,输出到串口
		if (DHT11_Read_TempAndHumidity(&DHT11_Data) == SUCCESS)
		{
			sprintf(cStr, "\r\n读取DHT11成功!\r\n\r\n湿度为%d.%d ％RH ，温度为 %d.%d℃ \r\n",
					DHT11_Data.humi_int, DHT11_Data.humi_deci, DHT11_Data.temp_int, DHT11_Data.temp_deci);
		}
		else
		{
			sprintf(cStr, "读取温湿度失败!\r\n");
		}
		printf("%s", cStr); //打印读取 DHT11 温湿度信息

		//发送到web服务器		
		sprintf(cStr, "/StmSet?temp=%d.%d&humi=%d.%d",
			DHT11_Data.temp_int, DHT11_Data.temp_deci, DHT11_Data.humi_int, DHT11_Data.humi_deci);
		
		//将温湿度发送到Web服务器
		http_Get(cStr,macUser_ESP8266_TcpServer_IP,macUser_ESP8266_TcpServer_Port);
		
		
		

		//printf("%s", cStr);
		//ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //发送 DHT11 温湿度信息到网络调试助手

		Delay_ms(2000);

		if (ucTcpClosedFlag) //检测是否失去连接
		{
			ESP8266_ExitUnvarnishSend(); //退出透传模式

			do
				ucStatus = ESP8266_Get_LinkStatus(); //获取连接状态
			while (!ucStatus);

			if (ucStatus == 4) //确认失去连接后重连
			{
				printf("\r\n正在重连热点和服务器 ......\r\n");

				while (!ESP8266_JoinAP(macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd))
					;

				while (!ESP8266_Link_Server(enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0))
					;

				printf("\r\n重连热点和服务器成功\r\n");
			}

			while (!ESP8266_UnvarnishSend())
				;
		}
	}
}
