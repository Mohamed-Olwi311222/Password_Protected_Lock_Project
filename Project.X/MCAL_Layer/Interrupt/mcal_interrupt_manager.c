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
    if ((INTERRUPT_ENABLE == PIE1bits.TMR1IE) && (INTERRUPT_OCCUR == PIR1bits.TMR1IF))
    {
        TIMER1_ISR();
    }
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
}
/**
 * @brief the interrupt manager for low priority interrupts
 */
void  __interrupt(low_priority) Interrupt_Manager_Low(void)
{
    if ((INTERRUPT_ENABLE == INTCONbits.T0IE) && (INTERRUPT_OCCUR == INTCONbits.T0IF))
    {
        TIMER0_ISR();
    }
 }
#else
void __interrupt() Interrupt_Manager(void)
{

}
#endif
