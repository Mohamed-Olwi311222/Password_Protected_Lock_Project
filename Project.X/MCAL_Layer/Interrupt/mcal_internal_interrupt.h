/* 
 * File:   mcal_internal_interrupt.h
 * Author: Mohamed olwi
 *
 * Created on 01 August 2024, 06:52
 */

#ifndef MCAL_INTERNAL_INTERRUPT_H
#define	MCAL_INTERNAL_INTERRUPT_H
/*----------------------------Header Files------------------------------------*/
#include "mcal_interrupt_config.h"
/*----------------------------Macros Functions Declarations-------------------*/
/*----------------TIMER0 Interrupt--------------*/
#if TIMER0_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/**
 * Clears the interrupt enable for the TIMER0 module
 */
#define TIMER0_INTERRUPT_DISABLE()                    (INTCONbits.T0IE = 0)
/**
 * Sets the interrupt enable for the TIMER0 module
 */
#define TIMER0_INTERRUPT_ENABLE()                     (INTCONbits.T0IE = 1)
/**
 * Clears the interrupt flag for the TIMER0 module
 */
#define TIMER0_INTERRUPT_FLAG_BIT_CLEAR()             (INTCONbits.T0IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/**
 * TIMER0 HIGH priority
 */
#define TIMER0_HIGH_PRIORITY()                        (INTCON2bits.TMR0IP = 1)
/**
 * TIMER0 LOW priority
 */
#define TIMER0_LOW_PRIORITY()                         (INTCON2bits.TMR0IP = 0)
#endif
#endif

/*----------------TIMER1 Interrupt--------------*/
#if TIMER1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/**
 * Clears the interrupt enable for the TIMER1 module
 */
#define TIMER1_INTERRUPT_DISABLE()                    (PIE1bits.TMR1IE = 0)
/**
 * Sets the interrupt enable for the TIMER1 module
 */
#define TIMER1_INTERRUPT_ENABLE()                     (PIE1bits.TMR1IE = 1)
/**
 * Clears the interrupt flag for the TIMER1 module
 */
#define TIMER1_INTERRUPT_FLAG_BIT_CLEAR()             (PIR1bits.TMR1IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/**
 * TIMER1 HIGH priority
 */
#define TIMER1_HIGH_PRIORITY()                        (IPR1bits.TMR1IP = 1)
/**
 * TIMER1 LOW priority
 */
#define TIMER1_LOW_PRIORITY()                         (IPR1bits.TMR1IP = 0)
#endif
#endif

/*----------------TIMER2 Interrupt--------------*/
#if TIMER2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/**
 * Clears the interrupt enable for the TIMER2 module
 */
#define TIMER2_INTERRUPT_DISABLE()                    (PIE1bits.TMR2IE = 0)
/**
 * Sets the interrupt enable for the TIMER2 module
 */
#define TIMER2_INTERRUPT_ENABLE()                     (PIE1bits.TMR2IE = 1)
/**
 * Clears the interrupt flag for the TIMER2 module
 */
#define TIMER2_INTERRUPT_FLAG_BIT_CLEAR()             (PIR1bits.TMR2IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/**
 * TIMER2 HIGH priority
 */
#define TIMER2_HIGH_PRIORITY()                        (IPR1bits.TMR2IP = 1)
/**
 * TIMER2 LOW priority
 */
#define TIMER2_LOW_PRIORITY()                         (IPR1bits.TMR2IP = 0)
#endif
#endif

/*----------------TIMER3 Interrupt--------------*/
#if TIMER3_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/**
 * Clears the interrupt enable for the TIMER3 module
 */
#define TIMER3_INTERRUPT_DISABLE()                    (PIE2bits.TMR3IE = 0)
/**
 * Sets the interrupt enable for the TIMER3 module
 */
#define TIMER3_INTERRUPT_ENABLE()                     (PIE2bits.TMR3IE = 1)
/**
 * Clears the interrupt flag for the TIMER3 module
 */
#define TIMER3_INTERRUPT_FLAG_BIT_CLEAR()             (PIR2bits.TMR3IF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
/**
 * TIMER3 HIGH priority
 */
#define TIMER3_HIGH_PRIORITY()                        (IPR2bits.TMR3IP = 1)
/**
 * TIMER3 LOW priority
 */
#define TIMER3_LOW_PRIORITY()                         (IPR2bits.TMR3IP = 0)
#endif
#endif

#endif	/* MCAL_INTERNAL_INTERRUPT_H */

