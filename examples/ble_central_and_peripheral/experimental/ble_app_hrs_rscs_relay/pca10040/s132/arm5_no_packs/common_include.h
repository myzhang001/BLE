#ifndef __common_include_h
#define __common_include_h



#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"
#include "ble_db_discovery.h"
#include "timer.h"
#include "data_send_handle.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "User_MacList.h"


extern uint32_t send_string_c(uint8_t conn_handle, uint8_t * p_string, uint16_t length);

extern  uint8_t Tick10msFlag ;	          //10msÊ±ÖÓ»ù×¼	



#endif

