//            头
//   ----             ----
// | 02/4 |         | 15/8 |
//   ----  --------   ----
//       | 04/2  13/7 |
//       |            |
//       | 05/1  12/6 |
//   ----  --------   ----
// | 16/0 |         | 14/5 |
//   ----             ----

#include <Servo.h>
#include <EEPROM.h>
#include "xinbot.h"
#include "webserver.h"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         

XinBot robot;

void setup()
{
	Serial.begin(9600);
	Serial.println("QuadBot-E Start!");
  delay(1000);
	webinit();
	robot.init();
// delay(1000);
	robot.Servo_PROGRAM_Zero();

	// 生成网页
	enableWebServer();
}

void loop()
{
	// 构建网页
	handleClient();

	if (Servo_PROGRAM >= 1 ) 
	{
//    	Serial.println(Servo_PROGRAM);
		switch (Servo_PROGRAM) 
		{
			case 1: // Standby 待机
				robot.standby();
				break;

			case 2: // Forward 前进
				robot.forward();
				break;

			case 3: // Backward 后退
				robot.backward();
				break;
			
			case 4: // Left shift 左移
				robot.leftshift();
				break;
			
			case 5: // Right shift 右移
				robot.rightshift();
				break;
			
			case 6: // Turn left 左转
				robot.turnleft();
				break;
			
			case 7: // Turn right 右转
				robot.turnright();
				break;
			
			case 8: // Lie 躺下
				robot.lie();
				break;
			
			case 9: // Say Hi 打招呼
				robot.hello();
				break;
			
			case 10: // Fighting 战斗
				robot.fighting();
				break;
			
			case 11: // Push up 俯卧撑
				robot.pushup();
				break;
			
			case 12: // Sleep 睡眠
				robot.sleep();
				break;
			
			case 13: // 舞步 1
				robot.dance1();
				break;
			
			case 14: // 舞步 2
				robot.dance2();
				break;
			
			case 15: // 舞步 3
				robot.dance3();
				break;
			
			case 99: // 待机
				robot.center();
				break;
			
			case 100: // 归零
				robot.zero();
				break;
		}
		Servo_PROGRAM = 0;
	}

	if(GPIO_ID < 100)
	{
		 Serial.print("GPIO_ID="); Serial.println(GPIO_ID);
    Serial.print("ival="); Serial.println(ival);
		robot.Set_PWM_to_Servo(GPIO_ID, ival.toInt());
		GPIO_ID = 100;
		ival = "";
	}
}
