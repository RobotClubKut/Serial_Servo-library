/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <stdio.h>
#include "Servo.h"

void EEPROM_rx(Servo_Data*);

int main()
{
    char buf[64];
    Servo_Data servo;
    UART_servo_Start();
    UART_PC_Start();
    CyGlobalIntEnable; /* Enable global interrupts. */
    CyDelay(1000);
    sprintf(buf, "Servo Start\n");
    UART_PC_PutString(buf);
    CyDelay(1000);
    Servo_Dataset(&servo, 3, 100, 10);
    init_stretch(&servo);
    init_speed(&servo);
    EEPROM_rx(&servo);
    for(;;)
    {
        //servo_set(&servo, -135, 0); /* -135 ~ 135 */
        CyDelay(1);
    }
}

void EEPROM_rx(Servo_Data* servo){
    uint8 i;
    unsigned char rx_eep[68];
    char buf[40];
    rx_eep[0] = RX_CMD | servo->id;
    rx_eep[1] = 0x00;
    for(i = 0; i < 2; i++){
            UART_servo_PutChar(rx_eep[i]);
    }
    while(UART_servo_GetRxBufferSize() < 68);
    for(i = 0; i < 68; i++){
        rx_eep[i] = (unsigned char)UART_servo_GetChar();
    }
    UART_servo_ClearRxBuffer();
    for(i = 4; i < 68; i++){
        sprintf(buf, "%3d:0x%x\n", (int)(i - 3), (int)rx_eep[i]);
        UART_PC_PutString(buf);
    }
}
/* [] END OF FILE */
