/* 
 * File:   application.c
 * Author: Mohamed olwi
 *
 * Created on 03 October 2024, 05:25
 */
#include "application.h"

Std_ReturnType main(void)
{
    Std_ReturnType ret = ecu_layer_initialize();
    uint8 exist_status = PASSWORD_NOT_EXIST;        /* A variable to store the status of the password if it exist in the EEPROM */
    uint8 pass_status = PASSWORD_WRONG;             /* A variable to store the status of the wrong and right passwords */
    
    if (E_NOT_OK == ret)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Check if the pass exist in the EEPROM, if not then its the first time for this system to run*/
        ret |= check_if_pass_exist(&exist_status);
        if (exist_status  == PASSWORD_NOT_EXIST)
        {
            /* Prompt the user to enter his new password */
            ret |= create_new_password();
        }
        else
        {
            /* If the password exist in the EEPROM, then the user must enter it to access the door*/
            while (1)
            {
                ret |= enter_password();
                /* Check the status of the entered password, if its right or wrong*/
                ret |= check_pass(&pass_status);
                ret |= lcd_4bit_send_command(&lcd, _LCD_CLEAR);     
                if (pass_status == PASSWORD_RIGHT)
                { 
                    ret |= lcd_4bit_send_string_pos(&lcd, 2, 5, (uint8 *)"Welcome Home");
                    buzzer_n_times(2);
                    ret |= unlock_door();
                    __delay_ms(10000);
                     ret |= lcd_4bit_send_command(&lcd, _LCD_CLEAR);     
                    ret |= lcd_4bit_send_string_pos(&lcd, 2, 5, (uint8 *)"Locking the Door...");
                    ret |= lock_door();
                }
                else
                {
                    ret |= lcd_4bit_send_string_pos(&lcd, 2, 5, (uint8 *)"WRONG Password");
                    /* Alert the people in the home that a password is entered wrong*/
                    buzzer_n_times(5);
                }
                __delay_ms(3000);
            }
        }
    }
    return (ret);
}
