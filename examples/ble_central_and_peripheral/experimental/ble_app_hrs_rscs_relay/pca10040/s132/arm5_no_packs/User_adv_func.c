#include "User_adv_func.h"

#include "ble.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"




extern uint32_t adv_report_parse(uint8_t type, uint8_array_t * p_advdata, uint8_array_t * p_typedata);




//∆•≈‰mac µÿ÷∑

bool User_Match_Adv_Addr(ble_gap_addr_t addr,uint8_t *dev_mac)
{
    uint8_t i = 0;
    
    if(dev_mac == NULL)return false;
    
    for( i = 0; i < BLE_GAP_ADDR_LEN;i++)
    {
        if(addr.addr[i] != dev_mac[i])
        {
            break;
        }
    }
    
    if(i < BLE_GAP_ADDR_LEN)
    {
       return false; 
    }
    else
    {
        return true;
    }
}


