#include "hal_gpio.h"

/*Reference to the Data Latch Register (Read and Write to Data Latch)*/
static volatile uint8 *lat_registers[] = {&LATA, &LATB, &LATC, &LATD, &LATE};
/*Reference to the Data Direction Control Registers*/
static volatile uint8 *tris_registers[] = {&TRISA, &TRISB, &TRISC, &TRISD, &TRISE};
/*Reference to the Port Status Register*/
static volatile uint8 *port_registers[] = {&PORTA, &PORTB, &PORTC, &PORTD, &PORTE};



/**
 * @brief initialize the given pin of the given config
 * @param _pin_config_t the configration given (i.e led, motor, etc..)
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType gpio_pin_initialize(const pin_config_t *_pin_config_t)
{
    Std_ReturnType ret = E_OK;
    if (NULL == _pin_config_t || _pin_config_t->pin > PORT_PIN_MAX_SIZE - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ret = gpio_pin_direction_initialize(_pin_config_t);
    }
    return (ret);
}


/**
 * @brief initialize the direction of a given pin
 * @param _pin_config_t the configration given (i.e led, motor, etc..)
 * @return E_OK if success otherwise E_NOT_OK
 */
#if GPIO_PORT_PIN_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType gpio_pin_direction_initialize(const pin_config_t *_pin_config_t)
{
    Std_ReturnType ret = E_OK;

    if (NULL == _pin_config_t || _pin_config_t->pin > PORT_PIN_MAX_SIZE - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        switch (_pin_config_t->direction)
        {
            case GPIO_DIRECTION_INPUT:
                SET_BIT(*tris_registers[_pin_config_t->port], _pin_config_t->pin);
                break;
            case GPIO_DIRECTION_OUTPUT:
                CLEAR_BIT(*tris_registers[_pin_config_t->port], _pin_config_t->pin);
                break;
            default:
                ret = E_NOT_OK;
                break;
        }
    }
    return (ret);
}
#endif

/**
 * @brief get the direction of a given pin
 * @param _pin_config_t the configration given (i.e led, motor, etc..)
 * @param direction_status the direction to write on the pin @ref direction_t
 * @return E_OK if success otherwise E_NOT_OK
 */
#if GPIO_PORT_PIN_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType gpio_pin_direction_status(const pin_config_t *_pin_config_t, direction_t *direction_status)
{
    Std_ReturnType ret = E_OK;

    if ((NULL == _pin_config_t) || (NULL == direction_status) || (_pin_config_t->pin > PORT_PIN_MAX_SIZE - 1))
    {
        ret = E_NOT_OK;
    }
    else
    {
        *direction_status = READ_BIT(*tris_registers[_pin_config_t->port], _pin_config_t->pin);
    }
    return (ret);
}
#endif

/**
 * @brief write logic to a given pin in a port
 * @param _pin_config_t the configration given (i.e led, motor, etc..)
 * @param logic the logic to write @ref logic_t
 * @return E_OK if success otherwise E_NOT_OK
 */
#if GPIO_PORT_PIN_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType gpio_pin_write_logic(const pin_config_t *_pin_config_t, logic_t logic)
{
    Std_ReturnType ret = E_OK;
    if (NULL == _pin_config_t || _pin_config_t->pin > PORT_PIN_MAX_SIZE - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        switch (logic)
        {
            case GPIO_HIGH :
                SET_BIT(*lat_registers[_pin_config_t->port], _pin_config_t->pin);
                break;
            case GPIO_LOW:
                CLEAR_BIT(*lat_registers[_pin_config_t->port], _pin_config_t->pin);
                break;
            default:
                CLEAR_BIT(*lat_registers[_pin_config_t->port], _pin_config_t->pin);
                ret = E_NOT_OK;
                break;
        }
    }
    return (ret);
}
#endif

/**
 * @brief read logic of a given pin
 * @param _pin_config_t the configration given (i.e led, motor, etc..)
 * @param logic the address to store the logic read @ref logic_t
 * @return E_OK if success otherwise E_NOT_OK
 */
#if GPIO_PORT_PIN_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType gpio_pin_read_logic(const pin_config_t *_pin_config_t, logic_t *logic)
{
    Std_ReturnType ret = E_OK;
    if (NULL == _pin_config_t || NULL == logic || _pin_config_t->pin > PORT_PIN_MAX_SIZE - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        *logic = READ_BIT(*port_registers[_pin_config_t->port], _pin_config_t->pin);
    }
    return (ret);   
}
#endif

/**
 * @brief toggle the logic of a given pin
 * @param _pin_config_t the configration given (i.e led, motor, etc..)
 * @return E_OK if success otherwise E_NOT_OK
 */
#if GPIO_PORT_PIN_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType gpio_pin_toggle_logic(const pin_config_t *_pin_config_t)
{
    Std_ReturnType ret = E_OK;
    if (NULL == _pin_config_t || _pin_config_t->pin > PORT_PIN_MAX_SIZE - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TOGGLE_BIT(*lat_registers[_pin_config_t->port], _pin_config_t->pin);
    }
    return (ret);
}
#endif

/**
 * @brief initialize the given port
 * @param port the port index given @ref port_index_t
 * @param direction the direction to write
 * @return E_OK if success otherwise E_NOT_OK
 */
#if GPIO_PORT_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType gpio_port_direction_initialize(const port_index_t port, uint8 direction)
{
    Std_ReturnType ret = E_OK;
    if (port > PORT_MAX_LENGTH - 1)
        ret = E_NOT_OK;
    else
    {
        *tris_registers[port] = direction;
    }
    return (ret);
}
#endif

/**
 * @brief read the direction of a given port
 * @param port the port index given @ref port_index_t
 * @param direction_status the address to store the direction read
 * @return E_OK if success otherwise E_NOT_OK
 */
#if GPIO_PORT_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType gpio_port_direction_status(const port_index_t port, uint8 *direction_status)
{
    Std_ReturnType ret = E_OK;
    if ((NULL == direction_status) || (port > PORT_MAX_LENGTH - 1))
    {
        ret = E_NOT_OK;
    }
    else
    {
        *direction_status = *tris_registers[port];
    }
    return (ret);
}
#endif

/**
 * @brief write logic to all pins of the port
 * @param port the port given @ref port_index_t
 * @param logic the logic to write on the port
 * @return E_OK if success otherwise E_NOT_OK
 */
#if GPIO_PORT_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType gpio_port_write_logic(const port_index_t port, uint8 logic)
{
    Std_ReturnType ret = E_OK;
    if (port > PORT_MAX_LENGTH - 1)
        ret = E_NOT_OK;
    else
        *lat_registers[port] = logic;
    return (ret);
}
#endif

/**
 * @brief read the logic on the pins of the port
 * @param port the port index given @ref port_index_t
 * @param logic the address to store the read logic
 * @return E_OK if success otherwise E_NOT_OK
 */
#if GPIO_PORT_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType gpio_port_read_logic(const port_index_t port, uint8 *logic)
{
    Std_ReturnType ret = E_OK;
    if ((NULL == logic) || (port > PORT_MAX_LENGTH - 1))
    {
        ret = E_NOT_OK;
    }
    else
    {
        *logic = *port_registers[port];
    }
    return (ret);
}
#endif

/**
 * @brief toggle the logic of the given port
 * @param port the port index given @ref port_index_t
 * @return E_OK if success otherwise E_NOT_OK
 */
#if GPIO_PORT_CONFIGRATIONS == CONFIG_ENABLE
Std_ReturnType gpio_port_toggle_logic(const port_index_t port)
{
    Std_ReturnType ret = E_OK;
    if (port > PORT_MAX_LENGTH - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        *lat_registers[port] ^= PORT_MASK;
    }
    return (ret);
}
#endif
