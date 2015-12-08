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

void mes(char buf[32]);

void Servo_Dataset(Servo_Data* servo, uint8 id, uint8 speed, uint8 stretch){
    servo->id = id;
    servo->speed = speed;
    servo->stretch = stretch;
    servo->state = STATE_SEND;
}

void EEPROM_rx(Servo_Data* servo){
    uint16 count = 0;
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

void read_ID(void){
    uint16 count = 0;
    uint8 i, id;
    unsigned char tx[4];
    unsigned char rx[5];
    char buf[32];
    tx[0] = 0xFF;
    tx[1] = 0x00;
    tx[2] = 0x00;
    tx[3] = 0x00;
    for(i = 0; i < 4; i++){
        UART_servo_PutChar(tx[i]);
    }
    while(UART_servo_GetRxBufferSize() < 5){
        count += 1;
        if(count > 500){
            count = 0;
            mes("error: read_id");
        }
        CyDelayUs(1);
    }
    for(i = 0; i < 5; i++){
        rx[i] = (unsigned char)UART_servo_GetChar();
    }
    CyDelay(500);
    id = (uint16) rx[4] & 0x1F;
    sprintf(buf, "id:%d\n", (int)id);
    UART_PC_PutString(buf);
    UART_servo_ClearRxBuffer();
}

cystatus init_ID(uint8 id){
    uint16 count = 0;
    uint8 i, rx_id;
    unsigned char tx[4];
    unsigned char rx[5];
    char buf[32];
    tx[0] = 0xE0 | id;
    tx[1] = 0x01;
    tx[2] = 0x01;
    tx[3] = 0x01;
    for(i = 0; i < 4; i++){
        UART_servo_PutChar(tx[i]);
    }
    while(UART_servo_GetRxBufferSize() < 5){
        count += 1;
        if(count > 500){
            count = 0;
            return CYRET_TIMEOUT;
        }
        CyDelayUs(1);
    }
    CyDelay(500);
    UART_servo_ClearRxBuffer();
    return CYRET_SUCCESS;
}

cystatus init_stretch(Servo_Data* servo){
    uint16 count = 0;
    uint8 i;
    unsigned char tx[3];
    tx[0] = 0xC0 | servo->id;
    tx[1] = 0x01;
    tx[2] = (unsigned char) servo->stretch;
    for(i = 0; i < 3; i++){
        UART_servo_PutChar(tx[i]);
    }
    while(UART_servo_GetRxBufferSize() < 6){
        count += 1;
        if(count > 500){
            count = 0;
            return CYRET_TIMEOUT;
        }
        CyDelayUs(1);
    }
    UART_servo_ClearRxBuffer();
    return CYRET_SUCCESS;
}

cystatus init_speed(Servo_Data* servo){
    uint16 count = 0;
    uint8 i;
    unsigned char tx[3];
    tx[0] = 0xC0 | servo->id;
    tx[1] = 0x02;
    tx[2] = (unsigned char) servo->speed;
    for(i = 0; i < 3; i++){
        UART_servo_PutChar(tx[i]);
    }
    while(UART_servo_GetRxBufferSize() < 6){
        count += 1;
        if(count > 500){
            count = 0;
            return CYRET_TIMEOUT;
        }
        CyDelayUs(1);
    }
    UART_servo_ClearRxBuffer();
    return CYRET_SUCCESS;
}

cystatus servo_serial(Servo_Data*, uint16 pos);
void data_send(uint8 id, uint16 pos);
cystatus data_wait(void);
int16 pos_get(void);

void angle_set(Servo_Data* servo, int16 angle){
    uint8 i;
    uint16 position;
    cystatus sts;
    position = (angle * 29.6296296296296296f) + 7500;
    sts = servo_serial(servo, position);
    if(sts == CYRET_SUCCESS){
        servo->angle = pos_get();
    } else if (sts == CYRET_TIMEOUT){
        mes("erorr: angle_set\n");
    }
}

void angle_get(Servo_Data *servo) {
    char buf[32];
    int16 pos;
    cystatus sts;
    sts = servo_serial(servo, 0);
    if(sts == CYRET_SUCCESS) {
        servo->angle = 0;
        pos = pos_get();
        if((pos >= -135) && (pos <= 135)) {
            sprintf(buf, "%d\n", pos);
            UART_PC_PutString(buf);
        }
    } else if (sts == CYRET_TIMEOUT) {
        mes("error: angle_get\n");
    }
}

cystatus servo_serial(Servo_Data *servo, uint16 pos) {
    cystatus wait;
    switch (servo->state) {
        case STATE_SEND: {
            data_send(servo->id, pos);
            servo->state = STATE_WAIT;
            break;
        }
        case STATE_WAIT: {
            wait = data_wait();
            if(wait == CYRET_EMPTY)
                return CYRET_EMPTY;
            if(wait == CYRET_TIMEOUT)
                return CYRET_TIMEOUT;
            servo->state = STATE_SEND;
        }
    }
    return CYRET_SUCCESS;
}

void data_send(uint8 id, uint16 pos) {
    uint8 i;
    unsigned char tx[3];
    tx[0] = 0x80 | id;
    tx[1] = (unsigned char)(pos >> 7) & 0x7F;
    tx[2] = (unsigned char) pos & 0x7F;
    UART_servo_PutArray(tx, 3);
}

cystatus data_wait(void) {
    uint16 count;
    if (UART_servo_GetRxBufferSize() < 6){
        count += 1;
        if(count > 500){
            count = 0;
            return CYRET_TIMEOUT;
        }
        CyDelayUs(1);
        return CYRET_EMPTY;
    }
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

void mes(char buf[32]){
    UART_PC_PutString(buf);
}
/* [] END OF FILE */
