/**
 * @file bsp_led.h
 * @brief Board Support Package LED Interface.
 */

#ifndef BSP_LED_H
#define BSP_LED_H

/**
 * @brief Initialize the physical LED pin.
 */
void bsp_led_init(void);

/**
 * @brief Toggle the state of the LED.
 */
void bsp_led_toggle(void);

#endif /* BSP_LED_H */
