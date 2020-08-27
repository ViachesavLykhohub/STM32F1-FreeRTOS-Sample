#include <abstractSTM32.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <stdint.h>

struct abst_pin led = {
    .port = AB_GPIOD,
    .num = 12,
    .dir = GPIO_MODE_OUTPUT,
    .mode = 0,
    .otype = GPIO_OTYPE_PP,
    .speed = GPIO_OSPEED_2MHZ,
    .pull_up_down = GPIO_PUPD_NONE,
    .is_inverse = false
};

struct abst_pin led_pwm = {
    .port = AB_GPIOD,
    .num = 13,
    .dir = GPIO_MODE_OUTPUT,
    .mode = 0,
    .otype = GPIO_OTYPE_PP,
    .speed = GPIO_OSPEED_2MHZ,
    .pull_up_down = GPIO_PUPD_NONE,
    .is_inverse = false
};

struct abst_pin button = {
    .port = AB_GPIOA,
    .num = 0,
    .dir = GPIO_MODE_INPUT,
    .mode = 0,
    .otype = GPIO_OTYPE_PP,
    .speed = GPIO_OSPEED_2MHZ,
    .pull_up_down = GPIO_PUPD_NONE,
    .is_inverse = false
};

struct abst_pin led_but = {
    .port = AB_GPIOD,
    .num = 14,
    .dir = GPIO_MODE_OUTPUT,
    .mode = 0,
    .otype = GPIO_OTYPE_PP,
    .speed = GPIO_OSPEED_2MHZ,
    .pull_up_down = GPIO_PUPD_PULLUP,
    .is_inverse = true
};

struct abst_pin_group buttons = {
    .port = AB_GPIOC,
    .num = 1 << 6 | 1 << 8 | 1 << 9 | 1 << 11,
    .dir = GPIO_MODE_INPUT,
    .mode = 0,
    .otype = GPIO_OTYPE_PP,
    .speed = GPIO_OSPEED_2MHZ,
    .pull_up_down = GPIO_PUPD_NONE,
    .is_inverse = 0b1111
};

struct abst_pin_group leds = {
    .port = AB_GPIOD,
    .num = 1 << 12 | 1 << 13 | 1 << 14 | 1 << 15,
    .dir = GPIO_MODE_OUTPUT,
    .mode = 0,
    .otype = GPIO_OTYPE_PP,
    .speed = GPIO_OSPEED_2MHZ,
    .pull_up_down = GPIO_PUPD_NONE,
    .is_inverse = false
};

int main(void)
{
    abst_init(16e6);
    abst_gpio_init(&led);
    abst_gpio_init(&led_pwm);
    abst_gpio_init(&led_but);
    abst_gpio_init(&button);
    
    while (1) {
        abst_digital_write(&led, 1);
        int brig = 0;
        int cnt = 0;
        while(cnt++ < 15) {
            abst_delay_ms(1e3);
            
            // Soft PWM
            brig += 50;
            if (brig > 255) 
                brig = 0;
            abst_pwm_soft(&led_pwm, brig);
            
            // Blinking
            abst_toggle(&led);

            // A button on a discovery board that contors led
            abst_digital_write(&led_but, abst_digital_read(&button));
        }
        abst_stop_pwm_soft(&led_pwm);

        // Pin groups

        // Running light
        abst_group_gpio_init(&buttons);
        abst_group_gpio_init(&leds);
        cnt = 0;
        uint16_t val = 1;
        while (cnt++ < 50) {
            val = val << 1;
            if (val > 1 << 4)
                val = 1;
            abst_group_digital_write(&leds, val);
            abst_delay_ms(1e2);
        }

        cnt = 0;
        // This example require buttons on PC6, PC8, PC9, PC11.
        // Press buttons to turn on leds
        while (cnt++ < 1000) {
            uint16_t vals = abst_group_digital_read(&buttons);
            abst_group_digital_write(&leds, vals);
            abst_delay_ms(1e1);
        }
    }
}
