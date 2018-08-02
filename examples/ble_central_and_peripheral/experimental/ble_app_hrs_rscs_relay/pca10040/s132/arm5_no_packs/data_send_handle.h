#ifndef __data_send_handle_h
#define __data_send_handle_h


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"
#include "ble_db_discovery.h"



extern void data_send();
extern void big_data_send_proc(uint16_t conn_handle,uint8_t *p_data,uint8_t length);



#endif


