/* 
 * File:   hal_timer2.h
 * Author: Mohamed olwi
 *
 * Created on 30 September 2024, 20:13
 */
#include "hal_timer2.h"
/*---------------Static Data types----------------------------------------------*/
#if TIMER2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
static INTERRUPT_HANDLER timer2_interrupt_handler = NULL; /* A pointer to the callback function when an interrupt is raised */
#endif
static uint8 preloaded_TMR2_value = ZERO_INIT;
static uint8 preloaded_PR2_value = ZERO_INIT;

/*---------------Static Data types End------------------------------------------*/

/*---------------Static Helper functions declerations---------------------------*/
#if TIMER2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
static inline Std_ReturnType timer2_set_interrupt_handler(INTERRUPT_HANDLER Interrupt_Handler);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
static inline void timer2_set_interrupt_priority(const timer2_t *timer2_obj);
#endif
#endif
/*---------------Static Helper functions declerations End-----------------------*/

/**
 * @brief: Initialize the timer2 peripheral object
 * @param timer2_obj the timer2 object to init
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer2_init(const timer2_t *timer2_obj)
{
    Std_ReturnType ret = E_OK;
 
    if (NULL == timer2_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Disable Timer2*/
        TIMER2_DISABLE_CONFIG();
        /* Configure the Prescaler */
        TIMER2_PRESCALER_SELECT_CONFIG(timer2_obj->prescaler_value);
        /* Configure the Postscaler */
        TIMER2_POSTSCALER_SELECT_CONFIG(timer2_obj->postscaler_value);
        /* Configure the preloaded value to store it in the timer2 TMR2 register */
        ret |= timer2_TMR2_write_value(timer2_obj, timer2_obj->TMR2_preloaded_value);
        /* Store a copy of the preloaded value to always write it to the register while handling ISR */
        preloaded_TMR2_value = timer2_obj->TMR2_preloaded_value;
        /* Configure the preloaded value to store it in the timer2 PR2 register */ 
        ret |= timer2_PR2_write_value(timer2_obj, timer2_obj->PR2_preloaded_value);
        /* Store a copy of the preloaded value to always write it to the register while handling ISR */
        preloaded_PR2_value = timer2_obj->PR2_preloaded_value;
        /* Configure the interrupt if enabled and its priority too */
#if TIMER2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        /* Enable TIMER2 interrupt */
        TIMER2_INTERRUPT_ENABLE();
        ret |= timer2_set_interrupt_handler(timer2_obj->timer2_interrupt_handler);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        timer2_set_interrupt_priority(timer2_obj);
#endif
#endif
        /* Enable Timer2 */
        TIMER2_ENABLE_CONFIG();
    }
    return (ret);
}
/**
 * @brief: Deinitialize the timer2 peripheral object
 * @param timer2_obj the timer2 object to deinit
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer2_deinit(const timer2_t *timer2_obj)
{
    Std_ReturnType ret = E_OK;
 
    if (NULL == timer2_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {
#if TIMER2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        /* Clear interrupt flag*/
        TIMER2_INTERRUPT_FLAG_BIT_CLEAR();
        /* Disable TIMER2 interrupt */
        TIMER2_INTERRUPT_DISABLE();
#endif
        /* Disable TIMER2 peripheral*/
        TIMER2_DISABLE_CONFIG();
    }
    return (ret);     
}
/**
 * @brief: Write the provided value to timer2 TMR2 register
 * @param timer2_obj the timer2 object to write to its TMR2 register
 * @param value the value to write to the timer2 TMR2 register
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType timer2_TMR2_write_value(const timer2_t *timer2_obj, timer2_TMR2_preload_value_t value)
{
    Std_ReturnType ret = E_OK;
 
    if (NULL == timer2_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {

        TIMER2_DISABLE_CONFIG();
        /*
        Preloading TMR2 adjusts the current cycle only. 
        The next cycle will start counting from 0 again, based on PR2. If you preload PR2,
        the timer period changes globally for all cycles. This is important if you want to:
        Temporarily shift an event forward without impacting future cycles.
        Adjust timing for a single operation while leaving the general configuration intact.
         */
        TMR2 = value;
        TIMER2_ENABLE_CONFIG();
    }
    return (ret);   
}
/**
 * @brief: Read and store the value from timer2 TMR2 register into the provided address
 * @param timer2_obj the timer2 object to read from its register
 * @param value the address to store the value of the timer2 TMR2 register
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer2_TMR2_read_value(const timer2_t *timer2_obj, timer2_TMR2_preload_value_t *value)
{
    Std_ReturnType ret = E_OK;
 
    if ((NULL == timer2_obj) || (NULL == value))
    {
        ret = E_NOT_OK;
    }
    else
    {

        TIMER2_DISABLE_CONFIG();
        *value = TMR2;
        TIMER2_ENABLE_CONFIG();
    }
    return (ret); 
}
/**
 * @brief: Write the provided value to timer2 PR2 register
 * @param timer2_obj the timer2 object to write to its PR2 register
 * @param value the value to write to the timer2 PR2 register
 * @return E_OK if success otherwise E_NOT_OK 
 */
Std_ReturnType timer2_PR2_write_value(const timer2_t *timer2_obj, timer2_PR2_preload_value_t value)
{
    Std_ReturnType ret = E_OK;
 
    if (NULL == timer2_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {

        TIMER2_DISABLE_CONFIG();
        PR2 = value;
        TIMER2_ENABLE_CONFIG();
    }
    return (ret);    
}
/**
 * @brief: Read and store the value from timer2 PR2 register into the provided address
 * @param timer2_obj the timer2 object to read from its register
 * @param value the address to store the value of the timer2 PR2 register
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType timer2_PR2_read_value(const timer2_t *timer2_obj, timer2_PR2_preload_value_t *value)
{
    Std_ReturnType ret = E_OK;
 
    if ((NULL == timer2_obj) || (NULL == value))
    {
        ret = E_NOT_OK;
    }
    else
    {

        TIMER2_DISABLE_CONFIG();
        *value = PR2;
        TIMER2_ENABLE_CONFIG();
    }
    return (ret); 
}

#if TIMER2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/**
 * @brief: A helper function to set the interrupt handler of the timer2 when TMR2 match with PR2
 * @param Interrupt_Handler the interrupt handler to call when an interrupt is raised
 * @return E_OK if success otherwise E_NOT_OK
 */
static inline Std_ReturnType timer2_set_interrupt_handler(INTERRUPT_HANDLER Interrupt_Handler)
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
        timer2_interrupt_handler = Interrupt_Handler;
    }
    return (ret);
}

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/**
 * @brief: A helper function to set the priority of the timer2 interrupt when TMR2 match with PR2
 * @param timer2_obj the timer2 perihperal object used
 */
static inline void timer2_set_interrupt_priority(const timer2_t *timer2_obj)
{
    INTERRUPT_PRIORITY_levels_ENABLE();
    INTERRUPT_Global_interrupt_LOW_ENABLE();
    INTERRUPT_Global_interrupt_HIGH_ENABLE();
    if (INTERRUPT_HIGH_PRIORITY == timer2_obj->timer2_interrupt_priority)
    {
        TIMER2_HIGH_PRIORITY();
    }
    else
    {
        TIMER2_LOW_PRIORITY();
    }
}
#endif

/**
 * @brief the interrupt service routine of timer2 module, will be called if the TMR2 matches with PR2
 */
void TIMER2_ISR(void)
{
    TIMER2_INTERRUPT_FLAG_BIT_CLEAR();
    TIMER2_DISABLE_CONFIG();
    /* Write to TMR2*/
    TMR2 = preloaded_TMR2_value;
    /* Write to PR2*/
    PR2 = preloaded_PR2_value;
    TIMER2_ENABLE_CONFIG();
    if (NULL != timer2_interrupt_handler)
    {
        timer2_interrupt_handler();
    }
}
#endif
