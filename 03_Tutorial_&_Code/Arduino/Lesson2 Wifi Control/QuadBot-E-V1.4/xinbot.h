#ifndef xinbot_h
#define xinbot_h

#include <Arduino.h>
#include <Servo.h>
#include "actionarray.h"

class XinBot{
public:
    Servo GPIO14SERVO;
    Servo GPIO12SERVO;
    Servo GPIO13SERVO;
    Servo GPIO15SERVO;
    Servo GPIO16SERVO;
    Servo GPIO5SERVO;
    Servo GPIO4SERVO;
    Servo GPIO2SERVO;

    void Servo_PROGRAM_Zero();
    void Set_PWM_to_Servo(int iServo, int iValue);
    void Servo_PROGRAM_Run(const int iMatrix[][ALLMATRIX], int iSteps);
    void Servo_PROGRAM_Center();
    void Servo_Setup();

    void standby();
    void forward();
    void backward();
    void leftshift();
    void rightshift();
    void turnleft();
    void turnright();
    void lie();
    void hello();
    void fighting();
    void pushup();
    void sleep();
    void dance1();
    void dance2();
    void dance3();
    void center();
    void zero();

    void init();
    void calibration();
    void run(float steps, int period);
    void walk(float steps, int period);
    void omniWalk(float steps, int T, bool side, float turn_factor);
    //void backward(float steps, int period);
    void turnL(float steps, int period);
    void turnR(float steps, int period);
    void moonwalkL(float steps, int period);
    void dance(float steps, int period);
    void upDown(float steps, int period);
    void pushUp(float steps, int period);
    void hello1();
    void jump();
    void home();
    void zero1();
    void frontBack(float steps, int period);

    void setServo(int id, float target);
    void reverseServo(int id);
    float getServo(int id);
    void moveServos(int time, float target[8]);

private:
    // Oscillator oscillator[8];
    // Servo servo[8];

    int board_pins[8];
    int trim[8];
    bool reverse[8];
    unsigned long _init_time;
    unsigned long _final_time;
    unsigned long _partial_time;
    float _increment[8];
    float _servo_position[8];

    int angToUsec(float value);
    void execute(float steps, int period[8], int amplitude[8], int offset[8], int phase[8]);
};

#endif
