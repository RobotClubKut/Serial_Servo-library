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
#include "PS2_Controller.h"

Servo_Data servo[3];

void free(void);
void kamae(void);
void karimen(void);

int main()
{
    char buf[32];
    PS2Controller psData;
    UART_servo_Start();
    UART_PC_Start();
    PWM_Start();    /* 73 ~ 233 中心153 */
    PS2_Start();
    CyGlobalIntEnable; /* Enable global interrupts. */
    CyDelay(1000);
    sprintf(buf, "Servo Start\n");
    UART_PC_PutString(buf);
    Servo_Dataset(&servo[0], 0, 100, 10, -5);
    Servo_Dataset(&servo[1], 1, 100, 10, -70);
    Servo_Dataset(&servo[2], 2, 100, 10, 130);
    CyDelay(1000);
    for(;;)
    {
        psData = PS2_Controller_get();
        sprintf(buf, "%d\n", (int)psData.CIRCLE);
        UART_PC_PutString(buf);
        if(psData.CIRCLE){
            kamae();
        }
        if(psData.START){
            karimen();
        }
        angle_keep(&servo[0]);
        angle_keep(&servo[1]);
        angle_keep(&servo[2]);
    }
}

void free(void) {
    angle_get(servo[0].id);
    angle_get(servo[1].id);
    angle_get(servo[2].id);
}

void kamae(void) {
    angle_set(&servo[0], -5);
    angle_set(&servo[1], -70);
    angle_set(&servo[2], 130);
    PWM_WriteCompare(133);
}

void karimen(void){
    angle_set(&servo[2], 0);
    angle_set(&servo[0], -5);
    angle_set(&servo[1], 45);
    PWM_WriteCompare(150);
}
/* [] END OF FILE */
