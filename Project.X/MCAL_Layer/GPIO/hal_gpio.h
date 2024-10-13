/* 
 * File:   hal_gpio.h
 * Author: Mohamed olwi
 *
 * Created on April 1, 2024, 4:11 PM
 */

#ifndef HAL_GPIO_H
#define	HAL_GPIO_H

/*----------------------------Header Files------------------------------------*/
#include <xc.h>
#include "../mcal_std_types.h"
#include "../device_config.h"
#include "hal_gpio_cfg.h"
/*---------------------------Macros Declarations-------------------------------*/
#define KHz *1000UL
#define MHz KHz KHz
#define _XTAL_FREQ (8 MHz)
#define PORT_PIN_MAX_SIZE               8
#define PORT_MAX_LENGTH                 5
#define DIRECTION_MAX_LENGTH            2
#define BIT_MASK                     (uint8)1
#define PORT_MASK                    0xFF
/*--------------------------Macro Functions-----------------------------------*/
#define CLEAR_BIT(_REG, BIT_POS)     (_REG &= ~(BIT_MASK << BIT_POS))
#define SET_BIT(_REG, BIT_POS)       (_REG |= (BIT_MASK << BIT_POS))
#define TOGGLE_BIT(_REG, BIT_POS)    (_REG ^= (BIT_MASK << BIT_POS))
#define READ_BIT(_REG, BIT_POS)      ((_REG & (BIT_MASK << BIT_POS)) >> BIT_POS)
#define MY_HWREG8(_x)                (*(volatile uint8 *)(_x))
/*--------------------------DataTypes-----------------------------------------*/
typedef union
{
    struct
    {
        unsigned LATC0 : 1;
        unsigned LATC1 : 1;
        unsigned LATC2 : 1;
        unsigned LATC3 : 1;
        unsigned LATC4 : 1;
        unsigned LATC5 : 1;
        unsigned LATC6 : 1;
        unsigned LATC7 : 1;
        
    };
    uint8 LATC_REGISTER;
}LATC_BITS_T;
#define LATC_REG   ((volatile LATC_BITS_T *) 0xF8B)

typedef enum
{
    GPIO_LOW = 0,
    GPIO_HIGH
}logic_t;

typedef enum
{
    GPIO_DIRECTION_OUTPUT = 0,
    GPIO_DIRECTION_INPUT
}direction_t;

typedef enum
{
    GPIO_PIN0 = 0,
    GPIO_PIN1,
    GPIO_PIN2,
    GPIO_PIN3,
    GPIO_PIN4,
    GPIO_PIN5,
    GPIO_PIN6,
    GPIO_PIN7
}pin_index_t;

typedef enum
{
    PORTA_INDEX = 0,
    PORTB_INDEX,
    PORTC_INDEX,
    PORTD_INDEX,
    PORTE_INDEX,
}port_index_t;

typedef struct
{
    uint8 port : 3;             /*@ref use port_index_t */
    uint8 pin  : 3;             /*@ref use pin_index_t  */
    uint8 direction : 1;        /*@ref use direction_t  */
    uint8 logic : 1;            /*@ref use logic_t      */
}pin_config_t;

/*----------------------------Function Prototypes-----------------------------*/

/**
 * @brief initialize the given pin of the given config
 * @param _pin_config_t the configration given (i.e led, motor, etc..)
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType gpio_pin_initialize(const pin_config_t *_pin_config_t);

/**
 * @brief initialize the direction of a given pin
 * @param _pin_config_t the configration given (i.e led, motor, etc..)
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType gpio_pin_direction_initialize(const pin_config_t *_pin_config_t);
/**
 * @brief write logic to a given pin in a port
 * @param _pin_config_t the configration given (i.e led, motor, etc..)
 * @param logic the logic to write @ref logic_t
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType gpio_pin_write_logic(const pin_config_t *_pin_config_t, logic_t logic);
/**
 * @brief read the logic on the pins of the port
 * @param port the port index given @ref port_index_t
 * @param logic the address to store the read logic
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType gpio_port_read_logic(const port_index_t port, uint8 *logic);
#endif	/* HAL_GPIO_H */

