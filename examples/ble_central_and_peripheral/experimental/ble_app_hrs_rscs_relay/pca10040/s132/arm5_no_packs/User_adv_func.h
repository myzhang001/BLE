#ifndef __user_adv_func_h
#define __user_adv_func_h

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"
#include "ble_db_discovery.h"






extern bool User_Match_Adv_Addr(ble_gap_addr_t addr,uint8_t *dev_mac);



#endif


