#include "data_send_handle.h"
#include "common_include.h"




uint8_t buffer_send[1000];       //发送缓冲区
uint8_t total_index  = 0;        //需要发送数据总索引
uint8_t send_current_index = 0;  //当前索引

static uint16_t send_conn_handle;  //发送conn_handle


void data_send()
{
    uint32_t    err_code;
    uint8_t  len;
    uint16_t p_length[4];
    
    
    if(send_current_index >= total_index)
    {
        return ;
    }
    //NRF_LOG_INFO("\r\nsend_current :%d  total_index :%d",send_current_index,total_index);
    while (1)
    {
         len = ( total_index - send_current_index > 20 )? (20):( total_index - send_current_index );
        
         p_length[0] = len;
        
         //NRF_LOG_INFO("\r\n__________len :%d",len);
        
         err_code = send_string_c(send_conn_handle,buffer_send + send_current_index,len);
        
         //err_code = ble_nus_string_send(&m_nus,buffer_send + send_current_index,&p_length[0]);
        
         // If transmission succeeded, increment payload. 
        if (err_code == NRF_SUCCESS)
        {
            send_current_index = send_current_index + len;
            
           if( send_current_index >= total_index)
           {
                break;
           }
        }
        else if (err_code != NRF_ERROR_INVALID_STATE && \

            err_code != NRF_ERROR_RESOURCES && \

            err_code != BLE_ERROR_INVALID_CONN_HANDLE)
        {
            APP_ERROR_CHECK(err_code);
        }
        else
        {
            NRF_LOG_INFO("\r\n  error");
            
            break;
        }
    }    
}



//数据发送
void big_data_send_proc(uint16_t conn_handle,uint8_t *p_data,uint8_t length)
{
    send_conn_handle = conn_handle;
    total_index =  length;
    send_current_index = 0;

    memcpy(&buffer_send[0],p_data,length);
   
    data_send();
}
