/*
 * Visualize the result using any of Serial Plotters (i.e. in Arduino IDE)
 * make TARGET=osciloscope PROFILE=release DEVICE=stm32f103c8t6 V=1 LOG=1 tidy all
 */


#include "abstractSTM32.h"
#include "abstractADC.h"
#include "abstractLOG.h"

struct abst_pin adc_channel = {
    .port = ABST_GPIOA,
    .num = 2,
    .mode = ABST_MODE_ANALOG,
    .adc_num = 1,
    .adc_channel = 2,
    .adc_sample_time = ABST_ADC_SMPR_SMP_7DOT5CYC,
    .adc_resolution = ABST_ADC_RES_12BIT,
    .otype = ABST_OTYPE_OD,
    .speed = ABST_OSPEED_50MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};

struct abst_pin pot_ch = {
    .port = ABST_GPIOA,
    .num = 0,
    .mode = ABST_MODE_ANALOG,
    .adc_num = 1,
    .adc_channel = 0,
    .adc_sample_time = ABST_ADC_SMPR_SMP_7DOT5CYC,
    .otype = ABST_OTYPE_OD,
    .speed = ABST_OSPEED_2MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};

struct abst_pin DC_OUT1 = {
    .port = ABST_GPIOA,
    .num = 12,
    .mode = ABST_MODE_OUTPUT,
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_2MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};

struct abst_pin DC_OUT2 = {
    .port = ABST_GPIOA,
    .num = 11,
    .mode = ABST_MODE_OUTPUT,
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_2MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};


int main(void)
{
    abst_init(8e6, 0);
    abst_log_init();

    abst_gpio_init(&DC_OUT1);
    abst_gpio_init(&DC_OUT2);

    abst_digital_write(&DC_OUT2, 0);

    struct abst_pin *pins_arr[] = {&adc_channel, &pot_ch};
    
    uint8_t N = sizeof(pins_arr) / sizeof(pins_arr[0]);
    volatile uint16_t adc_vals[N];

    enum abst_errors err = abst_adc_read_cont(pins_arr, adc_vals, N, 2, 2);

    while(1) {
        abst_pwm_soft(&DC_OUT1, adc_vals[1] / 16);

        abst_logf("%i\n", (int)adc_vals[0]);
        abst_delay_us(10);
    }
}