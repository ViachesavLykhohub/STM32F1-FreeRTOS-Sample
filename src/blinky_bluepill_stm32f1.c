#include <abstractSTM32.h>
#include <stdint.h>

struct abst_pin led = {
    .port = ABST_GPIOC,
    .num = 13,
    .mode = ABST_MODE_OUTPUT,
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_2MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};


int main(void)
{
    abst_init(8e6, 100);
    abst_gpio_init(&led);
    

    uint32_t period = 10e3; // ms.
    while (1) {
        while (abst_time_ms() % period < 5e3) {
            abst_toggle(&led);
            abst_delay_ms(2e2);
        }
        while (abst_time_ms() % period >= 5e3) {
            for (int i = 0; i < 255; i++) {
                abst_pwm_soft(&led, i);
                abst_delay_ms(5);
            }
            for (int i = 255; i > 0; i--){
                abst_pwm_soft(&led, i);
                abst_delay_ms(5);
            }
            abst_stop_pwm_soft(&led);
        }
    }
}
