/* 
 * File:   ecu_keypad.c
 * Author: Mohamed olwi
 *
 * Created on 02 June 2024, 15:59
 */
#include "ecu_keypad.h"

#if ECU_KEYPAD_ENABLED == ECU_ENABLED

/*---------------Static Helper functions declerations---------------------------*/
static void TIMER1_KEYPAD_ISR(void);
static void INTx_KEYPAD_ISR(void);
/*---------------Static Helper functions declerations End-----------------------*/

/*---------------Data types-----------------------------------------------------*/
static const uint8 btn_values[ECU_KEYPAD_ROWS][ECU_KEYPAD_COLS] = {
                                                               {'1', '2', '3'},
                                                               {'4', '5', '6'},
                                                               {'7', '8', '9'},
                                                               {'*', '0', '#'}
};
volatile logic_t cols_btn_pressed[ECU_KEYPAD_COLS] = {GPIO_LOW}; /* An array that contains the status of the 3 cols*/
uint8 row_counter = ZERO_INIT;                                   /* A counter for which row got high */
uint8 col_counter = 0xFF;                                        /* A counter for which col got high */
static volatile uint8 row_index = ZERO_INIT;                     /* A variable to store the last row that got logic value high */
volatile uint8 interrupt_flag = INTERRUPT_NOT_OCCUR;             /* A flag for INTx interrupt to stop updating row_index */
uint8 portb_status = 0x00;                                       /* A variable to store the status of PORTB to fix if the timer1 ended when the col is low */
/* The external INTx interrupts objects*/
static interrupt_INTx_t intx_cols[3] = {
    {
        .EXT_interrupt_handler = INTx_KEYPAD_ISR,
        .edge = INTERRUPT_RISING_EDGE,
        .source = INTERRUPT_EXTERNAL_INT0,  
        .priority = INTERRUPT_HIGH_PRIORITY,
        .mcu_pin = {.port = PORTB_INDEX, .pin = GPIO_PIN0, .direction = GPIO_DIRECTION_INPUT}
    },
    {
        .EXT_interrupt_handler = INTx_KEYPAD_ISR,
        .edge = INTERRUPT_RISING_EDGE,
        .source = INTERRUPT_EXTERNAL_INT1, 
        .priority = INTERRUPT_HIGH_PRIORITY,
        .mcu_pin = {.port = PORTB_INDEX, .pin = GPIO_PIN1, .direction = GPIO_DIRECTION_INPUT}
    },
    {
        .EXT_interrupt_handler = INTx_KEYPAD_ISR,
        .edge = INTERRUPT_RISING_EDGE,
        .source = INTERRUPT_EXTERNAL_INT2, 
        .priority = INTERRUPT_HIGH_PRIORITY,
        .mcu_pin = {.port = PORTB_INDEX, .pin = GPIO_PIN2, .direction = GPIO_DIRECTION_INPUT}
    }
};

/* The timer1 object with an interrupt time of 200 ms for bouncing effect */
static timer1_t timer1_obj = {
  .timer1_preloaded_value = 15536,
  .prescaler_value = TIMER1_PRESCALER_DIV_BY_8,
  .clock_src = _TIMER1_INT_SRC,
  .ext_clk_sync = _TIMER1_ASYNC,
  .rw_mode = _TIMER1_RW_16bit_OP,
  .ext_osc_enable = _TIMER1_OSC_DISABLE,
  .timer1_interrupt_handler = TIMER1_KEYPAD_ISR,
  .timer1_interrupt_priority = INTERRUPT_HIGH_PRIORITY
};
/*---------------Data types End-------------------------------------------------*/

/**
 * @brief: The timer1 ISR for debouncing
 */
static void TIMER1_KEYPAD_ISR(void)
{
    /* Check the actual state of the button */
    if (GPIO_HIGH == READ_BIT(portb_status, GPIO_PIN0))
    {
        cols_btn_pressed[INT0_PIN] = GPIO_HIGH;  /* Button confirmed pressed */
        col_counter = INT0_PIN;                  /* Save which col is high */
    }
    else if (GPIO_HIGH == READ_BIT(portb_status, GPIO_PIN1))
    {
        cols_btn_pressed[INT1_PIN] = GPIO_HIGH;
        col_counter = INT1_PIN;
    }
    else if (GPIO_HIGH == READ_BIT(portb_status, GPIO_PIN2))
    {
        cols_btn_pressed[INT2_PIN] = GPIO_HIGH;
        col_counter = INT2_PIN;
    }
    portb_status = 0x00;                /* Reset portb status variable */
    TIMER1_INTERRUPT_DISABLE();
    TIMER1_DISABLE_CONFIG();            /* Disable timer1 module */
}
/**
 * @brief: The INTx ISR when a button is pressed while scanning working, it will raise an interrupt
 *         as each col is connected to an INTx pin
 */
static void INTx_KEYPAD_ISR(void)
{
    interrupt_flag = INTERRUPT_OCCUR;                   /* Interrupt happened, save the row_index in the loop */
    gpio_port_read_logic(PORTB_INDEX, &portb_status);   /* Read the logic of PORTB to save the status of it if the col gets low */
    TIMER1_INTERRUPT_ENABLE();
    TIMER1_ENABLE_CONFIG();                             /* Start the timer1 module timer */
}

/**
 * @brief initialize the keypad pins
 * @param keypad_obj keypad config @ref keypad_t
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType keypad_initialize(const keypad_t *keypad_obj)
{
    Std_ReturnType ret = E_OK;
    uint8 l_row_counter = ZERO_INIT, l_col_counter = ZERO_INIT;
    
    if (NULL == keypad_obj)
    {
        ret = E_NOT_OK;
    }
    else
    {        
        timer1_init(&timer1_obj);           /* A timer with 75ms interrupt time until bouncing ends */
        TIMER1_INTERRUPT_DISABLE();
        TIMER1_DISABLE_CONFIG();            /* Disable the timer1 until an interrupt happens from INTx */
        /* Initialize the row pins */
        for (; l_row_counter < ECU_KEYPAD_ROWS; l_row_counter++)
        {
            ret |= gpio_pin_initialize(&(keypad_obj->keypad_rows_pins[l_row_counter]));
        }
        if (E_OK == ret)
        {
            /* Initialize the col pins that is connected to external INTx interrupt*/
            for (; l_col_counter < ECU_KEYPAD_COLS; l_col_counter++)
            {
                ret |= Interrupt_INTx_Init(&(intx_cols[l_col_counter]));
            }
        }
    }
    return (ret);    
}
/**
 * @brief: gets the value of the pressed button on the keypad
 * @note:  The cols must be connected to external INTx interrupt
 * @param keypad_obj keypad config @ref keypad_t
 * @param value pointer to an address to save the value read
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType keypad_get_value(const keypad_t *keypad_obj, uint8 *value)
{
    Std_ReturnType ret = E_OK;
    uint16 timeout_counter = ZERO_INIT;

    if ((NULL == keypad_obj) || (NULL == value))
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Make the returned value be NO_INPUT to make sure that no value is there*/
        *value = NO_INPUT;
        while (NO_INPUT == *value)
        {
            /* begin scanning for button pressed*/
            row_counter = 0;
            for (; row_counter < ECU_KEYPAD_ROWS; row_counter++)
            {
                if (INTERRUPT_NOT_OCCUR == interrupt_flag)
                {
                    /* Save the current row if no interrupt happened*/
                    row_index = row_counter;  
                    /* If an interrupt happen, the row wont change as the 
                       current value stored is the last row got high that caused the interrupt*/
                } 
                /*Write logic high to current row pin*/
                ret = gpio_pin_write_logic(&(keypad_obj->keypad_rows_pins[row_counter]), GPIO_HIGH); 
                /*value of the button pressed is read and saved in @value*/
                if (GPIO_HIGH == cols_btn_pressed[col_counter])
                {
                    *value = btn_values[row_index][col_counter];
                    cols_btn_pressed[col_counter] = GPIO_LOW;
                    break;
                }
                /*Write logic low to current row pin*/   
                ret = gpio_pin_write_logic(&(keypad_obj->keypad_rows_pins[row_counter]), GPIO_LOW);
            }
            /* Do a timeout if no value is entered */
            __delay_ms(100);
            timeout_counter += 100;  // Increase the timeout counter
            if (timeout_counter >= KEYPAD_ENTRY_TIMEOUT)
            {
                /* Timeout for entering a value*/
                *value = TIMEOUT;
                TIMER1_DISABLE_CONFIG();
                EXT_INT0_INTERRUPT_DISABLE();
                EXT_INT1_INTERRUPT_DISABLE();
                EXT_INT2_INTERRUPT_DISABLE();
                break;
            }
        }
        interrupt_flag = INTERRUPT_NOT_OCCUR; /* Reset the interrupt_flag to 0 */
    }
    return (ret);    
}
#endif