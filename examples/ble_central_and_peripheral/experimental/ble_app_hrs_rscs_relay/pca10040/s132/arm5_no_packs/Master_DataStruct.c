#include "Master_DataStruct.h"


#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


_datastruct_08F   System_08F={0     
};        //08F  系统变量

_datastruct_rm661 rm661_data;    //661 分配设备






//清零数据   device_index 0 - 7

void Master_ResetBuffer_08F_RealStatus(uint8_t device_index)
{
	//memset(&Device_08F_Array[device_index],0,sizeof(_real_staus_08F));
}


//清零所有数据   all
void Master_ResetBuffer_08F_RealStatus_All(void)
{
	//memset(&Device_08F_Array,0,sizeof(_real_staus_08F)*8);
}



//检查mac 地址合法性

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

//ret: 0  非法输入的地址


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




//检查数据是否可用
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

	device_list->index_sort[device_list->device_num] =  index;   //添加
	
    NRF_LOG_INFO("sort --------------- device_num :%d ",device_list->device_num);
    
	//添加后排序
	BubbleSort1(device_list->index_sort,device_list->device_num);    //排序
	
	device_list->max_index_decice = device_list->index_sort[device_list->device_num];
	

    #ifdef USER_UART_PRINT
    for(uint8_t i = 0 ; i < device_list->max_index_decice + 1 ; i++)
    {
        NRF_LOG_INFO("索引序号 %d :[%d] ",i,device_list->index_sort[i]);
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

    //删除后排序
	
	BubbleSort1(device_list->index_sort,device_list->device_num);
    
}






//往列表里面增加设备
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
    
	if(s_device_list->device_num == 0)   //设备列表为空
	{
		s_device_list->mac_list_array[0].index  = 0;
		memcpy(&s_device_list->mac_list_array[0].mac_addr,&mac_addr[0],MAC_LEN);   //拷贝mac地址和索引
        
        s_device_list->device_num += 1;											   //添加的设备总数+1
                                                                                   
        s_device_list->max_index_decice = 0;                                      //最大索引+1
        
        //Index_Sort_Push(s_device_list,i);                                        //更新所以列表  
        s_device_list->index_sort[0] = 0;
        
    }
	else
	{
        //NRF_LOG_INFO("DEVICE_LIST_num --- 2 %d ",s_device_list->device_num);
        if(s_device_list->device_num >= MAX_DEVICE_NUM )return 0;
		//设备列表不为空,检查mac 地址是否相同，相同返回错误，不同继续添加
        
        //1.索引所有设备 是否有重复设备
        for(i = 0; i < s_device_list->max_index_decice + 1;i++)
        {
            if(memcmp(&s_device_list->mac_list_array[i].mac_addr,&mac_addr[0],MAC_LEN) == 0)  //设备未添加
			{
                return 0;                                                                     //设备已经添加直接退出
            }
        }
        
        //2.没有重复设备则添加设备
		for(i = 0; i < s_device_list->max_index_decice + 2;i++)
		{
			if(memcmp(&s_device_list->mac_list_array[i].mac_addr,&mac_addr[0],MAC_LEN) != 0)  //设备未添加
			{
                if( Empty_Mac_List(s_device_list->mac_list_array[i]) == 1)                     //添加的位置为可用
                {
                    break;
                }
			}
            else
            {
                return 0;                                                                     //设备已经添加直接退出
            }
		}        
        
        {
            {
                s_device_list->device_num += 1;											     //索引加1
                
                s_device_list->mac_list_array[i].index  = i;                                 
                
                memcpy(&s_device_list->mac_list_array[i].mac_addr,&mac_addr[0],6);
                                                    
                //最大索引更新
                #if 1
                if(i >= s_device_list->max_index_decice)
                {
                  s_device_list->max_index_decice = i;  
                }
                #endif

                NRF_LOG_INFO("---------------最大索引 %d %d  ",i,s_device_list->max_index_decice);
                
                
                s_device_list->index_sort[s_device_list->device_num - 1] = i;
                
                BubbleSort1(s_device_list->index_sort,device_list->device_num);    //排序
                
                
                //Index_Sort_Push(s_device_list,i);                         //更新所以列表   

                //NRF_LOG_INFO("ZMY --------3 s_device_list->device_num %d  ",s_device_list->device_num);
                
            }
        }
	}
    
	return 0;
}


//删除设备  
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
	
    NRF_LOG_INFO("--------------断开连接 device_num  %d",s_device_list->device_num);
    NRF_LOG_INFO("--------------断开连接 max_index_decice  %d",s_device_list->max_index_decice);
    
    
    #if 0
    for(uint8_t i = 0; i < 8;i++)
    {
        NRF_LOG_INFO("-----------device list 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x",s_device_list->mac_list_array[0].mac_addr[0],s_device_list->mac_list_array[0].mac_addr[1],
        s_device_list->mac_list_array[0].mac_addr[2],s_device_list->mac_list_array[0].mac_addr[3],
        s_device_list->mac_list_array[0].mac_addr[4],s_device_list->mac_list_array[0].mac_addr[5]);
    }
    #endif

	if(s_device_list->device_num == 0)   //设备列表为空
	{
        NRF_LOG_INFO("错误返回");
		return 0;
	}
	else
	{
		for(i = 0;  i < s_device_list->max_index_decice + 2;i++)
		{
			if(memcmp(&s_device_list->mac_list_array[i].mac_addr[0],&mac_addr[0],6) == 0) //找到相同设备
			{
                NRF_LOG_INFO("------------------------已经匹配相同设备");
				memset(&s_device_list->mac_list_array[i],0,sizeof(_mac_index_fix));       //清除设备
                
                s_device_list->device_num -= 1;                                           //设备断开，设备总数减一
                
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
        
        //删除后排序
        BubbleSort1(s_device_list->index_sort,device_list->device_num);
        //排序后最大值 小于最大索引  最大索引 需要替换
        if( s_device_list->index_sort[device_list->device_num - 1] <=  s_device_list->max_index_decice)
        {
           s_device_list->max_index_decice = s_device_list->index_sort[device_list->device_num - 1];
        }

        #endif
        //Index_Sort_POP(s_device_list,i);                                                //更新索引列表
	}
	return 0;
}




//根据mac 地址 匹配 相应的 handle
uint16_t  mac_match_hanle(uint8_t mac[6])
{
    



}

//用于调试   打印设备的信息mac 地址
void Debug_Device_Info(void)
{

    NRF_LOG_INFO("debug");

    NRF_LOG_INFO("System_08F STRUCT  DATA =======");

    NRF_LOG_INFO("总设备个数:%d ",System_08F.Device_Num);
    
    #if 0
    for(uint8_t i = 0; i < ;i++)
    {
        NRF_LOG_INFO("设备实时数据:%d ",System_08F.Device_Num);
    }
    #endif
    
    NRF_LOG_INFO("总设备个数:%d ",System_08F.mac_index.device_num);
    
    NRF_LOG_INFO("最大索引:%d ",System_08F.mac_index.max_index_decice);
    
    for(uint8_t i = 0; i < MAX_DEVICE_NUM;i++)
    {
        NRF_LOG_INFO("索引排序:%d  %d ",i,System_08F.mac_index.index_sort[i]);
    }

    for(uint8_t i = 0; i < MAX_DEVICE_NUM;i++)
    {
        NRF_LOG_INFO("mac 地址 和索引: 0x%02x :0x%02x :0x%02x :0x%02x :0x%02x :0x%02x   ",System_08F.mac_index.mac_list_array[i].mac_addr[0],
        System_08F.mac_index.mac_list_array[i].mac_addr[1],
        System_08F.mac_index.mac_list_array[i].mac_addr[2],
        System_08F.mac_index.mac_list_array[i].mac_addr[3],
        System_08F.mac_index.mac_list_array[i].mac_addr[4],
        System_08F.mac_index.mac_list_array[i].mac_addr[5]);

        NRF_LOG_INFO("对应index :%d",System_08F.mac_index.mac_list_array[i].index);
    }    
}











