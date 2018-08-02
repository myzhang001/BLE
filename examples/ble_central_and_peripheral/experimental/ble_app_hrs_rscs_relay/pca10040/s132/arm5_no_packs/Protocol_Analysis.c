#include "Protocol_Analysis.h"
#include "Somputon_BLE_DataHandle.h"
#include "User_MacList.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

appRecvHandler_Cb  receive_data_from_app;   //数据接收回调



void regRevAppDataCb(appRecvHandler_Cb cb)
{
		receive_data_from_app = cb;
}



/**@brief Somputon_Init
 * Register a Somputon command word callback function for application
 
 * @param[in] cb Function will be called when data have been received complete from BLE.
 */
void Somputon_Init(appRecvHandler_Cb cb)
{
	regRevAppDataCb(cb);	
}






/**@brief     Function for handling clife command word in the application
 *
 * @param[in] command 			clife command word
 *         	  src_data    	    source data
 *         	  len 			    data length
 *
 * @return    void
 */
void App_RecvHandler(uint16_t conn_handle,uint16_t command, uint8_t* data,uint16_t len,_e_machine_model device_type,uint8_t mac_addr[6])
{
    
        uint8_t i = 0;
    
		switch(command)
		{			
			case ENCRYPTION_REQ_COMMAND:                                                //收到认证指令
					control_device_cmd(NULL);			      		
            
					break;
			case CLEAR_HISTORY_DATA_COMMAND:
					 clear_history_data_cmd();                                          //清除历史数据
					 break;			
			
			case GET_REAL_TIME_DATA_COMMAND_REPLY:					 
                    get_real_time_data_cmd(conn_handle,device_type,mac_addr,data,len);	 //获取实时时间

                    #if 0
                    for(i = 0; i < len ;i++)
                    {
                        NRF_LOG_INFO(" REALTIME DATA %d 0x%02x ",i,data[i]);
                    }
                    NRF_LOG_INFO(" device_type %d  ",device_type);
                    #endif  
					 break;		
			case BOND_COMMAND_REPLY:
					bond_cmd(conn_handle,device_type,mac_addr);                              //发送绑定指令
           
                    device_bond_status_update(conn_handle);                                  //绑定标志位
                    device_add_type(conn_handle,device_type);                                //添加设备类型

                    NRF_LOG_INFO("-------------------conn_handle %d device_type: %d",conn_handle,device_type);
            
            
                #if 0
                        #ifdef USER_UART_PRINT
                
                        NRF_LOG_INFO("BOND_COMMAND : conn_handle 0x%02x  device_type :0x%02x ",conn_handle,device_type);
                        for(uint8_t i = 0; i<6;i++)
                        {
                            NRF_LOG_INFO("mac [%d]: 0x%02x",i,mac_addr[i]);
                        }
                        #endif
                #endif
					 break;		
												
			case GET_HISTORY_DATA_TOTAL_PACKETS:
			case GET_HISTORY_DATA_COMMAND:
			case HISTORY_DATA_REPLY_FROM_APP:	
					 Clife_GetHisData(command,data,len);
					 break;
			
			case SET_DEVICE_TIME_COMMAND:			
					 set_device_time_cmd(data);				//先同步APP的时间					 
					 break;		
			
			case GET_DEVICE_TIME_COMMAND:						 
					  get_device_time_cmd(1);			    //上传时间
					 break;
			
			case CONTROL_DATA_COMMAND:
					 control_device_cmd(data);				//处理控制数据	   	
					break;			
			case CLEAR_BOND_COMMAND_REPLY:                  //清除绑定数据 
                    //bond_info_del(&bond_device_info,mac_addr,device_type);
                    break;
			default:				
					break;
		}		
			
}





