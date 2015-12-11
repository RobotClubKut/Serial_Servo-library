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

cystatus speed(void);
void free(uint16 *pwm_data);
void kamae(uint16 *pwm_data);
void men(uint16 *pwm_data);
void hukamen(uint16 *pwm_data);
void dou(uint16 *pwm_data);
void keep(uint16 *pwm_data);

int main()
{
    bit free_flag = 1;
    uint16 pwm_data = 157;
    char buf[32];
    cystatus speed_sts;
    PS2Controller psData;
    UART_servo_Start();
    UART_PC_Start();
    PWM_Start();    /* 73 ~ 233 中心153 */
    PS2_Start();
    CyGlobalIntEnable; /* Enable global interrupts. */
    /*  */
    CyDelay(1000);
    sprintf(buf, "Servo Start\n");
    UART_PC_PutString(buf);
    Servo_Dataset(&servo[0], 0, 100, 10, -27); /* -116 ~ 55 */
    Servo_Dataset(&servo[1], 1, 100, 10, -70); /* 117 ~ -100 */
    Servo_Dataset(&servo[2], 2, 100, 10, 110); /* -135 ~ 135 */
    CyDelay(1000);
    /*
    servo[0].speed = 127;
    servo[1].speed = 127;
    servo[2].speed = 127;
    speed_sts = speed();
    if(speed_sts == CYRET_SUCCESS){
        sprintf(buf, "speed: SUCCESS\n");
        UART_PC_PutString(buf);
    } else if(speed_sts == CYRET_EMPTY){
        sprintf(buf, "speed: EMPTY\n");
        UART_PC_PutString(buf);
    }
    */
    for(;;)
    {
        psData = PS2_Controller_get();
        /* 脱力モード */
        while(free_flag == 1){
            free(&pwm_data);
            psData = PS2_Controller_get();
            if(psData.START){
                free_flag = 0;
            }
        }
        if(psData.SELECT){
            if(psData.START){
                free_flag = 1;
            }
        }
        
        /* pwm_servo */
        if(psData.L1){
            pwm_data = 70;
            PWM_WriteCompare(pwm_data);
        } else if(psData.R1){
            pwm_data = 230;
            PWM_WriteCompare(pwm_data);
        } else{
            pwm_data = 157;
            PWM_WriteCompare(pwm_data);
        }
        
        /* servo0 */
        if(psData.LEFT){
            angle_set(&servo[0], -80);
        } else if(psData.RIGHT){
            angle_set(&servo[0], 30);
        } else {
            angle_set(&servo[0], -27);
        }
        
        /* servo1 */
        if(psData.UP){
            angle_set(&servo[1], 20);
        } else if(psData.DOWN){
            //angle_set(&servo[1], -70);
        } else{
            angle_set(&servo[1], -70);
        }
        
        /* 技 */
        if(psData.CIRCLE){
            men(&pwm_data);
        } else if(psData.CROSS){
            hukamen(&pwm_data);
        } else if(psData.SQUARE){
            dou(&pwm_data);
        } else {
            pwm_data = 157;
        }
        /* servo2 */
        if(psData.R2){
            angle_set(&servo[2], 30);
        } else if((psData.CIRCLE != 1) && (psData.CROSS != 1) && (psData.SQUARE != 1)){
            angle_set(&servo[2], 110);
        }
        keep(&pwm_data);
    }
}

cystatus speed(void){
    cystatus speed;
    speed = speed_set(&servo[0]);
    if(speed == CYRET_EMPTY){
        return CYRET_EMPTY;
    }
    speed = speed_set(&servo[1]);
    if(speed == CYRET_EMPTY){
        return CYRET_EMPTY;
    }
    speed = speed_set(&servo[2]);
    if(speed == CYRET_EMPTY){
        return CYRET_EMPTY;
    }
    return CYRET_SUCCESS;
}

void free(uint16 *pwm_data) {
    angle_get(servo[0].id);
    angle_get(servo[1].id);
    angle_get(servo[2].id);
    PWM_WriteCompare(*pwm_data);
}

void kamae(uint16 *pwm_data) {
    angle_set(&servo[1], -70);
    angle_set(&servo[2], 110);
    PWM_WriteCompare(*pwm_data);
}

void men(uint16 *pwm_data) {
    angle_set(&servo[1], 45);
    angle_set(&servo[2], -20);
    PWM_WriteCompare(*pwm_data);
}

void hukamen(uint16 *pwm_data){
    angle_set(&servo[1], 75);
    angle_set(&servo[2], -20);
    PWM_WriteCompare(*pwm_data);
}

void dou(uint16 *pwm_data){
    angle_set(&servo[1], 45);
    angle_set(&servo[2], -5);
    PWM_WriteCompare(*pwm_data);
}

void keep(uint16 *pwm_data){
    angle_keep(&servo[0]);
    angle_keep(&servo[1]);
    angle_keep(&servo[2]);
    PWM_WriteCompare(*pwm_data);
}
/* [] END OF FILE */
