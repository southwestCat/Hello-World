#ifndef MOTOR_H
#define MOTOR_H
#include "stm32f10x.h"

#define PWMPeriod   10000

#define SERVO_MIDDLE 455
#define SERVO_LEFT 515
#define SERVO_RIGHT 395

#define MotorOneForward  0
#define MotorTwoForward  0
#define MotorOneBack     1
#define MotorTwoBack     1
#define MotorSpdZero     0
#define LEFT 1
#define RIGHT 2
#define MIDDLE 0

extern void TIM_Configuration(void);
extern void NVIC_Configuration(void);
extern uint32_t SPEED_DESIRE;
extern int car_func_mode,car_func_data;
extern u16 ServoDuty;

extern void Motor_PWM_ChangeDuty(s32 MotorSpeed);
extern void Servo_PWM_ChangeDuty(s32 MotorSpeed);
extern void UserMotorSpeedSetThr(s32 MotorSpeed);
extern void UserMotorSpeedSetFou(s32 MotorSpeed);
extern void Motor_PID_setSpeed(s32 speed_desire);

void car_start_test(void);
void close_change_speed(void);
void car_stop(void);
void Servo_ChangDuty(u16 del, u8 direction);


#endif


















