/**
 * Example of ADC
 *
 * Call `make TARGET=adc_bluepill_f1 PROFILE=release DEVICE=stm32f103c8t6 V=1 tidy all`
 */ 

#include <abstractSTM32.h>
#include <stdint.h>
#include <stdio.h>

struct abst_pin acd_channel = {
    .port = ABST_GPIOA,
    .num = 0,
    .mode = ABST_MODE_ANALOG,
    .adc_num = 1,
    .adc_channel = 0,
    .adc_sample_time = ABST_ADC_SMPR_SMP_55DOT5CYC,
    .adc_resolution = ABST_ADC_RES_10BIT,
    .otype = ABST_OTYPE_OD,
    .speed = ABST_OSPEED_50MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};

struct abst_pin led_pwm = {
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
    abst_init(8e6, 0);

    abst_gpio_init(&led_pwm);
    abst_digital_write(&led_pwm, 1);
    abst_gpio_init(&acd_channel);

    abst_digital_write(&led_pwm, 0);

    while (1) {
        uint32_t val = abst_adc_read(&acd_channel);

        abst_pwm_soft(&led_pwm, (val * 256) / 1024);

        abst_delay_ms(1e2);
    }
}