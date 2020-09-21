/**
 * Example of usage hard PWM with STM32F4
 * To build and flash call
 * make TARGET=hard_pwm PROFILE=release DEVICE=stm32f407vgt6 V=1 tidy all
 * or
 * make TARGET=hard_pwm PROFILE=release DEVICE=stm32f103c8t6 V=1 tidy all
 */

#include <abstractSTM32.h>
#include <stdint.h>

struct abst_pin led = {
    .port = ABST_GPIOC, // ABST_GPIOD for STM32F4 Discovery
    .num = 13,
    .mode = ABST_MODE_OUTPUT,
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_2MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};

struct abst_pin led_pwm = { // Require external connected LED
    .port = ABST_GPIOA,
    .num = 8,
    .mode = ABST_MODE_OUTPUT,
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_50MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};


int main(void)
{
    abst_init(16e6, 100);
    abst_init_hard_pwm_tim1(16e6, 500);
    abst_gpio_init(&led);
    abst_gpio_init(&led_pwm);

    abst_digital_write(&led, 1);
    abst_digital_write(&led_pwm, 1);
    abst_delay_ms(2e3);


    uint8_t brigt[] = {0, 10, 100, 200, 255};
    uint8_t N = sizeof(brigt) / sizeof(brigt[0]);

    uint8_t i = 0;
    while (1) {
        abst_pwm_hard(&led_pwm, brigt[i++]);
        if (i > N)
            i = 0;
        
        abst_toggle(&led);
        abst_delay_ms(1e3);

    }
}
