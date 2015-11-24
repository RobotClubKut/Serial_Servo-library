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
#ifndef SERVO_H
#define SERVO_H

#include <project.h>

#define POS_CMD 0x80
#define SPD_CMD 0xC0
#define ID_CMD  0xE0

typedef struct{
    uint8 id;
    uint8 speed;
    uint8 stretch;
}Servo_Data;
void Servo_Dataset(Servo_Data*, uint8 id, uint8 speed, uint8 stretch);
void init_ID(Servo_Data*);
void init_stretch(Servo_Data*);
void init_speed(Servo_Data*);
void servo_set(Servo_Data*, int16 angle, uint8 power);

#endif /* CYAPICALLBACKS_H */   
/* [] END OF FILE */
