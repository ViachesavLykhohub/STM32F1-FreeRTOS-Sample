#include <abstractSTM32.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <stdint.h>

struct pin led = {
    .port = AB_GPIOD,
    .num = 12,
    .dir = GPIO_MODE_OUTPUT,
    .mode = 0,
    .otype = GPIO_OTYPE_PP,
    .speed = GPIO_OSPEED_2MHZ,
    .pull_up_down = GPIO_PUPD_NONE,
    .is_inverse = false
};


struct pin led_pwm = {
    .port = AB_GPIOD,
    .num = 13,
    .dir = GPIO_MODE_OUTPUT,
    .mode = 0,
    .otype = GPIO_OTYPE_PP,
    .speed = GPIO_OSPEED_2MHZ,
    .pull_up_down = GPIO_PUPD_NONE,
    .is_inverse = false
};

struct pin button = {
    .port = AB_GPIOA,
    .num = 0,
    .dir = GPIO_MODE_INPUT,
    .mode = 0,
    .otype = GPIO_OTYPE_PP,
    .speed = GPIO_OSPEED_2MHZ,
    .pull_up_down = GPIO_PUPD_NONE,
    .is_inverse = false
};

struct pin led_but = {
    .port = AB_GPIOD,
    .num = 14,
    .dir = GPIO_MODE_OUTPUT,
    .mode = 0,
    .otype = GPIO_OTYPE_PP,
    .speed = GPIO_OSPEED_2MHZ,
    .pull_up_down = GPIO_PUPD_NONE,
    .is_inverse = true
};

int main(void)
{
    abst_init(16e6);
    abst_gpio_init(&led);
    abst_gpio_init(&led_pwm);
    abst_gpio_init(&led_but);
    abst_gpio_init(&button);

    abst_digital_write(&led, 1);
    int brig = 0;
    int cnt = 0;    
    while(1) {
        abst_delay_ms(1e3);
        
        // Soft PWM
        brig += 50;
        if (brig > 255) 
            brig = 0;
        if (cnt++ > 10)
            abst_stop_pwm_soft(&led_pwm);
        else
            abst_pwm_soft(&led_pwm, brig);
        
        // Blinking
        abst_toggle(&led);

        // A button that contors a led
        abst_digital_write(&led_but, abst_digital_read(&button));
    }
}
