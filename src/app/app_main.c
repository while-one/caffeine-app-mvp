/**
 * @file app_main.c
 * @brief Main application logic implementation.
 */

#include "app_main.h"
#include "bsp_led.h"
#include <stdint.h>

void app_main(void)
{
    while (1)
    {
        bsp_led_toggle();

        /* Simple blocking delay for MVP */
        for (volatile uint32_t i = 0; i < 500000; i++)
        {
            __asm("nop");
        }
    }
}
