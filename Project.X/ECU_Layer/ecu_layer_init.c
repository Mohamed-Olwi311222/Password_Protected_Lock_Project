/* 
 * File:   ecu_layer_init.c
 * Author: Mohamed olwi
 *
 * Created on 02 June 2024, 17:44
 */
#include "ecu_layer_init.h"

/* Keypad for entering the password */
keypad_t keypad =   {
    /*rows*/
    .keypad_rows_pins[0].port = PORTC_INDEX,
    .keypad_rows_pins[0].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_rows_pins[0].logic = GPIO_HIGH,
    .keypad_rows_pins[0].pin = GPIO_PIN0,
    
    .keypad_rows_pins[1].port = PORTC_INDEX,
    .keypad_rows_pins[1].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_rows_pins[1].logic = GPIO_HIGH,
    .keypad_rows_pins[1].pin = GPIO_PIN1,
    
    .keypad_rows_pins[2].port = PORTC_INDEX,
    .keypad_rows_pins[2].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_rows_pins[2].logic = GPIO_HIGH,
    .keypad_rows_pins[2].pin = GPIO_PIN2,
    
    .keypad_rows_pins[3].port = PORTC_INDEX,
    .keypad_rows_pins[3].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_rows_pins[3].logic = GPIO_HIGH,
    .keypad_rows_pins[3].pin = GPIO_PIN3,
    /*Cols*/
    .keypad_cols_pins[0].port = PORTC_INDEX,
    .keypad_cols_pins[0].direction = GPIO_DIRECTION_INPUT,
    .keypad_cols_pins[0].logic = GPIO_LOW,
    .keypad_cols_pins[0].pin = GPIO_PIN4,
    
    .keypad_cols_pins[1].port = PORTC_INDEX,
    .keypad_cols_pins[1].direction = GPIO_DIRECTION_INPUT,
    .keypad_cols_pins[1].logic = GPIO_LOW,
    .keypad_cols_pins[1].pin = GPIO_PIN5,
    
    .keypad_cols_pins[2].port = PORTC_INDEX,
    .keypad_cols_pins[2].direction = GPIO_DIRECTION_INPUT,
    .keypad_cols_pins[2].logic = GPIO_LOW,
    .keypad_cols_pins[2].pin = GPIO_PIN6,
};

/* LCD for showing the message*/
char_lcd_4bit_t lcd = {
    .lcd_rs.port = PORTC_INDEX,
    .lcd_rs.pin = GPIO_PIN7,
    .lcd_rs.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_rs.logic = GPIO_LOW,
    
    .lcd_en.port = PORTD_INDEX,
    .lcd_en.pin = GPIO_PIN0,
    .lcd_en.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_en.logic = GPIO_LOW,
    
    .lcd_data[0].port = PORTD_INDEX,
    .lcd_data[0].pin = GPIO_PIN1,
    .lcd_data[0].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[0].logic = GPIO_LOW,
    
    .lcd_data[1].port = PORTD_INDEX,
    .lcd_data[1].pin = GPIO_PIN2,
    .lcd_data[1].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[1].logic = GPIO_LOW,
    
    .lcd_data[2].port = PORTD_INDEX,
    .lcd_data[2].pin = GPIO_PIN3,
    .lcd_data[2].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[2].logic = GPIO_LOW,
    
    .lcd_data[3].port = PORTD_INDEX,
    .lcd_data[3].pin = GPIO_PIN4,
    .lcd_data[3].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[3].logic = GPIO_LOW
};

/* DC motor 1 to act as lock 1*/
dc_motor_t  dc_motor_1 = {
    .dc_motor_pins[DC_MOTOR_PIN1].port = PORTA_INDEX,
    .dc_motor_pins[DC_MOTOR_PIN1].pin = GPIO_PIN0,
    .dc_motor_pins[DC_MOTOR_PIN1].logic = DC_MOTOR_OFF_STATUS,
    .dc_motor_pins[DC_MOTOR_PIN1].direction = GPIO_DIRECTION_OUTPUT,

    .dc_motor_pins[DC_MOTOR_PIN2].port = PORTA_INDEX,
    .dc_motor_pins[DC_MOTOR_PIN2].pin = GPIO_PIN1,
    .dc_motor_pins[DC_MOTOR_PIN2].logic = DC_MOTOR_OFF_STATUS,
    .dc_motor_pins[DC_MOTOR_PIN2].direction = GPIO_DIRECTION_OUTPUT
};

/* DC motor 2 to act as lock 2*/
dc_motor_t  dc_motor_2 = {
    .dc_motor_pins[DC_MOTOR_PIN1].port = PORTA_INDEX,
    .dc_motor_pins[DC_MOTOR_PIN1].pin = GPIO_PIN2,
    .dc_motor_pins[DC_MOTOR_PIN1].logic = DC_MOTOR_OFF_STATUS,
    .dc_motor_pins[DC_MOTOR_PIN1].direction = GPIO_DIRECTION_OUTPUT,

    .dc_motor_pins[DC_MOTOR_PIN2].port = PORTA_INDEX,
    .dc_motor_pins[DC_MOTOR_PIN2].pin = GPIO_PIN3,
    .dc_motor_pins[DC_MOTOR_PIN2].logic = DC_MOTOR_OFF_STATUS,
    .dc_motor_pins[DC_MOTOR_PIN2].direction = GPIO_DIRECTION_OUTPUT

};
Std_ReturnType ecu_layer_initialize(void)
{
    Std_ReturnType ret = E_OK;
    ret |= keypad_initialize(&keypad);
    ret |= lcd_4bit_initialize(&lcd);
    ret |= dc_motor_initialize(&dc_motor_1);
    ret |= dc_motor_initialize(&dc_motor_2);
    return (ret);
}