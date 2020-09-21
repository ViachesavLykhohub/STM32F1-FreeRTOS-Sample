/**
 * Example of using USART
 *
 * Call `make TARGET=usart PROFILE=release DEVICE=stm32f103c8t6 V=1 tidy all`
 * 
 * or 
 *
 * make TARGET=usart PROFILE=release DEVICE=stm32f407vgt6 V=1 tidy all
 */

#include <abstractSTM32.h>
#include <abstractUSART.h>

#include <libopencm3/stm32/usart.h>

#define len(arr) sizeof(arr)/sizeof(arr[0])

struct abst_usart USART = 
{
    .num = 3,
    .word_len = ABST_USART_WL_8_BITS,
    .num_stop_bits = ABST_USART_STOPBITS_1,
    .baud_rate = 9600,
};

struct abst_pin led = {
    .port = ABST_GPIOA,
    .num = 3,
    .mode = ABST_MODE_OUTPUT,
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_2MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
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
    abst_init(8e6, 100);
    abst_gpio_init(&led);
    abst_digital_write(&led, 1);

    abst_gpio_init(&TX);
    abst_gpio_init(&RX);
    abst_usart_init(&USART, 100);

    uint16_t data[] = {'-', '>', '<', '-', '\n'};

    abst_digital_write(&led, 0);
    while (1) {
        abst_usart_send_text(&USART, "Hello\n");
        abst_usart_send_msg(&USART, data, len(data));
        
        enum fifo_errors err = FIFO_OK;
        uint16_t rx_val = abst_usart_read_element(&USART, &err);
        if (err == FIFO_OK)
            abst_pwm_soft(&led, rx_val);
        
        abst_usart_send_msg(&USART, &rx_val, 1);
        
        abst_delay_ms(1e3);
    }
}
