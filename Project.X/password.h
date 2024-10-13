/* 
 * File:   password.h
 * Author: Mohamed olwi
 *
 * Created on 07 October 2024, 07:42
 */

#ifndef PASSWORD_H
#define	PASSWORD_H
/*----------------------------Header Files------------------------------------*/
#include "std_types.h"
#include "ECU_Layer/DC_Motor/ecu_dc_motor.h"
#include "ECU_Layer/keypad/ecu_keypad.h"
#include "ECU_Layer/Char_LCD/ecu_char_lcd.h"
#include "MCAL_Layer/EEPROM/mcal_eeprom.h"
#include "MCAL_Layer/Timers/Timer1/hal_timer1.h"
#include "password.h"
/*----------------------------Macros Declarations-----------------------------*/
#define PASSWORD_LIMIT                  8               /* The limit of the password to enter */
#define PASSWORD_EXIST                  1               /* Password exist in the EEPROM */
#define PASSWORD_NOT_EXIST              0               /* Password does not exist in the EEPROM */
#define EEPROM_STARTING_ADD             (uint16)0x00    /* The starting address to read/write in the EEPROM */
#define PASSWORD_WRONG                  0               /* Password entered is wrong */
#define PASSWORD_RIGHT                  1               /* Password entered is right */
#define MAX_LOCKS                       6               /* The number of max lock time options */
/*----------------------------Data Types--------------------------------------*/
extern keypad_t keypad;
extern char_lcd_4bit_t lcd;
extern dc_motor_t  dc_motor_1;
extern dc_motor_t  dc_motor_2;
extern pin_config_t buzzer;
/**
 * An enum for the lock time in secs
 */
typedef enum
{
    LOCK_FOR_30_SECS = 0 ,
    LOCK_FOR_1_MIN   = 1 ,
    LOCK_FOR_5_MIN   = 5 ,
    LOCK_FOR_15_MIN  = 15,
    LOCK_FOR_30_MIN  = 30,
    LOCK_FOR_60_MIN  = 60,
} lock_time_in_seconds_t;
/*----------------------------Function Prototypes-----------------------------*/
/**
 * @brief Prompt the user to enter his/her password
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType enter_password(void);
/**
 * @brief: Create a new password by storing the entered password in the EEPROM
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType  create_new_password(void);
/**
 * @brief: Compare the password entered with the actual password stored in the EEPROM
 * @param pass_status the address to store the status of the password that will be returned
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType check_pass(uint8 *pass_status);
/**
 * @brief: Check if the password entered exist in the EEPROM
 * @param exist_status The address to store the existence status
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType check_if_pass_exist(uint8 *exist_status);
/**
 * @brief: Make the buzzer produce a sound for n times
 * @param n_times the number of times that the buzzer should produce
 */
void inline buzzer_n_times(uint8 n_times);
/**
 * @brief: Make the 2 locks move right to close the door
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType inline lock_door(void);
/**
 * @brief: Make the 2 locks move left to open the door
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType inline unlock_door(void);
#endif	/* PASSWORD_H */

