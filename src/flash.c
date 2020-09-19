 /*
  * make TARGET=flash PROFILE=release DEVICE=stm32f103c8t6 V=1 tidy all
  */

#include <abstractSTM32.h>
#include <abstractUSART.h>
#include <abstractFLASH.h>
#include <abstractLOG.h>

struct abst_usart USART = 
{
    .num = 3,
    .word_len = ABST_USART_WL_8_BITS,
    .num_stop_bits = ABST_USART_STOPBITS_1,
    .baud_rate = 9600,
};

struct abst_pin TX = {
    .port = ABST_GPIOB,
    .num = 10,
    .mode = ABST_MODE_AF,
    .af_dir = ABST_AF_OUTPUT,
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_2MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};

struct abst_pin RX = {
    .port = ABST_GPIOB,
    .num = 11,
    .mode = ABST_MODE_AF,
    .af_dir = ABST_AF_INPUT,
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_2MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};

void usart3_isr(void)
{
    abst_usart_interraption_handler(&USART);
}

int main(void)
{
    abst_init(8e6, NULL);

    abst_gpio_init(&TX);
    abst_gpio_init(&RX);
    abst_usart_init(&USART, 1000);
    
    uint16_t N = 1024 / 4;
    uint32_t data[N];
    
    uint32_t addr = 0x08000000 + 60 * 1024;
    
//     uint32_t status = abst_flash_erase_page(addr);
    
    abst_flash_read(addr, N, data);
    
    while(1) {
        abst_delay_ms(10000);
        abst_usart_send_msg(&USART, data, N);
    }
}
