#include "xinbot.h"
#include <EEPROM.h>

// Backup servo value
int Running_Servo_POS [ALLMATRIX];
// Servo delay base time
int BASEDELAYTIME = 10; // 10 ms

void XinBot::Servo_PROGRAM_Zero()
{
	// 清除备份目前舵机数值
	for (int Index = 0; Index < ALLMATRIX; Index++) 
	{
		Running_Servo_POS[Index] = Servo_Act_0[Index];
	}

  	// 重新载入舵机预设数值
  	for (int iServo = 0; iServo < ALLSERVOS; iServo++) 
	{
		Set_PWM_to_Servo(iServo, Running_Servo_POS[iServo]);
		delay(10);
	}
}

void XinBot::Set_PWM_to_Servo(int iServo, int iValue)
{
//	int NewPWM = iValue + Servo_Offset[iServo];
//  int8_t eepromValue = (int8_t)EEPROM.read(iServo);
//  Serial.print("iServo="); Serial.println(iServo);
//  Serial.print("eepromValue="); Serial.println(eepromValue);
//  Serial.print("iValue="); Serial.println(iValue);
  int NewPWM = iValue + (int8_t)EEPROM.read(iServo);
//  Serial.print("NewPWM="); Serial.println(NewPWM);

  NewPWM = map(NewPWM, PWMRES_Min, PWMRES_Max, SERVOMIN, SERVOMAX);
//  Serial.print("NewPWM="); Serial.println(NewPWM);

	if (iServo >= 7) {
		GPIO2SERVO.write(NewPWM);
	} else if (iServo >= 6) {
		GPIO4SERVO.write(NewPWM);
	} else if (iServo >= 5) {
		GPIO5SERVO.write(NewPWM);
	} else if (iServo >= 4) {
		GPIO16SERVO.write(NewPWM);
	} else if (iServo >= 3) {
		GPIO15SERVO.write(NewPWM);
	} else if (iServo >= 2) {
		GPIO13SERVO.write(NewPWM);
	} else if (iServo >= 1) {
		GPIO12SERVO.write(NewPWM);
	} else if (iServo == 0) {
		GPIO14SERVO.write(NewPWM);
	}
}

// void XinBot::Servo_PROGRAM_Run(int iMatrix[][ALLMATRIX], int iSteps)
void XinBot::Servo_PROGRAM_Run(const int iMatrix[][ALLMATRIX], int iSteps)
{
    int INT_TEMP_A, INT_TEMP_B, INT_TEMP_C;

    for (int MainLoopIndex = 0; MainLoopIndex < iSteps; MainLoopIndex++) // iSteps 步骤主循环
    {
        int InterTotalTime = iMatrix[MainLoopIndex][ALLMATRIX - 1]; // InterTotalTime 此步骤总时间

        int InterDelayCounter = InterTotalTime / BASEDELAYTIME; // InterDelayCounter 此步骤基本延迟次数

        for (int InterStepLoop = 0; InterStepLoop < InterDelayCounter; InterStepLoop++) // 内插值循环
        {
            for (int ServoIndex = 0; ServoIndex < ALLSERVOS; ServoIndex++) // 舵机主循环
            {
                INT_TEMP_A = Running_Servo_POS[ServoIndex]; // 舵机现在的位置
                INT_TEMP_B = iMatrix[MainLoopIndex][ServoIndex]; // 舵机的目标位置
 
                if (INT_TEMP_A == INT_TEMP_B) // 舵机数值不变
                {
                    INT_TEMP_C = INT_TEMP_B;
                } 
                else if (INT_TEMP_A > INT_TEMP_B) // 舵机数值减少
                {
                    INT_TEMP_C =  map(BASEDELAYTIME * InterStepLoop, 0, InterTotalTime, 0, INT_TEMP_A - INT_TEMP_B); // PWM內差值 = map(执行次数时间累加, 开始时间, 結束时间, 內差起始值, 內差最大值)
                    if (INT_TEMP_A - INT_TEMP_C >= INT_TEMP_B) 
                    {
                        Set_PWM_to_Servo(ServoIndex, INT_TEMP_A - INT_TEMP_C);
                    }
                } 
                else if (INT_TEMP_A < INT_TEMP_B) // 舵机数值增加
                {
                    INT_TEMP_C =  map(BASEDELAYTIME * InterStepLoop, 0, InterTotalTime, 0, INT_TEMP_B - INT_TEMP_A); // PWM內差值 = map(执行次数时间累加, 开始时间, 結束时间, 內差起始值, 內差最大值)
                    if (INT_TEMP_A + INT_TEMP_C <= INT_TEMP_B) {
                    Set_PWM_to_Servo(ServoIndex, INT_TEMP_A + INT_TEMP_C);
                    }
                }
            }

            delay(BASEDELAYTIME);
        }

        // 备份当前舵机数值
        for (int Index = 0; Index < ALLMATRIX; Index++) 
        {
            Running_Servo_POS[Index] = iMatrix[MainLoopIndex][Index];
        }
    }
}

void XinBot::Servo_PROGRAM_Center()
{
    // 清除备份当前舵机数值
    for (int Index = 0; Index < ALLMATRIX; Index++) 
    {
        Running_Servo_POS[Index] = Servo_Act_1[Index];
    }

    // 重新载入舵机预设数值
    for (int iServo = 0; iServo < ALLSERVOS; iServo++) 
    {
        Set_PWM_to_Servo(iServo, Running_Servo_POS[iServo]);
        delay(10);
    }
}

void XinBot::init()
{
    // // Map between servos and board pins
    // board_pins[0] = D1; // Servo S0
    // board_pins[1] = D4; // Servo S1
    // board_pins[2] = D8; // Servo S2
    // board_pins[3] = D6; // Servo S3
    // board_pins[4] = D7; // Servo S4
    // board_pins[5] = D5; // Servo S5
    // board_pins[6] = D2; // Servo S6
    // board_pins[7] = D3; // Servo S7

    // // Trim values for zero position calibration.
    // trim[0] = 0;
    // trim[1] = -8;
    // trim[2] = 8;
    // trim[3] = 5;
    // trim[4] = 2;
    // trim[5] = -6;
    // trim[6] = 6;
    // trim[7] = 5;

    // // Set reverse movement
    // for (int i=0; i<8; i++) reverse[i] = false;

    // // Init an oscillator for each servo
    // for(int i=0; i<8; i++){
    //     oscillator[i].start();
    //     servo[i].attach(board_pins[i]);
    // }
    // zero();

    // Software PWM PIN
  	// GPIO14SERVO.attach(15, SERVOMIN, SERVOMAX);
  	// GPIO12SERVO.attach(13, SERVOMIN, SERVOMAX);
	// GPIO13SERVO.attach(12, SERVOMIN, SERVOMAX);
	// GPIO15SERVO.attach(14, SERVOMIN, SERVOMAX);
	// GPIO16SERVO.attach(2, SERVOMIN, SERVOMAX);
	// GPIO5SERVO.attach(4, SERVOMIN, SERVOMAX);
	// GPIO4SERVO.attach(5, SERVOMIN, SERVOMAX);
	// GPIO2SERVO.attach(16, SERVOMIN, SERVOMAX);
    GPIO14SERVO.attach(14, SERVOMIN, SERVOMAX);
  	GPIO12SERVO.attach(12, SERVOMIN, SERVOMAX);
	GPIO13SERVO.attach(13, SERVOMIN, SERVOMAX);
	GPIO15SERVO.attach(15, SERVOMIN, SERVOMAX);
	GPIO16SERVO.attach(16, SERVOMIN, SERVOMAX);
	GPIO5SERVO.attach(5, SERVOMIN, SERVOMAX);
	GPIO4SERVO.attach(4, SERVOMIN, SERVOMAX);
	GPIO2SERVO.attach(2, SERVOMIN, SERVOMAX);

//     // 清除备份目前舵机数值
//	 for (int Index = 0; Index < ALLMATRIX; Index++) 
//	 {
//	 	Running_Servo_POS[Index] = Servo_Act_0[Index];
//	 }

    // // 自动归零 增加组装便利性
    // Servo_PROGRAM_Zero();
}

void XinBot::Servo_Setup()
{
    GPIO14SERVO.write(Servo_Act_0[0]);
    GPIO12SERVO.write(Servo_Act_0[1]);
    GPIO13SERVO.write(Servo_Act_0[2]);
    GPIO15SERVO.write(Servo_Act_0[3]);
    GPIO16SERVO.write(Servo_Act_0[4]);
    GPIO5SERVO.write(Servo_Act_0[5]);
    GPIO4SERVO.write(Servo_Act_0[6]);
    GPIO2SERVO.write(Servo_Act_0[7]);
}

void XinBot::calibration()
{
    while (Serial.available() > 0) 
	{
		char command = Serial.read();

		// SERVO Set command
		if ( command == 'S' || command == 's' )  // 'S' is Servo Command "S,Servo_no,Angle"
		{
			Serial.print(command);
			Serial.print(',');
			int servoNo = Serial.parseInt();
			Serial.print(servoNo);
			Serial.print(',');
			int servoAngle = Serial.parseInt();
			Serial.print(servoAngle);
			Serial.println();

			switch (servoNo)
			{
				case 14:
					GPIO14SERVO.write(Servo_Act_0[0] + servoAngle);
					break;

				case 12:
					GPIO12SERVO.write(Servo_Act_0[1] + servoAngle);
					break;

				case 13:
					GPIO13SERVO.write(Servo_Act_0[2] + servoAngle);
					break;

				case 15:
					GPIO15SERVO.write(Servo_Act_0[3] + servoAngle);
					break;

				case 16:
					GPIO16SERVO.write(Servo_Act_0[4] + servoAngle);
					break;

				case 5:
					GPIO5SERVO.write(Servo_Act_0[5] + servoAngle);
					break;

				case 4:
					GPIO4SERVO.write(Servo_Act_0[6] + servoAngle);
					break;

				case 2:
					GPIO2SERVO.write(Servo_Act_0[7] + servoAngle);
					break;
				
				default:
					break;
			}
		}  // SERVO Set END
	}
}

void XinBot::standby()
{
    Servo_PROGRAM_Run(Servo_Prg_1, Servo_Prg_1_Step);
}

void XinBot::forward()
{
    Servo_PROGRAM_Run(Servo_Prg_2, Servo_Prg_2_Step);
}

void XinBot::backward()
{
    Servo_PROGRAM_Run(Servo_Prg_3, Servo_Prg_3_Step);
}

void XinBot::leftshift()
{
    Servo_PROGRAM_Run(Servo_Prg_4, Servo_Prg_4_Step);
}

void XinBot::rightshift()
{
    Servo_PROGRAM_Run(Servo_Prg_5, Servo_Prg_5_Step);
}

void XinBot::turnleft()
{
    Servo_PROGRAM_Run(Servo_Prg_6, Servo_Prg_6_Step);
}

void XinBot::turnright()
{
    Servo_PROGRAM_Run(Servo_Prg_7, Servo_Prg_7_Step);
}

void XinBot::lie()
{
    Servo_PROGRAM_Run(Servo_Prg_8, Servo_Prg_8_Step);
}

void XinBot::hello()
{
    Servo_PROGRAM_Run(Servo_Prg_9, Servo_Prg_9_Step);
	Servo_PROGRAM_Run(Servo_Prg_1, Servo_Prg_1_Step);
}

void XinBot::fighting()
{
    Servo_PROGRAM_Run(Servo_Prg_10, Servo_Prg_10_Step);
}

void XinBot::pushup()
{
    Servo_PROGRAM_Run(Servo_Prg_11, Servo_Prg_11_Step);
}

void XinBot::sleep()
{
    Servo_PROGRAM_Run(Servo_Prg_1, Servo_Prg_1_Step);
	Servo_PROGRAM_Run(Servo_Prg_12, Servo_Prg_12_Step);
}

void XinBot::dance1()
{
    Servo_PROGRAM_Run(Servo_Prg_13, Servo_Prg_13_Step);
}

void XinBot::dance2()
{
    Servo_PROGRAM_Run(Servo_Prg_14, Servo_Prg_14_Step);
}

void XinBot::dance3()
{
    Servo_PROGRAM_Run(Servo_Prg_15, Servo_Prg_15_Step);
}

void XinBot::center()
{
    Servo_PROGRAM_Center();
	delay(300);
}

void XinBot::zero()
{
    Servo_PROGRAM_Zero();
	delay(300);
}



void XinBot::turnR(float steps, int T=600)
{
    // int x_amp = 15;
    // int z_amp = 15;
    // int ap = 15;
    // int hi = 23;
    // int period[] = {T, T, T, T, T, T, T, T};
    // int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    // int offset[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi};
    // int phase[] = {0,180,90,90,180,0,90,90};

    // execute(steps, period, amplitude, offset, phase);
}

void XinBot::turnL(float steps, int T=600)
{
    // int x_amp = 15;
    // int z_amp = 15;
    // int ap = 15;
    // int hi = 23;
    // int period[] = {T, T, T, T, T, T, T, T};
    // int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    // int offset[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi};
    // int phase[] = {180,0,90,90,0,180,90,90};

    // execute(steps, period, amplitude, offset, phase);
}

void XinBot::dance(float steps, int T=600)
{
    // int x_amp = 0;
    // int z_amp = 40;
    // int ap = 30;
    // int hi = 20;
    // int period[] = {T, T, T, T, T, T, T, T};
    // int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    // int offset[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi};
    // int phase[] = {0,0,0,270,0,0,90,180};

    // execute(steps, period, amplitude, offset, phase);
}

void XinBot::frontBack(float steps, int T=600)
{
    // int x_amp = 30;
    // int z_amp = 25;
    // int ap = 20;
    // int hi = 30;
    // int period[] = {T, T, T, T, T, T, T, T};
    // int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    // int offset[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi};
    // int phase[] = {0,180,270,90,0,180,90,270};

    // execute(steps, period, amplitude, offset, phase);
}

void XinBot::run(float steps, int T=5000)
{
    // int x_amp = 15;
    // int z_amp = 15;
    // int ap = 15;
    // int hi = 15;
    // int front_x = 6;
    // int period[] = {T, T, T, T, T, T, T, T};
    // int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    // int offset[] = {    90+ap-front_x,
    //                     90-ap+front_x,
    //                     90-hi,
    //                     90+hi,
    //                     90-ap-front_x,
    //                     90+ap+front_x,
    //                     90+hi,
    //                     90-hi
    //                 };
    // int phase[] = {0,0,90,90,180,180,90,90};

    // execute(steps, period, amplitude, offset, phase);
}

void XinBot::omniWalk(float steps, int T, bool side, float turn_factor)
{
    // int x_amp = 15;
    // int z_amp = 15;
    // int ap = 15;
    // int hi = 23;
    // int front_x = 6 * (1-pow(turn_factor, 2));
    // int period[] = {T, T, T, T, T, T, T, T};
    // int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    // int offset[] = {    90+ap-front_x,
    //                     90-ap+front_x,
    //                     90-hi,
    //                     90+hi,
    //                     90-ap-front_x,
    //                     90+ap+front_x,
    //                     90+hi,
    //                     90-hi
    //                 };

    // int phase[8];
    // if (side){
    //     int phase1[] =  {0,   0,   90,  90,  180, 180, 90,  90};
    //     int phase2R[] = {0,   180, 90,  90,  180, 0,   90,  90};
    //     for (int i=0; i<8; i++)
    //         phase[i] = phase1[i]*(1-turn_factor) + phase2R[i]*turn_factor;
    // }
    // else{
    //     int phase1[] =  {0,   0,   90,  90,  180, 180, 90,  90};
    //     int phase2L[] = {180, 0,   90,  90,  0,   180, 90,  90};
    //     for (int i=0; i<8; i++)
    //         phase[i] = phase1[i]*(1-turn_factor) + phase2L[i]*turn_factor + oscillator[i].getPhaseProgress();
    // }

    // execute(steps, period, amplitude, offset, phase);
}

void XinBot::moonwalkL(float steps, int T=5000)
{
    // int z_amp = 45;
    // int period[] = {T, T, T, T, T, T, T, T};
    // int amplitude[] = {0,0,z_amp,z_amp,0,0,z_amp,z_amp};
    // int offset[] = {90, 90, 90, 90, 90, 90, 90, 90};
    // int phase[] = {0,0,0,120,0,0,180,290};

    // execute(steps, period, amplitude, offset, phase);
}

void XinBot::walk(float steps, int T=5000)
{
    // int x_amp = 15;
    // int z_amp = 20;
    // int ap = 20;
    // int hi = 10;
    // int front_x = 12;
    // int period[] = {T, T, T/2, T/2, T, T, T/2, T/2};
    // int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    // int offset[] = {   90+ap-front_x,
    //                             90-ap+front_x,
    //                             90-hi,
    //                             90+hi,
    //                             90-ap-front_x,
    //                             90+ap+front_x,
    //                             90+hi,
    //                             90-hi
    //                 };
    // int  phase[] = {90, 90, 270, 90, 270, 270, 90, 270};

    // for (int i=0; i<8; i++)
    // {
    //     oscillator[i].reset();
    //     oscillator[i].setPeriod(period[i]);
    //     oscillator[i].setAmplitude(amplitude[i]);
    //     oscillator[i].setPhase(phase[i]);
    //     oscillator[i].setOffset(offset[i]);
    // }

    // _final_time = millis() + period[0]*steps;
    // _init_time = millis();
    // bool side;
    // while (millis() < _final_time)
    // {
    //     side = (int)((millis()-_init_time) / (period[0]/2)) % 2;
    //     setServo(0, oscillator[0].refresh());
    //     setServo(1, oscillator[1].refresh());
    //     setServo(4, oscillator[4].refresh());
    //     setServo(5, oscillator[5].refresh());

    //     if (side == 0){
    //         setServo(3, oscillator[3].refresh());
    //         setServo(6, oscillator[6].refresh());
    //     }
    //     else{
    //         setServo(2, oscillator[2].refresh());
    //         setServo(7, oscillator[7].refresh());
    //     }
    //     delay(1);
    // }
}

void XinBot::upDown(float steps, int T=5000)
{
    // int x_amp = 0;
    // int z_amp = 35;
    // int ap = 20;
    // int hi = 25;
    // int front_x = 0;
    // int period[] = {T, T, T, T, T, T, T, T};
    // int amplitude[] = {x_amp,x_amp,z_amp,z_amp,x_amp,x_amp,z_amp,z_amp};
    // int offset[] = {    90+ap-front_x,
    //                     90-ap+front_x,
    //                     90-hi,
    //                     90+hi,
    //                     90-ap-front_x,
    //                     90+ap+front_x,
    //                     90+hi,
    //                     90-hi
    //                 };
    // int phase[] = {0,0,90,270,180,180,270,90};

    // execute(steps, period, amplitude, offset, phase);
}


void XinBot::pushUp(float steps, int T=600)
{
    // int z_amp = 40;
    // int x_amp = 65;
    // int hi = 30;
    // int period[] = {T, T, T, T, T, T, T, T};
    // int amplitude[] = {0,0,z_amp,z_amp,0,0,0,0};
    // int offset[] = {90,90,90-hi,90+hi,90-x_amp,90+x_amp,90+hi,90-hi};
    // int phase[] = {0,0,0,180,0,0,0,180};

    // execute(steps, period, amplitude, offset, phase);
}

void XinBot::hello1()
{
    // float sentado[]={90+15,90-15,90-65,90+65,90+20,90-20,90+10,90-10};
    // moveServos(150, sentado);
    // delay(200);

    // int z_amp = 40;
    // int x_amp = 60;
    // int T=350;
    // int period[] = {T, T, T, T, T, T, T, T};
    // int amplitude[] = {0,50,0,50,0,0,0,0};
    // int offset[] = {90+15,40,90-65,90,90+20,90-20,90+10,90-10};
    // int phase[] = {0,0,0,90,0,0,0,0};

    // execute(4, period, amplitude, offset, phase);

    // float goingUp[]={160,20,90,90,90-20,90+20,90+10,90-10};
    // moveServos(500, goingUp);
    // delay(200);
}



void XinBot::jump()
{
    // float sentado[]={90+15,90-15,90-65,90+65,90+20,90-20,90+10,90-10};
    // float ap = 20.0;
    // float hi = 35.0;
    // float salto[] = {90+ap,90-ap,90-hi,90+hi,90-ap*3,90+ap*3,90+hi,90-hi};
    // moveServos(150, sentado);
    // delay(200);
    // moveServos(0, salto);
    // delay(100);
    // home();
}

void XinBot::home()
{
    // int ap = 20;
    // int hi = 35;
    // int position[] = {90+ap,90-ap,90-hi,90+hi,90-ap,90+ap,90+hi,90-hi};
    // for (int i=0; i<8; i++) setServo(i, position[i]);
}

void XinBot::zero1()
{
    // for (int i=0; i<8; i++) setServo(i, 90);
}

void XinBot::reverseServo(int id)
{
    // if (reverse[id])
    //     reverse[id] = 0;
    // else
    //     reverse[id] = 1;
}


void XinBot::setServo(int id, float target)
{
    // if (!reverse[id])
    //     servo[id].writeMicroseconds(angToUsec(target+trim[id]));
    // else
    //     servo[id].writeMicroseconds(angToUsec(180-(target+trim[id])));
    // _servo_position[id] = target;
}

float XinBot::getServo(int id)
{
    // return _servo_position[id];
}


void XinBot::moveServos(int time, float target[8]) 
{
    // if (time>10)
    // {
    //     for (int i = 0; i < 8; i++)	_increment[i] = (target[i] - _servo_position[i]) / (time / 10.0);
    //     _final_time =  millis() + time;

    //     while (millis() < _final_time)
    //     {
    //         _partial_time = millis() + 10;
    //         for (int i = 0; i < 8; i++) setServo(i, _servo_position[i] + _increment[i]);
    //         while (millis() < _partial_time); //pause
    //     }
    // }
    // else
    // {
    //     for (int i = 0; i < 8; i++) setServo(i, target[i]);
    // }
    // for (int i = 0; i < 8; i++) _servo_position[i] = target[i];
}

void XinBot::execute(float steps, int period[8], int amplitude[8], int offset[8], int phase[8])
{
    // for (int i=0; i<8; i++)
    // {
    //     oscillator[i].setPeriod(period[i]);
    //     oscillator[i].setAmplitude(amplitude[i]);
    //     oscillator[i].setPhase(phase[i]);
    //     oscillator[i].setOffset(offset[i]);
    // }

    // unsigned long global_time = millis();

    // for (int i=0; i<8; i++) oscillator[i].setTime(global_time);

    // _final_time = millis() + period[0]*steps;
    // while (millis() < _final_time)
    // {
    //     for (int i=0; i<8; i++)
    //     {
    //         setServo(i, oscillator[i].refresh());
    //     }
    //     yield();
    // }
}

int XinBot::angToUsec(float value)
{
    // return value/180 * (MAX_PULSE_WIDTH-MIN_PULSE_WIDTH) + MIN_PULSE_WIDTH;
}
