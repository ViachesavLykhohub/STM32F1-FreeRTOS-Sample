#include <abstractSTM32.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <stdint.h>

struct pin led = {
    .port = GPIOD,
    .num = 12,
    .dir = GPIO_MODE_OUTPUT,
    .mode = GPIO_OTYPE_PP,
    .speed = GPIO_OSPEED_2MHZ,
    .pull_up_down = GPIO_PUPD_NONE,
    .is_inverse = false
};

int main(void)
{
    abst_gpio_init(led);
    while (1) {
        abst_digital_write(led, 1);
        volatile int i = 1e6;
        while (i--);
        abst_digital_write(led, 0);
        i = 1e6;
        while (i--);
    }
}
