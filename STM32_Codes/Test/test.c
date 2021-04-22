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
  * @brief  ��ȡ����������ֺʹ��ڵ������ַ�������Ϣ
  * @param  ��
  * @retval ��
  */
volatile uint8_t ucTcpClosedFlag = 0;

char cStr[1500] = {0}; //�������ڷ��͵�����

/**
  * @brief  ESP8266 ��Sta Tcp Client��͸��
  * @param  ��
  * @retval ��
  */
void ESP8266_StaTcpClient_UnvarnishTest(void)
{
	uint8_t ucStatus;			   //��������״̬��Ϣ
	DHT11_Data_TypeDef DHT11_Data; //��ȡ��ʪ����Ϣ

	printf("\r\n�������� ESP8266 ......\r\n");

	macESP8266_CH_ENABLE();

	ESP8266_AT_Test(); //����ATָ��

	ESP8266_Net_Mode_Choose(STA); //ѡ��ģʽ(wifiģʽ/�ȵ�ģʽ)

	while (!ESP8266_JoinAP(macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd))
		; //����WiFi

	ESP8266_Enable_MultipleId(DISABLE); //�رն�����ģʽ
	//���ӷ�����,����:1.Э��,2.Ŀ��IP,3.Ŀ��˿�,4.��������Single_ID_0
	while (!ESP8266_Link_Server(enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0))
		;

	while (!ESP8266_UnvarnishSend())
		; //����͸��ģʽ

	printf("\r\n���� ESP8266 ���\r\n");
	printf("\r\n��ʼ͸��......\r\n");

	while (1)
	{
		//��ȡ DHT11 ��ʪ����Ϣ,���������
		if (DHT11_Read_TempAndHumidity(&DHT11_Data) == SUCCESS)
		{
			sprintf(cStr, "\r\n��ȡDHT11�ɹ�!\r\n\r\nʪ��Ϊ%d.%d ��RH ���¶�Ϊ %d.%d�� \r\n",
					DHT11_Data.humi_int, DHT11_Data.humi_deci, DHT11_Data.temp_int, DHT11_Data.temp_deci);
		}
		else
		{
			sprintf(cStr, "��ȡ��ʪ��ʧ��!\r\n");
		}
		printf("%s", cStr); //��ӡ��ȡ DHT11 ��ʪ����Ϣ

		//���͵�web������		
		sprintf(cStr, "/StmSet?temp=%d.%d&humi=%d.%d",
			DHT11_Data.temp_int, DHT11_Data.temp_deci, DHT11_Data.humi_int, DHT11_Data.humi_deci);
		
		//����ʪ�ȷ��͵�Web������
		http_Get(cStr,macUser_ESP8266_TcpServer_IP,macUser_ESP8266_TcpServer_Port);
		
		
		

		//printf("%s", cStr);
		//ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //���� DHT11 ��ʪ����Ϣ�������������

		Delay_ms(2000);

		if (ucTcpClosedFlag) //����Ƿ�ʧȥ����
		{
			ESP8266_ExitUnvarnishSend(); //�˳�͸��ģʽ

			do
				ucStatus = ESP8266_Get_LinkStatus(); //��ȡ����״̬
			while (!ucStatus);

			if (ucStatus == 4) //ȷ��ʧȥ���Ӻ�����
			{
				printf("\r\n���������ȵ�ͷ����� ......\r\n");

				while (!ESP8266_JoinAP(macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd))
					;

				while (!ESP8266_Link_Server(enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0))
					;

				printf("\r\n�����ȵ�ͷ������ɹ�\r\n");
			}

			while (!ESP8266_UnvarnishSend())
				;
		}
	}
}
