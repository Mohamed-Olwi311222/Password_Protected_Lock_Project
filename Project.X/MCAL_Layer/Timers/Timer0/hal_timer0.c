/* 
 * File:   hal_timer0.c
 * Author: Mohamed olwi
 *
 * Created on 12 September 2024, 00:28
 */
#include "hal_timer0.h"

/*---------------Static Data types----------------------------------------------*/
#if TIMER0_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
static INTERRUPT_HANDLER timer0_interrupt_handler = NULL; /* A pointer to the callback function when an interrupt is raised */
#endif
static uint16 preloaded_value = ZERO_INIT;
/*---------------Static Data types End------------------------------------------*/

/*---------------Static Helper functions declerations---------------------------*/
static inline Std_ReturnType timer0_configure_clock_src(const timer0_t *timer0_obj);
static inline Std_ReturnType timer0_configure_ext_src_edge(const timer0_t *timer0_ob);
static inline Std_ReturnType timer0_configure_resolution_bits(const timer0_t *timer0_obj);
#if TIMER0_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
static inline Std_ReturnType timer0_set_interrupt_handler(INTERRUPT_HANDLER Interrupt_Handler);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
static inline Std_ReturnType timer0_set_interrupt_priority(const timer0_t *timer0_obj);
#endif
#endif
/*---------------Static Helper functions declerations End-----------------------*/

/**
 * @brief: Initialize the timer0 peripheral object
 * @param timer0_obj the timer0 object to init
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer0_init(const timer0_t *timer0_obj)
{
    Std_ReturnType ret = E_OK;
 
    if (NULL == timer0_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Disable Timer0*/
        TIMER0_DISABLE_CONFIG();
        /* Configure the Clock source and the edge if external clock source*/
        ret |= timer0_configure_clock_src(timer0_obj);
        /* Configure the resolution of the timer*/
        ret |= timer0_configure_resolution_bits(timer0_obj);
        /* Configure the Prescaler*/
        if (_TIMER0_PRESCALER_ENABLE == timer0_obj->prescaler_enable)
        {
            TIMER0_PRESCALER_ENABLE_CONFIG();
            TIMER0_PRESCALER_SELECT_CONFIG(timer0_obj->prescaler_value);
        }
        /* Configure the preloaded value to store it in the timer0 registers*/
        ret |= timer0_write_value(timer0_obj, timer0_obj->timer0_preloaded_value);
        /* Store a copy of the preloaded value to always write it to the register while handling the overflow*/
        preloaded_value = timer0_obj->timer0_preloaded_value;
        /* Configure the interrupt if enabled and its priority too*/
#if TIMER0_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        /* Enable TIMER0 interrupt */
        TIMER0_INTERRUPT_ENABLE();
        ret |= timer0_set_interrupt_handler(timer0_obj->timer0_interrupt_handler);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        ret |= timer0_set_interrupt_priority(timer0_obj);
#endif
#endif
        /* Enable Timer0*/
        TIMER0_ENABLE_CONFIG();
    }
    return (ret);
}
/**
 * @brief: Deinitialize the timer0 peripheral object
 * @param timer0_obj the timer0 object to init
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer0_deinit(const timer0_t *timer0_obj)
{
    Std_ReturnType ret = E_OK;
 
    if (NULL == timer0_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
#if TIMER0_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        /* Clear interrupt flag*/
        TIMER0_INTERRUPT_FLAG_BIT_CLEAR();
        /* Disable TIMER0 interrupt */
        TIMER0_INTERRUPT_DISABLE();
#endif
        /* Disable TIMER0 peripheral*/
        TIMER0_DISABLE_CONFIG();
    }
    return (ret);     
}
/**
 * @brief: Write the provided value to timer0 counter/timer register
 * @param timer0_obj the timer0 object to write to its register
 * @param value the value to write to the timer0 counter/timer register
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType timer0_write_value(const timer0_t *timer0_obj, timer0_preload_value_t value)
{
    Std_ReturnType ret = E_OK;
 
    if (NULL == timer0_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Increment Value by 2 because the increment is inhibited for the following two instruction cycles*/
        value += 2;
        /* Write to TMR0H*/
        TMR0H = (uint8)(value >> 8);
        /* Write to TMR0L*/
        TMR0L = (uint8)value;
    }
    return (ret);   
}

/**
 * @brief: Read and store the value from timer0 counter/timer register into the provided address
 * @param timer0_obj the timer0 object to read from its register
 * @param value the address to store the value of the timer0 counter/timer register
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer0_read_value(const timer0_t *timer0_obj, timer0_preload_value_t *value)
{
    Std_ReturnType ret = E_OK;
 
    if ((NULL == timer0_obj) || (NULL == value))
    {
        ret = E_NOT_OK;
    }
    else
    {
        *value = TMR0L;
        *value += (uint16)(TMR0H << 8);
    }
    return (ret); 
}

/**
 * @brief A helper function to configure the clock source of the timer0
 * @param timer0_obj the timer0 peripheral object to configure its clock source
 * @return E_OK if success otherwise E_NOT_OK
 */
static inline Std_ReturnType timer0_configure_clock_src(const timer0_t *timer0_obj)
{
    Std_ReturnType ret = E_OK;
    
    switch (timer0_obj->clock_src)
    {
        case _TIMER0_INT_SRC:
            TIMER0_INT_SRC_CONFIG();
            break;
        case _TIMER0_EXT_SRC:
            TIMER0_EXT_SRC_CONFIG();
            ret |= timer0_configure_ext_src_edge(timer0_obj);
            break;
        default:
            ret = E_NOT_OK;
            break;
    }
    return (ret);
}
/**
 * @brief A helper function to configure the edge if the clock source is external
 * @param timer0_obj the timer0 peripheral object to configure its edge
 * @return E_OK if success otherwise E_NOT_OK
 */
static inline Std_ReturnType timer0_configure_ext_src_edge(const timer0_t *timer0_obj)
{
    Std_ReturnType ret = E_OK;
    
    switch (timer0_obj->edge)
    {
        case _TIMER0_FALLING_EDGE:
            TIMER0_FALLING_EDGE_CONFIG();
            break;
        case _TIMER0_RISING_EDGE:
            TIMER0_RISING_EDGE_CONFIG();
            break;
        default:
            ret = E_NOT_OK;
            break;
    }
    return (ret);  
}
/**
 * @brief A helper function to configure the resolution bits of the timer0 counter/timer register
 * @param timer0_obj the timer0 peripheral object to configure its register resolution 
 * @return E_OK if success otherwise E_NOT_OK
 */
static inline Std_ReturnType timer0_configure_resolution_bits(const timer0_t *timer0_obj)
{
    Std_ReturnType ret = E_OK;
    
    switch (timer0_obj->resolution_bits)
    {
        case _TIMER0_8bit_RES:
            TIMER0_8bit_RES_CONFIG();
            break;
        case _TIMER0_16bit_RES:
            TIMER0_16bit_RES_CONFIG();
            break;
        default:
            ret = E_NOT_OK;
            break;
    }
    return (ret); 
}

#if TIMER0_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/**
 * @brief: A helper function to set the interrupt handler of the timer0 overflow interrupt
 * @param Interrupt_Handler the interrupt handler to call when an interrupt is raised
 * @return E_OK if success otherwise E_NOT_OK
 */
static inline Std_ReturnType timer0_set_interrupt_handler(INTERRUPT_HANDLER Interrupt_Handler)
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
        timer0_interrupt_handler = Interrupt_Handler;
    }
    return (ret);
}
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/**
 * @brief: A helper function to set the priority of the timer0 overflow interrupt
 * @param timer0_obj the timer0 perihperal object used
 * @return E_OK if success otherwise E_NOT_OK
 */
static inline Std_ReturnType timer0_set_interrupt_priority(const timer0_t *timer0_obj)
{
    Std_ReturnType ret = E_OK;
   
    INTERRUPT_PRIORITY_levels_ENABLE();
    INTERRUPT_Global_interrupt_LOW_ENABLE();
    INTERRUPT_Global_interrupt_HIGH_ENABLE();
    if (INTERRUPT_HIGH_PRIORITY == timer0_obj->timer0_interrupt_priority)
    {
        TIMER0_HIGH_PRIORITY();
    }
    else
    {
        TIMER0_LOW_PRIORITY();
    }
    return (ret);   
}
#endif
/**
 * @brief the interrupt service routine of timer0 module, will be called if the overflow happened
 */
void TIMER0_ISR(void)
{
    TIMER0_INTERRUPT_FLAG_BIT_CLEAR();
    /* Write to TMR0H*/
    TMR0H = (uint8)(preloaded_value >> 8);
    /* Write to TMR0L*/
    TMR0L = (uint8)preloaded_value;
    if (NULL != timer0_interrupt_handler)
    {
        timer0_interrupt_handler();
    }
}
#endif
