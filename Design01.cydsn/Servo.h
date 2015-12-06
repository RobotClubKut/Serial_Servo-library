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
/*
init_IDとread_IDは連続して使用できない
サーボの種類が違う時、数珠繋ぎで動かない場合あり
*/
#ifndef SERVO_H
#define SERVO_H

#include <project.h>
typedef struct{
    uint8 id;
    uint8 speed;
    uint8 stretch;
    uint8 state;
    int angle;
} Servo_Data;
void Servo_Dataset(Servo_Data*, uint8 id, uint8 speed, uint8 stretch);
void EEPROM_rx(Servo_Data*);
void init_ID(Servo_Data*);
void read_ID(void);
void init_stretch(Servo_Data*);
void init_speed(Servo_Data*);
void angle_set(Servo_Data*, int16 angle);
void angle_get(Servo_Data*);

#endif /* CYAPICALLBACKS_H */   
/* [] END OF FILE */
