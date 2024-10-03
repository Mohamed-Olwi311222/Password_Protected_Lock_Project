/* 
 * File:   hal_timer1.h
 * Author: Mohamed olwi
 *
 * Created on 20 September 2024, 04:02
 */

#ifndef HAL_TIMER1_H
#define	HAL_TIMER1_H
/*----------------------------Header Files------------------------------------*/
#include "../../mcal_std_types.h"
#include <xc.h>
#include "../../Interrupt/mcal_internal_interrupt.h"
/*----------------------------Macros Declarations-----------------------------*/
/*----------------T1CON bits----------------*/
/*--------TMR1ON bit--------*/
#define _TIMER1_ENABLED                      1               /* Timer1 enabled */
#define _TIMER1_DISABLED                     0               /* Timer1 disabled */

/*--------TMR1CS bit--------*/
#define _TIMER1_EXT_SRC                      1               /* Timer1 transition on pin RC0/T1OSO/T13CKI (on the rising edge) */
#define _TIMER1_INT_SRC                      0               /* Timer1 Internal instruction cycle clock (CLKO) */

/*--------T1SYNC bit--------*/
#define _TIMER1_SYNC                         1               /* Timer1 counter Sync with external clock source depending on TMR1CS bit */
#define _TIMER1_ASYNC                        0               /* Timer1 counter Async with external clock source depending on TMR1CS bit */

/*--------T1OSCEN bit-------*/
#define _TIMER1_OSC_ENABLE                   1               /* Timer1 oscillator enabled */
#define _TIMER1_OSC_DISABLE                  0               /* Timer1 oscillator disabled */

/*--------T1CKPS1:T1CKPS0 bits-*/
#define _TIMER1_PRESCALER_DISABLE            0               /* Timer1 prescaler is disabled */

/*--------RD16 bit----------*/
#define _TIMER1_RW_8bit_OP                   0               /* TIMER1 resolution bits as 2 8-bit operations */
#define _TIMER1_RW_16bit_OP                  1               /* TIMER1 resolution bits as 1 16-bit operation */
/*----------------------------Macros Functions Declarations-------------------*/
/*----------------T1CON bits----------------*/
/*--------TMR1ON bit--------*/
/** 
 *  Enable Timer1 peripheral
 */
#define TIMER1_ENABLE_CONFIG()               (T1CONbits.TMR1ON = _TIMER1_ENABLED) 
/**
 *  Disable Timer1 peripheral
 */
#define TIMER1_DISABLE_CONFIG()              (T1CONbits.TMR1ON = _TIMER1_DISABLED)

/*--------TMR1CS bit--------*/
/**
 * Configure Timer1 clock source as transition on pin RC0/T1OSO/T13CKI (on the rising edge)
 */
#define TIMER1_EXT_SRC_CONFIG()              (T1CONbits.TMR1CS = _TIMER1_EXT_SRC)              
/**
 * Configure Timer1 clock source as Internal instruction cycle clock (CLKO)
 */
#define TIMER1_INT_SRC_CONFIG()              (T1CONbits.TMR1CS = _TIMER1_INT_SRC)

/*--------T1SYNC bit--------*/
/**
 * Configure Timer1 counter to sync with External Clock Input depending on TMR1CS bit
 * If TMR1CS is 0 ignore it
 */
#define TIMER1_SYNC_WITH_EXT_CLK()           (T1CONbits.T1SYNC = _TIMER1_SYNC)
/**
 * Configure Timer1 counter to UNsync with External Clock Input depending on TMR1CS bit
 * If TMR1CS is 0 ignore it
 */
#define TIMER1_ASYNC_WITH_EXT_CLK()          (T1CONbits.T1SYNC = _TIMER1_ASYNC)

/*--------T1OSCEN bit-------*/
/**
 * Configure Timer1 oscillator to be enabled
 */
#define TIMER1_OSC_ENABLE()                  (T1CONbits.T1OSCEN = _TIMER1_OSC_ENABLE)
/**
 * Configure Timer1 oscillator to be disabled
 */
#define TIMER1_OSC_DISABLE()                 (T1CONbits.T1OSCEN = _TIMER1_OSC_DISABLE)

/*--------T1CKPS1:T1CKPS0 bits-*/
/**
 * Configure TIMER1 prescaler to be not assigned
 */
#define TIMER1_PRESCALER_DISABLE_CONFIG()    (T1CONbits.T1CKPS = _TIMER1_PRESCALER_DISABLE)
/**
 * Configure TIMER1 prescaler value depending on _VAL @ref timer1_prescaler_select_t
 */
#define TIMER1_PRESCALER_SELECT_CONFIG(_VAL) (T1CONbits.T1CKPS = _VAL)

/*--------T1RUN bit---------*/
/**
 * Get the status of timer1 clock
 * if 1 => Device clock is derived from Timer1 oscillator
 * if 0 => Device clock is derived from another source
 */
#define TIMER1_GET_CLK_STATUS()              (T1CONbits.T1RUN)

/*--------RD16 bit----------*/
/**
 * Configure TIMER1 resolution bits as 2 8-bit operations
 */
#define TIMER1_RW_8bit_RES_CONFIG()          (T1CONbits.T1RD16 = _TIMER1_RW_8bit_OP)
/**
 * Configure TIMER1 resolution bits as 1 16-bit operation
 */
#define TIMER1_RW_16bit_RES_CONFIG()         (T1CONbits.T1RD16 = _TIMER1_RW_16bit_OP)
/*----------------------------DataTypes---------------------------------------*/
/**
 * An enum for timer1 to select prescaler value from it
 */
typedef enum 
{
    TIMER1_PRESCALER_DISABLE,
    TIMER1_PRESCALER_DIV_BY_2,
    TIMER1_PRESCALER_DIV_BY_4,
    TIMER1_PRESCALER_DIV_BY_8
} timer1_prescaler_select_t;

typedef uint8 timer1_resolution_t;
typedef uint8 timer1_clk_src_t;
typedef uint8 timer1_ext_clk_sync_t;
typedef uint8 timer1_rw_mode_t;
typedef uint8 timer1_ext_osc_t;
typedef uint16 timer1_preload_value_t;
/**
 * struct timer1_t - a struct for timer1 peripheral
 * @timer1_preloaded_value: The value to store inside timer1 timer/counter register
 * @timer1_interrupt_handler: A pointer to the interrupt handler of timer1
 * @timer1_interrupt_priority: the priority of the interrupt
 * @prescaler_value: The value of prescaler if the prescaler assignment bit is set 
 * @clock_src: the source of the clock source (internal or external source)
 * @ext_clk_sync: A bit to decide to sync or not sync with external clock
 * @ext_osc_enable: A bit to decide to enable or disable the external oscillator
 * @rw_mode: A bit to decide to use one 16-bit register to read/write or 2 8-bit registers
 */
typedef struct
{
    timer1_preload_value_t timer1_preloaded_value;
#if TIMER1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_HANDLER timer1_interrupt_handler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    interrupt_priority_cfg timer1_interrupt_priority;
#endif
#endif
    timer1_prescaler_select_t prescaler_value;
    timer1_clk_src_t clock_src : 1;
    timer1_ext_clk_sync_t ext_clk_sync : 1; 
    timer1_ext_osc_t ext_osc_enable : 1;
    timer1_rw_mode_t rw_mode : 1;
    uint8 __RESERVED : 4;
} timer1_t;
/*----------------------------Function Prototypes-----------------------------*/
/**
 * @brief: Initialize the timer1 peripheral object
 * @param timer1_obj the timer1 object to init
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer1_init(const timer1_t *timer1_obj);
/**
 * @brief: Write the provided value to timer1 counter/timer register
 * @param timer1_obj the timer1 object to write to its register
 * @param value the value to write to the timer1 counter/timer register
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType timer1_write_value(const timer1_t *timer1_obj, timer1_preload_value_t value);
/**
 * @brief: Read and store the value from timer1 counter/timer register into the provided address
 * @param timer1_obj the timer1 object to read from its register
 * @param value the address to store the value of the timer1 counter/timer register
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer1_read_value(const timer1_t *timer1_obj, timer1_preload_value_t *value);
/**
 * @brief: Deinitialize the timer1 peripheral object
 * @param timer1_obj the timer1 object to deinit
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer1_deinit(const timer1_t *timer1_obj);
#endif	/* HAL_TIMER1_H */

