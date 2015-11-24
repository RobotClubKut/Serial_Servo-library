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

void Servo_Dataset(Servo_Data* servo, uint8 id, uint8 speed, uint8 stretch){
    servo->id = id;
    servo->speed = speed;
    servo->stretch = stretch;
}

void init_ID(Servo_Data* servo){
    uint8 i;
    unsigned char write_id[4];
    write_id[0] = ID_CMD | servo->id;
    write_id[1] = 0x01;
    write_id[2] = 0x01;
    write_id[3] = 0x01;
    for(i = 0; i < 4; i++){
        UART_servo_PutChar(write_id[i]);
    }
    while(UART_servo_GetRxBufferSize() < 5);
    UART_servo_ClearRxBuffer();
}

void init_stretch(Servo_Data* servo){
    uint8 i;
    unsigned char write_speed[3];
    write_speed[0] = SPD_CMD | servo->id;
    write_speed[1] = 0x01;
    write_speed[2] = (unsigned char) servo->stretch;
    for(i = 0; i < 3; i++){
        UART_servo_PutChar(write_speed[i]);
    }
    while(UART_servo_GetRxBufferSize() < 6);
    UART_servo_ClearRxBuffer();
}

void init_speed(Servo_Data* servo){
    uint8 i;
    unsigned char write_speed[3];
    write_speed[0] = SPD_CMD | servo->id;
    write_speed[1] = 0x02;
    write_speed[2] = (unsigned char) servo->speed;
    for(i = 0; i < 3; i++){
        UART_servo_PutChar(write_speed[i]);
    }
    while(UART_servo_GetRxBufferSize() < 6);
    UART_servo_ClearRxBuffer();
}

void servo_set(Servo_Data* servo, int16 angle, uint8 pf){
    uint8 i;
    uint16 pos, psition = 0;
    int16 ang = 0;
    char buf[32];
    unsigned char tx[3];
    unsigned char rx[6];
    
    if(pf == 1){
        psition = (angle * 29.6296296296296296f) + 7500;
        tx[0] = POS_CMD | servo->id;
        tx[1] = (unsigned char)(psition >> 7) & 0x7F;
        tx[2] = (unsigned char) psition & 0x7F;
        for(i = 0; i < 3; i++){
            UART_servo_PutChar(tx[i]);
        }
        while(UART_servo_GetRxBufferSize() < 6);
        UART_servo_ClearRxBuffer();
    } else {
        psition = angle * 0;
        tx[0] = POS_CMD | servo->id;
        tx[1] = (unsigned char)(psition >> 7) & 0x7F;
        tx[2] = (unsigned char) psition & 0x7F;
        for(i = 0; i < 3; i++){
            UART_servo_PutChar(tx[i]);
        }
        while(UART_servo_GetRxBufferSize() < 6);
        for(i = 0; i < 6; i++){
            rx[i] = (unsigned char) UART_servo_GetChar();
        }
        UART_servo_ClearRxBuffer();
        pos = (((uint16)rx[4] & 0x7f) << 7) | ((uint16)rx[5] & 0x7f);
        ang = ((int16)pos - 7500) / 29.6296296296296296f;
        if(ang < -135 || ang > 135){
            sprintf(buf, "Position Error\n");
        }else {
            sprintf(buf, "%d\n", ang);
        }
        UART_Debug_PutString(buf);
    }
}

/* [] END OF FILE */
