#include "Master_DataStruct.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "User_MacList.h"




_datastruct_08F   System_08F={0     
};        //08F  ϵͳ����

_datastruct_rm661 rm661_data;    //661 �����豸

_datastruct_rar312 rar312_data;  //rar312_data �����豸

_datastruct_sleepmonitor sleepmonitor_data;    //˯�߼��


_datastruct_curtain curtain_data;  //cuituain_data �����豸

_datastruct_pmi pmi_data;          //pmi �����豸


//��������   device_index 0 - 7

void Master_ResetBuffer_08F_RealStatus(uint8_t device_index)
{
	//memset(&Device_08F_Array[device_index],0,sizeof(_real_staus_08F));
}


//������������   all
void Master_ResetBuffer_08F_RealStatus_All(void)
{
	//memset(&Device_08F_Array,0,sizeof(_real_staus_08F)*8);
}



//���mac ��ַ�Ϸ���   0:Ϊ�Ƿ��ĵ�ַ   1���Ϸ���ַ

uint8_t  Check_Mac(uint8_t mac_addr[6])
{
	
	uint8_t ret;
	
	
	if(mac_addr[0]==0 &&mac_addr[1]==0 &&
		mac_addr[2]==0 &&mac_addr[3]==0 &&
		mac_addr[4]==0 &&mac_addr[5]==0 )
	{
		ret =  0;
	
	}
	else
	{
		ret = 1;
	}
	
	
	if(mac_addr[0]==0xff &&mac_addr[1]==0xff &&
		mac_addr[2]==0xff &&mac_addr[3]==0xff &&
		mac_addr[4]==0xff &&mac_addr[5]==0xff )
	{
		ret =  0;
	
	}
	else
	{
		ret = 1;
	}
	return ret;
}

//ret: 0  �Ƿ�����ĵ�ַ


uint8_t Mac_Compare(uint8_t src_mac[6],uint8_t dst_mac[6])
{
	
	uint8_t i = 0;
	
	
	if(Check_Mac(src_mac) == 0)
	{
		return 0;
	}
	if(Check_Mac(dst_mac) == 0)
	{
		return 0;
	}
	
	for(i = 0; i < 8;i++)
	{
		if(src_mac[i]!=dst_mac[i])
		{
			break;
		}
	}
	
	//if(  )
	return 0;

}




//��������Ƿ����
uint8_t Empty_Mac_List(_mac_index_fix addr_mac)
{
	uint8_t  ret;
	
	if( addr_mac.index == 0 &&  addr_mac.mac_addr[0]== 0x00 &&
		addr_mac.mac_addr[1]== 0x00 &&addr_mac.mac_addr[2]== 0x00 &&
		addr_mac.mac_addr[3]== 0x00 &&addr_mac.mac_addr[4]== 0x00 &&
		addr_mac.mac_addr[5]== 0x00 )
	{
	
		ret =1 ;
	
	}
	else
	{
		ret = 0;
	}

	return ret;
}



void BubbleSort1(uint8_t *arr,int sz){
    int i = 0;
    int j = 0;
   
    for(i=0;i<sz-1;i++){
        for(j=0;j<sz-i-1;j++){
            if(arr[j]>arr[j+1]){
                int tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}


void  Index_Sort_Push(_mac_index_data *device_list, uint8_t index)
{
	
	if(device_list == NULL)
	{
		return;
	}

	device_list->index_sort[device_list->device_num] =  index;   //���
	
    NRF_LOG_INFO("sort --------------- device_num :%d ",device_list->device_num);
    
	//��Ӻ�����
	BubbleSort1(device_list->index_sort,device_list->device_num);    //����
	
	device_list->max_index_decice = device_list->index_sort[device_list->device_num];
	

    #ifdef USER_UART_PRINT
    for(uint8_t i = 0 ; i < device_list->max_index_decice + 1 ; i++)
    {
        NRF_LOG_INFO("������� %d :[%d] ",i,device_list->index_sort[i]);
    }
    #endif

}




void Index_Sort_POP(_mac_index_data *device_list ,uint8_t index)
{
	
	uint8_t i = 0;
	
	
	if( device_list == NULL)
	{
		return ;
	}
	
	for(i = 0; i < device_list->device_num;i++)
	{
		if(device_list->index_sort[i]== index )
        {
            break;
        }
	}
	
	if(i < device_list->device_num)
    {
        device_list->index_sort[i] = device_list->device_num;
    }

    //ɾ��������
	
	BubbleSort1(device_list->index_sort,device_list->device_num);
    
}






//���б����������豸
uint8_t Add_Device_List(_mac_index_data *device_list,uint8_t mac_addr[6])
{
	
	uint8_t ret;
	uint8_t i,j,k;
	
	if(device_list == NULL)
	{
		return ret = 0;
	}

	_mac_index_data *s_device_list = device_list;
	
    //NRF_LOG_INFO("DEVICE_LIST_num -------------- 1 %d ",s_device_list->device_num);
    
	if(s_device_list->device_num == 0)   //�豸�б�Ϊ��
	{
		s_device_list->mac_list_array[0].index  = 0;
		memcpy(&s_device_list->mac_list_array[0].mac_addr,&mac_addr[0],MAC_LEN);   //����mac��ַ������
        
        s_device_list->device_num += 1;											   //��ӵ��豸����+1
                                                                                   
        s_device_list->max_index_decice = 0;                                      //�������+1
        
        //Index_Sort_Push(s_device_list,i);                                        //���������б�  
        s_device_list->index_sort[0] = 0;
        
    }
	else
	{
        //NRF_LOG_INFO("DEVICE_LIST_num --- 2 %d ",s_device_list->device_num);
        if(s_device_list->device_num >= MAX_DEVICE_NUM )return 0;
		//�豸�б�Ϊ��,���mac ��ַ�Ƿ���ͬ����ͬ���ش��󣬲�ͬ�������
        
        //1.���������豸 �Ƿ����ظ��豸
        for(i = 0; i < s_device_list->max_index_decice + 1;i++)
        {
            if(memcmp(&s_device_list->mac_list_array[i].mac_addr,&mac_addr[0],MAC_LEN) == 0)  //�豸δ���
			{
                return 0;                                                                     //�豸�Ѿ����ֱ���˳�
            }
        }
        
        //2.û���ظ��豸������豸
		for(i = 0; i < s_device_list->max_index_decice + 2;i++)
		{
			if(memcmp(&s_device_list->mac_list_array[i].mac_addr,&mac_addr[0],MAC_LEN) != 0)  //�豸δ���
			{
                if( Empty_Mac_List(s_device_list->mac_list_array[i]) == 1)                     //��ӵ�λ��Ϊ����
                {
                    break;
                }
			}
            else
            {
                return 0;                                                                     //�豸�Ѿ����ֱ���˳�
            }
		}        
        
        {
            {
                s_device_list->device_num += 1;											     //������1
                
                s_device_list->mac_list_array[i].index  = i;                                 
                
                memcpy(&s_device_list->mac_list_array[i].mac_addr,&mac_addr[0],6);
                                                    
                //�����������
                #if 1
                if(i >= s_device_list->max_index_decice)
                {
                  s_device_list->max_index_decice = i;  
                }
                #endif

                NRF_LOG_INFO("---------------������� %d %d  ",i,s_device_list->max_index_decice);
                
                
                s_device_list->index_sort[s_device_list->device_num - 1] = i;
                
                BubbleSort1(s_device_list->index_sort,device_list->device_num);    //����
                
                
                //Index_Sort_Push(s_device_list,i);                         //���������б�   

                //NRF_LOG_INFO("ZMY --------3 s_device_list->device_num %d  ",s_device_list->device_num);
                
            }
        }
	}
    
	return 0;
}


//ɾ���豸  
uint8_t Del_Device_List(_mac_index_data *device_list,uint8_t mac_addr[6])
{
	uint8_t ret;
	uint8_t i,j;
	
	if(device_list == NULL)
	{
		return ret = 0;
	}
    if( mac_addr[0] == 0x00 && mac_addr[1] == 0x00&&
        mac_addr[2] == 0x00 && mac_addr[3] == 0x00 &&
        mac_addr[4] == 0x00 &&mac_addr[5] == 0x00 )
    {
        return 0;
    }
    
	#if 1
    NRF_LOG_INFO("-------------MAC 0X%02x,0X%02x,0X%02x,0X%02x,0X%02x,0X%02x",mac_addr[0],mac_addr[1],mac_addr[2],
    mac_addr[3],
    mac_addr[4],
    mac_addr[5]);
    #endif
    
	_mac_index_data *s_device_list = device_list;
	
    NRF_LOG_INFO("--------------�Ͽ����� device_num  %d",s_device_list->device_num);
    NRF_LOG_INFO("--------------�Ͽ����� max_index_decice  %d",s_device_list->max_index_decice);
    
    
    #if 0
    for(uint8_t i = 0; i < 8;i++)
    {
        NRF_LOG_INFO("-----------device list 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x",s_device_list->mac_list_array[0].mac_addr[0],s_device_list->mac_list_array[0].mac_addr[1],
        s_device_list->mac_list_array[0].mac_addr[2],s_device_list->mac_list_array[0].mac_addr[3],
        s_device_list->mac_list_array[0].mac_addr[4],s_device_list->mac_list_array[0].mac_addr[5]);
    }
    #endif

	if(s_device_list->device_num == 0)   //�豸�б�Ϊ��
	{
        NRF_LOG_INFO("���󷵻�");
        
        s_device_list->max_index_decice = 0;                                      //�������+1
        
        //Index_Sort_Push(s_device_list,i);                                        //���������б�  
        s_device_list->index_sort[0] = 0;
        
		return 0;
	}
	else
	{
		for(i = 0;  i < s_device_list->max_index_decice + 1;i++)
		{
			if(memcmp(&s_device_list->mac_list_array[i].mac_addr[0],&mac_addr[0],6) == 0) //�ҵ���ͬ�豸
			{
                NRF_LOG_INFO("------------------------�Ѿ�ƥ����ͬ�豸");
				memset(&s_device_list->mac_list_array[i],0,sizeof(_mac_index_fix));       //����豸
                
                s_device_list->device_num -= 1;                                           //�豸�Ͽ����豸������һ
				break;
			}
		}
        
        #if 1
        for(j = 0; j < s_device_list->device_num + 1;j++)
        {
            if(s_device_list->index_sort[j] == i)
            {
                s_device_list->index_sort[j] = s_device_list->index_sort[s_device_list->device_num];
                s_device_list->index_sort[s_device_list->device_num] = 0;

                break;
            }
        }
        
        //ɾ��������
        BubbleSort1(s_device_list->index_sort,device_list->device_num);
        //��������ֵ С���������  ������� ��Ҫ�滻
        if( s_device_list->index_sort[device_list->device_num - 1] <=  s_device_list->max_index_decice)
        {
           s_device_list->max_index_decice = s_device_list->index_sort[device_list->device_num - 1];
        }

        #endif
        //Index_Sort_POP(s_device_list,i);                                                //���������б�
	}
	return 0;
}

void disconnect_del_info(uint16_t conn_handle, uint8_t mac_addr[6])
{
    //����handle �ҵ�device type
    _e_machine_model device_type;
    
    
    device_type = dev_info.ble_dev[conn_handle -1].model;
    
    
    if(device_type == E_INVALID)
    {
        return;
    }
    
   switch(device_type)
	{
		case E_RAR312:
            Del_Device_List(&rar312_data.mac_index,mac_addr);  
        
			break;
		case E_RM661:
            Del_Device_List(&rm661_data.mac_index,mac_addr);    //Ϊ�豸���ƥ���ϵ
        
			break;
		case E_SLEEP_MONITER:
            Del_Device_List(&sleepmonitor_data.mac_index,mac_addr);    //Ϊ�豸���ƥ���ϵ
			break;
		case E_CURTAIN:
             Del_Device_List(&curtain_data.mac_index,mac_addr);    //Ϊ�豸���ƥ���ϵ

			break;
		case E_06H:
			break;
		case E_PMI:
            Del_Device_List(&pmi_data.mac_index,mac_addr);    //Ϊ�豸���ƥ���ϵ
        
        
			break;
		case E_08A:
			break;
		case E_08F:
		
			break;
		case E_09A:
              Del_Device_List(&System_08F.mac_index,mac_addr);    //Ϊ�豸���ƥ���ϵ
        
			break;
		case E_09F:
			break;
		 
		 default:
			 break;
	}
    
    
    
    
    
    
    
    





}








//����mac ��ַ ƥ�� ��Ӧ�� handle   0xffff Ϊ����mac ��ַ
uint16_t  mac_match_hanle(_mac_index_data *device_list ,uint8_t mac[6])
{
	uint8_t i = 0;
	uint8_t conn_handle = 0;
	uint16_t ret;
	
    if( Check_Mac(mac) == 0)
	{
		return 0xffff;
	}
	
	if(device_list == NULL)
	{
		return 0xffff;
	}
	
	_mac_index_data *s_device_list = device_list;
	
	
	for(i = 0; i < s_device_list->device_num ; i++)
	{
		conn_handle = s_device_list->index_sort[i];
		
		if( memcmp( s_device_list->mac_list_array[conn_handle].mac_addr,mac,6) == 0 )  //�ҵ���ͬ�� mac ��ַ
		{
			
			 ret = s_device_list->mac_list_array[conn_handle].index;
			break;
		}
		
	}
	
	return ret;
}





//���ڵ���   ��ӡ�豸����Ϣmac ��ַ
void Debug_Device_Info(void)
{

    NRF_LOG_INFO("debug");

    NRF_LOG_INFO("System_08F STRUCT  DATA =======");

    NRF_LOG_INFO("���豸����:%d ",rar312_data.Device_Num);
    
    #if 0
    for(uint8_t i = 0; i < ;i++)
    {
        NRF_LOG_INFO("�豸ʵʱ����:%d ",rm661_data.);
    }
    #endif
    
    NRF_LOG_INFO("���豸����:%d ",rar312_data.mac_index.device_num);
    
    NRF_LOG_INFO("�������:%d ",rar312_data.mac_index.max_index_decice);
    
    for(uint8_t i = 0; i < MAX_DEVICE_NUM - 4;i++)
    {
        NRF_LOG_INFO("��������:%d  %d ",i,rar312_data.mac_index.index_sort[i]);
    }

    for(uint8_t i = 0; i < MAX_DEVICE_NUM - 4;i++)
    {
        NRF_LOG_INFO("mac ��ַ ������: 0x%02x :0x%02x :0x%02x :0x%02x :0x%02x :0x%02x   ",rar312_data.mac_index.mac_list_array[i].mac_addr[0],
        rar312_data.mac_index.mac_list_array[i].mac_addr[1],
        rar312_data.mac_index.mac_list_array[i].mac_addr[2],
        rar312_data.mac_index.mac_list_array[i].mac_addr[3],
        rar312_data.mac_index.mac_list_array[i].mac_addr[4],
        rar312_data.mac_index.mac_list_array[i].mac_addr[5]);

        NRF_LOG_INFO("��Ӧindex :%d",rar312_data.mac_index.mac_list_array[i].index);
    }    
}











