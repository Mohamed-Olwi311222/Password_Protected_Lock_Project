/* 
 * File:   ecu_layer_init.h
 * Author: Mohamed olwi
 *
 * Created on 02 June 2024, 17:44
 */

#ifndef ECU_LAYER_INIT_H
#define	ECU_LAYER_INIT_H
/*----------------------------Header Files------------------------------------*/
#include "DC_Motor/ecu_dc_motor.h"
#include "keypad/ecu_keypad.h"
#include "Char_LCD/ecu_char_lcd.h"
/*----------------------------DataTypes---------------------------------------*/
extern char_lcd_4bit_t lcd_1;
extern char_lcd_8bit_t lcd_2;
/*----------------------------Function Prototypes-----------------------------*/
Std_ReturnType ecu_layer_initialize(void);

#endif	/* ECU_LAYER_INIT_H */

