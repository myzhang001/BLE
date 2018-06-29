#include "User_adv_func.h"

#include "ble.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


extern uint32_t adv_report_parse(uint8_t type, uint8_array_t * p_advdata, uint8_array_t * p_typedata);
extern ble_gap_conn_params_t const m_connection_param;




#define TARGET_NAME_SET "SOMPUTON_08A"












/**@brief Parameters used when scanning. */
static ble_gap_scan_params_t const m_scan_params =
{
    .active   = 1,
    .interval = 0xa0,
    .window   = 0x50,
    .timeout  = 0,
    #if (NRF_SD_BLE_API_VERSION <= 2)
        .selective   = 0,
        .p_whitelist = NULL,
    #endif
    #if (NRF_SD_BLE_API_VERSION >= 3)
        .use_whitelist = 0,
    #endif
};




//ƥ��mac ��ַ
bool User_Match_Adv_Addr(ble_gap_addr_t addr,ble_gap_addr_t *dev_mac)
{
    uint8_t i = 0;
    
    if(dev_mac == NULL)return false;
    
    for( i = 0; i < BLE_GAP_ADDR_LEN;i++)
    {
        if(addr.addr[i] != dev_mac->addr[i])
        {
            break;
        }
    }
    
    if(i < BLE_GAP_ADDR_LEN)
    {
       return false; 
    }
    else
    {
        return true;
    }
}



    
 //ƥ��󶨱�־
bool ScanRsp_Match_Binding(ble_gap_evt_t  const *p_gap_evt)
{
    bool ret_value;
    
    if(p_gap_evt->params.adv_report.scan_rsp == USER_ADV_SCAN_RSP)
    {
        //�����㲥���е�ɨ��Ӧ��
        //���յ��Ĺ㲥������
        NRF_LOG_INFO("\r\n�㲥����:%02d", p_gap_evt->params.adv_report.type);
        //Ӧ���־λ  Ϊ1 �� ɨ��Ӧ�� 0 Ϊ�㲥����
        NRF_LOG_INFO("\r\n Ӧ���־λ%d  \r\n",p_gap_evt->params.adv_report.scan_rsp); 

        NRF_LOG_INFO("\r\n  mac ��ַƥ��ɹ�");
        //  ɨ�赽����Χ��mac ��ַ
        #if 1
        NRF_LOG_INFO("�豸�㲥��ַmac:%02x %02x %02x %02x %02x %02x ",p_gap_evt->params.adv_report.peer_addr.addr[0],\
        p_gap_evt->params.adv_report.peer_addr.addr[1],\
        p_gap_evt->params.adv_report.peer_addr.addr[2],\
        p_gap_evt->params.adv_report.peer_addr.addr[3],\
        p_gap_evt->params.adv_report.peer_addr.addr[4],\
        p_gap_evt->params.adv_report.peer_addr.addr[5]\
        );
        #endif
        //��ӡ���еĹ㲥���ݰ�

        NRF_LOG_INFO("\r\n-----------------------start-------------------");

        for(uint8_t adv_len = 0;adv_len < p_gap_evt->params.adv_report.dlen; adv_len++)
        {
            NRF_LOG_INFO("%02x",p_gap_evt->params.adv_report.data[adv_len]);
        }
        NRF_LOG_INFO("\r\n-----------------------end-------------------");
               
        ret_value = true;
    } 
    else
    {
        //return false;
        ret_value = false;
    }
    
    return ret_value;
}
   


void  find_target_device_mac(ble_evt_t const * p_ble_evt,ble_gap_addr_t  * device_addr)
{
    uint32_t      err_code;
    uint8_array_t adv_data;
    uint8_array_t dev_name;
    bool          do_connect = false;
    uint8_t       device_name[30];
    
    
    //For readibility.
    ble_gap_evt_t  const * p_gap_evt  = &p_ble_evt->evt.gap_evt;                    
    ble_gap_addr_t  const* peer_addr  = &p_gap_evt->params.adv_report.peer_addr;   //����Ե��豸�ĵ�ַ
    
    
    //Prepare advertisement report for parsing.
    adv_data.p_data = (uint8_t *)p_gap_evt->params.adv_report.data;
    adv_data.size   = p_gap_evt->params.adv_report.dlen;

    //Search for advertising names.
    bool found_name = false;
    err_code = adv_report_parse(BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME,
                                &adv_data,
                                &dev_name);

    
    if(err_code != NRF_SUCCESS)
    {
        // Look for the short local name if it was not found as complete.
        err_code = adv_report_parse(BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME, &adv_data, &dev_name);
        if (err_code != NRF_SUCCESS)
        {
            // If we can't parse the data, then exit.
            return;
        }
        else
        {
            found_name = true;
        }
    }
    else
    {
        found_name = true;
    }
    
    if(found_name)
    {
        //�ȶ�����ƥ�����
        memset(device_name,0,sizeof(device_name));
        memcpy(device_name,dev_name.p_data,dev_name.size);
     
        //uint8_t target_name_buff[20]={"SOMPUTON_08A"};

        if(strstr(device_name,TARGET_NAME_SET) != NULL)   //�ҵ�Ŀ���豸����
        {
            NRF_LOG_INFO("Name a match");   
            memcpy(device_addr,peer_addr,sizeof(ble_gap_addr_t));
        }      
    }
}


//����ƥ��ɨ��Ӧ��������ʶ�����
void match_scanrsp_func(ble_gap_evt_adv_report_t *adv_report,ble_gap_addr_t addr_mac)
{
    uint32_t err_code = 0;
    if(adv_report == NULL)return;
    
    
    if( User_Match_Adv_Addr(adv_report->peer_addr,&addr_mac) == true)   //mac ��ַ����
     {       
        if(adv_report->scan_rsp == 1)
        {
            //�ź�ǿ��
            NRF_LOG_INFO("\r\n�ź�ǿ��rssi:%02d",adv_report->rssi);
          
            //���յ��Ĺ㲥������
            NRF_LOG_INFO("\r\n�㲥����:%02d", adv_report->type);
            //Ӧ���־λ  Ϊ1 �� ɨ��Ӧ�� 0 Ϊ�㲥����
            NRF_LOG_INFO("\r\n Ӧ���־λ%d  \r\n",adv_report->scan_rsp); 
            
            NRF_LOG_INFO("\r\n  mac ��ַƥ��ɹ�");
            //  ɨ�赽����Χ��mac ��ַ
            
            NRF_LOG_INFO("�豸�㲥��ַmac:%02x %02x %02x %02x %02x %02x ",adv_report->peer_addr.addr[0],\
            adv_report->peer_addr.addr[1],\
            adv_report->peer_addr.addr[2],\
            adv_report->peer_addr.addr[3],\
            adv_report->peer_addr.addr[4],\
            adv_report->peer_addr.addr[5]\
            );
            
            //��ӡ���еĹ㲥���ݰ�
            
            NRF_LOG_INFO("\r\n-----------------------start-------------------");
            
            for(uint8_t adv_len = 0;adv_len < adv_report->dlen; adv_len++)
            {
                NRF_LOG_INFO("%02x",adv_report->data[adv_len]);
            }
            NRF_LOG_INFO("\r\n-----------------------end-------------------");
        
            if(adv_report->data[4] == 0x01)    //ɨ�赽Ӧ���־λ��״̬
            {
                // Initiate connection.
                err_code = sd_ble_gap_connect(&adv_report->peer_addr, &m_scan_params, &m_connection_param, 1);
                if (err_code != NRF_SUCCESS)
                {
                    NRF_LOG_ERROR("Connection Request Failed, reason %d", err_code);
                }
            } 
        }
    }
}







