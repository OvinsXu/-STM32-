/**
 * @file main.c
 * @author ovins
 * @brief 
 * @version 0.1
 * @date 2021-04-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
 
#include "stm32f10x.h"
#include "bsp_usart.h"			//串口通信
#include "bsp_SysTick.h"		//定时器
#include "bsp_esp8266.h"		//WiFi模块
#include "bsp_dht11.h"			//温湿度模块
//#include "bsp_led.h"  		//LED小灯
 
#include "test.h"						//功能代码部分
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main ( void )
{
	/* 初始化 */
  USART_Config ();                                                               //初始化串口1
	SysTick_Init ();                                                               //配置 SysTick 为 1ms 中断一次 
	ESP8266_Init ();                                                               //初始化WiFi模块使用的接口和外设
  DHT11_Init();																																	 //初始化温湿度模块
	//LED_GPIO_Config();                                                           //初始化RGB彩灯
	printf ( "\r\n外设初始化完成,开始连接网络...\r\n" );
	
	/*功能代码*/
  ESP8266_StaTcpClient_UnvarnishTest ();
	
	
  while ( 1 );
	
	
}

