/**
 * Example of using LCD display
 *
 * Call `make TARGET=lcd PROFILE=release all`
 */ 

#include <abstractSTM32.h>
#include <abstractLCD.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <stdint.h>
#include <stddef.h>

struct abst_lcd lcd = {
    .port = ABST_GPIOE,
    .VO = -1,
    .RC = 7,
    .RW = 10,
    .E = 11,
    .DB = 1 << 12 | 1 << 13 | 1 << 14 | 1 << 15,
    .LED = 9,
    .pwm_setting = ABST_SOFT_PWM,
    .delay_ms = NULL,
    .delay_us = NULL,
    .is_half_byte = 1
};

int main(void)
{
    abst_init(16e6, 0);
    abst_lcd_init(&lcd);
    abst_lcd_disp_contr(&lcd, 1, 1, 1);
    abst_lcd_set_led(&lcd, 255);

    while (1) {
        abst_lcd_entery_mode_set(&lcd, 1, 0);
        abst_lcd_clear_disp(&lcd);

        abst_lcd_put_str(&lcd, "Hello,");
        abst_lcd_set_cursor(&lcd, 1, 5);
        abst_lcd_put_str(&lcd, "world!");

        for (int i = 255; i > 0; i--) {
            abst_lcd_set_led(&lcd, i);
            abst_delay_ms(10);
        }
        for (int i = 0; i < 255; i++) {
            abst_lcd_set_led(&lcd, i);
            abst_delay_ms(10);
        }

        abst_delay_ms(3e3);

        abst_lcd_clear_disp(&lcd);

        abst_lcd_put_str_sm(&lcd, "This is still", 200);
        abst_lcd_set_cursor(&lcd, 1, 0);
        abst_lcd_put_str_sm(&lcd, "a test", 200);

        abst_delay_ms(1e3);

        abst_lcd_entery_mode_set(&lcd, 1, 1);
        abst_lcd_clear_disp(&lcd);
        abst_lcd_set_cursor(&lcd, 0, 16);

        abst_lcd_put_str_sm(&lcd, "HELLOOOO!!!!                    ", 300);
    }
}