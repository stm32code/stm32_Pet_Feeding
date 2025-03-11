#include "task.h"

// 链表 ---将多个数据结构(这里用的是结构体)以线性的方式排列

/*链表整体搭建顺序为

1.建立一个 结构体指针(head_handle) 设置指针指向 地址0

2.新建一个 结构体 初始化其各项数值---函数(timer_init)

3.将原有的指针(head_handle) 存放到 新建立的结构体(2建立的)中 以此形成链表--函数(timer_start)

    3.1 记录下原有指针(head_handle)防止原有指针丢失,
        历遍整个链表确定新指针(2建立的)和原有数据中的指针不重复 后 将原有指针(head_handle) 储存进 新指针中(2建立的)
    3.2 覆盖记录的指针地址(将修改好的指针(2建立的)赋值给(head_handle),存储,方便下次添加时历遍整个链表)
    3.注 整个链表是反向添加的,先添加的在后米娜,后添加的在前面,

4.将传入的指针从链表中剔除 --函数(timer_stop)

    4.1 建立一个二级指针 便于 记录修改地址
    4.2 循环遍历每一个元素
        4.2.1 判断当前元素是否是输入元素
            是  将当前元素(输入元素)的next值 赋值给上个元素的next
            否  将当前元素的next保存的地址,赋值给临时变量方便其访问下一个元素

5.到期执行及更新周期  --函数（timer_loop）

*/

// timer handle list head.
static struct Timer *head_handle = 0; // 定义一个初始指针,并赋值其他值为0

// Timer ticks
static unsigned int _timer_ticks = 0; // 定义一个变量用于记录时间判断数据

/**
 * @brief  Initializes the timer struct handle.
 * @param  handle: the timer handle strcut.
 * @param  timeout_cb: timeout callback.
 * @param  repeat: repeat interval time.
 * @retval None
 */
// 创建一个结构体储存定时器的结构体,并赋值
void timer_init(struct Timer *handle, void (*timeout_cb)(), unsigned int timeout, unsigned int repeat)
{
    // memset(handle, sizeof(struct Timer), 0);
    // 回调函数赋址(指针地址并非函数本身)
    handle->timeout_cb = timeout_cb;

    // 采集周期赋值(间隔时间)
    handle->timeout = _timer_ticks + timeout;

    // 将下一个地址初始化为空
    handle->next = 0;

    // 是否重复触发,只有重复和不重复,为防止输入错误,非0及为重复
    if (repeat != 0)
        // 重复触发的话，将间隔时间赋值给 repeat  ，  timeout用来记录上次触发时间
        handle->repeat = timeout;
    else
        handle->repeat = 0;
}

/**
 * @brief  Start the timer work, add the handle into work list.
 * @param  btn: target handle strcut.
 * @retval 0: succeed. -1: already exist.
 */
// 将新建的结构体添加进链表中  新加入的表指针地址
int timer_start(struct Timer *handle)
{
    // 将起始链表赋值给临时值
    struct Timer *target = head_handle;

    // 遍历链表确定如输入的是一个新链表-之前没有输入过
    while (target)
    {

        // 从链表第一个元素开始比对 如何和新加入的一致则退出函数并返回 -1 用来确定执行状态 (用于确定没有重复加入)
        if (target == handle)
            return -1; // already exist.

        // 确定本元素不是最后一个后,将下个一个元素地址赋值给临时值
        target = target->next;
    }

    // 将新加入的链表存储的下一级地址修改为保存的地址
    handle->next = head_handle;

    // 将新加入的链表地址存储
    head_handle = handle;

    // 比对通过增加后返回0 用来确定执行状态
    return 0;
}

/**
 * @brief  Stop the timer work, remove the handle off work list.
 * @param  handle: target handle strcut.
 * @retval None
 */
void timer_stop(struct Timer *handle)
{
    // 定义一个二级指针(指向指针的指针)
    struct Timer **curr;

    // 从第一个元素开始遍历 ,最后也给元素为0,历遍到后回自动跳出循环
    for (curr = &head_handle; *curr;)
    {

        struct Timer *entry = *curr;

        // 判断当前元素是不是输入元素(直接对比元素的内存地址)
        if (entry == handle)
        {

            // 将下一级的地址储存到上一级的next中

            // curr 为二级指针的变量(二级指针是数据类型,变量储存信息) 储存的是一个对应的地址(此地址在循环开始赋值,值为最后一个添加进来的链表结构体地址)
            // *curr 为二级指针解引用后的一级指针  其解引用后是 结构体本身,以指针方式保存为当前结构体的地址

            // 当前结构体的地址修改为下一个结构体的地址(影响原有数据)

            // 当前的值为储存的最后一个元素的地址 ,解引用之后, 将其next 从新储存到当前位置(上一个元素的next内)
            *curr = entry->next;
            //          free(entry);
        }
        else
            // 修改了直接指针变量存储的值(不影响原有数据)
            curr = &entry->next;
    }
}

/**
 * @brief  main loop.
 * @param  None.
 * @retval None
 */
void timer_loop()
{
    // 定义临时变量
    struct Timer *target;

    // 历遍整个链表
    for (target = head_handle; target; target = target->next)
    {

        // 当前元素的采集周期是否大于计时变量
        if ((int)target->timeout - (int)_timer_ticks <= 0)
        { // To prevent overflow

            // 判断是否开启了循环
            if (target->repeat == 0)
            {
                // 将未开启循环的元素剔除链表
                timer_stop(target);
            }
            else
            {
                // 初始化时候 已经将间隔时间赋值给repeat了，timeout用来记录的到期时间
                // 下次到期时间 =  当前时间 + 间隔时间
                target->timeout = _timer_ticks + target->repeat;
            }
            // 执行当前元素的回调函数
            target->timeout_cb();
        }
    }
}

/**
 * @brief  background ticks, timer repeat invoking interval 1ms.
 * @param  None.
 * @retval None.
 */
void timer_ticks()
{
    _timer_ticks++;
}
