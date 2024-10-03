/* 
 * File:   ecu_dc_motor.c
 * Author: Mohamed olwi
 *
 * Created on May 24, 2024, 2:00 AM
 */

#include "ecu_dc_motor.h"

#if ECU_DC_MOTOR_ENABLED == ECU_ENABLED

/**
 * @brief initialize the dc_motor 
 * @param dc_motor the pointer to the dc_motor config @ref dc_motor_t
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType dc_motor_initialize(const dc_motor_t *dc_motor)
{
    Std_ReturnType ret = E_OK;

    if (NULL == dc_motor)
    {
        ret = E_NOT_OK;
    }
    else
    {
        gpio_pin_initialize(&(dc_motor->dc_motor_pins[DC_MOTOR_PIN1]));
        gpio_pin_initialize(&(dc_motor->dc_motor_pins[DC_MOTOR_PIN2]));
    }
    return (ret);
}

/**
 * @brief make the motor turn right or forward
 * @param dc_motor the pointer to the dc_motor config @ref dc_motor_t
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType dc_motor_move_forward(const dc_motor_t *dc_motor)
{
    Std_ReturnType ret = E_OK;

    if (NULL == dc_motor)
    {
        ret = E_NOT_OK;
    }
    else
    {
        gpio_pin_write_logic(&(dc_motor->dc_motor_pins[DC_MOTOR_PIN1]), GPIO_HIGH);
        gpio_pin_write_logic(&(dc_motor->dc_motor_pins[DC_MOTOR_PIN2]), GPIO_LOW);
    }
    return (ret);
}

/**
 * @brief make the motor turn left or backward
 * @param dc_motor the pointer to the dc_motor config @ref dc_motor_t
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType dc_motor_move_backward(const dc_motor_t *dc_motor)
{
    Std_ReturnType ret = E_OK;

    if (NULL == dc_motor)
    {
        ret = E_NOT_OK;
    }
    else
    {    
        
        gpio_pin_write_logic(&(dc_motor->dc_motor_pins[DC_MOTOR_PIN1]), GPIO_LOW);
        gpio_pin_write_logic(&(dc_motor->dc_motor_pins[DC_MOTOR_PIN2]), GPIO_HIGH);
    }
    return (ret); 
}

/**
 * @brief make the motor stop
 * @param dc_motor the pointer to the dc_motor config @ref dc_motor_t
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType dc_motor_stop(const dc_motor_t *dc_motor)
{
    Std_ReturnType ret = E_OK;

    if (NULL == dc_motor)
    {
        ret = E_NOT_OK;
    }
    else
    {
        gpio_pin_write_logic(&(dc_motor->dc_motor_pins[DC_MOTOR_PIN1]), GPIO_LOW);
        gpio_pin_write_logic(&(dc_motor->dc_motor_pins[DC_MOTOR_PIN2]), GPIO_LOW);
    }
    return (ret);
}
#endif