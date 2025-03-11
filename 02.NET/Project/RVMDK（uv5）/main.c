// C��
#include <string.h>
// ��Ƭ��ͷ�ļ�
#include "sys.h"
// ����Э���
#include "onenet.h"
// �����豸
#include "esp8266.h"
// ������
#include "task.h"
#include "timer.h"
// Ӳ������
#include "delay.h"
#include "usart.h"
#include "git.h"
#include "led.h"
#include "dht11.h"
#include "adc.h"
#include "relay.h"
#include "key.h"
#include "timer.h"
#include "sg90.h"
#include "HX711.h"
#include "level.h"
#if OLED // OLED�ļ�����
#include "oled.h"
#endif
// ����״̬

// �����ʱ���趨
static Timer task1_id;
static Timer task2_id;
static Timer task3_id;

const char *topics[] = {S_TOPIC_NAME};
 
// �����ʼ��
void Net_Init()
{

#if OLED // OLED�ļ�����
    char str[50];
    OLED_Clear();
    // дOLED����
    sprintf(str, "---OPEN  WIFI---");
    OLED_ShowCH(0, 0, (unsigned char *)str);
    sprintf(str, "SSID:%s         ", SSID);
    OLED_ShowCH(0, 2, (unsigned char *)str);
    sprintf(str, "PASS:%s         ", PASS);
    OLED_ShowCH(0, 4, (unsigned char *)str);
    OLED_ShowCH(0, 6, "                ");
#endif
    ESP8266_Init();          // ��ʼ��ESP8266
    while (OneNet_DevLink()) // ����OneNET
        delay_ms(300);
		while (OneNet_Subscribe(topics, 1))  // ��������
        delay_ms(300);

    LED = ON;                    // �����ɹ�
    delay_ms(500);
    LED = OFF;

    Connect_Net = 60; // �����ɹ�
#if OLED              // OLED�ļ�����
    OLED_Clear();
#endif
}

// ����1
void task1(void)
{
   //1�������
 	Automation_Close();
}
// ����2
void task2(void)
{
    // �豸����
    if (Connect_Net == 180) {
#if OLED // OLED�ļ�����
        char str[50];
        // дOLED����
        sprintf(str, "---OPEN  WIFI---");
        OLED_ShowCH(0, 0, (unsigned char *)str);
        sprintf(str, "SSID:%s         ", SSID);
        OLED_ShowCH(0, 2, (unsigned char *)str);
        sprintf(str, "PASS:%s         ", PASS);
        OLED_ShowCH(0, 4, (unsigned char *)str);
        OLED_ShowCH(0, 6, "                ");

#endif
        ESP8266_Init();          // ��ʼ��ESP8266
        while (OneNet_DevLink()) // ����OneNET
            delay_ms(300);
        OneNet_Subscribe(topics, 1); // ��������

        Connect_Net = 60; // �����ɹ�
#if OLED                  // OLED�ļ�����
        OLED_Clear();
#endif
    }

    while (Read_Data(&Data_init) != MY_SUCCESSFUL)
        delay_ms(5); // ���´���������
    while (Update_oled_massage() != MY_SUCCESSFUL)
        delay_ms(5); // ����OLED
    while (Update_device_massage() != MY_SUCCESSFUL)
        delay_ms(5); // �����豸
    LED = ~LED;

    if (Data_init.App > 0) {
				OneNet_SendMqtt(1); // �������ݵ�APP
        Data_init.App = 0;
    }
}
// ����3
void task3(void)
{
    if (Data_init.App == 0) {
       Data_init.App = 1;
    }
} 
// ������
int main(void)
{

    
}
