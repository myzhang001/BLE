#include "slave_device_data.h"


#include "app_uart.h"
#include "app_fifo.h"
#include "nrf_drv_uart.h"



uint8_t UART_ReceiveBuffer[200];        //接收数据缓冲区
static _s_analsis_word  Common_Word;



appRecvHandler_Cb  master_receive_data_from_app;   //数据接收回调



uint16_t  master_control_data(_e_machine_model device_type);

_data_struct_control control_data={
.conn_handle = 0,


};     //处理下发数据结构



void master_send_string(uint8_t *p_data,uint8_t len);



uint8_t step_recv =0;



void WIFI_Decode(void)
{
    uint8_t RxData = 0;
    static uint8_t RxCnt = 0; 
    static uint8_t gHeadflag = 0,lenflag = 0;
    uint8_t temp;
    
    if(UART2_Read_Queue(&RxData)==0) 
    {
        return;
    }
    
    //app_uart_put(RxData);
    
    #if 1
    if(gHeadflag == 0)
    {
         if(RxData == 0x3A )
        {
            RxCnt = 0;
            UART_ReceiveBuffer[RxCnt] = RxData;
            gHeadflag = 1;
            lenflag   = 0;
        }
    }
    else
    {
        RxCnt++;
        UART_ReceiveBuffer[RxCnt] = RxData;

        //app_uart_put(RxCnt);
        
        if(lenflag == 0)
        {
            //if(RxCnt < 12)return;
            if(RxCnt == 12)
            {
                Common_Word.Common_World =  UART_ReceiveBuffer[11];	                 //获取命令字
                Common_Word.Common_World =  Common_Word.Common_World >> 8;
                Common_Word.Common_World |= UART_ReceiveBuffer[11 + 1];     
                
                Common_Word.Data_Length  = UART_ReceiveBuffer[DATA_LENGTH_INDIX_LOW];  //获取数据包长度
                Common_Word.Data_Length = Common_Word.Data_Length>>8;
                Common_Word.Data_Length |= UART_ReceiveBuffer[DATA_LENGTH_INDIX_HIGH];
                
                Common_Word.Device_Type = UART_ReceiveBuffer[DATA_DEVICE_TYPE_INDEX];
                
                memcpy(&Common_Word.MacAddr_Device[0],&UART_ReceiveBuffer[DATA_DEVICE_MAC_INDEX],6);
                
                
                if(UART_ReceiveBuffer[3] != PROTOCOL_VERSION)                       //协议版本号
                {
                    gHeadflag = 0;
                    return;
                }
                if(Common_Word.Common_World == CONTROL_DATA_COMMAND || Common_Word.Common_World == CONTROL_STA_COMMAND ||
                    Common_Word.Common_World == CLEAR_BOND_COMMAND || Common_Word.Common_World == BOND_COMMAND ||
                Common_Word.Common_World == ENCRYPTION_REQ_REPLY || Common_Word.Common_World == GET_DEVICE_TIME_COMMAND ||
                Common_Word.Common_World == SET_DEVICE_TIME_COMMAND || Common_Word.Common_World == GET_HISTORY_DATA_TOTAL_PACKETS ||
                Common_Word.Common_World == GET_REAL_TIME_DATA_COMMAND )
                {
                    lenflag = 1;
                }
                else
                {
                    gHeadflag = 0;
                }
            }
 
        }
        //else if( Common_Word.Common_World == 0x0040)
        else
        {
            if(RxCnt >= Common_Word.Data_Length + 2)
            {
                if(UART_ReceiveBuffer[Common_Word.Data_Length + 2] == Crc8(&UART_ReceiveBuffer[1],Common_Word.Data_Length + 1))
                {
                    app_uart_put(0xaa); 
                    app_uart_put(0xaa); 
                    app_uart_put(0xaa); 
                    
                    if(master_receive_data_from_app != NULL)
                    {
                        master_receive_data_from_app((uint16_t)0,Common_Word.Common_World,
                        &UART_ReceiveBuffer[DATA_CONTENT_INDEX],Common_Word.Data_Length - 11,
                        Common_Word.Device_Type,Common_Word.MacAddr_Device);
                    }
                    
                    Common_Word.Common_World = 0;
                    gHeadflag = 0;
                    UART_ReceiveBuffer[Common_Word.Data_Length + 2]  = 0;
                    memset(UART_ReceiveBuffer,0,20);
                }
                else
                {
                    gHeadflag = 0;
                    return;
                }
            }
        }
        #if 0
        else
        {
            gHeadflag = 0;
        }
        #endif        
    }  
    
    #endif 
}






void master_cb_init(appRecvHandler_Cb cb)
{
	master_receive_data_from_app = cb;	
}

uint16_t command_s = 0;



/**@brief     Function for handling clife command word in the application
 *
 * @param[in] command 			clife command word
 *         	  src_data    	    source data
 *         	  len 			    data length
 *
 * @return    void
 */
void Master_App_RecvHandler(uint16_t conn_handle,uint16_t command, uint8_t* data,uint16_t len,
                            _e_machine_model device_type,uint8_t mac_addr[6])
{
    
        uint8_t i = 0;
    
        command_s = command;
    
    
    
		switch(command)
		{			
			case ENCRYPTION_REQ_REPLY:
						      												
					break;
			case CLEAR_HISTORY_DATA_COMMAND:
					
					 break;			
			
			case GET_REAL_TIME_DATA_COMMAND_REPLY:					 
                   
					 break;		
			case BOND_COMMAND_REPLY:
					
					 break;		
												
			case GET_HISTORY_DATA_TOTAL_PACKETS:
			case GET_HISTORY_DATA_COMMAND:
			case HISTORY_DATA_REPLY_FROM_APP:	
					 
					 break;
			
			case SET_DEVICE_TIME_COMMAND:			
					 					 
					 break;		
			
			case GET_DEVICE_TIME_COMMAND:						 
					
					 break;
			
			case CONTROL_DATA_COMMAND:                      //下发控制数据
					
                control_data.conn_handle = master_control_data(device_type);
                control_data.conn_data_flag = 1;

                //app_uart_put(device_type);
            
                memcpy( control_data.data_buffer,data,len);
                control_data.length   = len;

                //master_send_string(control_data.data_buffer,len);
              
					break;			
			case CLEAR_BOND_COMMAND_REPLY:                  
                    
                    break;
			default:				
					break;
		}		
			
}



//根据设备类型查找 handle
uint16_t  master_control_data(_e_machine_model device_type)
{
    if(device_type ==  E_INVALID )
    {
        return 0xff;
    }
    
    uint16_t  conn_handle;
    
    
    for(uint8_t i = 0;i < sys_avaiable_table.total_avaiable_num + 1;i++)
    {
        if(sys_avaiable_table.dev_info[i].device_type == device_type)
        {
        
            conn_handle = sys_avaiable_table.dev_info[i].conn_handle ;
            break;
        }
    }
    
    return conn_handle;
}



void master_send_string(uint8_t *p_data,uint8_t len)
{
    for(uint8_t i = 0; i < len ;i++)
    {
        app_uart_put(p_data[i]);
    }
}




_uart_status  UART_SEND_STATUS;


//串口应答流程，所有串口发送数据
void master_uart_ack(void)
{
    switch(UART_SEND_STATUS)
    {
        case E_REAL_DATA:
                
        
            break;
        case E_CONTROL_ACK:
            break;

        default:
            break;
    
    }








}



