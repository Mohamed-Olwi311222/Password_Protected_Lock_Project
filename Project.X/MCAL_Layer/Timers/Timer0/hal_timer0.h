/* 
 * File:   hal_timer0.h
 * Author: Mohamed olwi
 *
 * Created on 12 September 2024, 00:28
 */

#ifndef HAL_TIMER0_H
#define	HAL_TIMER0_H
/*----------------------------Header Files------------------------------------*/
#include "../../mcal_std_types.h"
#include <xc.h>
#include "../../Interrupt/mcal_internal_interrupt.h"
/*----------------------------Macros Declarations-----------------------------*/
/*----------------T0CON bits----------------*/
/*--------TMR0ON bit--------*/
#define _TIMER0_ENABLED                      1               /* Timer0 enabled */
#define _TIMER0_DISABLED                     0               /* Timer0 disabled */

/*--------T08BIT bit--------*/
#define _TIMER0_8bit_RES                     1               /* Timer0 8-bit resolution */
#define _TIMER0_16bit_RES                    0               /* Timer0 16-bit resolution */

/*--------T0CS bit----------*/
#define _TIMER0_EXT_SRC                      1               /* Timer0 transition on T0CKL pin */
#define _TIMER0_INT_SRC                      0               /* Timer0 Internal instruction cycle clock (CLKO) */

/*--------T0SE bit----------*/
#define _TIMER0_FALLING_EDGE                 1               /* Timer0 increment on falling edge */
#define _TIMER0_RISING_EDGE                  0               /* Timer0 increment on rising edge */

/*--------PSA bit----------*/
#define _TIMER0_PRESCALER_ENABLE             0               /* Timer0 prescaler enable */
#define _TIMER0_PRESCALER_DISABLE            1               /* Timer0 prescaler disable */
/*----------------------------Macros Functions Declarations-------------------*/
/*----------------T0CON bits----------------*/
/*--------TMR0ON bit--------*/
/** 
 *  Enable Timer0 peripheral
 */
#define TIMER0_ENABLE_CONFIG()               (T0CONbits.TMR0ON = _TIMER0_ENABLED) 
/**
 *  Disable Timer0 peripheral
 */
#define TIMER0_DISABLE_CONFIG()              (T0CONbits.TMR0ON = _TIMER0_DISABLED)

/*--------T08BIT bit--------*/
/**
 * Configure TIMER0 resolution bits as 8-bit resolution
 */
#define TIMER0_8bit_RES_CONFIG()             (T0CONbits.T08BIT = _TIMER0_8bit_RES)
/**
 * Configure TIMER0 resolution bits as 16-bit resolution
 */
#define TIMER0_16bit_RES_CONFIG()            (T0CONbits.T08BIT = _TIMER0_16bit_RES)

/*--------T0CS bit----------*/
/**
 * Configure Timer0 clock source as transition on T0CKL pin
 */
#define TIMER0_EXT_SRC_CONFIG()              (T0CONbits.T0CS = _TIMER0_EXT_SRC)              
/**
 * Configure Timer0 clock soure as Internal instruction cycle clock (CLKO)
 */
#define TIMER0_INT_SRC_CONFIG()              (T0CONbits.T0CS = _TIMER0_INT_SRC)              

/*--------T0SE bit----------*/
/**
 * Configure Timer0 to increment on falling edge
 */
#define TIMER0_FALLING_EDGE_CONFIG()         (T0CONbits.T0SE = _TIMER0_FALLING_EDGE)         
/**
 * Configure Timer0 to increment on rising edge
 */
#define TIMER0_RISING_EDGE_CONFIG()          (T0CONbits.T0SE = _TIMER0_RISING_EDGE)

/*--------PSA bit----------*/
/**
 * Configure TIMER0 prescaler to be assigned
 */
#define TIMER0_PRESCALER_ENABLE_CONFIG()     (T0CONbits.PSA = _TIMER0_PRESCALER_ENABLE)
/**
 * Configure TIMER0 prescaler to be not assigned
 */
#define TIMER0_PRESCALER_DISABLE_CONFIG()    (T0CONbits.PSA = _TIMER0_PRESCALER_DISABLE)

/*--------T0PS2:T0PS0 bits-*/
/**
 * Configure TIMER0 prescaler value depending on _VAL @ref timer0_prescaler_select_t
 */
#define TIMER0_PRESCALER_SELECT_CONFIG(_VAL) (T0CONbits.T0PS = _VAL)
/*----------------------------DataTypes---------------------------------------*/
/**
 * An enum for timer0 to select prescaler value from it
 */
typedef enum 
{
    TIMER0_PRESCALER_DIV_BY_2,
    TIMER0_PRESCALER_DIV_BY_4,
    TIMER0_PRESCALER_DIV_BY_8,
    TIMER0_PRESCALER_DIV_BY_16,
    TIMER0_PRESCALER_DIV_BY_32,
    TIMER0_PRESCALER_DIV_BY_64,
    TIMER0_PRESCALER_DIV_BY_128,
    TIMER0_PRESCALER_DIV_BY_256
} timer0_prescaler_select_t;

typedef uint8 timer0_resolution_t;
typedef uint8 timer0_clk_src_t;
typedef uint8 timer0_prescaler_enable_t;
typedef uint8 timer0_edge_select_t;
typedef uint16 timer0_preload_value_t;
/**
 * struct timer0_t - a struct for timer0 peripheral
 * @timer0_preloaded_value: The value to store inside timer0 timer/counter register
 * @timer0_interrupt_handler: A pointer to the interrupt handler of timer0
 * @timer0_interrupt_priority: the priority of the interrupt
 * @prescaler_value: The value of prescaler if the prescaler assignment bit is set 
 * @prescaler_enable: Enable/Disable prescaler assignment bit
 * @resolution_bits: The resolution of the timer0 timer/counter register
 * @clock_src: the source of the clock source (internal or external source)
 * @edge: falling or rising edge if external source is selected
 */
typedef struct
{
    timer0_preload_value_t timer0_preloaded_value;
#if TIMER0_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_HANDLER timer0_interrupt_handler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    interrupt_priority_cfg timer0_interrupt_priority;
#endif
#endif
    timer0_prescaler_select_t prescaler_value;
    timer0_prescaler_enable_t prescaler_enable : 1;
    timer0_resolution_t resolution_bits : 1;
    timer0_clk_src_t clock_src : 1;
    timer0_edge_select_t edge : 1;
    uint8 __RESERVED : 4;
} timer0_t;
/*----------------------------Function Prototypes-----------------------------*/
/**
 * @brief: Initialize the timer0 peripheral object
 * @param timer0_obj the timer0 object to init
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer0_init(const timer0_t *timer0_obj);
/**
 * @brief: Write the provided value to timer0 counter/timer register
 * @param timer0_obj the timer0 object to write to its register
 * @param value the value to write to the timer0 counter/timer register
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType timer0_write_value(const timer0_t *timer0_obj, timer0_preload_value_t value);
/**
 * @brief: Deinitialize the timer0 peripheral object
 * @param timer0_obj the timer0 object to init
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer0_deinit(const timer0_t *timer0_obj);
#endif	/* HAL_TIMER0_H */

