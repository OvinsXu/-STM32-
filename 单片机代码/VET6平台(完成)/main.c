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
#include "bsp_usart.h"			//����ͨ��
#include "bsp_SysTick.h"		//��ʱ��
#include "bsp_esp8266.h"		//WiFiģ��
#include "bsp_dht11.h"			//��ʪ��ģ��
//#include "bsp_led.h"  		//LEDС��
 
#include "test.h"						//���ܴ��벿��
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main ( void )
{
	/* ��ʼ�� */
  USART_Config ();                                                               //��ʼ������1
	SysTick_Init ();                                                               //���� SysTick Ϊ 1ms �ж�һ�� 
	ESP8266_Init ();                                                               //��ʼ��WiFiģ��ʹ�õĽӿں�����
  DHT11_Init();																																	 //��ʼ����ʪ��ģ��
	//LED_GPIO_Config();                                                           //��ʼ��RGB�ʵ�
	printf ( "\r\n�����ʼ�����,��ʼ��������...\r\n" );
	
	/*���ܴ���*/
  ESP8266_StaTcpClient_UnvarnishTest ();
	
	
  while ( 1 );
	
	
}

