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

int main()
{
    uint16 i;
    char buf[40];
    Servo_Data servo;
    UART_servo_Start();
    UART_Debug_Start();
    CyGlobalIntEnable; /* Enable global interrupts. */
    CyDelay(1000);
    Servo_Dataset(&servo, 3, 100, 10);
    init_stretch(&servo);
    init_speed(&servo);
    
    for(;;)
    {
        servo_set(&servo, -135, 0); /* -135 ~ 135 */
        CyDelay(10);
    }
}
/* [] END OF FILE */
