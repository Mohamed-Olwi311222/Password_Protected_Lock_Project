/* 
 * File:   mcal_interrupt_manager.h
 * Author: Mohamed olwi
 *
 * Created on 01 August 2024, 06:53
 */

#ifndef MCAL_INTERRUPT_MANAGER_H
#define	MCAL_INTERRUPT_MANAGER_H
/*----------------------------Header Files-----------------------------------------------------------------*/
#include "mcal_interrupt_config.h"
/*----------------------------Function Prototypes----------------------------------------------------------*/

/*--------------------------------INTx ISRS------------------------------------*/
/**
 * @brief: The interrupt service routine of INT0, will be called if INT0 interrupt has been raised
 */
void INT0_ISR(void);
/**
 * @brief: The interrupt service routine of INT1, will be called if INT0 interrupt has been raised
 */
void INT1_ISR(void);
/**
 * @brief: The interrupt service routine of INT2, will be called if INT0 interrupt has been raised
 */
void INT2_ISR(void);
/*--------------------------------Internal Interrupts ISRS---------------------*/
/**
 * @brief the interrupt service routine of timer0 module, will be called if the overflow happened
 */
void TIMER0_ISR(void);

/**
 * @brief the interrupt service routine of timer1 module, will be called if the overflow happened
 */
void TIMER1_ISR(void);

/**
 * @brief the interrupt service routine of timer2 module, will be called if the TMR2 matches with PR2
 */
void TIMER2_ISR(void);

/**
 * @brief the interrupt service routine of timer3 module, will be called if the overflow happened
 */
void TIMER3_ISR(void);
#endif	/* MCAL_INTERRUPT_MANAGER_H */

