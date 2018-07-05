#include "Nus_Master_DataHandle.h"

#include "Protocol_Analysis.h"
#include "Somputon_BLE_DataHandle.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"





_s_analsis_word  Common_Word;




//crc8 У��
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



//������Чhandle
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


static uint8_t total_num = 0;  //�ܰ�����
static uint8_t end_num   = 0;  //���һ�����ݳ���



static uint8_t total_len = 0;
static uint8_t s_cnt_data = 0;      //��ʱ�������
static uint8_t s_data_buffer[100];  //�����ʱ����

static uint8_t step_status = 0;



//����������
void nus_data_handle(uint32_t nus_c_conn_handle, uint8_t *data, uint8_t length)
{
	
	uint8_t data_buffer[100];       	//�������ݻ�������С
	
	memcpy(data_buffer,data,length);
	//memset(data,0,length);
	
    uint8_t step = 0;
    
    
    #if 0           
    for(uint8_t i = 0; i< length;i++)
    {
        NRF_LOG_INFO("DATA[%d] 0x%02x",i,data_buffer[i]);
    }
    #endif

    #if 0
    if(data_buffer[0] == START_FLAG  && length == 20)   //��һ����ʼ
    {
        step = E_MULTI_DATA_START;
    }
    else if(data_buffer[0] != START_FLAG  && length < 20 )  //����������һ��
    {
        step = E_MULTI_DATA_END;
    
    }
    else if(data_buffer[0] != START_FLAG && length == 20) //������� �м�N ��
    {
         step = E_MULTI_DATA_MIDDLE;   
        
    }
    else if(data_buffer[0] == START_FLAG && length < 20  )  //�������е���20�ֽڵ����ݰ�
    {
        step = E_ONLY_DATA;
    }    
    else
    {
        return ;
    }
    
    switch(step)
    {
        case E_MULTI_DATA_START:
            
            Common_Word.Data_Length  = data_buffer[DATA_LENGTH_INDIX_LOW];    //��ȡ���ݰ�����
            Common_Word.Data_Length = Common_Word.Data_Length>>8;
            Common_Word.Data_Length |= data_buffer[DATA_LENGTH_INDIX_HIGH];

            //NRF_LOG_INFO("DATA LENGHT : %d",Common_Word.Data_Length);

            Common_Word.Common_World = data_buffer[DATA_COMMOND_WORD ];	   //��ȡ������
            Common_Word.Common_World = Common_Word.Common_World << 8;
            Common_Word.Common_World |= data_buffer[DATA_COMMOND_WORD +1 ];        

            //NRF_LOG_INFO("COMMAND WORLD:  0X%04x",Common_Word.Common_World);


            Common_Word.Device_Type = data_buffer[DATA_DEVICE_TYPE_INDEX];     //��ȡ�豸����

            memcpy(&Common_Word.MacAddr_Device,&data_buffer[DATA_DEVICE_MAC_INDEX],6);  //����mac ��ַ	

            if( ( Common_Word.Data_Length - 11 - 6 ) > 0 )   //�ж������
            {
                
               total_num  = (Common_Word.Data_Length - 11 - 6)/20 ;
               if( (Common_Word.Data_Length - 11 - 6)%20 != 0)
               {
                    total_num += 1;
                    end_num = (Common_Word.Data_Length - 11 - 6)%20;
               }
               s_cnt_data = 1;
               memcpy(s_data_buffer,data_buffer,6);       //������һ������

               step_status = 1;
               
            }
            else if( ( Common_Word.Data_Length - 11 - 6 ) ==  0 ) //ֻ��һ������
            {
                
                
            }
            break;
        case E_MULTI_DATA_END: 
            if( step_status == 1)
            {
                s_cnt_data++;
                
                if( end_num == 0)  //���һ��Ϊ0
                {
                    if( s_cnt_data >= total_num )
                    {
                        
                    }
                    memcpy(&s_data_buffer[6],data_buffer,20);       //������һ������ 
                    
                }
                else               //���һ�����ݲ�Ϊ0
                {
                
                    
                }
                
                

                
            }
            else
            {
            
                return ;
            }
        
            break;
        case E_MULTI_DATA_MIDDLE:
                
        
        
        
            break;
        case E_ONLY_DATA:
            
            break;
        default:
            break;
  
    }
    #endif
    
  
    if(data_buffer[0] == START_FLAG  && length == 20)   //��һ����ʼ
    {
        //step = E_ONLY_DATA;
        Common_Word.Data_Length  = data_buffer[DATA_LENGTH_INDIX_LOW];    //��ȡ���ݰ�����
		Common_Word.Data_Length    = Common_Word.Data_Length>>8;
		Common_Word.Data_Length |= data_buffer[DATA_LENGTH_INDIX_HIGH];
		
        NRF_LOG_INFO("DATA LENGHT : %d",Common_Word.Data_Length);
        
		Common_Word.Common_World = data_buffer[DATA_COMMOND_WORD ];	      //��ȡ������
		Common_Word.Common_World = Common_Word.Common_World << 8;
		Common_Word.Common_World |= data_buffer[DATA_COMMOND_WORD +1 ];        
		
        NRF_LOG_INFO("COMMAND WORLD:  0X%04x",Common_Word.Common_World);
        
		
		Common_Word.Device_Type = data_buffer[DATA_DEVICE_TYPE_INDEX];     //��ȡ�豸����
		
	    memcpy(&Common_Word.MacAddr_Device,&data_buffer[DATA_DEVICE_MAC_INDEX],6);  //����mac ��ַ
             
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
            
            if(s_data_buffer[Common_Word.Data_Length + 2]!= Crc8(&s_data_buffer[1],Common_Word.Data_Length + 1))  //crc8У��
            {
                //#ifdef UART_MASTER_TEST 
                NRF_LOG_INFO(" data_buffer crc 0x%02x",s_data_buffer[Common_Word.Data_Length]);
                
                NRF_LOG_INFO("CRC ERROR");
                //#endif
                //return;
            }
            //������������
            
            
            
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
    else if(data_buffer[0] == START_FLAG && length < 20  )                //�������е���20�ֽڵ����ݰ�
    {
        
        //step = E_ONLY_DATA;
        Common_Word.Data_Length  = data_buffer[DATA_LENGTH_INDIX_LOW];    //��ȡ���ݰ�����
		Common_Word.Data_Length = Common_Word.Data_Length>>8;
		Common_Word.Data_Length |= data_buffer[DATA_LENGTH_INDIX_HIGH];
		
        NRF_LOG_INFO("DATA LENGHT : %d",Common_Word.Data_Length);
        
		Common_Word.Common_World = data_buffer[DATA_COMMOND_WORD ];	      //��ȡ������
		Common_Word.Common_World = Common_Word.Common_World << 8;
		Common_Word.Common_World |= data_buffer[DATA_COMMOND_WORD +1 ];        
		
        NRF_LOG_INFO("COMMAND WORLD:  0X%04x",Common_Word.Common_World);
        
		
		Common_Word.Device_Type = data_buffer[DATA_DEVICE_TYPE_INDEX];     //��ȡ�豸����
		
	    memcpy(&Common_Word.MacAddr_Device,&data_buffer[DATA_DEVICE_MAC_INDEX],6);  //����mac ��ַ		 	  
			
		
		if(data_buffer[Common_Word.Data_Length + 2]!= Crc8(&data_buffer[1],Common_Word.Data_Length + 1))  //crc8У��
		{
			//#ifdef UART_MASTER_TEST 
			NRF_LOG_INFO(" data_buffer crc 0x%02x",Crc8(&data_buffer[1],Common_Word.Data_Length + 1));
			
			NRF_LOG_INFO("CRC ERROR");
			//#endif
			return;
		}
		//������������
		
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
    
    
    
    #if 0
	if(data_buffer[0]!= START_FLAG)	
	{
		return;
	}
	else
	{
		Common_Word.Data_Length  = data_buffer[DATA_LENGTH_INDIX_LOW];    //��ȡ���ݰ�����
		Common_Word.Data_Length = Common_Word.Data_Length>>8;
		Common_Word.Data_Length |= data_buffer[DATA_LENGTH_INDIX_HIGH];
		
        //NRF_LOG_INFO("DATA LENGHT : %d",Common_Word.Data_Length);
        
		Common_Word.Common_World = data_buffer[DATA_COMMOND_WORD ];	   //��ȡ������
		Common_Word.Common_World = Common_Word.Common_World << 8;
		Common_Word.Common_World |= data_buffer[DATA_COMMOND_WORD +1 ];        
		
        //NRF_LOG_INFO("COMMAND WORLD:  0X%04x",Common_Word.Common_World);
        
		
		Common_Word.Device_Type = data_buffer[DATA_DEVICE_TYPE_INDEX];     //��ȡ�豸����
		
	    memcpy(&Common_Word.MacAddr_Device,&data_buffer[DATA_DEVICE_MAC_INDEX],6);  //����mac ��ַ		 	  
			
		
		if(data_buffer[Common_Word.Data_Length + 2]!= Crc8(&data_buffer[1],Common_Word.Data_Length + 1))  //crc8У��
		{
			//#ifdef UART_MASTER_TEST 
			NRF_LOG_INFO(" data_buffer crc 0x%02x",data_buffer[Common_Word.Data_Length]);
			
			NRF_LOG_INFO("CRC ERROR");
			//#endif
			return;
		}
		//������������
		
		if(receive_data_from_app != NULL)
		{
			receive_data_from_app((uint16_t)nus_c_conn_handle,Common_Word.Common_World,
            &data_buffer[DATA_CONTENT_INDEX],Common_Word.Data_Length - 11,
            Common_Word.Device_Type,&Common_Word.MacAddr_Device);
		}
		
	}
    
    #endif
}


