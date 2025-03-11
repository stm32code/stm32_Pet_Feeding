#ifndef __TASK_H__
#define __TASK_H__
#include "sys.h"

typedef struct Timer
{
    unsigned int timeout;
    unsigned int repeat;
    void (*timeout_cb)(void);
    struct Timer *next;
} Timer;

// #ifdef __cplusplus
// extern "C" {
// #endif

void timer_init(struct Timer *handle, void (*timeout_cb)(), unsigned int timeout, unsigned int repeat);
int timer_start(struct Timer *handle);
void timer_stop(struct Timer *handle);
void timer_ticks(void);
void timer_loop(void);

// void timer_again(struct Timer* handle);
// void timer_set_repeat(struct Timer* handle, unsigned int repeat);

// #ifdef __cplusplus
// }
// #endif

#endif
