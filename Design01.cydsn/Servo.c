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

#define STATE_SEND 0
#define STATE_WAIT 1
#define STATE_GET 2

cystatus servo_serial(Servo_Data*);
void data_send(uint8 id);
cystatus data_wait(void);
int16 pos_get(void);

void Servo_Dataset(Servo_Data* servo, uint8 id, uint8 speed, uint8 stretch){
    servo->id = id;
    servo->speed = speed;
    servo->stretch = stretch;
    servo->state = STATE_SEND;
}

void EEPROM_rx(Servo_Data* servo){
    uint8 i;
    unsigned char tx[2];
    unsigned char rx[68];
    char buf[32];
    tx[0] = 0xA0 | servo->id;
    tx[1] = 0x00;
    for(i = 0; i < 2; i++){
            UART_servo_PutChar(rx[i]);
    }
    for(i = 0; i < 68; i++){
        rx[i] = (unsigned char)UART_servo_GetChar();
    }
    UART_servo_ClearRxBuffer();
    for(i = 4; i < 68; i++){
        sprintf(buf, "%3d:0x%x\n", (int)(i - 3), (int)rx[i]);
        UART_PC_PutString(buf);
    }
}

void init_ID(Servo_Data* servo){
    uint8 i, id;
    unsigned char tx[4];
    unsigned char rx[5];
    char buf[32];
    tx[0] = 0xE0 | (uint16)servo->id;
    tx[1] = 0x01;
    tx[2] = 0x01;
    tx[3] = 0x01;
    for(i = 0; i < 4; i++){
        UART_servo_PutChar(tx[i]);
    }
    while(UART_servo_GetRxBufferSize() < 5);
    for(i = 0; i < 5; i++){
        rx[i] = (unsigned char) UART_servo_GetChar();
    }
    id = (uint16) rx[4] & 0x1F;
    sprintf(buf, "id:%d\n", (int)id);
    UART_PC_PutString(buf);
    UART_servo_ClearRxBuffer();
}

void read_ID(void){
    uint8 i, id;
    unsigned char tx[4];
    unsigned char rx[5];
    char buf[40];
    tx[0] = 0xFF;
    tx[1] = 0x00;
    tx[2] = 0x00;
    tx[3] = 0x00;
    for(i = 0; i < 4; i++){
        UART_servo_PutChar(tx[i]);
    }
    while(UART_servo_GetRxBufferSize() < 5);
    for(i = 0; i < 5; i++){
        rx[i] = (unsigned char)UART_servo_GetChar();
    }
    UART_servo_ClearRxBuffer();
    id = (uint16) rx[4] & 0x1F;
    sprintf(buf, "id:%d\n", (int)id);
    UART_PC_PutString(buf);
}

void init_stretch(Servo_Data* servo){
    uint8 i;
    unsigned char tx[3];
    tx[0] = 0xC0 | servo->id;
    tx[1] = 0x01;
    tx[2] = (unsigned char) servo->stretch;
    for(i = 0; i < 3; i++){
        UART_servo_PutChar(tx[i]);
    }
    UART_servo_ClearRxBuffer();
}

void init_speed(Servo_Data* servo){
    uint8 i;
    unsigned char tx[3];
    tx[0] = 0xC0 | servo->id;
    tx[1] = 0x02;
    tx[2] = (unsigned char) servo->speed;
    for(i = 0; i < 3; i++){
        UART_servo_PutChar(tx[i]);
    }
    UART_servo_ClearRxBuffer();
}

void angle_set(Servo_Data* servo, int16 angle){
    uint8 i;
    uint16 psition;
    unsigned char tx[3];
    psition = (angle * 29.6296296296296296f) + 7500;
    tx[0] = 0x80 | servo->id;
    tx[1] = (unsigned char)(psition >> 7) & 0x7F;
    tx[2] = (unsigned char) psition & 0x7F;
    UART_servo_PutArray(tx, 3);
    CyDelay(1);
}

void angle_get(Servo_Data *servo) {
    char buf[32];
    cystatus sts;
    sts = servo_serial(servo);
    if(sts == CYRET_SUCCESS) {
        servo->angle = pos_get();
        if((servo->angle >= -135) && (servo->angle <= 135)) {
            sprintf(buf, "%d\n", servo->angle);
            UART_PC_PutString(buf);
        }
    } else if (sts == CYRET_EMPTY) {
        sprintf(buf, "error: send\n");
        UART_PC_PutString(buf);
    }
}

cystatus servo_serial(Servo_Data *servo) {
    switch (servo->state) {
        case STATE_SEND: {
            data_send(servo->id);
            servo->state = STATE_WAIT;
            break;
        }
        case STATE_WAIT: {
            if(data_wait() == CYRET_EMPTY)
                return CYRET_EMPTY;
            servo->state = STATE_SEND;
        }
    }
    return CYRET_SUCCESS;
}

void data_send(uint8 id) {
    uint8 i;
    unsigned char tx[3];
    tx[0] = 0x80 | id;
    tx[1] = 0x00;
    tx[2] = 0x00;
    for(i = 0; i < 3; i++){
        UART_servo_PutChar(tx[i]);
    }
}

cystatus data_wait(void) {
    char buf[32];
    if (UART_servo_GetRxBufferSize() < 6)
        return CYRET_EMPTY;
    return CYRET_SUCCESS;
}

int16 pos_get(void) {
    uint8 i;
    uint16 pos;
    int16 angle;
    unsigned char rx[6];
    for(i = 0; i < 6; i++){
        rx[i] = (unsigned char) UART_servo_GetChar();
    }
    UART_servo_ClearRxBuffer();
    pos = (((uint16)rx[4] & 0x7f) << 7) | ((uint16)rx[5] & 0x7f);
    angle = ((int16)pos - 7500) / 29.6296296296296296f;
    return angle;
}
/*
cystatus angle_get(Servo_Data* servo){
    static uint8 state = 0;
    uint8 i;
    uint16 pos;
    int16 ang;
    
    char buf[32];
    unsigned char tx[3];
    unsigned char rx[6];
    tx[0] = 0x80 | servo->id;
    tx[1] = 0x00;
    tx[2] = 0x00;
    for(i = 0; i < 3; i++){
        UART_servo_PutChar(tx[i]);
    }
    
    if (UART_servo_GetRxBufferSize() < 6)
        return CYRET_EMPTY;
    
    for(i = 0; i < 6; i++){
        rx[i] = (unsigned char) UART_servo_GetChar();
    }
    UART_servo_ClearRxBuffer();
    pos = (((uint16)rx[4] & 0x7f) << 7) | ((uint16)rx[5] & 0x7f);
    ang = ((int16)pos - 7500) / 29.6296296296296296f;
    if((ang >= -135) && (ang <= 135)){
        sprintf(buf, "%d\n", ang);
        UART_PC_PutString(buf);
    }
    return CYRET_SUCCESS; 
}
*/
/* [] END OF FILE */
