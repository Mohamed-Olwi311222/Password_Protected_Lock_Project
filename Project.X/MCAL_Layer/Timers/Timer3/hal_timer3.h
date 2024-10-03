/* 
 * File:   hal_timer3.h
 * Author: Mohamed olwi
 *
 * Created on 02 October 2024, 02:52
 */

#ifndef HAL_TIMER3_H
#define	HAL_TIMER3_H
/*----------------------------Header Files------------------------------------*/
#include "../../mcal_std_types.h"
#include <xc.h>
#include "../../Interrupt/mcal_internal_interrupt.h"
/*----------------------------Macros Declarations-----------------------------*/
/*----------------T3CON bits----------------*/
/*--------TMR3ON bit--------*/
#define _TIMER3_ENABLED                      1               /* Timer3 enabled */
#define _TIMER3_DISABLED                     0               /* Timer3 disabled */

/*--------TMR3CS bit--------*/
#define _TIMER3_EXT_SRC                      1               /* External clock input from Timer1 oscillator or T13CKI (on the rising edge after the first falling edge) */
#define _TIMER3_INT_SRC                      0               /* Timer3 Internal instruction cycle clock (CLKO) */

/*--------T3SYNC bit--------*/
#define _TIMER3_SYNC                         1               /* Timer3 counter Sync with external clock source depending on TMR3CS bit */
#define _TIMER3_ASYNC                        0               /* Timer3 counter Async with external clock source depending on TMR3CS bit */

/*--------RD16 bit----------*/
#define _TIMER3_RW_8bit_OP                   0               /* TIMER3 resolution bits as 2 8-bit operations */
#define _TIMER3_RW_16bit_OP                  1               /* TIMER3 resolution bits as 1 16-bit operation */
/*----------------------------Macros Functions Declarations-------------------*/
/*----------------T3CON bits----------------*/
/*--------TMR3ON bit--------*/
/** 
 *  Enable Timer3 peripheral
 */
#define TIMER3_ENABLE_CONFIG()               (T3CONbits.TMR3ON = _TIMER3_ENABLED) 
/**
 *  Disable Timer3 peripheral
 */
#define TIMER3_DISABLE_CONFIG()              (T3CONbits.TMR3ON = _TIMER3_DISABLED)

/*--------TMR3CS bit--------*/
/**
 * Configure External clock input from TIMER3 oscillator or T13CKI (on the rising edge after the first falling edge)
 */
#define TIMER3_EXT_SRC_CONFIG()              (T3CONbits.TMR3CS = _TIMER3_EXT_SRC)              
/**
 * Configure Timer3 clock source as Internal instruction cycle clock (CLKO)
 */
#define TIMER3_INT_SRC_CONFIG()              (T3CONbits.TMR3CS = _TIMER3_INT_SRC)

/*--------T3SYNC bit--------*/
/**
 * Configure Timer3 counter to sync with External Clock Input depending on TMR3CS bit
 * If TMR3CS is 0 ignore it
 */
#define TIMER3_SYNC_WITH_EXT_CLK()           (T3CONbits.T3SYNC = _TIMER3_SYNC)
/**
 * Configure Timer3 counter to UNsync with External Clock Input depending on TMR3CS bit
 * If TMR3CS is 0 ignore it
 */
#define TIMER3_ASYNC_WITH_EXT_CLK()          (T3CONbits.T3SYNC = _TIMER3_ASYNC)

/*--------T3CKPS1:T3CKPS0 bits-*/
/**
 * Configure TIMER3 prescaler value depending on _VAL @ref timer3_prescaler_select_t
 */
#define TIMER3_PRESCALER_SELECT_CONFIG(_VAL) (T3CONbits.T3CKPS = _VAL)

/*--------RD16 bit----------*/
/**
 * Configure TIMER3 resolution bits as 2 8-bit operations
 */
#define TIMER3_RW_8bit_RES_CONFIG()          (T3CONbits.T3RD16 = _TIMER3_RW_8bit_OP)
/**
 * Configure TIMER3 resolution bits as 1 16-bit operation
 */
#define TIMER3_RW_16bit_RES_CONFIG()         (T3CONbits.T3RD16 = _TIMER3_RW_16bit_OP)
/*----------------------------DataTypes---------------------------------------*/
/**
 * An enum for timer3 to select prescaler value from it
 */
typedef enum 
{
    TIMER3_PRESCALER_DIV_BY_1,
    TIMER3_PRESCALER_DIV_BY_2,
    TIMER3_PRESCALER_DIV_BY_4,
    TIMER3_PRESCALER_DIV_BY_8
} timer3_prescaler_select_t;

typedef uint8 timer3_resolution_t;
typedef uint8 timer3_clk_src_t;
typedef uint8 timer3_ext_clk_sync_t;
typedef uint8 timer3_rw_mode_t;
typedef uint16 timer3_preload_value_t;
/**
 * struct timer3_t - a struct for timer3 peripheral
 * @timer3_preloaded_value: The value to store inside timer3 timer/counter register
 * @timer3_interrupt_handler: A pointer to the interrupt handler of timer3
 * @timer3_interrupt_priority: the priority of the interrupt
 * @prescaler_value: The value of prescaler if the prescaler assignment bit is set 
 * @clock_src: the source of the clock source (internal or external source)
 * @ext_clk_sync: A bit to decide to sync or not sync with external clock
 * @rw_mode: A bit to decide to use one 16-bit register to read/write or 2 8-bit registers
 */
typedef struct
{
    timer3_preload_value_t timer3_preloaded_value;
#if TIMER3_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_HANDLER timer3_interrupt_handler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    interrupt_priority_cfg timer3_interrupt_priority;
#endif
#endif
    timer3_prescaler_select_t prescaler_value;
    timer3_clk_src_t clock_src : 1;
    timer3_ext_clk_sync_t ext_clk_sync : 1; 
    timer3_rw_mode_t rw_mode : 1;
    uint8 __RESERVED : 5;
} timer3_t;
/*----------------------------Function Prototypes-----------------------------*/
/**
 * @brief: Initialize the timer3 peripheral object
 * @param timer3_obj the timer3 object to init
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer3_init(const timer3_t *timer3_obj);
/**
 * @brief: Write the provided value to timer3 counter/timer register
 * @param timer3_obj the timer3 object to write to its register
 * @param value the value to write to the timer3 counter/timer register
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType timer3_write_value(const timer3_t *timer3_obj, timer3_preload_value_t value);
/**
 * @brief: Read and store the value from timer3 counter/timer register into the provided address
 * @param timer3_obj the timer3 object to read from its register
 * @param value the address to store the value of the timer3 counter/timer register
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer3_read_value(const timer3_t *timer3_obj, timer3_preload_value_t *value);
/**
 * @brief: Deinitialize the timer3 peripheral object
 * @param timer3_obj the timer3 object to deinit
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer3_deinit(const timer3_t *timer3_obj);
#endif	/* HAL_TIMER3_H */

