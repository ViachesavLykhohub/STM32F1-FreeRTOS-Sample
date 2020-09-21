/**
 * Example of using LOG thought USART. Requite connected USB-USART to the USART3
 *
 * Call `make TARGET=log PROFILE=release DEVICE=stm32f103c8t6 V=1 LOG=1 tidy all`
 * 
 * or 
 *
 * make TARGET=log PROFILE=release DEVICE=stm32f407vgt6 V=1 tidy LOG=1 all
 */

#include <abstractSTM32.h>
#include <abstractLOG.h>

struct abst_pin led = {
    .port = ABST_GPIOA,
    .num = 8,
    .mode = ABST_MODE_OUTPUT,
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_2MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};

int main(void)
{
    abst_init(16e6, 100);
    abst_gpio_init(&led);
    abst_log_init();

    volatile uint32_t time = 0;

    while(1) {
        abst_digital_write(&led, 1);
        abst_log("Led is turned on\n");
        abst_delay_ms(1e3);
        
        time = abst_time_ms();
        abst_logf("Current time %u ms\n", time);

        abst_digital_write(&led, 0);
        abst_log("Led is turned off\n");        
        abst_delay_ms(1e3);
    }
}
