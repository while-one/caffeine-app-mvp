/**
 * @file main.c
 * @brief Application entry point.
 */

#include "app_main.h"
#include "bsp_led.h"

int main(void)
{
    /* Initialize the board support package */
    bsp_led_init();

    /* Start the application logic */
    app_main();

    /* Should never reach here */
    return 0;
}
