/* 
 * File:   hal_timer2.h
 * Author: Mohamed olwi
 *
 * Created on 30 September 2024, 20:13
 */

#ifndef HAL_TIMER2_H
#define	HAL_TIMER2_H
/*----------------------------Header Files------------------------------------*/
#include "../../mcal_std_types.h"
#include <xc.h>
#include "../../Interrupt/mcal_internal_interrupt.h"
/*----------------------------Macros Declarations-----------------------------*/
/*----------------T2CON bits----------------*/
/*--------TMR2ON bit--------*/
#define _TIMER2_ENABLED                      1               /* Timer2 enabled */
#define _TIMER2_DISABLED                     0               /* Timer2 disabled */
/*----------------------------Macros Functions Declarations-------------------*/
/*----------------T2CON bits----------------*/
/*--------TMR2ON bit--------*/
/** 
 *  Enable Timer2 peripheral
 */
#define TIMER2_ENABLE_CONFIG()               (T2CONbits.TMR2ON = _TIMER2_ENABLED) 
/**
 *  Disable Timer2 peripheral
 */
#define TIMER2_DISABLE_CONFIG()              (T2CONbits.TMR2ON = _TIMER2_DISABLED)
/**
 * Configure TIMER2 prescaler value depending on _VAL @ref timer2_prescaler_select_t
 */
#define TIMER2_PRESCALER_SELECT_CONFIG(_VAL) (T2CONbits.T2CKPS = _VAL)
/**
 * Configure TIMER2 prescaler value depending on _VAL @ref timer2_postscaler_select_t
 */
#define TIMER2_POSTSCALER_SELECT_CONFIG(_VAL) (T2CONbits.TOUTPS = _VAL)
/*----------------------------DataTypes---------------------------------------*/

/**
 * An enum for timer2 to select postscaler value from it
 */
typedef enum 
{
    TIMER2_POSTSCALER_DIV_BY_1,
    TIMER2_POSTSCALER_DIV_BY_2,
    TIMER2_POSTSCALER_DIV_BY_3,
    TIMER2_POSTSCALER_DIV_BY_4,
    TIMER2_POSTSCALER_DIV_BY_5,
    TIMER2_POSTSCALER_DIV_BY_6,
    TIMER2_POSTSCALER_DIV_BY_7,
    TIMER2_POSTSCALER_DIV_BY_8,
    TIMER2_POSTSCALERR_DIV_BY_9,
    TIMER2_POSTSCALERR_DIV_BY_10,
    TIMER2_POSTSCALERR_DIV_BY_11,
    TIMER2_POSTSCALERR_DIV_BY_12,
    TIMER2_POSTSCALERR_DIV_BY_13,
    TIMER2_POSTSCALERR_DIV_BY_14,
    TIMER2_POSTSCALERR_DIV_BY_15,
    TIMER2_POSTSCALERR_DIV_BY_16
} timer2_postscaler_select_t;

/**
 * An enum for timer2 to select prescaler value from it
 */
typedef enum 
{
    TIMER2_PRESCALER_DIV_BY_1,
    TIMER2_PRESCALER_DIV_BY_4,
    TIMER2_PRESCALER_DIV_BY_16
} timer2_prescaler_select_t;

typedef uint8 timer2_TMR2_preload_value_t;
typedef uint8 timer2_PR2_preload_value_t;

/**
 * struct timer2_t - a struct for timer2 peripheral
 * @timer2_preloaded_value: The value to store inside timer2 8-bit TMR2 register
 * @PR2_preloaded_value: The value to store inside timer2 8-bit PR2 register
 * @timer2_interrupt_handler: A pointer to the interrupt handler of timer2
 * @timer2_interrupt_priority: the priority of the interrupt
 * @postscaler_value: The value of postscaler
 * @prescaler_value: The value of prescaler
 */
typedef struct
{
    timer2_TMR2_preload_value_t TMR2_preloaded_value;
    timer2_PR2_preload_value_t PR2_preloaded_value;
#if TIMER2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_HANDLER timer2_interrupt_handler;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    interrupt_priority_cfg timer2_interrupt_priority;
#endif
#endif
    timer2_postscaler_select_t postscaler_value;
    timer2_prescaler_select_t prescaler_value;
} timer2_t;
/*----------------------------Function Prototypes-----------------------------*/

/**
 * @brief: Initialize the timer2 peripheral object
 * @param timer2_obj the timer2 object to init
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer2_init(const timer2_t *timer2_obj);
/**
 * @brief: Write the provided value to timer2 TMR2 register
 * @param timer2_obj the timer2 object to write to its TMR2 register
 * @param value the value to write to the timer2 TMR2 register
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType timer2_TMR2_write_value(const timer2_t *timer2_obj, timer2_TMR2_preload_value_t value);
/**
 * @brief: Read and store the value from timer2 TMR2 register into the provided address
 * @param timer2_obj the timer2 object to read from its register
 * @param value the address to store the value of the timer2 TMR2 register
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer2_TMR2_read_value(const timer2_t *timer2_obj, timer2_TMR2_preload_value_t *value);
/**
 * @brief: Write the provided value to timer2 PR2 register
 * @param timer2_obj the timer2 object to write to its PR2 register
 * @param value the value to write to the timer2 PR2 register
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType timer2_PR2_write_value(const timer2_t *timer2_obj, timer2_PR2_preload_value_t value);
/**
 * @brief: Read and store the value from timer2 PR2 register into the provided address
 * @param timer2_obj the timer2 object to read from its register
 * @param value the address to store the value of the timer2 PR2 register
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer2_PR2_read_value(const timer2_t *timer2_obj, timer2_PR2_preload_value_t *value);
/**
 * @brief: Deinitialize the timer2 peripheral object
 * @param timer2_obj the timer2 object to init
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer2_deinit(const timer2_t *timer2_obj);
#endif	/* HAL_TIMER2_H */

