/* 
 * File:   hal_timer1.c
 * Author: Mohamed olwi
 *
 * Created on 20 September 2024, 04:02
 */
#include "hal_timer1.h"

/*---------------Static Data types----------------------------------------------*/
#if TIMER1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
static INTERRUPT_HANDLER timer1_interrupt_handler = NULL; /* A pointer to the callback function when an interrupt is raised */
#endif
static uint16 preloaded_value = ZERO_INIT;
/*---------------Static Data types End------------------------------------------*/

/*---------------Static Helper functions declerations---------------------------*/
static inline void timer1_configure_clock_src(const timer1_t *timer1_obj);
static inline void timer1_configure_rw_mode(const timer1_t *timer1_obj);
static inline void timer1_configure_ext_clk_sync(const timer1_t *timer1_obj);
static inline void timer1_configure_timer1_oscillator(const timer1_t *timer1_obj);
#if TIMER1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
static inline Std_ReturnType timer1_set_interrupt_handler(INTERRUPT_HANDLER Interrupt_Handler);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
static inline void timer1_set_interrupt_priority(const timer1_t *timer1_obj);
#endif
#endif
/*---------------Static Helper functions declerations End-----------------------*/

/**
 * @brief: Initialize the timer1 peripheral object
 * @param timer1_obj the timer1 object to init
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer1_init(const timer1_t *timer1_obj)
{
    Std_ReturnType ret = E_OK;
 
    if (NULL == timer1_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Disable Timer1*/
        TIMER1_DISABLE_CONFIG();
        /* Configure the Clock source */
        timer1_configure_clock_src(timer1_obj);
        /* Configure the timer1 oscillator */
        timer1_configure_timer1_oscillator(timer1_obj);
        /* Configure the 16-Bit Read/Write Mode of the timer */
        timer1_configure_rw_mode(timer1_obj);
        /* Configure the Prescaler */
        if (!(TIMER1_PRESCALER_DISABLE == timer1_obj->prescaler_value))
        {
            TIMER1_PRESCALER_SELECT_CONFIG(timer1_obj->prescaler_value);
        }
        /* Configure the preloaded value to store it in the timer1 registers */
        ret |= timer1_write_value(timer1_obj, timer1_obj->timer1_preloaded_value);
        /* Store a copy of the preloaded value to always write it to the register while handling the overflow */
        preloaded_value = timer1_obj->timer1_preloaded_value;
        /* Configure the interrupt if enabled and its priority too */
#if TIMER1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        /* Enable TIMER1 interrupt */
        TIMER1_INTERRUPT_ENABLE();
        ret |= timer1_set_interrupt_handler(timer1_obj->timer1_interrupt_handler);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        timer1_set_interrupt_priority(timer1_obj);
#endif
#endif
        /* Enable Timer1 */
        TIMER1_ENABLE_CONFIG();
    }
    return (ret);
}
/**
 * @brief: Write the provided value to timer1 counter/timer register
 * @param timer1_obj the timer1 object to write to its register
 * @param value the value to write to the timer1 counter/timer register
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType timer1_write_value(const timer1_t *timer1_obj, timer1_preload_value_t value)
{
    Std_ReturnType ret = E_OK;
 
    if (NULL == timer1_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if (_TIMER1_RW_8bit_OP == timer1_obj->rw_mode)
        {
            TIMER1_DISABLE_CONFIG();
            /* Write to TMR1H*/
            TMR1H = (uint8)(value >> 8);
            /* Write to TMR1L*/
            TMR1L = (uint8)value;
            TIMER1_ENABLE_CONFIG();
        }
        else
        {
            /* Write to TMR1H*/
            TMR1H = (uint8)(value >> 8);
            /* Write to TMR1L*/
            TMR1L = (uint8)value;
        }
    }
    return (ret);   
}
/**
 * @brief: A helper function to configure the timer1 oscillator
 * @param timer1_obj the timer1 peripheral object to configure the timer1 oscillator
 */
static inline void timer1_configure_timer1_oscillator(const timer1_t *timer1_obj)
{
    /* Check if the timer1 oscillator is enabled */
    if (_TIMER1_OSC_ENABLE == timer1_obj->ext_osc_enable)
    {
        TIMER1_OSC_ENABLE();
        __delay_ms(500);
    }
    else
    {
        TIMER1_OSC_DISABLE();
    }
}
/**
 * @brief: A helper function to configure the clock source of the timer1
 * @param timer1_obj the timer1 peripheral object to configure its clock source
 */
static inline void timer1_configure_clock_src(const timer1_t *timer1_obj)
{
    /* Check if the timer1 works as a counter mode (external src)
       or timer mode (internal src) */
    if (_TIMER1_INT_SRC == timer1_obj->clock_src)
    {
        TIMER1_INT_SRC_CONFIG();
    }
    else
    {
        /* Configure whether to sync or not with the external clock input*/
        timer1_configure_ext_clk_sync(timer1_obj);
        TIMER1_EXT_SRC_CONFIG();
    }
}
/**
 * @brief: A helper function to configure the read/write mode of the timer1 to be either
 *         two 8-bit registers or one 16-bit register
 * @param timer1_obj the timer1 peripheral object to configure its read/write mode
 */
static inline void timer1_configure_rw_mode(const timer1_t *timer1_obj)
{
    switch (timer1_obj->rw_mode)
    {
        case _TIMER1_RW_16bit_OP:
            TIMER1_RW_16bit_RES_CONFIG();
            break;
        case _TIMER1_RW_8bit_OP:
            TIMER1_RW_8bit_RES_CONFIG();
            break;
        default:
            break;
    }
}
/**
 * @brief: A helper function to configure whether to sync or not with the external clock source
 * @param timer1_obj the timer 1 peripheral object to configure the external clock synchronization
 */
static inline void timer1_configure_ext_clk_sync(const timer1_t *timer1_obj)
{
    switch (timer1_obj->ext_clk_sync)
    {
        case _TIMER1_SYNC:
            TIMER1_SYNC_WITH_EXT_CLK();
            break;
        case _TIMER1_ASYNC:
            TIMER1_ASYNC_WITH_EXT_CLK();
            break;
        default:
            break;
    }
}
#if TIMER1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/**
 * @brief: A helper function to set the interrupt handler of the timer1 overflow interrupt
 * @param Interrupt_Handler the interrupt handler to call when an interrupt is raised
 * @return E_OK if success otherwise E_NOT_OK
 */
static inline Std_ReturnType timer1_set_interrupt_handler(INTERRUPT_HANDLER Interrupt_Handler)
{
    Std_ReturnType ret = E_OK;
   
    /* If the interrupt priority is disabled then enable the peripheral interrupt
    and global interrupts*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_DISABLE
        INTERRUPT_Peripheral_interrupt_ENABLE();
        INTERRUPT_Global_interrupt_ENABLE();
#endif
    if (NULL == Interrupt_Handler)
    {
        ret = E_NOT_OK;
    }
    else
    {
        timer1_interrupt_handler = Interrupt_Handler;
    }
    return (ret);
}
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/**
 * @brief: A helper function to set the priority of the timer1 overflow interrupt
 * @param timer1_obj the timer1 perihperal object used
 */
static inline void timer1_set_interrupt_priority(const timer1_t *timer1_obj)
{
    INTERRUPT_Global_interrupt_LOW_ENABLE();
    if (INTERRUPT_HIGH_PRIORITY == timer1_obj->timer1_interrupt_priority)
    {
        TIMER1_HIGH_PRIORITY();
    }
    else
    {
        TIMER1_LOW_PRIORITY();
    }
}
#endif
/**
 * @brief the interrupt service routine of timer1 module, will be called if the overflow happened
 */
void TIMER1_ISR(void)
{
    TIMER1_INTERRUPT_FLAG_BIT_CLEAR();
    if (NULL != timer1_interrupt_handler)
    {
        timer1_interrupt_handler();
    }
}
#endif
