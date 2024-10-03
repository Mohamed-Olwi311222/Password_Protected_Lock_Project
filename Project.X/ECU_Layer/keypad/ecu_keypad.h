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
#include "../ecu_layer_cgf.h"
/*----------------------------Macros Declarations-----------------------------*/
#define ECU_KEYPAD_ROWS  3
#define ECU_KEYPAD_COLS  3
/*----------------------------DataTypes---------------------------------------*/

/**
 * struct keypad_t - struct for keypad module
 * @keypad_rows_pins: pins of the keypad rows @ref pin_config_t
 * @keypad_cols_pins: pins of the keypad cols @ref pin_config_t
*/
typedef struct
{
  pin_config_t keypad_rows_pins[ECU_KEYPAD_ROWS];
  pin_config_t keypad_cols_pins[ECU_KEYPAD_COLS];
}keypad_t;
/*----------------------------Function Prototypes-----------------------------*/

/**
 * @brief initialize the keypad pins
 * @param keypad_obj keypad config @ref keypad_t
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType keypad_initialize(const keypad_t *keypad_obj);

/**
 * @brief gets the value of the pressed button on the keypad
 * @param keypad_obj keypad config @ref keypad_t
 * @param value pointer to an address to save the value read
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType keypad_get_value(const keypad_t *keypad_obj, uint8 *value);


#endif	/* ECU_KEYPAD_H */

