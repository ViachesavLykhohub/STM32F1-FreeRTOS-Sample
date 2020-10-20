/**
 * Example of using USART
 *
 * Call `make PROFILE=release DEVICE=stm32f103c8t6 V=1 tidy all`
 * 
 * or 
 *
 * make `PROFILE=release DEVICE=stm32f407vgt6 V=1 tidy all`
 */

#include <abstractSTM32.h>
#include <abstractUSART.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <libopencm3/stm32/rcc.h>

#include <stdio.h>

struct abst_usart USART = 
{
    .num = 3,
    .word_len = ABST_USART_WL_8_BITS,
    .num_stop_bits = ABST_USART_STOPBITS_1,
    .baud_rate = 115200,
};

struct abst_pin TX = {
#ifdef STM32F1
    .port = ABST_GPIOB,
    .num = 10,
#endif
#ifdef STM32F4
    .port = ABST_GPIOD,
    .num = 8,
#endif
    .mode = ABST_MODE_AF,
    .af = 7, // For STM32F4
    .af_dir = ABST_AF_OUTPUT,  // For STM32F1
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_2MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = true
};

struct abst_pin RX = {
#ifdef STM32F1
    .port = ABST_GPIOB,
    .num = 11,
#endif
#ifdef STM32F4
    .port = ABST_GPIOD,
    .num = 9,
#endif
    .mode = ABST_MODE_AF,
    .af = 7, // For STM32F4
    .af_dir = ABST_AF_INPUT, // For STM32F1
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_2MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = false
};

struct abst_pin led = {
    .port = ABST_GPIOC,
    .num = 13,
    .mode = ABST_MODE_OUTPUT,
    .otype = ABST_OTYPE_PP,
    .speed = ABST_OSPEED_2MHZ,
    .pull_up_down = ABST_PUPD_NONE,
    .is_inverse = true
};

SemaphoreHandle_t usart_sem;
static const TickType_t SEM_TIMEOUT = 5;
void usart3_isr(void)
{
    if (xSemaphoreTakeFromISR(usart_sem, SEM_TIMEOUT) == pdTRUE || 1) {
        abst_usart_interraption_handler(&USART);
        xSemaphoreGiveFromISR(usart_sem, NULL);
    }
}

static void task_send_msg(void *param)
{
    uint32_t T = *((uint32_t *)param);
    
    while (1) {
        char buff[50];
        snprintf(buff, 50, "Time: %i, T: %i\n", (int)xTaskGetTickCount(), (int)T);
        
        if (xSemaphoreTake(usart_sem, SEM_TIMEOUT) == pdTRUE) {
            abst_usart_send_text(&USART, buff);
            xSemaphoreGive(usart_sem);
        }
        vTaskDelay(T);
    }
}

int main(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    abst_gpio_init(&led);
    abst_digital_write(&led, 1);
    abst_gpio_init(&TX);
    abst_gpio_init(&RX);
    abst_usart_init(&USART, 100);
    
    usart_sem = xSemaphoreCreateBinary();
    xSemaphoreGive(usart_sem);
    uint32_t *delays = pvPortMalloc(sizeof(uint32_t) * 2);
    delays[0] = 700;
    delays[1] = 650;
    
    abst_usart_send_text(&USART, "Initialized\n");
    
    BaseType_t task_creation_status0 = 
        xTaskCreate(&task_send_msg,
                    "Usart0",
                    configMINIMAL_STACK_SIZE + 100,
                    (void *)(delays + 0),
                    configMAX_PRIORITIES - 1,
                    NULL);
        
    BaseType_t task_creation_status1 = 
        xTaskCreate(&task_send_msg,
                    "Usart1",
                    configMINIMAL_STACK_SIZE + 100,
                    (void *)(delays + 1),
                    configMAX_PRIORITIES - 1,
                    NULL);
    
    
    if (task_creation_status0 != pdPASS || task_creation_status1 != pdPASS){
        while(1);
    }
    
    abst_usart_send_text(&USART, "Start scheduler\n");
    vTaskStartScheduler();
}

void vApplicationTickHook(void)
{
    abst_sys_tick_handler();
}

extern void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) 
{
    while(1);
}
