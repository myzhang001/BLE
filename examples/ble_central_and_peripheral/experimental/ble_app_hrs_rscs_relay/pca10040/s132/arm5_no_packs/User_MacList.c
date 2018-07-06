#include "User_MacList.h"
#include "sdk_config.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "Somputon_BLE_DataHandle.h"

#include "slave_device_data.h"
#include "app_uart.h"
#include "app_fifo.h"
#include "nrf_drv_uart.h"

#include "ble_gap.h"
#include "ble_advdata.h"
#include "ble_advertising.h"


_t_dev_mac_match dev_info;              //连接设备的所有信息

_bond_device_info  bond_device_info;    //绑定设备信息




uint8_t mac_addr_list[6];     //mac 地址



// 检查设备地址连接个数
bool dev_check_empty(void)
{
    if(dev_info.device_num < NRF_SDH_BLE_CENTRAL_LINK_COUNT)
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
    return dev_info.device_num;

}


void USER_DEBUG_printf(void)
{
    
    NRF_LOG_INFO("dev_info.device_num:%02x",dev_info.device_num);
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
    if(dev_info.device_num >0 )dev_info.device_num -=1; 
    
    memset(dev_info.ble_dev[connected_handle - 1].mac_addr,0,6);
}




//匹配断开的设备信息,用于调试
void  Debug_Device_match_info(uint16_t  connected_handle,uint8_t *addr_mac)
{
    
    uint16_t degug_connected_handle = 0;                   //断开连接的handle
    
    degug_connected_handle = dev_info.ble_dev[connected_handle - 1].conn_handle;
    
    NRF_LOG_INFO("device connedted handle %d is disconnected ",degug_connected_handle);

    
    //显示断开连接的mac 地址 
    
    NRF_LOG_INFO("device disconnedted mac addr %02x %02x %02x %02x %02x %02x  ",dev_info.ble_dev[connected_handle -1].mac_addr[0],
    dev_info.ble_dev[connected_handle -1].mac_addr[1],
    dev_info.ble_dev[connected_handle -1].mac_addr[2],
    dev_info.ble_dev[connected_handle -1].mac_addr[3],
    dev_info.ble_dev[connected_handle -1].mac_addr[4],
    dev_info.ble_dev[connected_handle -1].mac_addr[5]);
    
    
    NRF_LOG_INFO("device_type %d ",dev_info.ble_dev[connected_handle -1].model);
    
    memcpy(addr_mac,&dev_info.ble_dev[connected_handle - 1].mac_addr[0],6);
    
}


//数组换序

uint8_t change_array_index(uint8_t *p,int y)
 {
     uint8_t  *q;
     int temp;
     q=p+y-1;
     while(p<q)
     {
         temp=*p;
         *p=*q;
         *q=temp;
         p++;
         q--;
     }
}


//连接上的设备地址
void Debug_Device_match_connected_mac(ble_gap_addr_t mac_peer_addr , uint16_t handle)
{
    if(handle > 10)
    {
        return;
    }

    memcpy(dev_info.ble_dev[handle-1].mac_addr,mac_peer_addr.addr,6);       //保存mac 地址
    change_array_index(dev_info.ble_dev[handle-1].mac_addr,6);              //mac 地址倒序保持和app 上显示一致
      
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

//获取连接时间的rssi 

void Device_Get_Rssi(uint16_t conn_handle,int8_t rssi)
{
    dev_info.ble_dev[conn_handle-1].rssi = rssi;
}


//返回rssi值
int8_t Ret_Device_Rssi(uint16_t conn_handle)
{
    return dev_info.ble_dev[conn_handle-1].rssi;
}




_s_avaiable_device   sys_avaiable_table;   //可用设备列表全局


//初始化可用设备类型为0xff  默认初始化的时候调用

void sys_avaiable_device_type(void)
{
    uint8_t i = 0;
    
    for(i = 0;  i < 8;i++)
    {
        dev_info.ble_dev[i].model = E_INVALID;
        sys_avaiable_table.dev_info[i].device_type = E_INVALID;
        
    }
}


//根据设备类型 ，返回相同设备类型的列表
_s_avaiable_device Find_Avaiable_Device(_e_machine_model device_type)
{
    uint8_t i = 0;
    
    _s_avaiable_device  s_device;
    
    for(i = 0; i < sys_avaiable_table.total_avaiable_num;i++)
    {
        if(sys_avaiable_table.dev_info[i].device_type == device_type)
        {
            s_device.total_avaiable_num +=1;
            s_device.dev_info[s_device.total_avaiable_num  - 1].conn_handle = sys_avaiable_table.dev_info[i].conn_handle;
            s_device.dev_info[s_device.total_avaiable_num  - 1].device_type = sys_avaiable_table.dev_info[i].device_type;
        }
    }

    return s_device;
}


//更新可用设备列表
void Device_Update_Avaiable_Table(void)
{
    uint8_t i = 0,num= 0;
    
    static uint8_t time_cnt = 0;
    
    
    for(i = 0; i < 8;i++)
    {
        if(dev_info.ble_dev[i].conn_handle != 0 && dev_info.ble_dev[i].conn_handle != 0xFFFF)
        {
            #if 0
            NRF_LOG_INFO("可用设备列表  conn_handle %d     ",dev_info.ble_dev[i].conn_handle); 
            NRF_LOG_INFO("------NUM %d ",num);
            #endif
            
            sys_avaiable_table.dev_info[num].conn_handle = dev_info.ble_dev[i].conn_handle;   //保存handle
            sys_avaiable_table.dev_info[num].device_type = dev_info.ble_dev[i].model;         //保存类型
            num++;
           // NRF_LOG_INFO("------device type %d ",dev_info.ble_dev[i].model);
            
        }
    }
    
    sys_avaiable_table.total_avaiable_num = num;
    
    #if 0
    if( time_cnt++ > 200)
    {
        NRF_LOG_INFO("可用设备列表总数 %d ",sys_avaiable_table.total_avaiable_num);
        
        for(i = 0; i < 8;i++)
        {
            NRF_LOG_INFO("可用设备列表 %d  conn_handle %d   device_type %d  ",i,sys_avaiable_table.dev_info[i].conn_handle,
            sys_avaiable_table.dev_info[i].device_type);  
        }
        time_cnt = 0;
    }
    #endif
    
    
}



//检查 设备是否绑定  ，当有设备连接后 需要调用判断
uint8_t check_bound_device(void)
{
    uint8_t  ret;
    uint8_t  i,j;
    
    for(i = 0; i < 8;i++)
    {
        if(dev_info.ble_dev[i].bond_stauts  == 0X01)  
        {
            j++;
        }
    }
    
   ret = j;
    
   return ret; 
}

//更新设备的绑定标志，收到绑定指令后调用

void  device_bond_status_update(uint8_t conn_handle)
{
    dev_info.ble_dev[conn_handle -1].bond_stauts = 1;
    
    
}

//清除绑定标志位  ，收到主机设备清除绑定信息后调用
void  device_bond_status_clear(uint8_t conn_handle)
{
    dev_info.ble_dev[conn_handle -1].bond_stauts = 0;
}



//添加设备类型,设备绑定的时候调用
void device_add_type(uint8_t conn_handle ,_e_machine_model device_type)
{
    dev_info.ble_dev[conn_handle - 1].model = device_type;
    
    
    
}

//删除设备类型 ，设备断开连接或者删除绑定信息的时候调用
void device_del_type(uint8_t conn_handle )
{
    dev_info.ble_dev[conn_handle - 1].model = E_INVALID; 
    sys_avaiable_table.dev_info[conn_handle - 1].conn_handle = 0xffff;
    sys_avaiable_table.dev_info[conn_handle - 1].device_type = E_INVALID; 
}

//增加设备个数
void device_total_add(void)
{
    if(dev_info.device_num < 8) dev_info.device_num++;
}


//减少设备个数
void device_total_del(void)
{
    if(dev_info.device_num > 0) dev_info.device_num--;
}

//打印所有连接的数据
void printf_all_dev_info(void)
{
  
    for(uint8_t i = 0; i < 8;i++)
    {
    
        NRF_LOG_INFO(" device conn_handle %d " , dev_info.ble_dev[i].conn_handle);

        NRF_LOG_INFO("device  mac addr %02x %02x %02x %02x %02x %02x  ",dev_info.ble_dev[i].mac_addr[0],
        dev_info.ble_dev[i].mac_addr[1],
        dev_info.ble_dev[i].mac_addr[2],
        dev_info.ble_dev[i].mac_addr[3],
        dev_info.ble_dev[i].mac_addr[4],
        dev_info.ble_dev[i].mac_addr[5]);
    
        NRF_LOG_INFO("device_type %d ",dev_info.ble_dev[i].model);
        
    }
   
}

//绑定设备数据添加
uint8_t  bond_info_add(_bond_device_info *bond_info , uint8_t mac_addr[6],_e_machine_model device_type)
{
   
    uint8_t i = 0;
    
    if(bond_info == NULL )return 0;

    
    
    if( bond_info->device_num == 0)
    {
        memcpy(&bond_info->bond_device[0].mac_addr[0],&mac_addr[0],6);
        bond_info->bond_device[0].device_type   = device_type;
        bond_info->device_num = 1;
    }
    else
    {
        for(i = 0 ;  i < bond_info->device_num;i++)
        {
            if(memcmp(&bond_info->bond_device[i].mac_addr[0],&mac_addr[0],6) == 0)   //设备已经添加
			{
                return 0;                                                            //设备已经添加直接退出
            }
            else
            {
                break;  
            }
        }
        
        bond_info->device_num += 1;

        memcpy(&bond_info->bond_device[bond_info->device_num].mac_addr[0],mac_addr,6);
        bond_info->bond_device[bond_info->device_num].device_type = device_type;
        
    }   
}



//删除绑定设备
uint8_t bond_info_del(_bond_device_info *bond_info , uint8_t mac_addr[6],_e_machine_model device_type)
{
    uint8_t i = 0;
    
    if(bond_info == NULL )return 0;

    _bond_device_info *s_bond = bond_info;
    
    
    
    if(s_bond->device_num == 0)
    {
        NRF_LOG_INFO("ERROR ");
       
    }
    else
    {
        for(i = 0; i < s_bond->device_num; i++)
        {
            if(memcmp(&s_bond->bond_device[i].mac_addr[0],&mac_addr[0],6) == 0) //找到相同设备
			{
                NRF_LOG_INFO("------------------------已经匹配相同设备");
				memset(&s_bond->bond_device[i],0,sizeof(_bond_device));       //清除设备
                
                s_bond->device_num -= 1;                                           //设备断开，设备总数减一
				break;
			}
            
        }
    
    }
}


#define SCAN_INTERVAL                   0x00A0                                      /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW                     0x0050                                      /**< Determines scan window in units of 0.625 millisecond. */
#define SCAN_TIMEOUT                    0


/**@brief Parameters used when scanning. */
static ble_gap_scan_params_t const mscan_params =
{
    .active   = 1,
    .interval = SCAN_INTERVAL,
    .window   = SCAN_WINDOW,
    .timeout  = SCAN_TIMEOUT,
    #if (NRF_SD_BLE_API_VERSION <= 2)
        .selective   = 0,
        .p_whitelist = NULL,
    #endif
    #if (NRF_SD_BLE_API_VERSION >= 3)
        .use_whitelist = 0,
    #endif
};

#define MIN_CONNECTION_INTERVAL         (uint16_t) MSEC_TO_UNITS(7.5, UNIT_1_25_MS) /**< Determines minimum connection interval in milliseconds. */
#define MAX_CONNECTION_INTERVAL         (uint16_t) MSEC_TO_UNITS(30, UNIT_1_25_MS)  /**< Determines maximum connection interval in milliseconds. */
#define SLAVE_LATENCY                   0                                           /**< Determines slave latency in terms of connection events. */
#define SUPERVISION_TIMEOUT             (uint16_t) MSEC_TO_UNITS(4000, UNIT_10_MS)  /**< Determines supervision time-out in units of 10 milliseconds. */


/**@brief Connection parameters requested for connection. */
 ble_gap_conn_params_t const mconnection_param =
{
    MIN_CONNECTION_INTERVAL,
    MAX_CONNECTION_INTERVAL,
    SLAVE_LATENCY,
    SUPERVISION_TIMEOUT
};
#define APP_BLE_CONN_CFG_TAG            1                                           /**< A tag identifying the SoftDevice BLE configuration. */


//连接绑定设备列表里的设备

void connected_bond_list_device(_bond_device_info *bond_info,
    ble_gap_evt_adv_report_t *adv_report)
{
    if( bond_info == NULL)
        return;
    
    uint8_t i = 0;
    uint32_t err_code;
    uint8_t status;
    
    
    static uint8_t mac_addr[6];
    
    mac_addr[5]  = adv_report->peer_addr.addr[0];
    mac_addr[4]  = adv_report->peer_addr.addr[1];
    mac_addr[3]  = adv_report->peer_addr.addr[2];
    mac_addr[2]  = adv_report->peer_addr.addr[3];
    mac_addr[1]  = adv_report->peer_addr.addr[4];
    mac_addr[0]  = adv_report->peer_addr.addr[5];
    
    
    for(i = 0; i < bond_info->device_num;i++)
    {
        if(memcmp(&bond_info->bond_device[i].mac_addr[0],mac_addr,6) != 0)  //找到设备
        {
            err_code = sd_ble_gap_connect(&adv_report->peer_addr,
                                  &mscan_params,
                                  &mconnection_param,
                                  APP_BLE_CONN_CFG_TAG);

            if (err_code == NRF_SUCCESS)
            {
                // scan is automatically stopped by the connect
                //err_code = bsp_indication_set(BSP_INDICATE_IDLE);
               // APP_ERROR_CHECK(err_code);
                NRF_LOG_INFO("Connecting to target %02x%02x%02x%02x%02x%02x",
                         adv_report->peer_addr.addr[0],
                         adv_report->peer_addr.addr[1],
                         adv_report->peer_addr.addr[2],
                         adv_report->peer_addr.addr[3],
                         adv_report->peer_addr.addr[4],
                         adv_report->peer_addr.addr[5]
                         );
            }
            
           
        }
    }
    
   

           
     

}










extern uint8_t s_complete_flag ;


void data_send_proc(void)
{
    static uint8_t step;
    static uint8_t i = 0;
    uint8_t conn_handle = 0;
    uint8_t data_buffer[5] = {0};
    
    
    NRF_LOG_INFO("连接设备总数 %d \r\n",dev_info.device_num);
    
    //step = E_SEND_STATUS;
    
    if(control_data.conn_data_flag == 1)
    {
        step = E_CONTROL_DATA;
    }
    
    switch(step)
    {
        case E_SEND_STATUS:                         //发送绑定认证指令
           // if(s_complete_flag == 1)   
            //bond_data_send(0);
            //bond_data_send(1);

            #if 1
               if(dev_info.device_num != 0)  
               {
                   if( i < dev_info.device_num)
                    {
                        conn_handle = sys_avaiable_table.dev_info[i].conn_handle;  //获取conn_handle
                        i++;
                        NRF_LOG_INFO("-----------index  : %d",conn_handle);
                        
                        if(dev_info.ble_dev[conn_handle - 1].bond_stauts == 0)   
                        {
                            bond_data_send(conn_handle - 1 );
                        }  
                    }
                    else
                    {
                        step = E_REAL_TIME_DATA;
                        i = 0;
                    }
               }
            #endif
               
             break;
        case E_REAL_TIME_DATA:
            
            NRF_LOG_INFO("------------------real time data send ------------");
            
            if(dev_info.device_num != 0)  
            {
                if( i < dev_info.device_num)
                {
                    conn_handle = sys_avaiable_table.dev_info[i].conn_handle;  //获取conn_handle
                    NRF_LOG_INFO("----conn_handle %d",conn_handle);
                    real_data_send(conn_handle - 1);
                    i++;
                } 
                else
                {
                    step = E_CONTROL_DATA;                                    //进入下发设备流程
                    i = 0;
                }
            }
             break;
        case E_CONTROL_DATA:
             
            #if 0
            data_buffer[0] = 0x00;      //灯
            data_buffer[1] = 0x01;      //aroma
            control_data_send(0,0x01,data_buffer,2);
            #endif
              
            //master_send_string(&control_data.data_buffer[0],2);
        
            //app_uart_put(control_data.conn_handle);
            //app_uart_put(control_data.conn_handle >> 8);
        #if 1
            if(control_data.conn_data_flag == 0)
            {
                step = E_SEND_STATUS;
                return;
            }
        #endif
            
            if(control_data.conn_handle > 8)
            {
                control_data.conn_data_flag = 0;
                step = E_SEND_STATUS; 
            }
            else
            {
                
                //master_send_string(control_data.data_buffer,control_data.length);
                #if 1
                control_data_send(control_data.conn_handle - 1,0,
                control_data.data_buffer,control_data.length);

                control_data.conn_data_flag = 0;
                
                memset(control_data.data_buffer,0,control_data.length);
                #endif
                step = E_SEND_STATUS; 
            }
            
             break;
        default:
             break;

    }
}




    
