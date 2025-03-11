#ifndef _ONENET_H_
#define _ONENET_H_

#include "stm32f10x_it.h"
#include "git.h"
extern U8 Connect_Net;

_Bool OneNet_DevLink(void);

void OneNet_SendMqtt(U8 Cmd);
void OneNet_SendData(void);

void OneNet_RevPro(unsigned char *cmd);
void Link_OneNet(u8 Link);
_Bool OneNet_Subscribe(const char *topics[], unsigned char topic_cnt);
_Bool OneNet_Publish(const char *topic, const char *msg);


#endif
