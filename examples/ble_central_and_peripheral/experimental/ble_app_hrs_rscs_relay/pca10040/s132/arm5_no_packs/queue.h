#ifndef __queue_h
#define __queue_h

#include <stdint.h>
#include <stdbool.h>
#include "nrf.h"

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;



typedef struct
{
    int head;
    int tail;
    u8 *elements;
}QTypeDef;


void Send_Queue_init(u8 *RAMSatrtAddr,u16 RAMLen);
u16  Send_Queue_length(void);
bool Send_Queue_push(u8 value);
u8   Send_Queue_pop(void);
bool Send_Queue_empty(void);

#endif


