/* 
 * File:   mcal_interrupt_manager.c
 * Author: Mohamed olwi
 *
 * Created on 01 August 2024, 06:51
 */
#include "mcal_interrupt_manager.h"
     
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/**
 * @brief the interrupt manager for high priority interrupts
 */
void __interrupt() Interrupt_Manager_High(void)
{
#if TIMER0_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
    if ((INTERRUPT_ENABLE == INTCONbits.T0IE) && (INTERRUPT_OCCUR == INTCONbits.T0IF))
    {
        TIMER0_ISR();
    }
#endif
#if EXTERNAL_INTERRUPT_ENABLE == INTERRUPT_FEATURE_ENABLE
#if EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    if ((INTERRUPT_ENABLE == INTCONbits.INT0IE) && (INTERRUPT_OCCUR == INTCONbits.INT0IF))
    {
        INT0_ISR(); // Call the ISR function
    }
    if ((INTERRUPT_ENABLE == INTCON3bits.INT2IE) && (INTERRUPT_OCCUR == INTCON3bits.INT2IF))
    {
        INT2_ISR(); // Call the ISR function
    }
#endif
#endif
}
/**
 * @brief the interrupt manager for low priority interrupts
 */
void  __interrupt(low_priority) Interrupt_Manager_Low(void)
{
#if TIMER3_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
    if ((INTERRUPT_ENABLE == PIE2bits.TMR3IE) && (INTERRUPT_OCCUR == PIR2bits.TMR3IF))
    {
        TIMER3_ISR();
    }
#endif
#if TIMER2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
    if ((INTERRUPT_ENABLE == PIE1bits.TMR2IE) && (INTERRUPT_OCCUR == PIR1bits.TMR2IF))
    {
        TIMER2_ISR();
    }
#endif
#if TIMER1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
    if ((INTERRUPT_ENABLE == PIE1bits.TMR1IE) && (INTERRUPT_OCCUR == PIR1bits.TMR1IF))
    {
        TIMER1_ISR();
    }
#endif
#if EXTERNAL_INTERRUPT_ENABLE == INTERRUPT_FEATURE_ENABLE
    #if EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    if ((INTERRUPT_ENABLE == INTCON3bits.INT1IE) && (INTERRUPT_OCCUR == INTCON3bits.INT1IF))
    {
        INT1_ISR(); // Call the ISR function
    }  
    #endif
#endif
}
#else
void __interrupt() Interrupt_Manager(void)
{
#if TIMER0_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
    if ((INTERRUPT_ENABLE == INTCONbits.T0IE) && (INTERRUPT_OCCUR == INTCONbits.T0IF))
    {
        TIMER0_ISR();
    }
#endif
    #if EXTERNAL_INTERRUPT_ENABLE == INTERRUPT_FEATURE_ENABLE
    #if EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    /*------------------------INTx External Interrupts------------------------*/
    /*---------------INT0-------------------------*/
    if ((INTERRUPT_ENABLE == INTCONbits.INT0IE) && (INTERRUPT_OCCUR == INTCONbits.INT0IF))
    {
        INT0_ISR(); // Call the ISR function
    }
    /*---------------INT1-------------------------*/
    if ((INTERRUPT_ENABLE == INTCON3bits.INT1IE) && (INTERRUPT_OCCUR == INTCON3bits.INT1IF))
    {
        INT1_ISR(); // Call the ISR function
    }
    /*---------------INT2-------------------------*/
    if ((INTERRUPT_ENABLE == INTCON3bits.INT2IE) && (INTERRUPT_OCCUR == INTCON3bits.INT2IF))
    {
        INT2_ISR(); // Call the ISR function
    }
    #endif
    #endif

}
#endif
