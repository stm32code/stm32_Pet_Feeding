// 网络设备
#include "esp8266.h"

// 协议文件
#include "onenet.h"
#include "mqttkit.h"
#include "delay.h"

// 硬件驱动
#include "usart.h"
#include "led.h"
#include "dht11.h"
#include "relay.h"

//  C库
#include <string.h>
#include <stdio.h>

extern unsigned char esp8266_buf[256];

U8 Connect_Net = 0;
//==========================================================
//	函数名称：	OneNet_DevLink
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	无
//
//	返回参数：	1-成功	0-失败
//
//	说明：		与onenet平台建立连接
//==========================================================
_Bool OneNet_DevLink(void)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // 协议包

	unsigned char *dataPtr;

	_Bool status = 1;

	printf("PROID: %s,	AUIF: %s,	DEVID:%s\r\n", PROID, AUTH_INFO, DEVID);

	if (MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len); // 上传平台

		dataPtr = ESP8266_GetIPD(250); // 等待平台响应
		if (dataPtr != NULL)
		{
			if (MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch (MQTT_UnPacketConnectAck(dataPtr))
				{
				case 0:
					printf("Tips:	连接成功\r\n");
					status = 0;
					break;

				case 1:
					printf("WARN:	连接失败：协议错误\r\n");
					break;
				case 2:
					printf("WARN:	连接失败：非法的clientid\r\n");
					break;
				case 3:
					printf("WARN:	连接失败：服务器失败\r\n");
					break;
				case 4:
					printf("WARN:	连接失败：用户名或密码错误\r\n");
					break;
				case 5:
					printf("WARN:	连接失败：非法链接(比如token非法)\r\n");
					break;

				default:
					printf("ERR:	连接失败：未知错误\r\n");
					break;
				}
			}
		}

		MQTT_DeleteBuffer(&mqttPacket); // 删包
	}
	else
		printf("WARN:	MQTT_PacketConnect Failed\r\n");

	return status;
}
unsigned char OneNet_FillBuf(char *buf)
{
	char text[256];

	memset(text, 0, sizeof(text));

	strcpy(buf, ",;");

	// memset(text, 0, sizeof(text));
	// sprintf(text, "Temp,%02d.%02d;", DHT11_Data.temp_int, DHT11_Data.temp_deci); // Temp是数据流的一个名称，temper是温度值
	// strcat(buf, text);

	// memset(text, 0, sizeof(text));
	// sprintf(text, "Humi,%02d.%02d;", DHT11_Data.humi_int, DHT11_Data.humi_deci); // Temp是数据流的一个名称，temper是温度值
	// strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "Open,%03d;", device_state_init.open); // Temp是数据流的一个名称，temper是温度值
	strcat(buf, text);

	return strlen(buf);
}
unsigned char MQTT_FillBuf(char *buf)
{
	char text[256];
	memset(text, 0, sizeof(text));

	strcpy(buf, "{");

	memset(text, 0, sizeof(text));
	sprintf(text, "\"door\":\"%d\",", device_state_init.open);
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "\"water\":\"%d\",", device_state_init.water);
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "\"eat\":\"%d\",", device_state_init.eat);
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "\"food\":\"%.1f\",", Data_init.wight);
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "\"food_v\":\"%.1f\",", threshold_value_init.wight_value);
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "\"temp\":\"%.1f\",", Data_init.Temp);
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "\"humi\":\"%.1f\"", Data_init.Humi);
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

	return strlen(buf);
}
unsigned char Mqtt_Pwd_Info(char *buf)
{
	char text[128];
	memset(text, 0, sizeof(text));

	strcpy(buf, "{");

	memset(text, 0, sizeof(text));
	sprintf(text, "\"Pwd\":\"%d\"", box_pwd_init.pwd); // Temp是数据流的一个名称，temper是温度值
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

	return strlen(buf);
}
unsigned char Mqtt_Flage_Info(char *buf)
{
	char text[128];
	memset(text, 0, sizeof(text));

	strcpy(buf, "{");

	memset(text, 0, sizeof(text));
	sprintf(text, "\"Open\":\"%d\"", device_state_init.open); // Temp是数据流的一个名称，temper是温度值
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

	return strlen(buf);
}
unsigned char Mqtt_Fan_Info(char *buf)
{
	char text[128];
	memset(text, 0, sizeof(text));

	strcpy(buf, "{");

	memset(text, 0, sizeof(text));
	// sprintf(text, "\"Fan\":\"%d\"", device_state_init.fan_state); // Temp是数据流的一个名称，temper是温度值
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

	return strlen(buf);
}
//==========================================================
//	函数名称：	OneNet_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	无
//
//	说明：
//==========================================================
void OneNet_SendData(void)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // 协议包

	char buf[128];

	short body_len = 0, i = 0;
	memset(buf, 0, sizeof(buf));
	// 封包
	body_len = OneNet_FillBuf(buf); // 获取当前需要发送的数据流的总长度

	if (body_len)
	{
		// 封包
		if (MQTT_PacketSaveData(DEVID, body_len, NULL, 5, &mqttPacket) == 0) // 封包
		{
			for (; i < body_len; i++)
				mqttPacket._data[mqttPacket._len++] = buf[i];

			ESP8266_SendData(mqttPacket._data, mqttPacket._len); // 上传数据到平台

			MQTT_DeleteBuffer(&mqttPacket); // 删包
		}
		else
			printf("WARN:	EDP_NewBuffer Failed\r\n");
	}
}
//==========================================================
//	函数名称：	OneNet_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	无
//
//	说明：
//==========================================================
void OneNet_SendMqtt(U8 Cmd)
{
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // 协议包
	char buf[254];
	short body_len = 0;
	memset(buf, 0, sizeof(buf));
	switch (Cmd)
	{
	case 1:
		body_len = MQTT_FillBuf(buf); // 数据流
		break;
	case 2:
		body_len = Mqtt_Pwd_Info(buf); // 数据流
		break;
	case 3:
		body_len = Mqtt_Flage_Info(buf); // 数据流
		break;
	case 4:
		body_len = Mqtt_Flage_Info(buf); // 数据流
		break;
	default:
		break;
	}

	if (body_len)
	{
		// 封包
		if (MQTT_PacketPublish(MQTT_PUBLISH_ID, P_TOPIC_NAME, buf, body_len, MQTT_QOS_LEVEL1, 0, 1, &mqttPacket) == 0)
		{
			ESP8266_SendData(mqttPacket._data, mqttPacket._len); // 向平台发送订阅请求
			MQTT_DeleteBuffer(&mqttPacket);						 // 删包
		}
		else
			printf("WARN\r\n");
	}
}
//==========================================================
//	函数名称：	OneNet_RevPro
//
//	函数功能：	平台返回数据检测
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // 协议包

	char *req_payload = NULL;
	char *cmdid_topic = NULL;

	unsigned short topic_len = 0;
	unsigned short req_len = 0;

	unsigned char type = 0;
	unsigned char qos = 0;
	static unsigned short pkt_id = 0;

	short result = 0;

	//---------------------------------------------步骤一：获取返回数据类型---------------------------------------------
	type = MQTT_UnPacketRecv(cmd);
	switch (type)
	{
		//---------------------------------------------步骤二：调用函数解析-------------------------------------------------
	case MQTT_PKT_CMD: // 命令下发

		result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len); // 解出topic和消息体
		if (result == 0)
		{
			printf("cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);

			if (MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0) // 命令回复组包
			{
				printf("Tips:	Send CmdResp\r\n");
				ESP8266_SendData(mqttPacket._data, mqttPacket._len); // 向平台发送订阅请求
				MQTT_DeleteBuffer(&mqttPacket);						 // 删包
			}
		}

		break;

	case MQTT_PKT_PUBLISH: // 接收的Publish消息

		result = MQTT_UnPacketPublish(cmd, &cmdid_topic, &topic_len, &req_payload, &req_len, &qos, &pkt_id);
		if (result == 0)
		{
			printf("topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n",
				   cmdid_topic, topic_len, req_payload, req_len);

			switch (qos)
			{
			case 1: // 收到publish的qos为1，设备需要回复Ack

				if (MQTT_PacketPublishAck(pkt_id, &mqttPacket) == 0)
				{
					printf("Tips:	Send PublishAck\r\n");
					ESP8266_SendData(mqttPacket._data, mqttPacket._len); // 向平台发送订阅请求
					MQTT_DeleteBuffer(&mqttPacket);
				}

				break;

			case 2: // 收到publish的qos为2，设备先回复Rec
					// 平台回复Rel，设备再回复Comp
				if (MQTT_PacketPublishRec(pkt_id, &mqttPacket) == 0)
				{
					printf("Tips:	Send PublishRec\r\n");
					ESP8266_SendData(mqttPacket._data, mqttPacket._len); // 向平台发送订阅请求
					MQTT_DeleteBuffer(&mqttPacket);
				}
				break;

			default:
				break;
			}
		}

		break;

	case MQTT_PKT_PUBACK: // 发送Publish消息，平台回复的Ack

		if (MQTT_UnPacketPublishAck(cmd) == 0)
			printf("Tips:	MQTT Publish Send OK\r\n");
		Connect_Net = 60; // 入网成功
		break;

	case MQTT_PKT_PUBREC: // 发送Publish消息，平台回复的Rec，设备需回复Rel消息

		if (MQTT_UnPacketPublishRec(cmd) == 0)
		{
			printf("Tips:	Rev PublishRec\r\n");
			if (MQTT_PacketPublishRel(MQTT_PUBLISH_ID, &mqttPacket) == 0)
			{
				printf("Tips:	Send PublishRel\r\n");
				ESP8266_SendData(mqttPacket._data, mqttPacket._len); // 向平台发送订阅请求
				MQTT_DeleteBuffer(&mqttPacket);
			}
		}

		break;

	case MQTT_PKT_PUBREL: // 收到Publish消息，设备回复Rec后，平台回复的Rel，设备需再回复Comp

		if (MQTT_UnPacketPublishRel(cmd, pkt_id) == 0)
		{
			printf("Tips:	Rev PublishRel\r\n");
			if (MQTT_PacketPublishComp(MQTT_PUBLISH_ID, &mqttPacket) == 0)
			{
				printf("Tips:	Send PublishComp\r\n");
				ESP8266_SendData(mqttPacket._data, mqttPacket._len); // 向平台发送订阅请求
				MQTT_DeleteBuffer(&mqttPacket);
			}
		}

		break;

	case MQTT_PKT_PUBCOMP: // 发送Publish消息，平台返回Rec，设备回复Rel，平台再返回的Comp

		if (MQTT_UnPacketPublishComp(cmd) == 0)
		{
			printf("Tips:	Rev PublishComp\r\n");
		}

		break;

	case MQTT_PKT_SUBACK: // 发送Subscribe消息的Ack

		if (MQTT_UnPacketSubscribe(cmd) == 0)
			printf("Tips:	MQTT Subscribe OK\r\n");
		else
			printf("Tips:	MQTT Subscribe Err\r\n");

		break;

	case MQTT_PKT_UNSUBACK: // 发送UnSubscribe消息的Ack

		if (MQTT_UnPacketUnSubscribe(cmd) == 0)
			printf("Tips:	MQTT UnSubscribe OK\r\n");
		else
			printf("Tips:	MQTT UnSubscribe Err\r\n");

		break;

	default:
		result = -1;
		break;
	}
	//---------------------------------------------步骤三：命令处理---------------------------------------------

	ESP8266_Clear(); // 清空缓存

	if (result == -1)
		return;
	// 解析json
	if (req_payload != NULL)
	{
		massage_parse_json((char *)req_payload);
	}
	//---------------------------------------------步骤四：释放内存---------------------------------------------
	if (type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}
}

//==========================================================
//	函数名称：	void Link_OneNet(u8 Link)
//
//	函数功能：	连接判断
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：
//==========================================================
void Link_OneNet(u8 Link)
{
	if (!Link)
	{
		ESP8266_Init();			 // 初始化ESP8266
		while (OneNet_DevLink()) // 接入OneNET
			delay_ms(500);
	}
}
//==========================================================
//	函数名称：	OneNet_Subscribe
//
//	函数功能：	订阅
//
//	入口参数：	topics：订阅的topic
//				topic_cnt：topic个数
//
//	返回参数：	SEND_TYPE_OK-成功	SEND_TYPE_SUBSCRIBE-需要重发
//
//	说明：
//==========================================================
_Bool OneNet_Subscribe(const char *topics[], unsigned char topic_cnt)
{

	unsigned char i = 0;
	unsigned char *dataPtr;
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // 协议包

	for (; i < topic_cnt; i++)
		printf("Subscribe Topic: %s\r\n", topics[i]);

	//---------------------------------------------步骤一：组包---------------------------------------------
	if (MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL2, topics, topic_cnt, &mqttPacket) == 0)
	{
		//---------------------------------------------步骤二：发送数据-----------------------------------------
		ESP8266_SendData(mqttPacket._data, mqttPacket._len); // 向平台发送订阅请求
		dataPtr = ESP8266_GetIPD(500);						 // 等待平台响应
		if (MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_SUBACK)
		{
			//---------------------------------------------步骤三：删包---------------------------------------------
			printf("Tips:	MQTT Subscribe OK\r\n");
			MQTT_DeleteBuffer(&mqttPacket); // 删包
			return 0;
		}
		else
		{
			//---------------------------------------------步骤三：删包---------------------------------------------
			printf("Tips:	MQTT Subscribe Err\r\n");
			MQTT_DeleteBuffer(&mqttPacket); // 删包
			return 1;
		}
	}

	return 1;
}

//==========================================================
//	函数名称：	OneNet_Publish
//
//	函数功能：	发布消息
//
//	入口参数：	topic：发布的主题
//				msg：消息内容
//
//	返回参数：	0-成功	1-需要重送
//
//	说明：
//==========================================================
_Bool OneNet_Publish(const char *topic, const char *msg)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // 协议包

	// printf( "Publish Topic: %s, Msg: %s\r\n", topic, msg);

	if (MQTT_PacketPublish(MQTT_PUBLISH_ID, topic, msg, strlen(msg), MQTT_QOS_LEVEL1, 0, 1, &mqttPacket) != 1)
	{

		ESP8266_SendData(mqttPacket._data, mqttPacket._len); // 向平台发送订阅请求
		MQTT_DeleteBuffer(&mqttPacket);						 // 删包
	}
	return 0;
}
