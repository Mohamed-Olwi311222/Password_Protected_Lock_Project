/* 
 * File:   ecu_dc_motor.h
 * Author: Mohamed olwi
 *
 * Created on May 24, 2024, 2:00 AM
 */

#ifndef ECU_DC_MOTOR_H
#define	ECU_DC_MOTOR_H
/*----------------------------Header Files------------------------------------*/
#include "../ecu_layer_cgf.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
/*----------------------------Macros Declarations-----------------------------*/
#define DC_MOTOR_ON_STATUS      0x01U
#define DC_MOTOR_OFF_STATUS     0x00U

#define DC_MOTOR_MAX_PINS       0x02U
#define DC_MOTOR_PIN1           0x00U
#define DC_MOTOR_PIN2           0x01U
/*----------------------------DataTypes---------------------------------------*/

/**
 * struct dc_motor_t - a struct defining the 2 motor pins
 * @dc_motor_pins: the pins of the dc_motor
 */
typedef struct
{
    pin_config_t dc_motor_pins[DC_MOTOR_MAX_PINS];
}dc_motor_t;
/*----------------------------Function Prototypes-----------------------------*/

/**
 * @brief initialize the dc_motor 
 * @param dc_motor the pointer to the dc_motor config @ref dc_motor_t
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType dc_motor_initialize(const dc_motor_t *dc_motor);

/**
 * @brief make the motor turn right or forward
 * @param dc_motor the pointer to the dc_motor config @ref dc_motor_t
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType dc_motor_move_forward(const dc_motor_t *dc_motor);

/**
 * @brief make the motor turn left or backward
 * @param dc_motor the pointer to the dc_motor config @ref dc_motor_t
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType dc_motor_move_backward(const dc_motor_t *dc_motor);

/**
 * @brief make the motor stop
 * @param dc_motor the pointer to the dc_motor config @ref dc_motor_t
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType dc_motor_stop(const dc_motor_t *dc_motor);

#endif	/* ECU_DC_MOTOR_H */

