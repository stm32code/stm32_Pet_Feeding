// C库
#include <string.h>
// 单片机头文件
#include "sys.h"
// 网络协议层
#include "onenet.h"
// 网络设备
#include "esp8266.h"
// 任务栏
#include "task.h"
#include "timer.h"
// 硬件驱动
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
#if OLED // OLED文件存在
#include "oled.h"
#endif
// 联网状态

// 软件定时器设定
static Timer task1_id;
static Timer task2_id;
static Timer task3_id;

const char *topics[] = {S_TOPIC_NAME};
 
// 网络初始化
void Net_Init()
{

#if OLED // OLED文件存在
    char str[50];
    OLED_Clear();
    // 写OLED内容
    sprintf(str, "---OPEN  WIFI---");
    OLED_ShowCH(0, 0, (unsigned char *)str);
    sprintf(str, "SSID:%s         ", SSID);
    OLED_ShowCH(0, 2, (unsigned char *)str);
    sprintf(str, "PASS:%s         ", PASS);
    OLED_ShowCH(0, 4, (unsigned char *)str);
    OLED_ShowCH(0, 6, "                ");
#endif
    ESP8266_Init();          // 初始化ESP8266
    while (OneNet_DevLink()) // 接入OneNET
        delay_ms(300);
		while (OneNet_Subscribe(topics, 1))  // 订阅主题
        delay_ms(300);

    LED = ON;                    // 入网成功
    delay_ms(500);
    LED = OFF;

    Connect_Net = 60; // 入网成功
#if OLED              // OLED文件存在
    OLED_Clear();
#endif
}

// 任务1
void task1(void)
{
   //1秒计算器
 	Automation_Close();
}
// 任务2
void task2(void)
{
    // 设备重连
    if (Connect_Net == 180) {
#if OLED // OLED文件存在
        char str[50];
        // 写OLED内容
        sprintf(str, "---OPEN  WIFI---");
        OLED_ShowCH(0, 0, (unsigned char *)str);
        sprintf(str, "SSID:%s         ", SSID);
        OLED_ShowCH(0, 2, (unsigned char *)str);
        sprintf(str, "PASS:%s         ", PASS);
        OLED_ShowCH(0, 4, (unsigned char *)str);
        OLED_ShowCH(0, 6, "                ");

#endif
        ESP8266_Init();          // 初始化ESP8266
        while (OneNet_DevLink()) // 接入OneNET
            delay_ms(300);
        OneNet_Subscribe(topics, 1); // 订阅主题

        Connect_Net = 60; // 入网成功
#if OLED                  // OLED文件存在
        OLED_Clear();
#endif
    }

    while (Read_Data(&Data_init) != MY_SUCCESSFUL)
        delay_ms(5); // 更新传感器数据
    while (Update_oled_massage() != MY_SUCCESSFUL)
        delay_ms(5); // 更新OLED
    while (Update_device_massage() != MY_SUCCESSFUL)
        delay_ms(5); // 更新设备
    LED = ~LED;

    if (Data_init.App > 0) {
				OneNet_SendMqtt(1); // 发送数据到APP
        Data_init.App = 0;
    }
}
// 任务3
void task3(void)
{
    if (Data_init.App == 0) {
       Data_init.App = 1;
    }
} 
// 主函数
int main(void)
{

    
}
