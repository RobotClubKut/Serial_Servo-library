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
void keep(void);

int main()
{
    uint8 c_flag;
    uint8 s_flag;
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
    Servo_Dataset(&servo[0], 0, 100, 10);
    Servo_Dataset(&servo[1], 1, 100, 10);
    Servo_Dataset(&servo[2], 2, 100, 10);
    CyDelay(1000);
    for(;;)
    {
        //PWM_WriteCompare(153);
        /*
        psData = PS2_Controller_get();
        if(psData.CIRCLE){
            if(c_flag){
                kamae();
                }
            c_flag = 0;
        } else {
            c_flag = 1;
        }
        if(psData.START){
            if(s_flag){
                free();
                }
           s_flag = 0;
        } else {
            s_flag = 1;
        }
        keep();
        */
        free();
    }
}

void free(void) {
    angle_get(&servo[0]);
    CyDelay(1);
    angle_get(&servo[1]);
    CyDelay(1);
    angle_get(&servo[2]);
    CyDelay(1);
}

void kamae(void) {
    angle_set(&servo[0], -5);
    angle_set(&servo[1], -110);
    angle_set(&servo[2], 130);
    PWM_WriteCompare(133);
}

void keep(void) {
    angle_set(&servo[0], servo[0].angle);
    angle_set(&servo[1], servo[1].angle);
    angle_set(&servo[2], servo[2].angle);

}
/* [] END OF FILE */
