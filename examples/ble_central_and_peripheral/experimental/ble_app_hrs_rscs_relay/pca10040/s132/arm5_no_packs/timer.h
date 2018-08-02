#ifndef __timer_h
#define __timer_h

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"




typedef struct{
	uint8_t s_timer10ms_flag;
	uint8_t s_timer20ms_flag;
	uint8_t s_timer50ms_flag;
	uint8_t s_timer100ms_flag;
	uint8_t s_timer1s_flag;
	uint8_t s_timer1min_flag;
	uint8_t s_timer1hour_flag;
}_systime;

 
typedef struct {
	uint8_t Num10ms;
	uint8_t Num20ms;
	uint8_t Num100ms;
	uint8_t Num1s;
	uint8_t Num1min;
	uint8_t Num1hour;
	
}_timer;




void sys_tick_handle(void);



#endif


