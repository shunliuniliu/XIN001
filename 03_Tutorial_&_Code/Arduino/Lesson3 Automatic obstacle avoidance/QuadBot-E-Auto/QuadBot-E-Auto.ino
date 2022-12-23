/* Wire：
   -VCC               = 3.3V/5.5V
   -Trig_RX_SCL_I/O   = D3
   -Echo_TX_SDA       NONE
   -GND               = GND
*/

#include <Servo.h>
#include <EEPROM.h>
#include "xinbot.h"
#include "webserver.h"

XinBot robot;

#define  AVOIDDIS  30 // Avoidance distance

float       distance;
const int   trig_echo=D3;               // Trig_RX_SCL_I/O

void setup()
{
	Serial.begin(9600);
	Serial.println("QuadBot-E Start!");
  delay(1000);
	robot.init();
	robot.Servo_PROGRAM_Zero();
  delay(300);
  pinMode(trig_echo,OUTPUT);            // Set Trig_RX_SCL_I/O pin as output
}

void loop()
{
  pinMode(trig_echo,OUTPUT);            // Set Trig_RX_SCL_I/O pin as output
  digitalWrite(trig_echo,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_echo,LOW);          // Set Trig_RX_SCL_I/O pin to output 10US high-level pulse trigger signal    
  pinMode(trig_echo,INPUT);             // Set Trig_RX_SCL_I/O pin as input，receive the distance signal fed back by the module
  distance  = pulseIn(trig_echo,HIGH);  // Count the received high level time
  distance  = distance*340/2/10000;     // Calculate distance --> 1：sound velocity：340M/S  2：the actual distance is half of the sound speed distance 3：The counting clock is 1US
  Serial.println(distance);
  pinMode(trig_echo,OUTPUT);            // Set Trig_RX_SCL_I/O pin as output，prepare for next measurement
  delay(30);                            // After the completion of a single measurement, add a 30mS delay to conduct the next measurement。
                                        // Prevent the last residual wave from being measured during close distance measurement, resulting in inaccurate measurement.
	
	if (distance<AVOIDDIS && distance>0) 
	{
    robot.center();
    delay(100);
    robot.turnleft();
	}
  else if(distance >= AVOIDDIS)
  {
    robot.forward();
  }
}
