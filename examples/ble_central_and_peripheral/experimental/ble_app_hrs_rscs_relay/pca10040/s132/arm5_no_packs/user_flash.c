
#include "user_flash.h"


static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

static void fstorage_evt_handler(nrf_fstorage_evt_t * p_evt);


NRF_FSTORAGE_DEF(nrf_fstorage_t fstorage) =
{
    /* Set a handler for fstorage events. */
    .evt_handler = fstorage_evt_handler,

    /* These below are the boundaries of the flash space assigned to this instance of fstorage.
     * You must set these manually, even at runtime, before nrf_fstorage_init() is called.
     * The function nrf5_flash_end_addr_get() can be used to retrieve the last address on the
     * last page of flash available to write data. */
    .start_addr = 0x3e000,
    .end_addr   = 0x3ffff,
};




static void print_flash_info(nrf_fstorage_t * p_fstorage)
{
    printf("========| flash info |========");
    printf("erase unit: \t%d bytes",      p_fstorage->p_flash_info->erase_unit);
    printf("program unit: \t%d bytes",    p_fstorage->p_flash_info->program_unit);
    printf("==============================");
}


static uint32_t nrf5_flash_end_addr_get()
{
    uint32_t const bootloader_addr = NRF_UICR->NRFFW[0];
    uint32_t const page_sz         = NRF_FICR->CODEPAGESIZE;
    uint32_t const code_sz         = NRF_FICR->CODESIZE;

    return (bootloader_addr != 0xFFFFFFFF ?
            bootloader_addr : (code_sz * page_sz));
}

static uint32_t m_data          = 0xBADC0FFE;

static void fstorage_evt_handler(nrf_fstorage_evt_t * p_evt)
{
    if (p_evt->result != NRF_SUCCESS)
    {
        printf("--> Event received: ERROR while executing an fstorage operation.");
        return;
    }

    switch (p_evt->id)
    {
        case NRF_FSTORAGE_EVT_WRITE_RESULT:
        {
            printf("--> Event received: wrote %d bytes at address 0x%x.",
                         p_evt->len, p_evt->addr);
        } break;

        case NRF_FSTORAGE_EVT_ERASE_RESULT:
        {
            printf("--> Event received: erased %d page from address 0x%x.",
                         p_evt->len, p_evt->addr);
        } break;

        default:
            break;
    }
}
void wait_for_flash_ready(nrf_fstorage_t const * p_fstorage)
{
    /* While fstorage is busy, sleep and wait for an event. */
    while (nrf_fstorage_is_busy(p_fstorage))
    {
        power_manage();
    }
}
static char     m_hello_world[] = "hello world";


uint8_t data_buffer[20]={0x3a,0x01,0x02};


uint8_t readdata_buffer[20];

void user_fstoreage_init(void)
{
    
    uint32_t err_code;
    
    nrf_fstorage_api_t * p_fs_api;
    
    p_fs_api = &nrf_fstorage_sd;

    err_code = nrf_fstorage_init(&fstorage, p_fs_api, NULL);
    APP_ERROR_CHECK(err_code);

    
   (void) nrf5_flash_end_addr_get();
    
    print_flash_info(&fstorage);
    
      /* Let's write to flash. */
    printf("Writing \"%x\" to flash.", m_data);
    err_code = nrf_fstorage_write(&fstorage, 0x3e000, &m_data, sizeof(m_data), NULL);
    APP_ERROR_CHECK(err_code);

    wait_for_flash_ready(&fstorage);
    printf("Done.");
    
     printf("Writing \"%s\" to flash.", m_hello_world);
    err_code = nrf_fstorage_write(&fstorage, 0x3e004, data_buffer, sizeof(data_buffer), NULL);
    APP_ERROR_CHECK(err_code);

    wait_for_flash_ready(&fstorage);
    printf("Done.");
    
    printf("read \"\" to flash.");
    err_code = nrf_fstorage_read(&fstorage, 0x3e004, readdata_buffer, sizeof(m_hello_world));
    APP_ERROR_CHECK(err_code);
    
    
    wait_for_flash_ready(&fstorage);
    printf("Done.");
    
    for(uint8_t i = 0; i<20;i++)
    {
        printf("data[%d]:0x%02x",i,readdata_buffer[i]);
    }

    __nop();
#if 0
    printf("Use 'read' to read bytes from the flash.");
    printf("Use 'write' to write bytes to the flash.");
    printf("Use 'erase' to erase flash pages.");
    printf("Use 'flasharea' to print and configure the flash read boundaries.");
#endif
    
}


    