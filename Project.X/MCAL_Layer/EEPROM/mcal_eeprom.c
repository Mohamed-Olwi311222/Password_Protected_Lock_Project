/* 
 * File:   mcal_eeprom.c
 * Author: Mohamed olwi
 *
 * Created on 13 August 2024, 07:40
 */
#include "mcal_eeprom.h"

/**
 * @brief Write a byte to a specific address in the EEPROM
 * @param byte_add the address to save the data into
 * @param byte_data the data to save inside the EEPROM memory address
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType Data_EEPROM_Write_Byte(uint16 byte_add, const uint8 byte_data)
{
    Std_ReturnType ret = E_OK;
    /*Save the Global Interrupt status*/
    uint8 GIE_status = INTCONbits.GIE;
    /*Set the EEPROM Address: Load the EEPROM address into the EEADR and EEADRH registers*/
    EEADRH = (uint8)((byte_add >> 8) & 0x03);
    EEADR = (uint8)(byte_add & 0xFF);
    /*Load Data: Move the data to be written into the EEDATA register*/
    EEDATA = byte_data;
    /*Configure the EECON1 register to point to EEPROM memory and enable write operations by setting the WREN bit.*/
    EEPGD_ACCESS_DATA_EEPROM_MEM();
    CFGS_ACCESS_DATA_EEPROM();
    WREN_EEPROM_ENABLE_WRITE();
    /*Disable interrupts, perform a specific unlock sequence (0x55 and 0xAA written to EECON2), and then start the write by setting the WR bit.*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_DISABLE
    INTERRUPT_Global_interrupt_DISABLE();
#endif
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EEPROM_BEGIN_WRITE_ERASE();
    /*Wait Until the Write operation is completed*/
    while(EECON1bits.WR == 1);
    /*Inhibits write cycles to Flash program/data EEPROM*/
    WREN_EEPROM_DISABLE_WRITE();
    /*Remove Interrupt Flag*/
    CLEAR_EEIF_FLAG();
    /*Restore Interrupt Status*/
    INTCONbits.GIE = GIE_status;
  
    return (ret);   
}

/**
 * @brief Read a byte from a specific address in the EEPROM
 * @param byte_add the address to Read the data from
 * @param byte_data the address to save the data read from the EEPROM
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType Data_EEPROM_Read_Byte(uint16 byte_add, uint8 *const byte_data)
{
    Std_ReturnType ret = E_OK;
    if (NULL == byte_data)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /*Set the EEPROM Address: The address from which data is to be read is loaded into the EEADR register*/
        EEADRH = (uint8)((byte_add >> 8) & 0x03);
        EEADR = (uint8)(byte_add & 0xFF);
        /*Configure Access: Ensure that the access is set to EEPROM memory by clearing the EEPGD and CFGS bits*/
        EEPGD_ACCESS_DATA_EEPROM_MEM();
        CFGS_ACCESS_DATA_EEPROM();
        /*Initiate the Read: Set the RD bit in the EECON1 register to start the EEPROM read operation*/
        EEPROM_BEGIN_READ();
        /*Wait at least 2 NOP(); Required for latency at high frequencies.*/
        NOP();
        NOP();
        /*Retrieve the Data: After the read operation*/
        *byte_data = EEDATA;
    }
    return (ret);  
}

/**
 * @brief Store an array to a specific address in the EEPROM
 * @param byte_add the address to save the data into
 * @param arr the starting address of the array of data to save inside the EEPROM memory address
 * @param arr_size The size of the array to store
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType Data_EEPROM_Write_Array(const uint16 eeprom_add, const uint8 *const arr, const uint8 arr_size)
{
    Std_ReturnType ret = E_OK;
    uint8 counter = ZERO_INIT;
    uint16 temp_add = eeprom_add;
    
    if (NULL == arr)
    {
        ret = E_NOT_OK;
    }
    else
    {
        for (; counter < arr_size; counter++)
        {
            ret |= Data_EEPROM_Write_Byte(temp_add, arr[counter]);
            temp_add++;
        }
    }
    return (ret);
}

/**
 * @brief Read an array from a specific address in the EEPROM
 * @param byte_add the address to Read the data from
 * @param arr the address to save the array read from the EEPROM
 * @param arr_size The size of the array to store
 * @return E_OK if success otherwise E_NOT_OK
 */
Std_ReturnType Data_EEPROM_Read_Array(const uint16 eeprom_add, uint8 *const arr, const uint8 arr_size)
{
    Std_ReturnType ret = E_OK;
    uint8 counter = ZERO_INIT;
    uint16 temp_add = eeprom_add;
    
    if (NULL == arr)
    {
        ret = E_NOT_OK;
    }
    else
    {
        for (; counter < arr_size; counter++)
        {
            ret |= Data_EEPROM_Read_Byte(temp_add, &(arr[counter]));
            temp_add++;
        }
    }
    return (ret);
}