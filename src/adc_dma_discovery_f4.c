/**
 * Example of using an ADC in continious mode with DMA
 *
 * `make TARGET=adc_dma_discovery_f4 PROFILE=release DEVICE=stm32f407vgt6 V=1 tidy all`
 */ 

#include <abstractSTM32.h>
#include <abstractLCD.h>
#include <abstractADC.h>

#include <stdint.h>
#include <stdio.h>

struct abst_pin adc_channel = {
    .port = ABST_GPIOC,
    .num = 3,
    .mode = ABST_MODE_ANALOG,
    .adc_num = 1,
    .adc_channel = 13,
    .adc_sample_time = ABST_ADC_SMPR_SMP_480CYC,
    .adc_resolution = ABST_ADC_RES_10BIT,
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_50MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};

struct abst_pin adc_temperature = { // Require external connection to some analog sensor
    .port = ABST_GPIOB,
    .num = 1,
    .mode = ABST_MODE_ANALOG,
    .adc_num = 1,
    .adc_channel = 9,
    .adc_sample_time = ABST_ADC_SMPR_SMP_480CYC,
    .adc_resolution = ABST_ADC_RES_10BIT,
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_50MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};

struct abst_pin led_pwm = {
    .port = ABST_GPIOD,
    .num = 13,
    .mode = ABST_MODE_OUTPUT,
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_2MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};

struct abst_lcd lcd = {
    .port = ABST_GPIOE,
    .VO = -1,
    .RC = 7,
    .RW = 10,
    .E = 11,
    .DB = 1 << 12 | 1 << 13 | 1 << 14 | 1 << 15,
    .LED = 9,
    .pwm_setting = ABST_NO_PWM,
    .delay_ms = 0,
    .delay_us = 0,
    .is_half_byte = 1
};

int main(void)
{
    abst_init(16e6, 0);

    abst_gpio_init(&led_pwm);
    abst_digital_write(&led_pwm, 1);
    abst_gpio_init(&adc_channel);
    abst_gpio_init(&adc_temperature);


    abst_lcd_init(&lcd);
    abst_lcd_disp_contr(&lcd, 1, 0, 0);
    abst_lcd_set_led(&lcd, 255);

    abst_digital_write(&led_pwm, 0);

    volatile uint16_t val[] = {0, 0};
    struct abst_pin *pins_arr[] = {&adc_channel, &adc_temperature};

    enum abst_errors err = abst_adc_read_cont(pins_arr, val, 2, 8, 2);
    if (err != ABST_OK) {
        uint8_t N = 20;
        char buff[N];
        snprintf(buff, N, "Error: %i", err);
        abst_lcd_put_str(&lcd, buff);
        abst_delay_ms(10e6);
    }
    while (1) {

        abst_pwm_soft(&led_pwm, (val[0] * 256) / 1024);

        uint8_t N = 20;
        char buff_val[N];
        char buff_temp[N];

        snprintf(buff_val, N, "Val: %u", (unsigned int)val[0]);
        snprintf(buff_temp, N, "Temp: %u", (unsigned int)val[1]);

        abst_lcd_clear_disp(&lcd);
        abst_lcd_put_str(&lcd, buff_val);
        abst_lcd_set_cursor(&lcd, 1, 0);
        abst_lcd_put_str(&lcd, buff_temp);

        abst_delay_ms(5e2);
    }
}