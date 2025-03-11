#include "massage.h"
// 网络设备
#include "esp8266.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "git.h"

// 获取全局变量
extern Data_TypeDef Data_init;                 // 设备数据
extern Device_Satte_Typedef device_state_init; // 设备状态

// 解析json数据
mySta massage_parse_json(char *message)
{

    cJSON *cjson_test = NULL; // 检测json格式
    cJSON *cjson_data = NULL; // 数据

    // 定义数据类型
    char *cjson_device_id;
    u8 cjson_cmd, cjson_direction, massgae; // 指令,方向

    /* 解析整段JSO数据 */
    cjson_test = cJSON_Parse(message);
    if (cjson_test == NULL)
    {
        // 解析失败 printf("parse fail.\n");
        return MY_FAIL;
    }

    /* 依次根据名称提取JSON数据（键值对） */
    cjson_cmd = cJSON_GetObjectItem(cjson_test, "cmd")->valueint;
    cjson_direction = cJSON_GetObjectItem(cjson_test, "direction")->valueint;

    /* 解析嵌套json数据 */
    cjson_data = cJSON_GetObjectItem(cjson_test, "data");
    cjson_device_id = cJSON_GetObjectItem(cjson_data, "device_id")->valuestring;

    if (cjson_direction == PLATFORM_SEND && cjson_device_id == Device_ID)
    {
        switch (cjson_cmd)
        {
        case 0x01: //
            // LED = ~LED;
            massgae = cJSON_GetObjectItem(cjson_data, "online")->valueint;
            break;
        case 0x02: // 心跳包
            massgae = cJSON_GetObjectItem(cjson_data, "heart")->valueint;
            break;
        case 0x03: // 数据包
            massgae = cJSON_GetObjectItem(cjson_data, "mag")->valueint;
            if (massgae)
            {
                /* code */
            }
            break;
        case 0x04: // 主动上报应答
            massgae = cJSON_GetObjectItem(cjson_data, "mag")->valueint;
            if (massgae)
            {
                /* code */
            }
            break;
        case 0x05: // 平台下发应答
            massgae = cJSON_GetObjectItem(cjson_data, "mag")->valueint;
            if (massgae)
            {
                /* code */
            }
            break;
        default:
            break;
        }
    }
    /* 清空JSON对象(整条链表)的所有数据 */
    cJSON_Delete(cjson_test);
    cJSON_Delete(cjson_data);

    return MY_SUCCESSFUL;
}
// 设备心跳包
mySta heart_json_init()
{
    cJSON *usr = NULL;
    cJSON *cjson_data = NULL; // 数据
    char *data;

    usr = cJSON_CreateObject(); // 创建根数据对象
    cjson_data = cJSON_CreateObject();

    cJSON_AddItemToObject(usr, "cmd", cJSON_CreateNumber(0x02));               // 根节点下添加数字
    cJSON_AddItemToObject(usr, "direction", cJSON_CreateNumber(DERVICE_SEND)); // 根节点下添加字符

    /* 添加一个嵌套的JSON数据（添加一个链表节点） */
    cJSON_AddItemToObject(cjson_data, "device_id", cJSON_CreateString(Device_ID));
    cJSON_AddItemToObject(cjson_data, "heart", cJSON_CreateNumber(0x01));
    cJSON_AddItemToObject(usr, "data", cjson_data);

    data = cJSON_PrintUnformatted(usr); // 将json形式打印成正常字符串形式(带有\r\n)
    // printf("%s", data);                 // 通过串口打印出来
    ESP8266_SendData((unsigned char *)data, strlen(data)); // 发送数据节点

    /*********************/
    /*		释放内存	  */
    /*********************/
    // cJSON_Delete(cjson_data);
    cJSON_Delete(usr);

    // delay_ms(50);
    cJSON_free(data);

    return MY_SUCCESSFUL;
}
// 设备数据包
mySta data_json_init()
{
    cJSON *usr = NULL;
    cJSON *cjson_data = NULL; // 数据
    char *data;

    usr = cJSON_CreateObject(); // 创建根数据对象
    cjson_data = cJSON_CreateObject();

    cJSON_AddItemToObject(usr, "cmd", cJSON_CreateNumber(0x03));               // 根节点下添加数字
    cJSON_AddItemToObject(usr, "direction", cJSON_CreateNumber(DERVICE_SEND)); // 根节点下添加字符

    /* 添加一个嵌套的JSON数据（添加一个链表节点） */
    cJSON_AddItemToObject(cjson_data, "device_id", cJSON_CreateString("12345678"));
    cJSON_AddItemToObject(cjson_data, "temp", cJSON_CreateNumber(Data_init.temperatuer));
    cJSON_AddItemToObject(cjson_data, "light", cJSON_CreateNumber(Data_init.light));
    cJSON_AddItemToObject(cjson_data, "led_state", cJSON_CreateNumber(device_state_init.led_state));
    cJSON_AddItemToObject(cjson_data, "hot_led_state", cJSON_CreateNumber(device_state_init.hot_led_state));
    cJSON_AddItemToObject(cjson_data, "fan_state", cJSON_CreateNumber(device_state_init.fan_state));
    cJSON_AddItemToObject(usr, "data", cjson_data);

    data = cJSON_PrintUnformatted(usr); // 将json形式打印成正常字符串形式(带有\r\n)
    // printf("%s", data);                 // 通过串口打印出来
    ESP8266_SendData((unsigned char *)data, strlen(data)); // 发送数据节点

    /*********************/
    /*		释放内存	  */
    /*********************/
    // cJSON_Delete(cjson_data);
    cJSON_Delete(usr);

    // delay_ms(50);
    cJSON_free(data);

    return MY_SUCCESSFUL;
}
// 注册包
mySta login_json_init()
{
    cJSON *usr = NULL;
    cJSON *cjson_data = NULL; // 数据
    char *data;

    usr = cJSON_CreateObject(); // 创建根数据对象
    cjson_data = cJSON_CreateObject();

    cJSON_AddItemToObject(usr, "cmd", cJSON_CreateNumber(0x01));               // 根节点下添加数字
    cJSON_AddItemToObject(usr, "direction", cJSON_CreateNumber(DERVICE_SEND)); // 根节点下添加字符

    /* 添加一个嵌套的JSON数据（添加一个链表节点） */
    cJSON_AddItemToObject(cjson_data, "device_id", cJSON_CreateString("12345678"));
    cJSON_AddItemToObject(cjson_data, "online", cJSON_CreateNumber(0x00));
    cJSON_AddItemToObject(usr, "data", cjson_data);

    data = cJSON_PrintUnformatted(usr); // 将json形式打印成正常字符串形式(带有\r\n)
    // printf("%s", data);                 // 通过串口打印出来
    ESP8266_SendData((unsigned char *)data, strlen(data)); // 发送数据节点

    /*********************/
    /*		释放内存	  */
    /*********************/
    // cJSON_Delete(cjson_data);
    cJSON_Delete(usr);

    // delay_ms(50);
    cJSON_free(data);

    return MY_SUCCESSFUL;
}
