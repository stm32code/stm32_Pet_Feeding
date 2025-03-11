#ifndef _MASSAGE__H_
#define _MASSAGE__H_

#include "stm32f10x_it.h"
#include "cjson.h"
#include "string.h"
#include <stdio.h>
#include "git.h"
// 定义设备编号
#define Device_ID "12345678"

// 解析json数据
mySta massage_parse_json(char *message);
// 设备心跳包
mySta heart_json_init(void);
// 注册包
mySta login_json_init(void);
// 设备数据包
mySta data_json_init(void);
// 更新OLED显示屏中内容
mySta Update_oled_massage(void);
// 更新设备状态
mySta Update_device_massage(Device_Satte_Typedef *device_state);

#endif
