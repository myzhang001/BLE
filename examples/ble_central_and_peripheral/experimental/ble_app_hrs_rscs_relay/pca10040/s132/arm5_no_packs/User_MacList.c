#include "User_MacList.h"
#include "sdk_config.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"



_t_dev_mac_match dev_info;    //连接设备的所有信息



// 检查设备地址连接个数
bool dev_check_empty(void)
{
    if(dev_info.empty_flag < NRF_SDH_BLE_CENTRAL_LINK_COUNT)
    {
        return true;
    }
    else
    {
        return false;
    }
}


//重置结构体数据
void Device_Info_Reset(_t_ble_status *s_ble_info)
{
    if(s_ble_info == NULL)return;
    
    memset(s_ble_info,0,sizeof(_t_ble_status));
   
}

//返回已连接的个数

uint8_t Device_Info_Connected_num(void)
{
    return dev_info.empty_flag;

}


void USER_DEBUG_printf(void)
{
    NRF_LOG_INFO("dev_info.empty_flag:%02x",dev_info.empty_flag);


}


#if 0
//设备信息写入
void Device_Info_Push(void)
{
    dev_info.ble_dev[0].

}
#endif


//返回已经连接设备的



//设备断开连接
void Device_Disconnected_handle(uint16_t  connected_handle)
{
    dev_info.ble_dev[connected_handle - 1].conn_handle = BLE_CONN_HANDLE_INVALID;
    if(dev_info.empty_flag >0 )dev_info.empty_flag -=1; 
}

//匹配断开的设备信息,用于调试

void Debug_Device_match_info(uint16_t  connected_handle)
{
    
    uint16_t degug_connected_handle = 0;                   //断开连接的handle
    
    degug_connected_handle = dev_info.ble_dev[connected_handle -1].conn_handle;
    
    NRF_LOG_INFO("device connedted handle %d is disconnected ",degug_connected_handle);

    
    //显示断开连接的mac 地址 
    
    NRF_LOG_INFO("device disconnedted mac addr %02x %02x %02x %02x %02x %02x ",dev_info.ble_dev[connected_handle -1].mac_addr[0],
    dev_info.ble_dev[connected_handle -1].mac_addr[1],
    dev_info.ble_dev[connected_handle -1].mac_addr[2],
    dev_info.ble_dev[connected_handle -1].mac_addr[3],
    dev_info.ble_dev[connected_handle -1].mac_addr[4],
    dev_info.ble_dev[connected_handle -1].mac_addr[5]);
}

//连接上的设备地址
void Debug_Device_match_connected_mac(ble_gap_addr_t mac_peer_addr , uint16_t handle)
{
    if(handle > 10)
    {
        return;
    }

    memcpy(dev_info.ble_dev[handle-1].mac_addr,mac_peer_addr.addr,6);       //保存mac 地址
      
    dev_info.ble_dev[handle-1].conn_handle = handle;                        //保存handle        
    
    NRF_LOG_INFO("dev.handle :%02x ",handle);
    NRF_LOG_INFO("ADD MAC addr :%02x %02x %02x %02x %02x %02x",dev_info.ble_dev[handle-1].mac_addr[0],
    dev_info.ble_dev[handle-1].mac_addr[1],
    dev_info.ble_dev[handle-1].mac_addr[2],
    dev_info.ble_dev[handle-1].mac_addr[3],
    dev_info.ble_dev[handle-1].mac_addr[4],
    dev_info.ble_dev[handle-1].mac_addr[5]);
}

//绑定计时间  1s 调用一次

void Device_bind_count(void)
{
    if(dev_info.scan_bind_flag == 1)
    {
        if(dev_info.bing_timeout_cnt > 0)dev_info.bing_timeout_cnt--;
        else{dev_info.scan_bind_flag = 0;}
    }
}

//返回绑定状态
_e_band_status Ret_Device_Bind_status(void)
{
   return dev_info.scan_bind_flag;
}

//返回绑定剩余时间

uint8_t Ret_Device_Bind_Time(void)
{
    return dev_info.bing_timeout_cnt;
}




