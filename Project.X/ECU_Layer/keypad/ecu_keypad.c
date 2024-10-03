/* 
 * File:   ecu_keypad.c
 * Author: Mohamed olwi
 *
 * Created on 02 June 2024, 15:59
 */
#include "ecu_keypad.h"

#if ECU_KEYPAD_ENABLED == ECU_ENABLED
static const uint8 btn_values[ECU_KEYPAD_ROWS][ECU_KEYPAD_COLS] = {
                                                               {'1', '2', '3'},
                                                               {'4', '5', '6'},
                                                               {'7', '8', '9'},
                                                               {'*', '0', '#'}
};
/**
 * @brief initialize the keypad pins
 * @param keypad_obj keypad config @ref keypad_t
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType keypad_initialize(const keypad_t *keypad_obj)
{
    Std_ReturnType ret = E_OK;
    uint8 l_row_counter = ZERO_INIT, l_col_counter = ZERO_INIT;
    
    if (NULL == keypad_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {    
        for (; l_row_counter < ECU_KEYPAD_ROWS; l_row_counter++)
        {
            ret = gpio_pin_initialize(&(keypad_obj->keypad_rows_pins[l_row_counter]));
            if (E_NOT_OK == ret)
            {
                break;
            }
        }
        if (E_OK == ret)
        {
            for (; l_col_counter < ECU_KEYPAD_COLS; l_col_counter++)
            {
                ret = gpio_pin_direction_initialize(&(keypad_obj->keypad_cols_pins[l_col_counter]));
                if (E_NOT_OK == ret)
                {
                    break;
                }
            }
        }
    }
    return (ret);    
}

/**
 * @brief gets the value of the pressed button on the keypad
 * @param keypad_obj keypad config @ref keypad_t
 * @param value pointer to an address to save the value read
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType keypad_get_value(const keypad_t *keypad_obj, uint8 *value)
{
    Std_ReturnType ret = E_OK;
    uint8 l_row_counter = ZERO_INIT, l_col_counter = ZERO_INIT, l_counter = ZERO_INIT;
    logic_t col_logic = GPIO_LOW;
 
    if ((NULL == keypad_obj) || (NULL == value))
    {
        ret = E_NOT_OK;
    }
    else
    {
        for (; l_row_counter < ECU_KEYPAD_ROWS; l_row_counter++)
        {
            /*write logic low to all rows pins*/
            for (l_counter = 0; l_counter < ECU_KEYPAD_ROWS; l_counter++)
            {
                if (E_NOT_OK == ret)
                {
                    break;
                }
                ret = gpio_pin_write_logic(&(keypad_obj->keypad_rows_pins[l_counter]), GPIO_LOW);
            }
            if (E_NOT_OK == ret)
            {
                break;
            }
            /*Write logic high to current row pin*/
           ret = gpio_pin_write_logic(&(keypad_obj->keypad_rows_pins[l_row_counter]), GPIO_HIGH);

           /*check if current row pin make contact with any col pin*/
           for (l_col_counter = 0; l_col_counter < ECU_KEYPAD_COLS; l_col_counter++)
           {
                if (E_NOT_OK == ret)
                {
                    break;
                }
                ret = gpio_pin_read_logic(&(keypad_obj->keypad_cols_pins[l_col_counter]), &col_logic);
               /*value of the button pressed is read and saved in @value*/
                if (GPIO_HIGH == col_logic)
                {
                    *value = btn_values[l_row_counter][l_col_counter];
                }
           }
        }
    }
    return (ret);    
}
#endif