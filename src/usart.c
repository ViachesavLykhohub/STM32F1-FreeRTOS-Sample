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
#ifdef STM32F1
    .port = ABST_GPIOA,
    .num = 3,
#endif 
#ifdef STM32F4
    .port = ABST_GPIOD,
    .num = 12,
#endif
    .mode = ABST_MODE_OUTPUT,
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_2MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};

struct abst_pin TX = {
#ifdef STM32F1
    .port = ABST_GPIOB,
    .num = 10,
#endif
#ifdef STM32F4
    .port = ABST_GPIOD,
    .num = 8,
#endif
    .mode = ABST_MODE_AF,
    .af = 7, // For STM32F4
    .af_dir = ABST_AF_OUTPUT,  // For STM32F1
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_2MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};

struct abst_pin RX = {
#ifdef STM32F1
    .port = ABST_GPIOB,
    .num = 11,
#endif
#ifdef STM32F4
    .port = ABST_GPIOD,
    .num = 9,
#endif
    .mode = ABST_MODE_AF,
    .af = 7, // For STM32F4
    .af_dir = ABST_AF_INPUT, // For STM32F1
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
    uint32_t AHB;
#ifdef STM32F1
    AHB = 8e6;
#endif
#ifdef STM32F4
    AHB = 16e6;
#endif
    abst_init(AHB, 100);
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
//         if (err == FIFO_OK)
//             abst_pwm_soft(&led, rx_val);
        
        abst_usart_send_msg(&USART, &rx_val, 1);
        
        abst_toggle(&led);
        abst_delay_ms(1e3);
    }
}
