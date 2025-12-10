#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "sys.h"



#define MotorA1	PCout(5)
#define MotorA2	PCout(4)

#define MotorB1	PCout(3)
#define MotorB2	PCout(2)

#define MotorC1	PBout(5)
#define MotorC2	PBout(4)

#define MotorD1	PCout(8)
#define MotorD2	PCout(9)

void Motor_Init(void);//电机PWM初始化并设置电机的刷新速率50-499
void Set_Motor(int MotorA,int MotorB,int MotorC,int MotorD);

#endif

/*************************************END************************************/



