

#include "master_voice_hub.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "Protocol_Analysis.h"
#include "Nus_Master_DataHandle.h"
#include "Master_voice_hub.h"
#include "Master_DataStruct.h"
#include "User_MacList.h"

#include "nordic_common.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "app_uart.h"
#include "app_fifo.h"
#include "nrf_drv_uart.h"

static uint8_t send_buffer[256];



typedef struct{
    uint8_t device_type;                    //设备类型
    uint8_t mac_addr[6];                    //mac地址
    uint8_t p_data[25];                        //数据内容
    uint8_t length;                         //数据长度
    
}_real_master_uart;

_real_master_uart  real_status_master[8];    //所有设备的运行数据



//通过连接句柄 查找该设备的运行数据
_real_master_uart match_device_real_status(uint8_t device_type, uint16_t conn_handle)
{

    _real_master_uart s_real_data;
    uint8_t index = 0;
    uint8_t mac_addr[6];
    
    switch(device_type)
	{
		case E_RAR312:
            //(&rar312_data.mac_index,mac_addr) 
            //通过handle  找mac 地址
            memcpy(mac_addr,dev_info.ble_dev[conn_handle - 1].mac_addr,6);  //获取到conn_handle  对应的mac 地址
            
            index = mac_match_hanle(&rar312_data.mac_index,mac_addr);
            #if 1
            for(uint8_t i = 0; i < 6;i++)
            {
                NRF_LOG_INFO("MAC[%d] 0x%02x",i,mac_addr[i]);
            }
            NRF_LOG_INFO("------index  0x%02x",index);
            #endif
            
            
            memcpy(&s_real_data.p_data[0],&rar312_data.Device_rar312_Array[index].real_temp,3);
            
            s_real_data.device_type = E_RAR312;
            memcpy(&s_real_data.mac_addr[0],&mac_addr[0],6);
            s_real_data.length      = 3;
            
        
			break;
		case E_RM661:
            //(&rm661_data.mac_index,mac_addr);    //为设备添加匹配关系
            memcpy(mac_addr,dev_info.ble_dev[conn_handle - 1].mac_addr,6);  //获取到conn_handle  对应的mac 地址
            
            index = mac_match_hanle(&rm661_data.mac_index,mac_addr);

            s_real_data.device_type = E_RM661;
            memcpy(&s_real_data.mac_addr[0],&mac_addr[0],6);
            s_real_data.length      = 2;
        
            #if 0
            for(uint8_t i = 0; i < 6;i++)
            {
                NRF_LOG_INFO("MAC[%d] 0x%02x",i,mac_addr[i]);
            }
            NRF_LOG_INFO("mac index --- %d",index);  
            #endif
        
            memcpy(&s_real_data.p_data[0],&rm661_data.Device_rm661_Array[index].real_light,2);
            
            #if 1
            for(uint8_t j = 0; j < 2;j++)
            {
                NRF_LOG_INFO("rm661--------------data[%d] 0x%02x",j,s_real_data.p_data[j]);
            }
            #endif
            
            
			break;
		case E_SLEEP_MONITER:
            
            memcpy(mac_addr,dev_info.ble_dev[conn_handle - 1].mac_addr,6);  //获取到conn_handle  对应的mac 地址
            
            index = mac_match_hanle(&sleepmonitor_data.mac_index,mac_addr);

            s_real_data.device_type = E_SLEEP_MONITER;
            memcpy(&s_real_data.mac_addr[0],&mac_addr[0],6);
            s_real_data.length      = 6;
        
            #if 0
            for(uint8_t i = 0; i < 6;i++)
            {
                NRF_LOG_INFO("MAC[%d] 0x%02x",i,mac_addr[i]);
            }
            NRF_LOG_INFO("mac index --- %d",index);  
            #endif
        
            memcpy(&s_real_data.p_data[0],&sleepmonitor_data.Device_sleep_Array[index].real_data,6);
            
            #if 1
            for(uint8_t j = 0; j < 6;j++)
            {
                NRF_LOG_INFO("sleep--------------data[%d] 0x%02x",j,s_real_data.p_data[j]);
            }
            #endif
        
        
			break;
		case E_CURTAIN:
             //(&curtain_data.mac_index,mac_addr);    //为设备添加匹配关系
            memcpy(mac_addr,dev_info.ble_dev[conn_handle - 1].mac_addr,6);  //获取到conn_handle  对应的mac 地址
            
            index = mac_match_hanle(&curtain_data.mac_index,mac_addr);

            memcpy(&s_real_data.p_data[0],&curtain_data.Device_curtain_Array[index].real_run,3);
            
         
            s_real_data.device_type = E_CURTAIN;
            memcpy(&s_real_data.mac_addr[0],&mac_addr[0],6);
            s_real_data.length      = 3;
        
        

			break;
		case E_06H:
			break;
		case E_PMI:
            memcpy(mac_addr,dev_info.ble_dev[conn_handle - 1].mac_addr,6);  //获取到conn_handle  对应的mac 地址
            
            index = mac_match_hanle(&pmi_data.mac_index,mac_addr);

            memcpy(&s_real_data.p_data[0],&pmi_data.Device_pmi_Array[index].real_data[0],12);
            
         
            s_real_data.device_type = E_PMI;
            memcpy(&s_real_data.mac_addr[0],&mac_addr[0],6);
            s_real_data.length      = 12;
        
        
			break;
		case E_08A:
			break;
		case E_08F:
		
			break;
		case E_09A:
              //(&System_08F.mac_index,mac_addr);    //为设备添加匹配关系
			break;
		case E_09F:
			break;
		 
		 default:
			 break;
	}
    
    return s_real_data;
}



//上传所有设备的运行数据,定时上传5秒一次

void  uart_send_all_data(void)
{
    uint8_t length = 0;
    uint16_t conn_handle;
    _e_machine_model device_type;
    uint8_t i = 0;
    uint8_t data_buffer[100];
    uint8_t index_cnt = 0;
    
    static _real_master_uart real_uart_data;
    
    NRF_LOG_INFO("====================================================== ");
    
    
    send_buffer[0] = START_FLAG;                                //数据头

    send_buffer[1] = 0x00;                                      //数据长度
    send_buffer[2] = CONTROL_CMD_INDEX+length;                  //
    
    
    send_buffer[3] = PROTOCOL_VERSION;                          //协议版本号
    send_buffer[4] = 0x00;                                      //设备类型 
    //memcpy(&send_buffer[5],,6);                               //获取mac 地址
    memset(&send_buffer[5],0,6);

    send_buffer[11] =  (uint8_t)(CONTROL_DATA_COMMAND >> 8);    //命令控制字
    send_buffer[12] =  (uint8_t)CONTROL_DATA_COMMAND;           //命令控制字

    //data 内容
    
    //检查可用设备列表
    index_cnt = 0;
    
    
    //NRF_LOG_INFO("device_num %d ",dev_info.device_num);
    
    
    if(dev_info.device_num != 0)  
    {
        for(i = 0; i < dev_info.device_num; i++)
        {
            conn_handle = sys_avaiable_table.dev_info[i].conn_handle;
            device_type = sys_avaiable_table.dev_info[i].device_type;
            
            if( (device_type != E_INVALID) &&  (conn_handle < 9))
            {
                real_uart_data = match_device_real_status(device_type,conn_handle);

                //NRF_LOG_INFO("zmy-------conn_handle %d device_type %d",conn_handle,device_type);
                
                #if 0
                for(uint8_t j = 0; j < real_uart_data.length ;j++)
                {
                
                    NRF_LOG_INFO("DATA[%d] 0x%02x",j,real_uart_data.p_data[j]);
                }
                #endif
                
                memcpy(&data_buffer[index_cnt+1],&real_uart_data.device_type,real_uart_data.length + 7);
                
                index_cnt = index_cnt + real_uart_data.length + 7;
                
                #if 0
                for(uint8_t j = 0; j < index_cnt ;j++)
                {
                    NRF_LOG_INFO("test----------DATA[%d] 0x%02x",j,data_buffer[j]);
                }
                #endif
                
            }
        }
    }
    
    data_buffer[0] = dev_info.device_num;                        //第一个字节存放总的设备个数        

    memcpy(&send_buffer[13],data_buffer,index_cnt + 1);          
    send_buffer[1] = 0x00;                                        //数据长度
    send_buffer[2] = CONTROL_CMD_INDEX + index_cnt + 1;           //
    
    
    send_buffer[CONTROL_CMD_INDEX + index_cnt + 2 + 1] = Crc8(&send_buffer[1],CONTROL_CMD_INDEX + index_cnt + 2);          //crc 校验

    for(i = 0; i < CONTROL_CMD_INDEX + 3 + index_cnt +1;i++)
    {
        #if 0
        NRF_LOG_INFO("real_data[%d] :0x%02x ",i,send_buffer[i]);
        #else
        app_uart_put(send_buffer[i]);
        #endif
    }
}


//上传所有设备的运行数据,定时上传5秒一次

void  uart_send_all_data_2(void)
{
    uint8_t length = 0;
    uint16_t conn_handle;
    _e_machine_model device_type;
    uint8_t i = 0;
    uint8_t data_buffer[100];
    uint8_t index_cnt = 0;
    
    static _real_master_uart real_uart_data;
    
    NRF_LOG_INFO("====================================================== ");
    
    
    send_buffer[0] = START_FLAG;                                //数据头

    send_buffer[1] = 0x00;                                      //数据长度
    send_buffer[2] = CONTROL_CMD_INDEX+length;                  //
    
    
    send_buffer[3] = PROTOCOL_VERSION;                          //协议版本号
    send_buffer[4] = 0x00;                                      //设备类型 
    //memcpy(&send_buffer[5],,6);                               //获取mac 地址
    memset(&send_buffer[5],0,6);

    send_buffer[11] =  (uint8_t)(CONTROL_DATA_COMMAND >> 8);    //命令控制字
    send_buffer[12] =  (uint8_t)CONTROL_DATA_COMMAND;           //命令控制字

    //data 内容
    
    //检查可用设备列表
    index_cnt = 0;
    
    //NRF_LOG_INFO("device_num %d ",dev_info.device_num);
    memset(data_buffer,0,80);
    
    if(dev_info.device_num != 0)  
    {
        for(i = 0; i < dev_info.device_num; i++)
        {
            conn_handle = sys_avaiable_table.dev_info[i].conn_handle;
            device_type = sys_avaiable_table.dev_info[i].device_type;
            
            if( (device_type != E_INVALID) &&  (conn_handle < 9))
            {
                real_uart_data = match_device_real_status(device_type,conn_handle);

                switch(device_type)
                {
                    case E_RAR312:
                        
                        memcpy(data_buffer,&real_uart_data.device_type,real_uart_data.length + 7);
                           #if 0 
                        for(uint8_t j = 0; j < real_uart_data.length + 7;j++)
                        {
                            NRF_LOG_INFO("RAR312----DATA[%d] 0x%02x",j,data_buffer[j]);
                        }
                   #endif
                        break;
                    case E_RM661:
                        
                        memcpy(&data_buffer[10],&real_uart_data.device_type,real_uart_data.length + 7);
                    
                         #if 0
                        for(uint8_t j = 0; j < real_uart_data.length + 7;j++)
                        {
                            NRF_LOG_INFO("RM661----DATA[%d] 0x%02x",j,data_buffer[10+j]);
                        }
                        #endif
                    
                        break;
                    case E_SLEEP_MONITER:
                        
                        memcpy(&data_buffer[19],&real_uart_data.device_type,real_uart_data.length + 7);
                    
                         #if 0
                        for(uint8_t j = 0; j < real_uart_data.length + 7;j++)
                        {
                            NRF_LOG_INFO("sleep----DATA[%d] 0x%02x",j,data_buffer[19+j]);
                        }
                        #endif
                    
                    
                        break;
                    case E_CURTAIN:
                        
                        memcpy(&data_buffer[37],&real_uart_data.device_type,real_uart_data.length + 7);
                    
                          #if 0
                        for(uint8_t j = 0; j < real_uart_data.length + 7;j++)
                        {
                            NRF_LOG_INFO("curtain ----DATA[%d] 0x%02x",j,data_buffer[37+j]);
                        }
                        #endif
                    
                        break;
                    case E_06H:
                        
                        break;
                    case E_PMI:
                        memcpy(&data_buffer[47],&real_uart_data.device_type,real_uart_data.length + 7);
                    
                    #if 0
                        for(uint8_t j = 0; j < real_uart_data.length + 7;j++)
                        {
                            NRF_LOG_INFO("pmi----DATA[%d] 0x%02x",j,data_buffer[47+j]);
                        }
                        #endif
                        break;
                    case E_08A:
                        break;
                    case E_08F:

                        break;
                    case E_09A:
                        
                        break;
                    case E_09F:
                        break;
                     
                     default:
                         break;
                }
            }
        }
    }
    

    memcpy(&send_buffer[13],data_buffer,66 );          
    send_buffer[1] = 0x00;                                        //数据长度
    send_buffer[2] = CONTROL_CMD_INDEX + 66;                      //
    
    
    send_buffer[CONTROL_CMD_INDEX + 66 + 2 ] = Crc8(&send_buffer[1],CONTROL_CMD_INDEX + 66 + 1);          //crc 校验

    for(i = 0; i < CONTROL_CMD_INDEX + 3 + 66 ;i++)
    {
        #if 0
        NRF_LOG_INFO("real_data[%d] :0x%02x ",i,send_buffer[i]);
        #else
        app_uart_put(send_buffer[i]);
        #endif
    }
}




