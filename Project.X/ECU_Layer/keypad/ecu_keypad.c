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

/*---------------Static Data types----------------------------------------------*/
static const uint8 btn_values[ECU_KEYPAD_ROWS][ECU_KEYPAD_COLS] = {
                                                               {'1', '2', '3'},
                                                               {'4', '5', '6'},
                                                               {'7', '8', '9'},
                                                               {'*', '0', '#'}
};
static volatile logic_t cols_btn_pressed[ECU_KEYPAD_COLS] = {GPIO_LOW}; /* An array that contains the status of the 3 cols*/
static uint8 row_counter = ZERO_INIT;                                   /* A counter for the inner loop in the read function */
static volatile uint8 row_index = ZERO_INIT;                            /* A variable to store the last row that got logic value high */
static volatile uint8 debounce_flag = 0;                                /* A flag for bouncing effect, if its 1 the bouncing exist otherwise bouncing stopped*/

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
/*---------------Static Data types End------------------------------------------*/

/* The timer1 object with an interrupt time of 125ms for bouncing effect */
static timer1_t timer1_obj = {
  .timer1_preloaded_value = 3036,
  .prescaler_value = TIMER1_PRESCALER_DIV_BY_4,
  .clock_src = _TIMER1_INT_SRC,
  .ext_clk_sync = _TIMER1_ASYNC,
  .rw_mode = _TIMER1_RW_16bit_OP,
  .ext_osc_enable = _TIMER1_OSC_DISABLE,
  .timer1_interrupt_handler = TIMER1_KEYPAD_ISR,
  .timer1_interrupt_priority = INTERRUPT_HIGH_PRIORITY
};
/**
 * @brief: The timer1 ISR for debouncing
 */
static void TIMER1_KEYPAD_ISR(void)
{
    /* Check the actual state of the button */
    debounce_flag = 0;
    if (GPIO_HIGH == PORTBbits.RB0)
    {
        cols_btn_pressed[INT0_PIN] = GPIO_HIGH;  /* Button confirmed pressed */
    }
    else if (GPIO_HIGH == PORTBbits.RB1)
    {
        cols_btn_pressed[INT1_PIN] = GPIO_HIGH;
    }
    else if (GPIO_HIGH == PORTBbits.RB2)
    {
        cols_btn_pressed[INT2_PIN] = GPIO_HIGH;
    }
    timer1_deinit(&timer1_obj);
}
/**
 * @brief: The INTx ISR when a button is pressed while scanning working, it will raise an interrupt
 *         as each col is connected to an INTx pin
 */
static void INTx_KEYPAD_ISR(void)
{
    row_index = row_counter;                /* Save the current high row when a col got high*/
    if (!debounce_flag)
    {
        debounce_flag = 1;                  /* bouncing starts, further interrupt raised will do nothing */
        timer1_init(&timer1_obj);           /* A timer with 125ms interupt time until bouncing ends */
    }        
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
    {    /* Initialize the row pins */
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
    uint8 l_col_counter = ZERO_INIT, l_counter = ZERO_INIT;
    
    if ((NULL == keypad_obj) || (NULL == value))
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Make the returned value be NO_INPUT to make sure that no value is there*/
        *value = NO_INPUT;
        /*write logic low to all rows pins*/
        for (l_counter = 0; l_counter < ECU_KEYPAD_ROWS; l_counter++)
        {
            if (E_NOT_OK == ret)
            {
                break;
            }
            ret = gpio_pin_write_logic(&(keypad_obj->keypad_rows_pins[l_counter]), GPIO_LOW);
        }
        /* begin scanning for button pressed*/
        for (; row_counter < ECU_KEYPAD_ROWS; row_counter++)
        {
            /* Break if the value is read*/
            if (NO_INPUT != *value)
            {
                break;
            }
            /*Write logic high to current row pin*/
            ret = gpio_pin_write_logic(&(keypad_obj->keypad_rows_pins[row_counter]), GPIO_HIGH);       
            /*check if current row pin make contact with any col pin*/
            for (l_col_counter = 0; l_col_counter < ECU_KEYPAD_COLS; l_col_counter++)
            {
                /*value of the button pressed is read and saved in @value*/
                if (GPIO_HIGH == cols_btn_pressed[l_col_counter])
                 {
                     *value = btn_values[row_index][l_col_counter];
                     cols_btn_pressed[l_col_counter] = GPIO_LOW;
                     break;
                 }
            }
        }
    }
    return (ret);    
}
#endif