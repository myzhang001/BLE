#include "queue.h"


static  u16 Send_Queue_max_length = 0;
static  u16 Send_Queue_max_internal_length = 0;
static QTypeDef Send_Queue;

void Send_Queue_init(u8 *RAMSatrtAddr,u16 RAMLen)
{
    Send_Queue.elements=RAMSatrtAddr;
    Send_Queue_max_length=RAMLen-1;
    Send_Queue_max_internal_length=RAMLen;
    Send_Queue.head = 0;
}

bool Send_Queue_push(u8 value)
{
    u16 next = (Send_Queue.head + 1) % (Send_Queue_max_internal_length);
    if (next == Send_Queue.tail)
    {
        return false;
    }

    Send_Queue.elements[Send_Queue.head] = value;
    Send_Queue.head = next;

    return false;
}

u8 Send_Queue_pop(void)
{
    u16 next = (Send_Queue.tail + 1) % Send_Queue_max_internal_length;
    u8 value = Send_Queue.elements[Send_Queue.tail];
    Send_Queue.tail = next;

    return value;
}

u8 Send_Queue_peek(void)
{
    return Send_Queue.elements[Send_Queue.tail];
}



u16 Send_Queue_length(void)
{
    return (Send_Queue_max_internal_length + Send_Queue.head - Send_Queue.tail) \
            % Send_Queue_max_internal_length;
}

u16 Send_Queue_available(void)
{
    return Send_Queue_max_length - Send_Queue_length();
}

bool Send_Queue_full(void)
{
    return Send_Queue_length() == Send_Queue_max_length;
}

bool Send_Queue_empty(void)
{
    return Send_Queue_length() == 0;
}