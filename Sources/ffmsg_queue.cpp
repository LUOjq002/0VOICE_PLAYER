#include "ffmsg_queue.h"

#include <libavutil/mem.h>
void msg_free_res(AVMessage *msg)
{
    if (!msg || !msg->obj)
    {
        return;
    }
    msg->free_l(msg->obj);
    msg->obj = NULL;
}

int msg_queue_put_private(MessageQueue *q, AVMessage *msg)
{
    if (q->abort_request)
    {
        return -1;
    }

    AVMessage *msg1 = q->recycle_msg; // 回收池中有消息
    if (msg1)
    {
        q->recycle_msg = msg1->next;
        q->recycle_count++;
    }
    else
    {
         q->alloc_count++;
         msg1 = (AVMessage *)av_mallocz(sizeof(AVMessage));
    }

    *msg1 = *msg;
    msg1->next = NULL;

    // 新消息插入到尾部
    if (!q->first_msg)
    {
        q->first_msg = msg1;
    }
    else
    {
        q->last_msg->next = msg1;
        
    }

    q->last_msg = msg1;
    q->nb_messages++;
    SDL_CondSignal(q->cond);
}
