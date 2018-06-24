#include "User_MacList.h"
#include "sdk_config.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "Somputon_BLE_DataHandle.h"





_t_dev_mac_match dev_info;    //�����豸��������Ϣ


uint8_t mac_addr_list[6];     //mac ��ַ



// ����豸��ַ���Ӹ���
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


//���ýṹ������
void Device_Info_Reset(_t_ble_status *s_ble_info)
{
    if(s_ble_info == NULL)return;
    
    memset(s_ble_info,0,sizeof(_t_ble_status));
   
}

//���������ӵĸ���

uint8_t Device_Info_Connected_num(void)
{
    return dev_info.device_num;

}


void USER_DEBUG_printf(void)
{
    NRF_LOG_INFO("dev_info.device_num:%02x",dev_info.device_num);
}


#if 0
//�豸��Ϣд��
void Device_Info_Push(void)
{
    dev_info.ble_dev[0].

}
#endif


//�����Ѿ������豸��



//�豸�Ͽ�����
void Device_Disconnected_handle(uint16_t  connected_handle)
{
    dev_info.ble_dev[connected_handle - 1].conn_handle = BLE_CONN_HANDLE_INVALID;
    if(dev_info.device_num >0 )dev_info.device_num -=1; 
    
    memset(dev_info.ble_dev[connected_handle - 1].mac_addr,0,6);
}




//ƥ��Ͽ����豸��Ϣ,���ڵ���
void  Debug_Device_match_info(uint16_t  connected_handle,uint8_t *addr_mac)
{
    
    uint16_t degug_connected_handle = 0;                   //�Ͽ����ӵ�handle
    
    degug_connected_handle = dev_info.ble_dev[connected_handle - 1].conn_handle;
    
    NRF_LOG_INFO("device connedted handle %d is disconnected ",degug_connected_handle);

    
    //��ʾ�Ͽ����ӵ�mac ��ַ 
    
    NRF_LOG_INFO("device disconnedted mac addr %02x %02x %02x %02x %02x %02x  ",dev_info.ble_dev[connected_handle -1].mac_addr[0],
    dev_info.ble_dev[connected_handle -1].mac_addr[1],
    dev_info.ble_dev[connected_handle -1].mac_addr[2],
    dev_info.ble_dev[connected_handle -1].mac_addr[3],
    dev_info.ble_dev[connected_handle -1].mac_addr[4],
    dev_info.ble_dev[connected_handle -1].mac_addr[5]);
    
    
    NRF_LOG_INFO("device_type %d ",dev_info.ble_dev[connected_handle -1].model);
    
    memcpy(addr_mac,&dev_info.ble_dev[connected_handle - 1].mac_addr[0],6);
    
}


//���黻��

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


//�����ϵ��豸��ַ
void Debug_Device_match_connected_mac(ble_gap_addr_t mac_peer_addr , uint16_t handle)
{
    if(handle > 10)
    {
        return;
    }

    memcpy(dev_info.ble_dev[handle-1].mac_addr,mac_peer_addr.addr,6);       //����mac ��ַ
    change_array_index(dev_info.ble_dev[handle-1].mac_addr,6);              //mac ��ַ���򱣳ֺ�app ����ʾһ��
      
    dev_info.ble_dev[handle-1].conn_handle = handle;                        //����handle        
    
    NRF_LOG_INFO("dev.handle :%02x ",handle);
    NRF_LOG_INFO("ADD MAC addr :%02x %02x %02x %02x %02x %02x",dev_info.ble_dev[handle-1].mac_addr[0],
    dev_info.ble_dev[handle-1].mac_addr[1],
    dev_info.ble_dev[handle-1].mac_addr[2],
    dev_info.ble_dev[handle-1].mac_addr[3],
    dev_info.ble_dev[handle-1].mac_addr[4],
    dev_info.ble_dev[handle-1].mac_addr[5]);
}

//�󶨼�ʱ��  1s ����һ��

void Device_bind_count(void)
{
    if(dev_info.scan_bind_flag == 1)
    {
        if(dev_info.bing_timeout_cnt > 0)dev_info.bing_timeout_cnt--;
        else{dev_info.scan_bind_flag = 0;}
    }
}

//���ذ�״̬
_e_band_status Ret_Device_Bind_status(void)
{
   return dev_info.scan_bind_flag;
}

//���ذ�ʣ��ʱ��
uint8_t Ret_Device_Bind_Time(void)
{
    return dev_info.bing_timeout_cnt;
}

//��ȡ����ʱ���rssi 

void Device_Get_Rssi(uint16_t conn_handle,int8_t rssi)
{
    dev_info.ble_dev[conn_handle-1].rssi = rssi;
}


//����rssiֵ
int8_t Ret_Device_Rssi(uint16_t conn_handle)
{
    return dev_info.ble_dev[conn_handle-1].rssi;
}


//�����豸���Ͳ����豸��Ϣ  ,�豸����  �豸���  �豸 conn_handle

typedef struct{
    uint16_t conn_handle;
    _e_machine_model device_type;
}_s_avaiable_info;


typedef struct{
    uint8_t  total_avaiable_num;      //�������豸�ܸ���
    _s_avaiable_info dev_info[8];
}_s_avaiable_device;

_s_avaiable_device   sys_avaiable_table;   //�����豸�б�ȫ��




//�����豸���� ��������ͬ�豸���͵��б�
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


//���¿����豸�б�
void Device_Update_Avaiable_Table(void)
{
    uint8_t i = 0,num= 0;
    
    for(i = 0; i < 8;i++)
    {
        if(dev_info.ble_dev[i].conn_handle != 0 && dev_info.ble_dev[i].conn_handle != 0xFFFF)
        {
            #if 0
            NRF_LOG_INFO("�����豸�б�  conn_handle %d     ",dev_info.ble_dev[i].conn_handle); 
            NRF_LOG_INFO("------NUM %d ",num);
            #endif
            num++;
            sys_avaiable_table.dev_info[i].conn_handle = dev_info.ble_dev[i].conn_handle;   //����handle
            sys_avaiable_table.dev_info[i].device_type = dev_info.ble_dev[i].model;         //��������
            
           // NRF_LOG_INFO("------device type %d ",dev_info.ble_dev[i].model);
            
        }
    }
    
    sys_avaiable_table.total_avaiable_num = num;
    
    
    NRF_LOG_INFO("�����豸�б����� %d ",sys_avaiable_table.total_avaiable_num);
    
    
    for(i = 0; i < 8;i++)
    {
        NRF_LOG_INFO("�����豸�б� %d  conn_handle %d   device_type %d  ",i,sys_avaiable_table.dev_info[i].conn_handle,
        sys_avaiable_table.dev_info[i].device_type);  
    }
}



//��� �豸�Ƿ��  �������豸���Ӻ� ��Ҫ�����ж�
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

//�����豸�İ󶨱�־���յ���ָ������

void  device_bond_status_update(uint8_t conn_handle)
{
    dev_info.ble_dev[conn_handle -1].bond_stauts = 1;
}

//����󶨱�־λ  ���յ������豸�������Ϣ�����
void  device_bond_status_clear(uint8_t conn_handle)
{
    dev_info.ble_dev[conn_handle -1].bond_stauts = 0;
}



//����豸����,�豸�󶨵�ʱ�����
void device_add_type(uint8_t conn_handle ,_e_machine_model device_type)
{
    dev_info.ble_dev[conn_handle - 1].model = device_type;
}

//ɾ���豸���� ���豸�Ͽ����ӻ���ɾ������Ϣ��ʱ�����
void device_del_type(uint8_t conn_handle )
{
    dev_info.ble_dev[conn_handle - 1].model = 0; 
}

//�����豸����
void device_total_add(void)
{
    if(dev_info.device_num < 8) dev_info.device_num++;
}


//�����豸����
void device_total_del(void)
{
    if(dev_info.device_num > 0) dev_info.device_num--;
}

//��ӡ�������ӵ�����
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




void data_send_proc(void)
{
    static uint8_t step;
    static uint8_t i = 0;
    uint8_t conn_handle = 0;
    
    NRF_LOG_INFO("�����豸���� %d \r\n",dev_info.device_num);
    
    //step = E_REAL_TIME_DATA;
    
    switch(step)
    {
        case E_SEND_STATUS:                         //���Ͱ���ָ֤��
               
               if(dev_info.device_num != 0)  
               {
                   if( i < dev_info.device_num)
                    {
                        conn_handle = sys_avaiable_table.dev_info[i].conn_handle;  //��ȡconn_handle
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
             break;
        case E_REAL_TIME_DATA:
            
            NRF_LOG_INFO("------------------real time data send ------------");
            
            if(dev_info.device_num != 0)  
            {
                if( i < dev_info.device_num)
                {
                    conn_handle = sys_avaiable_table.dev_info[i].conn_handle;  //��ȡconn_handle
                    real_data_send(conn_handle - 1);
                    i++;
                } 
                else
                {
                    step = E_SEND_STATUS;
                    i = 0;
                }
            }
             break;
        case E_CONTROL_DATA:
             
             break;
        default:
             break;

    }
}




    
