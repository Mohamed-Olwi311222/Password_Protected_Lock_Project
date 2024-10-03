/* 
 * File:   mcal_INTx_external_interrupt.h
 * Author: Mohamed olwi
 *
 * Created on 10 August 2024, 08:00 AM
 */

#ifndef MCAL_INTX_EXTERNAL_INTERRUPT_H
#define	MCAL_INTX_EXTERNAL_INTERRUPT_H

#if EXTERNAL_INTERRUPT_ENABLE == INTERRUPT_FEATURE_ENABLE
/*----------------------------Header Files------------------------------------*/
#include "mcal_interrupt_config.h"

/*----------------------------Macros Functions Declarations-------------------*/
#if EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE

/******INT0******/

/**
 * ENABLE INT0 interrupt
 */
#define EXT_INT0_INTERRUPT_ENABLE()                     (INTCONbits.INT0IE = 1)
/**
 * DISABLE INT0 interrupt
 */
#define EXT_INT0_INTERRUPT_DISABLE()                    (INTCONbits.INT0IE = 0)
/**
 * CLEAR INT0 flag bit
 */
#define EXT_INT0_INTERRUPT_FLAG_BIT_CLEAR()             (INTCONbits.INT0IF = 0)
/**
 * INT0 interrupt on rising edge
 */
#define EXT_INT0_ON_RISING_EDGE()                       (INTCON2bits.INTEDG0 = 1)
/**
 * INT0 interrupt on falling edge
 */
#define EXT_INT0_ON_FALLING_EDGE()                      (INTCON2bits.INTEDG0 = 0)

/******INT1******/
/**
 * ENABLE INT1 interrupt
 */
#define EXT_INT1_INTERRUPT_ENABLE()                     (INTCON3bits.INT1IE = 1)
/**
 * DISABLE INT1 interrupt
 */
#define EXT_INT1_INTERRUPT_DISABLE()                    (INTCON3bits.INT1IE = 0)
/**
 * CLEAR INT1 flag bit
 */
#define EXT_INT1_INTERRUPT_FLAG_BIT_CLEAR()             (INTCON3bits.INT1IF = 0)
/**
 * INT1 interrupt on rising edge
 */
#define EXT_INT1_ON_RISING_EDGE()                       (INTCON2bits.INTEDG1 = 1)
/**
 * INT1 interrupt on falling edge
 */
#define EXT_INT1_ON_FALLING_EDGE()                      (INTCON2bits.INTEDG1 = 0)

/******INT2******/
/**
 * ENABLE INT2 interrupt
 */
#define EXT_INT2_INTERRUPT_ENABLE()                     (INTCON3bits.INT2IE = 1)
/**
 * DISABLE INT2 interrupt
 */
#define EXT_INT2_INTERRUPT_DISABLE()                    (INTCON3bits.INT2IE = 0)
/**
 * CLEAR INT2 flag bit
 */
#define EXT_INT2_INTERRUPT_FLAG_BIT_CLEAR()             (INTCON3bits.INT2IF = 0)
/**
 * INT2 interrupt on rising edge
 */
#define EXT_INT2_ON_RISING_EDGE()                       (INTCON2bits.INTEDG1 = 1)
/**
 * INT2 interrupt on falling edge
 */
#define EXT_INT2_ON_FALLING_EDGE()                      (INTCON2bits.INTEDG1 = 0)

/**PRIORITY FOR EXTERNAL INTERRUPTS**/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/**
 * INT1 HIGH priority
 */
#define EXT_INT1_HIGH_PRIORITY()                        (INTCON3bits.INT1P = 1)
/**
 * INT1 LOW priority
 */
#define EXT_INT1_LOW_PRIORITY()                         (INTCON3bits.INT1P = 0)
/**
 * INT2 HIGH priority
 */
#define EXT_INT2_HIGH_PRIORITY()                        (INTCON3bits.INT2P = 1)
/**
 * INT2 LOW priority
 */
#define EXT_INT2_LOW_PRIORITY()                         (INTCON3bits.INT2P = 0)
#endif
#endif


/*----------------------------DataTypes---------------------------------------*/

/**
 * an enum for external interrupts for contolling the falling and rising edges
 */
typedef enum
{
    INTERRUPT_FALLING_EDGE = 0,                         /*Interrupt on falling edge*/
    INTERRUPT_RISING_EDGE = 1                           /*Interrupt on rising edge*/
}interrupt_INTx_edge;

/**
 * an enum for external interrupts for identifying which INTx caused the interrupt
 */
typedef enum
{
    INTERRUPT_EXTERNAL_INT0 = 0,                        /*INT0 interrupt is the source*/
    INTERRUPT_EXTERNAL_INT1 = 1,                        /*INT1 interrupt is the source*/
    INTERRUPT_EXTERNAL_INT2 = 2,                        /*INT2 interrupt is the source*/
}interrupt_INTx_src;

/**
 * struct interrupt_INTx_t - a struct for external interrupts
 * @EXT_interrupt_handler: A pointer to the interrupt handler of INTx
 * @mcu_pin: the mcu pin that caused the interrupt
 * @edge: falling or rising edge
 * @source: the source of the interrupt
 * @priority: the priority of the interrupt
 */
typedef struct
{
    INTERRUPT_HANDLER EXT_interrupt_handler;
    pin_config_t mcu_pin;
    interrupt_INTx_edge edge;
    interrupt_INTx_src source;
    interrupt_priority_cfg priority;
}interrupt_INTx_t;

/*----------------------------Function Prototypes-----------------------------*/

/**
 * @brief: A software interface to initialize the given external INTx interrupt
 * @param int_obj the external INTx interrupt object
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType Interrupt_INTx_Init(const interrupt_INTx_t *int_obj);
/**
 * @brief: A software interface to deinitialize the given external INTx interrupt
 * @param int_obj the external INTx interrupt object
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType Interrupt_INTx_Deinit(const interrupt_INTx_t *int_obj);


#endif	

#endif	/* MCAL_INTX_EXTERNAL_INTERRUPT_H */

