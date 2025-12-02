#pragma once

#include "SDL2/SDL.h"

typedef struct AVMessage {
    int what;               // 消息类型
    int arg1;               // 参数1
    int arg2;               // 参数2
    void *obj;              // 如果arg1 arg2还不够存储消息则使用该参数
    void (*free_l)(void *obj); // obj的对象是分配的，这里要给出函数怎么释放
    struct AVMessage *next; // 下一个消息
} AVMessage;

typedef struct MessageQueue { // 消息队列
    AVMessage *first_msg, *last_msg; // 消息头, 消息尾部
    int nb_messages;        // 有多少个消息
    int abort_request;      // 请求终止消息队列
    SDL_mutex *mutex;       // 互斥量
    SDL_cond *cond;         // 条件变量
    AVMessage *recycle_msg; // 消息循环使用
    int recycle_count;      // 循环的次数, 局部性原理
    int alloc_count;        // 分配的次数
} MessageQueue;

void msg_free_res(AVMessage *msg);

int msg_queue_put_private(MessageQueue *q, AVMessage *msg);