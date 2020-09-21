/**
 * Example of using LCD display
 *
 * Call `make TARGET=adc_discovery_f4 PROFILE=release DEVICE=stm32f407vgt6 V=1 tidy all`
 */ 

#include <abstractSTM32.h>
#include <abstractLCD.h>
#include <abstractADC.h>
#include <stdint.h>
#include <stdio.h>

struct abst_pin acd_channel = {
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
    abst_init(16e6, 100);

    abst_gpio_init(&led_pwm);
    abst_digital_write(&led_pwm, 1);
    abst_gpio_init(&acd_channel);

    abst_lcd_init(&lcd);
    abst_lcd_disp_contr(&lcd, 1, 0, 0);
    abst_lcd_set_led(&lcd, 255);

    abst_digital_write(&led_pwm, 0);

    while (1) {
        uint16_t val = abst_adc_read(&acd_channel);

        abst_pwm_soft(&led_pwm, (val * 256) / 1024);

        uint8_t N = 20;
        char buff[N];
        snprintf(buff, N, "Val: %u", val);

        abst_lcd_clear_disp(&lcd);
        abst_lcd_put_str(&lcd, buff);

        abst_delay_ms(1e2);
    }
}
