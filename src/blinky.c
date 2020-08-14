#include <abstractSTM32.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <stdint.h>

struct pin led = {
    .port = AB_GPIOD,
    .num = 12,
    .dir = GPIO_MODE_OUTPUT,
    .mode = GPIO_OTYPE_PP,
    .otype = GPIO_OTYPE_PP,
    .speed = GPIO_OSPEED_2MHZ,
    .pull_up_down = GPIO_PUPD_NONE,
    .is_inverse = false
};

int main(void)
{
    abst_init();
    abst_gpio_init(led);
    abst_digital_write(led, 1);
    while(1) {
        delay_ms(1e3);
        abst_toggle(led);
    }
}
