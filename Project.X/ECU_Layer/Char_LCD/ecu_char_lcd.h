/* 
 * File:   ecu_char_lcd.h
 * Author: acemido
 *
 * Created on July 2, 2024, 5:18 PM
 */

#ifndef ECU_CHAR_LCD_H
#define	ECU_CHAR_LCD_H
/*----------------------------Header Files------------------------------------*/
#include "../ecu_layer_cgf.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include <string.h>
/*----------------------------Macros Declarations-----------------------------*/
#define _LCD_CLEAR                      0x01
#define _LCD_RETURN_HOME                0x02
#define _LCD_ENTRY_MODE                 0x06
#define _LCD_CURSOR_OFF_DISPLAY_ON      0x0C
#define _LCD_CURSOR_OFF_DISPLAY_OFF     0x08
#define _LCD_CURSOR_ON_BLINK_ON         0x0F
#define _LCD_CURSOR_ON_BLINK_OFF        0x0E
#define _LCD_DISPLAY_SHIFT_RIGHT        0x1C
#define _LCD_DISPLAY_SHIFT_LEFT         0x18
#define _LCD_8BIT_MODE_2_LINE           0x38
#define _LCD_4BIT_MODE_2_LINE           0x28
#define _LCD_CGRAM_START                0x40
#define _LCD_DDRAM_START                0x80

#define ROW1                            1
#define ROW2                            2
#define ROW3                            3
#define ROW4                            4
/*----------------------------DataTypes---------------------------------------*/

/**
 * struct char_lcd_4bit_t - struct for character lcd with 4 bit config
 * @lcd_rs: the register select pin config @ref pin_config_t
 * @lcd_en: the enable pin config @ref pin_config_t
 * @lcd_data: the 4 data lines of the lcd with 4 bit config @ref pin_config_t
*/
typedef struct 
{
    pin_config_t lcd_rs;
    pin_config_t lcd_en;
    pin_config_t lcd_data[4];
} char_lcd_4bit_t;

/**
 * struct char_lcd_8bit_t - struct for character lcd with 8 bit config
 * @lcd_rs: the register select pin config @ref pin_config_t
 * @lcd_en: the enable pin config @ref pin_config_t
 * @lcd_data: the 8 data lines of the lcd with 8 bit config @ref pin_config_t
*/
typedef struct 
{
    pin_config_t lcd_rs;
    pin_config_t lcd_en;
    pin_config_t lcd_data[8];
} char_lcd_8bit_t;
/*----------------------------Function Prototypes-----------------------------*/

/**
 * @brief initialize the 4bit lcd with instruction
 * @param lcd the 4bit lcd to initialize
 * @return E_OK if status success otherwise E_NOT_OK
 */
Std_ReturnType lcd_4bit_initialize(const char_lcd_4bit_t *lcd);

/**
 * @brief send command to an lcd with 4bit configuration
 * @param lcd the lcd 4bit configuration
 * @param command the command to send
 * @return E_OK if status success otherwise E_NOT_OK 
 */
Std_ReturnType lcd_4bit_send_command(const char_lcd_4bit_t *lcd, uint8 command);

/**
 * @brief send a char to an lcd with 4bit configuration
 * @param lcd the lcd 4bit configuration
 * @param data the char to send
 * @return E_OK if status success otherwise E_NOT_OK
 */
Std_ReturnType lcd_4bit_send_char_data(const char_lcd_4bit_t *lcd, uint8 data);

/**
 * @brief send a char to an lcd with 4bit configuration with required row and col
 * @param lcd the lcd 4bit configuration
 * @param row the row required of the lcd which will start printing to it
 * @param col the column required of the lcd
 * @param data the char to send
 * @return E_OK if status success otherwise E_NOT_OK 
 */
Std_ReturnType lcd_4bit_send_char_data_pos(const char_lcd_4bit_t *lcd, uint8 row, uint8 col, uint8 data); 

/**
 * @brief send a string to an lcd with 4bit configuration
 * @param lcd the lcd 4bit configuration
 * @param str the string to send
 * @return E_OK if status success otherwise E_NOT_OK
 */
Std_ReturnType lcd_4bit_send_string(const char_lcd_4bit_t *lcd, uint8 *str);

/**
 * @brief send a string to an lcd with 4bit configuration with required row and col
 * @param lcd the lcd 4bit configuration
 * @param row the row required of the lcd which will start printing to it
 * @param row the row required of the lcd which will start printing to it
 * @param str the string to send
 * @return E_OK if status success otherwise E_NOT_OK
 */
Std_ReturnType lcd_4bit_send_string_pos(const char_lcd_4bit_t *lcd, uint8 row, uint8 col, uint8 *str);

/**
 * @brief send a custom generated character to lcd with 4 bit config
 * @param lcd the lcd 4bit configuration
 * @param row the row required of the lcd which will start printing to it
 * @param row the row required of the lcd which will start printing to it
 * @param _char the custom character to display
 * @param mem_pos the index of the CGRAM in which the custom char will be stored
 * @return E_OK if status success otherwise E_NOT_OK
 */
Std_ReturnType lcd_4bit_send_custom_char(const char_lcd_4bit_t *lcd, uint8 row, uint8 col, const uint8 _char[], uint8 mem_pos);

/**
 * @brief initialize the 8bit lcd with instruction
 * @param lcd the 8bit lcd to initialize
 * @return E_OK if status success otherwise E_NOT_OK 
 */
Std_ReturnType lcd_8bit_initialize(const char_lcd_8bit_t *lcd);

/**
 * @brief send a command to an lcd with 8bit configuration
 * @param lcd the lcd 8bit configuration
 * @param command the command to send
 * @return E_OK if status success otherwise E_NOT_O 
 */
Std_ReturnType lcd_8bit_send_command(const char_lcd_8bit_t *lcd, uint8 command);

/**
 * @brief send a char to an lcd with 8bit configuration
 * @param lcd the lcd 8bit configuration
 * @param data the char to send
 * @return E_OK if status success otherwise E_NOT_OK
 */
Std_ReturnType lcd_8bit_send_char_data(const char_lcd_8bit_t *lcd, uint8 data);

/**
 * @brief send a char to an lcd with 8bit configuration with required row and col
 * @param lcd the lcd 8bit configuration
 * @param row the row required of the lcd which will start printing to it
 * @param col the column required of the lcd
 * @param data the char to send
 * @return E_OK if status success otherwise E_NOT_OK 
 */
Std_ReturnType lcd_8bit_send_char_data_pos(const char_lcd_8bit_t *lcd, uint8 row, uint8 col, uint8 data); 

/**
 * @brief send a string to an lcd with 8bit configuration
 * @param lcd the lcd 8bit configuration
 * @param str the string to send
 * @return E_OK if status success otherwise E_NOT_OK
 */
Std_ReturnType lcd_8bit_send_string(const char_lcd_8bit_t *lcd, uint8 *str);

/**
 * @brief send a string to an lcd with 8bit configuration with required row and col
 * @param lcd the lcd 8-bit configuration
 * @param row the row required of the lcd which will start printing to it
 * @param col the column required of the lcd
 * @param str the string to send
 * @return E_OK if status success otherwise E_NOT_OK
 */
Std_ReturnType lcd_8bit_send_string_pos(const char_lcd_8bit_t *lcd, uint8 row, uint8 col, uint8 *str);

/**
 * @brief send a custom generated character to lcd with 8-bit config
 * @param lcd the lcd 8-bit configuration
 * @param row the row required of the lcd which will start printing to it
 * @param row the row required of the lcd which will start printing to it
 * @param _char the custom character to display
 * @param mem_pos the index of the CGRAM in which the custom char will be stored
 * @return E_OK if status success otherwise E_NOT_OK
 */
Std_ReturnType lcd_8bit_send_custom_char(const char_lcd_8bit_t *lcd, uint8 row, uint8 col, const uint8 _char[], uint8 mem_pos);

/**
 * @brief convert a 1 byte unsigned int to string
 * @param value the value to convert
 * @param str the array of 4(3 + 1 null term) elements so store the number in it
 * @return E_OK if status success otherwise E_NOT_OK 
 */
Std_ReturnType convert_uint8_to_string(uint8 value, uint8 *str);

/**
 * @brief convert a 2 byte unsigned int to string
 * @param value the value to convert
 * @param str the array of 6(5 + 1 null term) elements so store the number in it
 * @return E_OK if status success otherwise E_NOT_OK 
 */
Std_ReturnType convert_uint16_to_string(uint16 value, uint8 *str);
#endif	/* ECU_CHAR_LCD_H */

