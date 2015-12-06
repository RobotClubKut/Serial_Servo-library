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
    char buf[32];
    Servo_Data servo[3];
    UART_servo_Start();
    UART_PC_Start();
    CyGlobalIntEnable; /* Enable global interrupts. */
    CyDelay(1000);
    sprintf(buf, "Servo Start\n");
    UART_PC_PutString(buf);
    Servo_Dataset(&servo[0], 0, 100, 10);
    Servo_Dataset(&servo[1], 1, 100, 10);
    Servo_Dataset(&servo[2], 2, 100, 10);
    CyDelay(1000);
    for(;;)
    {
        //angle_set(&servo[0], -5);
        //angle_set(&servo[1], -100);
        //angle_set(&servo[2], 70);
        //angle_get(&servo[0]);
        //angle_get(&servo[1]);
        //angle_get(&servo[2]);
        CyDelay(1);
    }
}

/* [] END OF FILE */
