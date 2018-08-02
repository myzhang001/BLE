#include "Nus_Master_DataHandle.h"

#include "Protocol_Analysis.h"
#include "Somputon_BLE_DataHandle.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


somputon_data_recv_t  data_recv; 


_s_analsis_word  Common_Word;




//crc8 校验
unsigned char Crc8( unsigned char *Address, unsigned char Length )
{
		static unsigned char crc;
		static unsigned char crcbuff;
		static unsigned char i;
		crc = 0;
	
		while( Length-- )
		{
			crcbuff = *Address ++;
			for(i = 0; i < 8; i++)
			{
				if((crc ^ crcbuff) & 0x01)
				{
					crc ^= 0x18;
					crc >>= 1;
					crc |= 0x80;
				}
				else
				{
					crc >>= 1;
				}
				crcbuff >>= 1;
			}
		}
		return crc;
}



//过滤无效handle
bool NUS_C_Filter_Connected_Handle(uint32_t handle)
{
	if(handle == 0 ||handle == 65535)
	{
		return false;
	}
	else
	{
		return true;
	}
}



typedef enum{
    E_MULTI_DATA_START  = 0X00,
    E_MULTI_DATA_END    = 0x01,
    E_MULTI_DATA_MIDDLE = 0x02,
    E_ONLY_DATA         = 0x03

}_e_data_status;


static uint8_t total_num = 0;  //总包数据
static uint8_t end_num   = 0;  //最后一包数据长度



static uint8_t total_len = 0;
static uint8_t s_cnt_data = 0;      //临时计算包数
static uint8_t s_data_buffer[100];  //存放临时变量

static uint8_t step_status = 0;

uint16_t turn_to_u16(uint8_t high, uint8_t low)
{
	uint16_t data_all = 0;
	data_all = high;
	data_all = data_all<<8;
	data_all |= low&0x00FF;
	
	return data_all;
}

//处理串口数据
void nus_data_handle(uint32_t nus_c_conn_handle, uint8_t *data, uint8_t length)
{
	
	uint8_t data_buffer[100];       	//接收数据缓冲区大小
	
	memcpy(data_buffer,data,length);
	
	
    uint8_t step = 0;
    
    
    #if 0         
    for(uint8_t i = 0; i< length;i++)
    {
        NRF_LOG_INFO("DATA[%d] 0x%02x",i,data_buffer[i]);
    }
    #endif

        #if 1
        if((data_buffer[0]== 0x3a)&&(data_recv.receive_start_flag== false))
		{
            //NRF_LOG_INFO("Flag OK!\r\n");
			memcpy(&data_recv.rece_data[data_recv.data_index],data_buffer,length);
			data_recv.data_index += length;			
			data_recv.total_len= turn_to_u16(data_recv.rece_data[1],data_recv.rece_data[2])+3;						
			data_recv.receive_start_flag = true;			
		}
		else if((data_recv.rece_data[0] == 0x3a)&&(data_recv.receive_start_flag == true))
		{
			
			if(data_recv.data_index < data_recv.total_len)
			{
//								printf("still recv\r\n");
				memcpy(&data_recv.rece_data[data_recv.data_index],data_buffer,length);
				data_recv.data_index += length; 
			}

		}
		else
		{
//								printf("other data\r\n");
				data_recv.data_index = 0;
				data_recv.receive_start_flag = false;
				data_recv.total_len = 0;
				memset(data_recv.rece_data,0,sizeof(data_recv.rece_data));
		}

		if(data_recv.data_index >= data_recv.total_len)										//receive data end					
		{				
//			for(uint8_t i=0;i<clife_data_recv.data_index;i++)
//				printf("0x%02x ",clife_data_recv.rece_data[i]);
//			printf("\r\n");		
			if( data_recv.rece_data[data_recv.data_index - 1] != Crc8( &data_recv.rece_data[1], data_recv.data_index - 2 ) )
			{
				//printf("APP->BT: crc error\r\n");
				memset(data_recv.rece_data,0,sizeof(data_recv.rece_data));
				data_recv.receive_start_flag = false;
				data_recv.total_len = 0;
				data_recv.data_index = 0;				
				return;
			}
			//handle_clife_data(data_recv.rece_data,data_recv.data_index);
            
            //NRF_LOG_INFO("APP->BT: crc ok\r\n");
            
            Common_Word.Data_Length  = data_recv.rece_data[DATA_LENGTH_INDIX_LOW];    //获取数据包长度
            Common_Word.Data_Length = Common_Word.Data_Length>>8;
            Common_Word.Data_Length |= data_recv.rece_data[DATA_LENGTH_INDIX_HIGH];

            //NRF_LOG_INFO("DATA LENGHT : %d",(uint8_t)Common_Word.Data_Length);

            Common_Word.Common_World = data_recv.rece_data[DATA_COMMOND_WORD];	   //获取命令字
            Common_Word.Common_World = Common_Word.Common_World << 8;
            Common_Word.Common_World |= data_recv.rece_data[DATA_COMMOND_WORD +1 ];        

            NRF_LOG_INFO("COMMAND WORLD:  0X%04x",Common_Word.Common_World);

            Common_Word.Device_Type = data_recv.rece_data[DATA_DEVICE_TYPE_INDEX];     //获取设备类型
            
            NRF_LOG_INFO("Device_Type :%d",Common_Word.Device_Type);

            
            memcpy(&Common_Word.MacAddr_Device,&data_recv.rece_data[DATA_DEVICE_MAC_INDEX],6);  //拷贝mac 地址
            
            
            if(receive_data_from_app != NULL)
            {
                //receive_data_from_app(Common_Word.Common_World,&data_recv.rece_data[DATA_CONTENT_INDEX],Common_Word.Data_Length - 11); 
                receive_data_from_app((uint16_t)nus_c_conn_handle,Common_Word.Common_World,
                &s_data_buffer[DATA_CONTENT_INDEX],Common_Word.Data_Length - 11,
                Common_Word.Device_Type,Common_Word.MacAddr_Device);
            }
            
            
			memset(data_recv.rece_data,0,sizeof(data_recv.rece_data));
			data_recv.receive_start_flag = false;
			data_recv.total_len = 0;
			data_recv.data_index = 0;
		}
    
    #endif
    
    #if 0
    if(data_buffer[0] == START_FLAG  && length == 20)   //第一包起始
    {
        //step = E_ONLY_DATA;
        Common_Word.Data_Length  = data_buffer[DATA_LENGTH_INDIX_LOW];    //获取数据包长度
		Common_Word.Data_Length    = Common_Word.Data_Length>>8;
		Common_Word.Data_Length |= data_buffer[DATA_LENGTH_INDIX_HIGH];
		
        NRF_LOG_INFO("DATA LENGHT : %d",Common_Word.Data_Length);
        
		Common_Word.Common_World = data_buffer[DATA_COMMOND_WORD ];	      //获取命令字
		Common_Word.Common_World = Common_Word.Common_World << 8;
		Common_Word.Common_World |= data_buffer[DATA_COMMOND_WORD +1 ];        
		
        NRF_LOG_INFO("COMMAND WORLD:  0X%04x",Common_Word.Common_World);
        
		
		Common_Word.Device_Type = data_buffer[DATA_DEVICE_TYPE_INDEX];     //获取设备类型
		
	    memcpy(&Common_Word.MacAddr_Device,&data_buffer[DATA_DEVICE_MAC_INDEX],6);  //拷贝mac 地址
             
        step_status = 1;
        
        memcpy(&s_data_buffer,data_buffer,20);
        
        
        #if 0
        for(uint8_t i = 0; i < 20;i++)
        {
            NRF_LOG_INFO("ZMY_DATA[%d] = 0x%02x ",i,s_data_buffer[i]);
        }
        #endif
        
         //step = E_MULTI_DATA_START;
    }
    else if(data_buffer[0] != START_FLAG && length < 20) 
    {
        if( step_status == 1)
        {
            memcpy(&s_data_buffer[20],data_buffer,length);
            step_status = 0;
            
            #if 0
            for(uint8_t i = 0; i < length;i++)
            {
                NRF_LOG_INFO("ZMY_DATA[%d] = 0x%02x ",i,s_data_buffer[20+i]);
            }
            #endif
            
           //memcpy(&data_buffer[20],s_data_buffer,20 + length - 1);

            #if 0
            for(uint8_t i = 0; i < 20 + length;i++)
            {
                NRF_LOG_INFO("ZMY_DATA[%d] = 0x%02x ",i,s_data_buffer[i]);
            }
            #endif
            
            if(s_data_buffer[Common_Word.Data_Length + 2]!= Crc8(&s_data_buffer[1],Common_Word.Data_Length + 1))  //crc8校验
            {
                //#ifdef UART_MASTER_TEST 
                NRF_LOG_INFO(" data_buffer crc 0x%02x",s_data_buffer[Common_Word.Data_Length]);
                
                NRF_LOG_INFO("CRC ERROR");
                //#endif
                //return;
            }
            //处理数据内容
            
            
            
            if(receive_data_from_app != NULL)
            {
                receive_data_from_app((uint16_t)nus_c_conn_handle,Common_Word.Common_World,
                &s_data_buffer[DATA_CONTENT_INDEX],Common_Word.Data_Length - 11,
                Common_Word.Device_Type,Common_Word.MacAddr_Device);
            }
            
            //memset(,,);
        }
        else
        {
            step_status = 0;
            return ;
        }
       
         //step = E_MULTI_DATA_MIDDLE;   
    }
    else if(data_buffer[0] == START_FLAG && length < 20  )                //正常所有低于20字节的数据包
    {
        
        //step = E_ONLY_DATA;
        Common_Word.Data_Length  = data_buffer[DATA_LENGTH_INDIX_LOW];    //获取数据包长度
		Common_Word.Data_Length = Common_Word.Data_Length>>8;
		Common_Word.Data_Length |= data_buffer[DATA_LENGTH_INDIX_HIGH];
		
        NRF_LOG_INFO("DATA LENGHT : %d",Common_Word.Data_Length);
        
		Common_Word.Common_World = data_buffer[DATA_COMMOND_WORD ];	      //获取命令字
		Common_Word.Common_World = Common_Word.Common_World << 8;
		Common_Word.Common_World |= data_buffer[DATA_COMMOND_WORD +1 ];        
		
        NRF_LOG_INFO("COMMAND WORLD:  0X%04x",Common_Word.Common_World);
        
		
		Common_Word.Device_Type = data_buffer[DATA_DEVICE_TYPE_INDEX];     //获取设备类型
		
	    memcpy(&Common_Word.MacAddr_Device,&data_buffer[DATA_DEVICE_MAC_INDEX],6);  //拷贝mac 地址		 	  
			
		
		if(data_buffer[Common_Word.Data_Length + 2]!= Crc8(&data_buffer[1],Common_Word.Data_Length + 1))  //crc8校验
		{
			//#ifdef UART_MASTER_TEST 
			NRF_LOG_INFO(" data_buffer crc 0x%02x",Crc8(&data_buffer[1],Common_Word.Data_Length + 1));
			
			NRF_LOG_INFO("CRC ERROR");
			//#endif
			return;
		}
		//处理数据内容
		
		if(receive_data_from_app != NULL)
		{
			receive_data_from_app((uint16_t)nus_c_conn_handle,Common_Word.Common_World,
            &data_buffer[DATA_CONTENT_INDEX],Common_Word.Data_Length - 11,
            Common_Word.Device_Type,Common_Word.MacAddr_Device);
		}
        
    }    
    else
    {
        return ;
    }
    #endif
    
   
}


