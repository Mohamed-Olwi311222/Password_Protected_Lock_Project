/* 
 * File:   hal_timer3.h
 * Author: Mohamed olwi
 *
 * Created on 02 October 2024, 02:52
 */
#include "hal_timer3.h"
/*---------------Static Data types----------------------------------------------*/
#if TIMER3_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
static INTERRUPT_HANDLER timer3_interrupt_handler = NULL; /* A pointer to the callback function when an interrupt is raised */
#endif
static uint16 preloaded_value = ZERO_INIT;
/*---------------Static Data types End------------------------------------------*/

/*---------------Static Helper functions declerations---------------------------*/
static inline void timer3_configure_clock_src(const timer3_t *timer3_obj);
static inline void timer3_configure_rw_mode(const timer3_t *timer3_obj);
static inline void timer3_configure_ext_clk_sync(const timer3_t *timer3_obj);
#if TIMER3_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
static inline Std_ReturnType timer3_set_interrupt_handler(INTERRUPT_HANDLER Interrupt_Handler);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
static inline void timer3_set_interrupt_priority(const timer3_t *timer3_obj);
#endif
#endif
/*---------------Static Helper functions declerations End-----------------------*/

/**
 * @brief: Initialize the timer3 peripheral object
 * @param timer3_obj the timer3 object to init
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer3_init(const timer3_t *timer3_obj)
{
    Std_ReturnType ret = E_OK;
 
    if (NULL == timer3_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Disable Timer3*/
        TIMER3_DISABLE_CONFIG();
        /* Configure the Clock source */
        timer3_configure_clock_src(timer3_obj);
        /* Configure the 16-Bit Read/Write Mode of the timer */
        timer3_configure_rw_mode(timer3_obj);
        /* Configure the Prescaler */
        TIMER3_PRESCALER_SELECT_CONFIG(timer3_obj->prescaler_value);
        /* Configure the preloaded value to store it in the timer3 registers */
        ret |= timer3_write_value(timer3_obj, timer3_obj->timer3_preloaded_value);
        /* Store a copy of the preloaded value to always write it to the register while handling the overflow */
        preloaded_value = timer3_obj->timer3_preloaded_value;
        /* Configure the interrupt if enabled and its priority too */
#if TIMER3_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        /* Enable TIMER3 interrupt */
        TIMER3_INTERRUPT_ENABLE();
        ret |= timer3_set_interrupt_handler(timer3_obj->timer3_interrupt_handler);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        timer3_set_interrupt_priority(timer3_obj);
#endif
#endif
        /* Enable Timer3 */
        TIMER3_ENABLE_CONFIG();
    }
    return (ret);
}
/**
 * @brief: Deinitialize the timer3 peripheral object
 * @param timer3_obj the timer3 object to deinit
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer3_deinit(const timer3_t *timer3_obj)
{
    Std_ReturnType ret = E_OK;
 
    if (NULL == timer3_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
#if TIMER3_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        /* Clear interrupt flag*/
        TIMER3_INTERRUPT_FLAG_BIT_CLEAR();
        /* Disable TIMER3 interrupt */
        TIMER3_INTERRUPT_DISABLE();
#endif
        /* Disable TIMER3 peripheral*/
        TIMER3_DISABLE_CONFIG();
    }
    return (ret);     
}
/**
 * @brief: Write the provided value to timer3 counter/timer register
 * @param timer3_obj the timer3 object to write to its register
 * @param value the value to write to the timer3 counter/timer register
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType timer3_write_value(const timer3_t *timer3_obj, timer3_preload_value_t value)
{
    Std_ReturnType ret = E_OK;
 
    if (NULL == timer3_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if (_TIMER3_RW_8bit_OP == timer3_obj->rw_mode)
        {
            TIMER3_DISABLE_CONFIG();
            /* Write to TMR3H*/
            TMR3H = (uint8)(value >> 8);
            /* Write to TMR3L*/
            TMR3L = (uint8)value;
            TIMER3_ENABLE_CONFIG();
        }
        else
        {
            /* Write to TMR3H*/
            TMR3H = (uint8)(value >> 8);
            /* Write to TMR3L*/
            TMR3L = (uint8)value;
        }
    }
    return (ret);   
}

/**
 * @brief: Read and store the value from timer3 counter/timer register into the provided address
 * @param timer3_obj the timer3 object to read from its register
 * @param value the address to store the value of the timer3 counter/timer register
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer3_read_value(const timer3_t *timer3_obj, timer3_preload_value_t *value)
{
    Std_ReturnType ret = E_OK;
 
    if ((NULL == timer3_obj) || (NULL == value))
    {
        ret = E_NOT_OK;
    }
    else
    {
        if (_TIMER3_RW_8bit_OP == timer3_obj->rw_mode)
        {
            TIMER3_DISABLE_CONFIG();
            *value = TMR3L;
            *value += (uint16)(TMR3H << 8);
            TIMER3_ENABLE_CONFIG();
        }
        else
        {
            *value = TMR3L;
            *value += (uint16)(TMR3H << 8);
        }
    }
    return (ret); 
}
/**
 * @brief: A helper function to configure the clock source of the timer3
 * @param timer3_obj the timer3 peripheral object to configure its clock source
 */
static inline void timer3_configure_clock_src(const timer3_t *timer3_obj)
{
    /* Check if the timer3 works as a counter mode (external src)
       or timer mode (internal src) */
    if (_TIMER3_INT_SRC == timer3_obj->clock_src)
    {
        TIMER3_INT_SRC_CONFIG();
    }
    else
    {
        /* Configure whether to sync or not with the external clock input*/
        timer3_configure_ext_clk_sync(timer3_obj);
        TIMER3_EXT_SRC_CONFIG();
    }
}
/**
 * @brief: A helper function to configure the read/write mode of the timer3 to be either
 *         two 8-bit registers or one 16-bit register
 * @param timer3_obj the timer3 peripheral object to configure its read/write mode
 */
static inline void timer3_configure_rw_mode(const timer3_t *timer3_obj)
{
    switch (timer3_obj->rw_mode)
    {
        case _TIMER3_RW_16bit_OP:
            TIMER3_RW_16bit_RES_CONFIG();
            break;
        case _TIMER3_RW_8bit_OP:
            TIMER3_RW_8bit_RES_CONFIG();
            break;
        default:
            break;
    }
}
/**
 * @brief: A helper function to configure whether to sync or not with the external clock source
 * @param timer3_obj the timer3 peripheral object to configure the external clock synchronization
 */
static inline void timer3_configure_ext_clk_sync(const timer3_t *timer3_obj)
{
    switch (timer3_obj->ext_clk_sync)
    {
        case _TIMER3_SYNC:
            TIMER3_SYNC_WITH_EXT_CLK();
            break;
        case _TIMER3_ASYNC:
            TIMER3_ASYNC_WITH_EXT_CLK();
            break;
        default:
            break;
    }
}
#if TIMER3_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/**
 * @brief: A helper function to set the interrupt handler of the timer3 overflow interrupt
 * @param Interrupt_Handler the interrupt handler to call when an interrupt is raised
 * @return E_OK if success otherwise E_NOT_OK
 */
static inline Std_ReturnType timer3_set_interrupt_handler(INTERRUPT_HANDLER Interrupt_Handler)
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
        timer3_interrupt_handler = Interrupt_Handler;
    }
    return (ret);
}
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/**
 * @brief: A helper function to set the priority of the timer3 overflow interrupt
 * @param timer3_obj the timer3 perihperal object used
 */
static inline void timer3_set_interrupt_priority(const timer3_t *timer3_obj)
{
    INTERRUPT_PRIORITY_levels_ENABLE();
    INTERRUPT_Global_interrupt_LOW_ENABLE();
    INTERRUPT_Global_interrupt_HIGH_ENABLE();
    if (INTERRUPT_HIGH_PRIORITY == timer3_obj->timer3_interrupt_priority)
    {
        TIMER3_HIGH_PRIORITY();
    }
    else
    {
        TIMER3_LOW_PRIORITY();
    }
}
#endif
/**
 * @brief the interrupt service routine of timer3 module, will be called if the overflow happened
 */
void TIMER3_ISR(void)
{
    TIMER3_INTERRUPT_FLAG_BIT_CLEAR();
    if (_TIMER3_RW_8bit_OP == T3CONbits.T3RD16)
    {
        TIMER3_DISABLE_CONFIG();
        /* Write to TMR3H*/
        TMR3H = (uint8)(preloaded_value >> 8);
        /* Write to TMR3L*/
        TMR3L = (uint8)preloaded_value;
        TIMER3_ENABLE_CONFIG();
    }
    else
    {
        /* Write to TMR3H*/
        TMR3H = (uint8)(preloaded_value >> 8);
        /* Write to TMR3L*/
        TMR3L = (uint8)preloaded_value;
    }
    if (NULL != timer3_interrupt_handler)
    {
        timer3_interrupt_handler();
    }
}
#endif

