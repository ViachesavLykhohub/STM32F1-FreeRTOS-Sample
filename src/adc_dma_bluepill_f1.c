/**
 * Example of using an ADC in continious mode with DMA
 *
 * `make TARGET=adc_dma_bluepill_f1 PROFILE=release DEVICE=stm32f103c8t6 V=1 tidy all`
 */ 

#include <abstractSTM32.h>
#include <abstractLCD.h>
#include <abstractADC.h>

#include <stdint.h>
#include <stdio.h>


struct abst_pin adc_channel = {
    .port = ABST_GPIOA,
    .num = 0,
    .mode = ABST_MODE_ANALOG,
    .adc_num = 1,
    .adc_channel = 0,
    .adc_sample_time = ABST_ADC_SMPR_SMP_55DOT5CYC,
    .adc_resolution = ABST_ADC_RES_12BIT,
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
    abst_init(8e6, 100);

    abst_gpio_init(&led_pwm);
    abst_digital_write(&led_pwm, 0);

    abst_gpio_init(&adc_channel);


    volatile uint16_t val[] = {0};
    struct abst_pin *pins_arr[] = {&adc_channel};

    enum abst_errors err = abst_adc_read_cont(pins_arr, val, 1, 2, 2);
    if (err != ABST_OK) {
        while (1) {
            abst_digital_write(&led_pwm, 1);
            abst_delay_ms(1e2);
            abst_digital_write(&led_pwm, 0);
            abst_delay_ms(1e2);
        }
    }
         
    while (1) {
        abst_pwm_soft(&led_pwm, (unsigned int) val[0] * 255 / 4095);
        abst_delay_ms(1e2);
    }
}
