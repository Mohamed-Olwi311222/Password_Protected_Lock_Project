/* 
 * File:   password.h
 * Author: Mohamed olwi
 *
 * Created on 07 October 2024, 07:42
 */
#include "password.h"
#include "MCAL_Layer/Timers/Timer0/hal_timer0.h"
/*---------------Static Helper functions declerations---------------------------*/
static Std_ReturnType inline lock_system(void);
static void inline reset_password_array(void);
static void inline increment_tries(void);
static void timer0_interrupt_handler(void);
/*---------------Static Helper functions declerations End-----------------------*/
void timer0_interrupt_handler(void);
/*---------------Static Data types----------------------------------------------*/
static uint8 password[PASSWORD_LIMIT] = {0};            /* An array to store the entered password */
static volatile uint8 secs = ZERO_INIT;                 /* The number of seconds remaining in lock time */
static uint8 secs_str[BYTE_STR_SIZE] = {ZERO_INIT};     /* The array to contain the converted secs value */
static uint8 minutes = ZERO_INIT;                       /* The number of minutes remaining in lock time */
static uint8 minutes_str[BYTE_STR_SIZE] = {ZERO_INIT};  /* The array to contain the converted minutes value */
/* An array that contains each lock period */
static lock_time_in_seconds_t lock_time[MAX_LOCKS] = {
                                                    LOCK_FOR_30_SECS, 
                                                    LOCK_FOR_1_MIN,
                                                    LOCK_FOR_5_MIN,
                                                    LOCK_FOR_15_MIN,
                                                    LOCK_FOR_30_MIN,
                                                    LOCK_FOR_60_MIN
                                                     };
static uint8 lock_active = 1;                           /* A flag for deactivating the lock if the lock time is done*/
static uint8 attempts = 0;                              /* Number of wrong attempts */
static uint8 timer0_flag = 1;                           /* A flag to indicate that the lock time is finished*/
/*---------------Static Data types End------------------------------------------*/

/**
 * @brief Prompt the user to enter his/her password
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType enter_password(void)
{
    Std_ReturnType ret = E_OK;
    uint8 val = NO_INPUT;
    uint8 pass_index = 0;
    
    /* If the lock is active and no.wrong attempts >= 3*/
    if (lock_active && attempts >= 3)
    {
        /* Lock the system*/
        ret |= lock_system();
        /* Reset the flag so the system doesn't lock until a new wrong attempt happened*/
        lock_active = 0;
    }
    ret |= lcd_4bit_send_command(&lcd, _LCD_CLEAR);
    ret |= lcd_4bit_send_string_pos(&lcd, 1, 1, (uint8 *)"Enter Your");
    ret |= lcd_4bit_send_string_pos(&lcd, 2, 5, (uint8 *)"password");
    
    while (pass_index < PASSWORD_LIMIT)
    {
        /* Read the value entered through the connected keypad */
        ret |= keypad_get_value(&keypad, &val);
        
        /* If the value entered is valid, save it in the password array*/
        if (NO_INPUT != val && TIMEOUT != val)
        {
            ret |= lcd_4bit_send_string_pos(&lcd, 3, pass_index + 1, (uint8 *)"*");
            password[pass_index] = val;
            pass_index++;
        }
        /* If a timeout happened, prompt the user a message and start all over again*/
        else if (TIMEOUT == val)
        {
            ret |= lcd_4bit_send_command(&lcd, _LCD_CLEAR);
            ret |= lcd_4bit_send_string_pos(&lcd, 1, 1, (uint8 *)"Timeout! Try again.");
            /* Reset the password array that store the password entered if a timeout happened*/
            reset_password_array();
            pass_index = 0;
            __delay_ms(3000);
            ret |= lcd_4bit_send_command(&lcd, _LCD_CLEAR);
            ret |= lcd_4bit_send_string_pos(&lcd, 1, 1, (uint8 *)"Enter Your");
            ret |= lcd_4bit_send_string_pos(&lcd, 2, 5, (uint8 *)"password");
        }
    }
    return (ret);
}

/**
 * @brief: Create a new password by storing the entered password in the EEPROM
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType  create_new_password(void)
{
        Std_ReturnType ret = E_OK;
        ret |= lcd_4bit_send_string_pos(&lcd, 1, 1, (uint8 *)"Enter Your");
        ret |= lcd_4bit_send_string_pos(&lcd, 2, 5, (uint8 *)"New password");
        __delay_ms(3000);
        ret |= lcd_4bit_send_command(&lcd, _LCD_CLEAR);
        ret |= lcd_4bit_send_string_pos(&lcd, 1, 1, (uint8 *)"The password length");
        ret |= lcd_4bit_send_string_pos(&lcd, 2, 5, (uint8 *)"Is 8 digits");
        __delay_ms(3000);
        /* Make the user enter his/her password */
        ret |= enter_password();
        /* Store the entered password in the EEPROM */
        ret |= Data_EEPROM_Write_Array(EEPROM_STARTING_ADD, password, PASSWORD_LIMIT);
        ret |= lcd_4bit_send_command(&lcd, _LCD_CLEAR);
        ret |= lcd_4bit_send_string_pos(&lcd, 1, 1, (uint8 *)"Password Saved :)");
        __delay_ms(3000);
        /* Reset the password array that store the entered password*/
        reset_password_array();
        return (ret);
}
/**
 * @brief: Compare the password entered with the actual password stored in the EEPROM
 * @param pass_status the address to store the status of the password that will be returned
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType check_pass(uint8 *pass_status)
{
    Std_ReturnType ret = E_OK;
    uint8 counter = ZERO_INIT;
    /* Temp array to read the actual password from the EEPROM */
    uint8 temp_password[PASSWORD_LIMIT] = {ZERO_INIT};
    
    /* Assume the password is right */
    *pass_status = PASSWORD_RIGHT;
    ret |= Data_EEPROM_Read_Array(EEPROM_STARTING_ADD, temp_password, PASSWORD_LIMIT);
    for (; counter < PASSWORD_LIMIT; counter++)
    {
        /* If the EEPROM pass not equal the entered pass, then the entered pass is wrong*/
        if (temp_password[counter] != password[counter])
        {
            *pass_status = PASSWORD_WRONG;
            /* Increment no.wrong attempts */
            attempts++; 
            /* Make the lock flag to 1 to lock the system if the wrong attempts >= 3*/
            lock_active = 1;
            break;
        }
    }
    /* Reset the no.wrong attempts if the password entered is right*/
    if (PASSWORD_RIGHT == *pass_status)
    {
        /* Reset the number of attempts if a correct password entered*/
        attempts = 0;
    }
    /* Attempts works as an index to the lock_time array but the difference is 3
       as the lock only works when attempts >= 3*/
    if ((attempts < MAX_LOCKS + 3) && (attempts >= 3))
    {
       minutes = (lock_time[attempts - 3]);
    }
    /* If the attempts is more than the MAX_LOCKS then it will always lock down for an hour*/
    else if ((attempts >= 3))
    {
        minutes = (lock_time[5]);
    }
    return (ret);
}

/**
 * @brief: Check if the password entered exist in the EEPROM
 * @param exist_status The address to store the existence status
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType check_if_pass_exist(uint8 *exist_status)
{
    Std_ReturnType ret = E_OK;
    uint8 counter = ZERO_INIT;
    /* Assume that the password exist*/
    *exist_status = PASSWORD_EXIST;
    ret |= Data_EEPROM_Read_Array(EEPROM_STARTING_ADD, password, PASSWORD_LIMIT);
    for (; counter < PASSWORD_LIMIT; counter++)
    {
        /* If any value entered is equal to NO_INPUT, the password doesnt exist*/
        if (password[counter] == NO_INPUT)
        {
            *exist_status = PASSWORD_NOT_EXIST;
            break;
        }
    }
    /* Reset the password array that store the entered password*/
    reset_password_array();
    return (ret);
}
/**
 * @brief: Make the buzzer produce a sound for n times
 * @param n_times the number of times that the buzzer should produce
 */
void inline buzzer_n_times(uint8 n_times)
{
    uint8 counter = ZERO_INIT;
    
    for (; counter < n_times; counter++)
    {
        gpio_pin_write_logic(&buzzer, GPIO_HIGH);
        __delay_ms(500);
        gpio_pin_write_logic(&buzzer, GPIO_LOW);
        __delay_ms(500);
    }
}

/**
 * @brief: Make the 2 locks move right to close the door
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType inline lock_door(void)
{
    Std_ReturnType ret = E_OK;
    ret |= dc_motor_move_forward(&dc_motor_1);
    ret |= dc_motor_move_forward(&dc_motor_2);
    __delay_ms(3000);
    ret |= dc_motor_stop(&dc_motor_1);
    ret |= dc_motor_stop(&dc_motor_2);
    return (ret);
}
 /**
 * @brief: Make the 2 locks move left to open the door
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType inline unlock_door(void)
{
    Std_ReturnType ret = E_OK;
    ret |= dc_motor_move_backward(&dc_motor_1);
    ret |= dc_motor_move_backward(&dc_motor_2);
    __delay_ms(3000);
    ret |= dc_motor_stop(&dc_motor_1);
    ret |= dc_motor_stop(&dc_motor_2);
    return (ret);
}

/**
 * @brief: The interrupt handler of the timer0 overflow interrupt
 */
static void timer0_interrupt_handler(void)
{
    if (0 == secs)
    {
        if (0 == minutes)
        {
            /* Lock time is done*/
            timer0_flag = 0;
        }
        minutes--;
        secs = 60;
    }
    secs--;
}
static Std_ReturnType inline lock_system(void)
{
    Std_ReturnType ret = E_OK;
    /* A timer0 object to raise an interrupt each 1 second*/
    timer0_t timer0_obj = {
        .timer0_preloaded_value = 0x0BDC,
        .clock_src = _TIMER0_INT_SRC,
        .edge = _TIMER0_RISING_EDGE,
        .resolution_bits = _TIMER0_16bit_RES,
        .prescaler_enable = _TIMER0_PRESCALER_ENABLE,
        .prescaler_value = TIMER0_PRESCALER_DIV_BY_32,
        .timer0_interrupt_handler = timer0_interrupt_handler,
        .timer0_interrupt_priority = INTERRUPT_LOW_PRIORITY
    };
    /* 3 attempts => 30 secs*/
    if (3 == attempts)
    {
        secs = 29;
    }
    else
    {
        /* As the minutes will be more than 1 so the secs will be updated in the ISR when
           minutes decrement by 1*/
        secs = 0;
    }
    /*Init the timer0 module*/
    ret |= timer0_init(&timer0_obj);
    
    ret |= lcd_4bit_send_command(&lcd, _LCD_CLEAR);
    ret |= lcd_4bit_send_string_pos(&lcd, 1, 1, (uint8 *)"You have been locked");
    ret |= lcd_4bit_send_string_pos(&lcd, 2, 1, (uint8 *)"From the system for:");
    ret |= lcd_4bit_send_string_pos(&lcd, 3, 4,(uint8 *)":");

    /* Start locking the system until the lock time finishes */
    timer0_flag = 1;
    while (timer0_flag)
    {
        ret |= convert_uint8_to_string(minutes, minutes_str);
        ret |= convert_uint8_to_string(secs, secs_str);

        ret |= lcd_4bit_send_string_pos(&lcd, 3, 1, minutes_str);

        ret |= lcd_4bit_send_string_pos(&lcd, 3, 5, secs_str);
    }
    /* Activate the lock if further wrong attempts entered */
    lock_active = 1;
    /* Deinitialize the timer0 as the lock time is finished */
    timer0_deinit(&timer0_obj);
    return (ret);
}
/**
 * @brief: Reset the password array that store the entered password
 */
static void inline reset_password_array(void)
{
    uint8 counter = ZERO_INIT;
    for (; counter < PASSWORD_LIMIT; counter++)
        password[counter] = NO_INPUT;
}