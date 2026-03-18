/**
 * @file bsp_led.c
 * @brief Board Support Package LED Implementation.
 */

#include "bsp_led.h"
#include <stddef.h>
#include "cfn_hal_gpio.h"
/* Private Hardware Definition --------------------------------------*/

#if defined(BLINKY_STM32F407VG_HWV0001)
#include "cfn_hal_gpio_port.h"
/* Example 1: Green LED on STM32F407 Discovery (PD12) */
#define LED_GPIO_PORT CFN_HAL_GPIO_PORT_D
#define LED_GPIO_PIN  CFN_HAL_GPIO_PIN_12
#elif defined(BLINKY_STM32F417VG_HWV0001)
#include "cfn_hal_gpio.h"
#include "cfn_hal_gpio_port.h"
/* Example 2: LED on a different board (e.g., Nucleo PA5) */
#define LED_GPIO_PORT CFN_HAL_GPIO_PORT_A
#define LED_GPIO_PIN  CFN_HAL_GPIO_PIN_5
#else
/* Mock/Linux Fallback */
#define LED_GPIO_PORT 0
#define LED_GPIO_PIN  0
#endif

static cfn_hal_gpio_t led_driver;

void bsp_led_init(void)
{
#if defined(BLINKY_STM32F407VG_HWV0001) || defined(BLINKY_STM32F417VG_HWV0001)
    cfn_hal_gpio_phy_t phy = {
        .port = (void *) (uintptr_t) LED_GPIO_PORT,
    };

    cfn_hal_gpio_pin_config_t config = { .pin_mask = LED_GPIO_PIN,
                                         .mode = CFN_HAL_GPIO_CONFIG_MODE_OUTPUT_PP,
                                         .pull = CFN_HAL_GPIO_CONFIG_PULL_NOPULL,
                                         .speed = CFN_HAL_GPIO_CONFIG_SPEED_LOW,
                                         .strength = CFN_HAL_GPIO_CONFIG_STRENGTH_LOW,
                                         .default_state = CFN_HAL_GPIO_STATE_LOW };

    (void) cfn_hal_gpio_construct(&led_driver, NULL, &phy);
    (void) cfn_hal_gpio_init(&led_driver);
    (void) cfn_hal_gpio_pin_config(&led_driver, &config);
#else
    /* No physical GPIO on Linux native placeholder */
    (void) led_driver;
#endif
}

void bsp_led_toggle(void)
{
#if defined(BLINKY_STM32F407VG_HWV0001) || defined(BLINKY_STM32F417VG_HWV0001)
    (void) cfn_hal_gpio_pin_toggle(&led_driver, LED_GPIO_PIN);
#endif
}
