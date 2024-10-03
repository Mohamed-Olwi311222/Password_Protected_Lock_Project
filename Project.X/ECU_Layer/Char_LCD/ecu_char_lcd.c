/* 
 * File:   ecu_char_lcd.c
 * Author: Mohamed Olwi
 *
 * Created on July 2, 2024, 5:18 PM
 */
#include "ecu_char_lcd.h"

#if ECU_CHAR_LCD_ENABLED == ECU_ENABLED
/**
 * @brief A helper function to send 4 bits to the lcd with 4 pins config
 * @param lcd the 4-bit lcd
 * @param data_command the 4 bits of data or the command to send
 * @return E_OK if status success otherwise E_NOT_OK
 */
static Std_ReturnType lcd_send_4bits(const char_lcd_4bit_t *lcd, uint8 data_command);

/**
 * @brief A helper function to send an enable signal to the lcd with 4 pins config
 * @param lcd the 4-bit lcd
 * @return E_OK if status success otherwise E_NOT_OK
 */
static Std_ReturnType lcd_4bit_send_enable_signal(const char_lcd_4bit_t *lcd);

/**
 * @brief A helper function to set the cursor of the lcd with 4 pins config
 * @param lcd the 4-bit lcd
 * @param row the row of the cursor to set
 * @param col the column of the cursor to set
 * @return E_OK if status success otherwise E_NOT_OK
 */
static Std_ReturnType lcd_4bit_set_cursor(const char_lcd_4bit_t *lcd, uint8 row, uint8 col);

/**
 * @brief A helper function to send an enable signal to the lcd with 8 pins config
 * @param lcd the 8-bit lcd
 * @return E_OK if status success otherwise E_NOT_OK
 */
static Std_ReturnType lcd_8bit_send_enable_signal(const char_lcd_8bit_t *lcd);

/**
 * @brief A helper function to set the cursor of the lcd with 8 pins config
 * @param lcd the 8-bit lcd
 * @param row the row of the cursor to set
 * @param col the column of the cursor to set
 * @return E_OK if status success otherwise E_NOT_OK
 */
static Std_ReturnType lcd_8bit_set_cursor(const char_lcd_8bit_t *lcd, uint8 row, uint8 col);

Std_ReturnType lcd_4bit_initialize(const char_lcd_4bit_t *lcd)
{
	Std_ReturnType ret = E_OK; 
    uint8 lcd_data_counter = ZERO_INIT;
	if (NULL == lcd)
	{
	    ret = E_NOT_OK;
	}
	else
	{
        ret = gpio_pin_initialize(&(lcd->lcd_rs));
        if (E_OK == ret)
        {
            ret = gpio_pin_initialize(&(lcd->lcd_en));
            if (E_OK == ret)
            {
                for (lcd_data_counter = ZERO_INIT; lcd_data_counter < 4; lcd_data_counter++)
                {
                    ret = gpio_pin_initialize(&(lcd->lcd_data[lcd_data_counter]));
                    if (E_NOT_OK == ret)
                    {
                        break;
                    }
                }
                if (E_OK == ret)
                {
                    /*The Initialization by instructions*/
                    __delay_ms(20);
                    ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
                    __delay_ms(5);
                    ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
                    __delay_us(150);
                    ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
                    /*Optional Instruction*/
                    if (E_OK == ret)
                    {
                        ret |= lcd_4bit_send_command(lcd, _LCD_CLEAR);
                        ret |= lcd_4bit_send_command(lcd, _LCD_RETURN_HOME);
                        ret |= lcd_4bit_send_command(lcd, _LCD_ENTRY_MODE);
                        ret |= lcd_4bit_send_command(lcd, _LCD_CURSOR_OFF_DISPLAY_ON);
                        ret |= lcd_4bit_send_command(lcd, _LCD_4BIT_MODE_2_LINE);
                        ret |= lcd_4bit_send_command(lcd, _LCD_DDRAM_START);
                    }
                }
            }
        }
    }
	return (ret);
}

Std_ReturnType lcd_4bit_send_command(const char_lcd_4bit_t *lcd, uint8 command)
{
	Std_ReturnType  ret = E_OK; 
	if (NULL == lcd)
	{
	    ret = E_NOT_OK;
	}
	else
	{
        ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_LOW);
        if (E_OK == ret)
        {
            ret = lcd_send_4bits(lcd, command >> 4);
            if (E_OK == ret)
                ret = lcd_4bit_send_enable_signal(lcd);
            if (E_OK == ret)
            {
                ret = lcd_send_4bits(lcd, command);
                if (E_OK == ret)
                    ret = lcd_4bit_send_enable_signal(lcd);
            }
        }
    }
	return (ret);
}

Std_ReturnType lcd_4bit_send_char_data(const char_lcd_4bit_t *lcd, uint8 data)
{
	Std_ReturnType  ret = E_OK; 
	if (NULL == lcd)
	{
	    ret = E_NOT_OK;
	}
	else
	{
        ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_HIGH);
        if (E_OK == ret)
        {
            ret = lcd_send_4bits(lcd, data >> 4);
            if (E_OK == ret)
                ret = lcd_4bit_send_enable_signal(lcd);
            if (E_OK == ret)
            {
                ret = lcd_send_4bits(lcd, data);
                if (E_OK == ret)
                    ret = lcd_4bit_send_enable_signal(lcd);
            }
        }
    }


	return (ret);
}

Std_ReturnType lcd_4bit_send_char_data_pos(const char_lcd_4bit_t *lcd, uint8 row, uint8 col, uint8 data)
{
	Std_ReturnType  ret = E_OK; 
	if (NULL == lcd)
	{
	    ret = E_NOT_OK;
	}
	else
	{
        ret = lcd_4bit_set_cursor(lcd, row, col);
        if (E_OK == ret)
            ret = lcd_4bit_send_char_data(lcd, data);
    }


	return (ret);
}
 
Std_ReturnType lcd_4bit_send_string(const char_lcd_4bit_t *lcd, uint8 *str)
{
	Std_ReturnType  ret = E_OK; 
	if (NULL == lcd || NULL == str)
	{
	    ret = E_NOT_OK;
	}
	else
	{
        while (*str)
        {
            ret = lcd_4bit_send_char_data(lcd, *str++);
            if (E_NOT_OK == ret)
            {
                break;
            }
        }
    }


	return (ret);
}

Std_ReturnType lcd_4bit_send_string_pos(const char_lcd_4bit_t *lcd, uint8 row, uint8 col, uint8 *str)
{
	Std_ReturnType  ret = E_OK; 
	if (NULL == lcd || NULL == str)
	{
	    ret = E_NOT_OK;
	}
	else
	{
        ret = lcd_4bit_set_cursor(lcd, row, col);
        if (E_OK == ret)
        {
            ret = lcd_4bit_send_string(lcd, str);
        }
    }
	return (ret);
}

Std_ReturnType lcd_4bit_send_custom_char(const char_lcd_4bit_t *lcd, uint8 row, uint8 col, const uint8 _char[], uint8 mem_pos)
{
	Std_ReturnType  ret = E_OK; 
    uint8 lcd_counter = ZERO_INIT;

	if (NULL == lcd)
	{
    	ret = E_NOT_OK;
	}
	else
	{
		/*mem_pos serves as an index in the CGRAM addresses*/
		ret = lcd_4bit_send_command(lcd, (_LCD_CGRAM_START) + (mem_pos * 8));
		if (E_OK == ret)
		{
			for (lcd_counter = 0; lcd_counter < 7; lcd_counter++)
			{
				ret = lcd_4bit_send_char_data(lcd, _char[lcd_counter]);
				if (E_NOT_OK == ret)
				{
					break;
				}
			}
			ret = lcd_4bit_send_char_data_pos(lcd, row, col, mem_pos);
        }
    }

	return (ret);
}



Std_ReturnType lcd_8bit_initialize(const char_lcd_8bit_t *lcd)
{
	Std_ReturnType  ret = E_OK; 
    uint8 lcd_data_counter = ZERO_INIT;
	if (NULL == lcd)
	{
	    ret = E_NOT_OK;
	}
	else
	{
        ret = gpio_pin_initialize(&(lcd->lcd_rs));
        if (E_OK == ret)
        {
            ret = gpio_pin_initialize(&(lcd->lcd_en));
            if (E_OK == ret)
            {
                for (lcd_data_counter = ZERO_INIT; lcd_data_counter < 8; lcd_data_counter++)
                {
                    ret = gpio_pin_initialize(&(lcd->lcd_data[lcd_data_counter]));
                    if (E_NOT_OK == ret)
                    {
                        break;
                    }
                }
            }
			/*The Initialization by instructions*/
			__delay_ms(20);
			ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
			__delay_ms(5);
			ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
			__delay_us(150);
			ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
			/*Optional Instruction*/
			if (E_OK == ret)
			{
				ret |= lcd_8bit_send_command(lcd, _LCD_CLEAR);
				ret |= lcd_8bit_send_command(lcd, _LCD_RETURN_HOME);
				ret |= lcd_8bit_send_command(lcd, _LCD_ENTRY_MODE);
				ret |= lcd_8bit_send_command(lcd, _LCD_CURSOR_OFF_DISPLAY_ON);
				ret |= lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
				ret |= lcd_8bit_send_command(lcd, _LCD_DDRAM_START);
			}
        }
    }
	return (ret);
}

Std_ReturnType lcd_8bit_send_command(const char_lcd_8bit_t *lcd, uint8 command)
{
	Std_ReturnType  ret = E_OK; 
	uint8 counter = ZERO_INIT;

	if (NULL == lcd)
	{
	    ret = E_NOT_OK;
	}
	else
	{
		ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_LOW);
		if (E_OK == ret)
		{
			for (counter = ZERO_INIT; counter < 8; counter++)
			{
				ret = gpio_pin_write_logic(&(lcd->lcd_data[counter]), (command >> counter) & BIT_MASK);
				if (E_NOT_OK == ret)
				{
					break;
				}
			}
			if (E_OK == ret)
			{
				ret = lcd_8bit_send_enable_signal(lcd);
			}
		}
    }
	return (ret);
}

Std_ReturnType lcd_8bit_send_char_data(const char_lcd_8bit_t *lcd, uint8 data)
{
	Std_ReturnType  ret = E_OK; 
	uint8 counter = ZERO_INIT;

	if (NULL == lcd)
	{
	    ret = E_NOT_OK;
	}
	else
	{
		ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_HIGH);
		if (E_OK == ret)
		{
			for (counter = ZERO_INIT; counter < 8; counter++)
			{
				ret = gpio_pin_write_logic(&(lcd->lcd_data[counter]), (data >> counter) & BIT_MASK);
				if (E_NOT_OK == ret)
				{
					break;
				}
			}
			if (E_OK == ret)
			{
				ret = lcd_8bit_send_enable_signal(lcd);
			}
		}
    }
	return (ret);
}

Std_ReturnType lcd_8bit_send_char_data_pos(const char_lcd_8bit_t *lcd, uint8 row, uint8 col, uint8 data)
{
	Std_ReturnType  ret = E_OK; 
	if (NULL == lcd)
	{
	    ret = E_NOT_OK;
	}
	else
	{
		ret = lcd_8bit_set_cursor(lcd, row, col);
		if (E_OK == ret)
		{
			ret = lcd_8bit_send_char_data(lcd, data);
		}
    }
	return (ret);
}
 
Std_ReturnType lcd_8bit_send_string(const char_lcd_8bit_t *lcd, uint8 *str)
{
	Std_ReturnType  ret = E_OK; 
	if (NULL == lcd)
	{
	    ret = E_NOT_OK;
	}
	else
	{
		while (*str)
		{
			ret = lcd_8bit_send_char_data(lcd, *str++);
			if (E_NOT_OK == ret)
			{
				break;
			}
		}
    }
	return (ret);
}

Std_ReturnType lcd_8bit_send_string_pos(const char_lcd_8bit_t *lcd, uint8 row, uint8 col, uint8 *str)
{
	Std_ReturnType  ret = E_OK; 
	if (NULL == lcd || NULL == str)
	{
	    ret = E_NOT_OK;
	}
	else
	{
		ret = lcd_8bit_set_cursor(lcd, row, col);
		if (E_OK == ret)
		{
			ret = lcd_8bit_send_string(lcd, str);
		}
    }
	return (ret);
}

Std_ReturnType lcd_8bit_send_custom_char(const char_lcd_8bit_t *lcd, uint8 row, uint8 col, const uint8 _char[], uint8 mem_pos)
{
	Std_ReturnType  ret = E_OK; 
	uint8 lcd_counter = ZERO_INIT;
    
	if (NULL == lcd)
	{
	    ret = E_NOT_OK;
	}
	else
	{
		/*mem_pos serves as an index in the CGRAM addresses*/
		ret = lcd_8bit_send_command(lcd, (_LCD_CGRAM_START) + (mem_pos * 8));
		if (E_OK == ret)
		{
			for (lcd_counter = 0; lcd_counter < 7; lcd_counter++)
			{
				ret = lcd_8bit_send_char_data(lcd, _char[lcd_counter]);
				if (E_NOT_OK == ret)
				{
					break;
				}
			}
			ret = lcd_8bit_send_char_data_pos(lcd, row, col, mem_pos);
		}
    }
	return (ret);
}

static Std_ReturnType lcd_send_4bits(const char_lcd_4bit_t *lcd, uint8 data_command)
{
    Std_ReturnType ret = E_OK;
    uint8 counter = ZERO_INIT;

    if (NULL == lcd)
    {
        ret = E_NOT_OK;
    }
    else
    {
        for (counter = ZERO_INIT; counter < 4; counter++)
        {
            ret = gpio_pin_write_logic(&(lcd->lcd_data[counter]), (data_command >> counter) & BIT_MASK);
            if (E_NOT_OK == ret)
            {
                break;
            }
        }
    }
    return (ret);
}
static Std_ReturnType lcd_4bit_set_cursor(const char_lcd_4bit_t *lcd, uint8 row, uint8 col)
{
	Std_ReturnType ret = E_OK;

	if (NULL == lcd)
	{
		ret = E_NOT_OK;
	}
	else
	{
		col--;
		if (UINT8_MAX == col)
		{
			col = 0;
		}
		switch (row)
		{
            case ROW1:
                ret = lcd_4bit_send_command(lcd, _LCD_DDRAM_START + col);
                break;
            case ROW2:
                ret = lcd_4bit_send_command(lcd, 0xC0 + col);
                break;
            case ROW3:
                ret = lcd_4bit_send_command(lcd, 0x94 + col);
                break;
            case ROW4:
                ret = lcd_4bit_send_command(lcd, 0xD4 + col);
                break;
            default:
                ret = E_NOT_OK;
                break;
		}
	}
	return (ret);
}
static Std_ReturnType lcd_4bit_send_enable_signal(const char_lcd_4bit_t *lcd)
{
    Std_ReturnType ret = E_OK;

    if (NULL == lcd)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_HIGH);
        __delay_us(5);
        if (E_OK == ret)
        {
            ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_LOW);
        }
    }
    return (ret);
}
static Std_ReturnType lcd_8bit_send_enable_signal(const char_lcd_8bit_t *lcd)
{
	Std_ReturnType ret = E_OK;

	if (NULL == lcd)
	{
		ret = E_NOT_OK;
	}
	else
	{
		ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_HIGH);
        __delay_us(5);
		ret |= gpio_pin_write_logic(&(lcd->lcd_en), GPIO_LOW);
	}
	return (ret);
}
static Std_ReturnType lcd_8bit_set_cursor(const char_lcd_8bit_t *lcd, uint8 row, uint8 col)
{
	Std_ReturnType ret = E_OK;

	if (NULL == lcd)
	{
		ret = E_NOT_OK;
	}
	else
	{
		col--;
		if (UINT8_MAX == col)
		{
			col = 0;
		}
		switch (row)
		{
            case ROW1:
                ret = lcd_8bit_send_command(lcd, _LCD_DDRAM_START + col);
                break;
            case ROW2:
                ret = lcd_8bit_send_command(lcd, 0xC0 + col);
                break;
            case ROW3:
                ret = lcd_8bit_send_command(lcd, 0x94 + col);
                break;
            case ROW4:
                ret = lcd_8bit_send_command(lcd, 0xD4 + col);
                break;
            default:
                ret = E_NOT_OK;
                break;
		}
	}
	return (ret);
}

Std_ReturnType convert_uint8_to_string(uint8 value, uint8 *str)
{
	Std_ReturnType  ret = E_OK; 
    uint8 temp_str[BYTE_STR_SIZE] = {0};
    uint8 counter = 0;
    
	if (NULL == str)
	{
	    ret = E_NOT_OK;
	}
	else
	{
		memset(str, ' ', BYTE_STR_SIZE - 1);
        str[5] = '\0';
        /* Store the value given in the temp_str followed by null termination */
		if (snprintf((char *)temp_str, BYTE_STR_SIZE, "%u", value) < 0)
		{
			ret = E_NOT_OK;
		}
        /* Repeat until the current char in temp_str is null*/
        /* So the str given have the value in it followed by space and a null termination */
        /* Instead of str full of null termination each time a different value is given */
        /* e.g: value = 1 , str = {1, , ,\0} , but not str = {1 ,\0, \0, \0} */
        while (temp_str[counter] != '\0')
        {
            str[counter] = temp_str[counter];
            counter++;
        }
    }
	return (ret);
}

Std_ReturnType convert_uint16_to_string(uint16 value, uint8 *str)
{
	Std_ReturnType  ret = E_OK; 
    uint8 temp_str[SHORT_STR_SIZE] = {0};
    uint8 counter = 0;

	if (NULL == str)
	{
	    ret = E_NOT_OK;
	}
	else
	{
		memset(str, ' ', SHORT_STR_SIZE - 1);
        str[5] = '\0';
        /* Store the value given in the temp_str followed by null termination */
		if (snprintf((char *)temp_str, SHORT_STR_SIZE, "%u", value) < 0)
		{
			ret = E_NOT_OK;
		}
        /* Repeat until the current char in temp_str is null*/
        /* So the str given have the value in it followed by space and a null termination */
        /* Instead of str full of null termination each time a different value is given */
        /* e.g: value = 123 , str = {1,2,3, , ,\0} , but not str = {1, 2, 3, \0, \0, \0} */
        while (temp_str[counter] != '\0')
        {
            str[counter] = temp_str[counter];
            counter++;
        }
    }
	return (ret);
}
#endif