/*
 * Visualize the result using any of Serial Plotters (i.e. in Arduino IDE)
 * make TARGET=osciloscope PROFILE=release DEVICE=stm32f103c8t6 V=1 LOG=1 tidy all
 */


#include "abstractSTM32.h"
#include "abstractADC.h"
#include "abstractLOG.h"

struct abst_pin adc_channel = {
    .port = ABST_GPIOA,
    .num = 1,
    .mode = ABST_MODE_ANALOG,
    .adc_num = 1,
    .adc_channel = 1,
    .adc_sample_time = ABST_ADC_SMPR_SMP_7DOT5CYC,
    .adc_resolution = ABST_ADC_RES_12BIT,
    .otype = ABST_OTYPE_OD,
    .speed = ABST_OSPEED_50MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};


int main(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    abst_init(72e6, 100);
    abst_log_init(115200, 5000);


    struct abst_pin *pins_arr[] = {&adc_channel};
    
    uint8_t N = sizeof(pins_arr) / sizeof(pins_arr[0]);
    volatile uint16_t adc_vals[N];

    enum abst_errors err = abst_adc_read_cont(pins_arr, adc_vals, N, 2, 2);

    while(1) {
        abst_logf("%i\n", (int)((adc_vals[0]) > 1500));
        abst_delay_us(10);
    }
}
