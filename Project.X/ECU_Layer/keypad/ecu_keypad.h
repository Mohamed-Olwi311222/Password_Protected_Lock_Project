/* 
 * File:   ecu_keypad.h
 * Author: Mohamed olwi
 *
 * Created on 02 June 2024, 15:59
 */

#ifndef ECU_KEYPAD_H
#define	ECU_KEYPAD_H
/*----------------------------Header Files------------------------------------*/
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/Interrupt/mcal_INTx_external_interrupt.h"
#include "../../MCAL_Layer/Timers/Timer1/hal_timer1.h"
#include "../ecu_layer_cgf.h"
/*----------------------------Macros Declarations-----------------------------*/
#define ECU_KEYPAD_ROWS  4              /* The number of keypad rows */
#define ECU_KEYPAD_COLS  3              /* The number of keypad cols */
#define NO_INPUT         (uint8)0xFF    /* A value to indicate that no value is read*/
#define INT0_PIN         0              /* An index to the array of the cols logic status*/
#define INT1_PIN         1              /* An index to the array of the cols logic status*/
#define INT2_PIN         2              /* An index to the array of the cols logic status*/
/*----------------------------DataTypes---------------------------------------*/

/**
 * struct keypad_t - struct for keypad module
 * @keypad_rows_pins: pins of the keypad rows @ref pin_config_t
 * The cols of the keypad must be connected to INT0 ~ INT2 pins
*/
typedef struct
{
    pin_config_t keypad_rows_pins[ECU_KEYPAD_ROWS];
}keypad_t;
/*----------------------------Function Prototypes-----------------------------*/
/**
 * @brief initialize the keypad pins
 * @param keypad_obj keypad config @ref keypad_t
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType keypad_initialize(const keypad_t *keypad_obj);
/**
 * @brief: gets the value of the pressed button on the keypad
 * @note:  The cols must be connected to external INTx interrupt
 * @param keypad_obj keypad config @ref keypad_t
 * @param value pointer to an address to save the value read
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType keypad_get_value(const keypad_t *keypad_obj, uint8 *value);

#endif	/* ECU_KEYPAD_H */

