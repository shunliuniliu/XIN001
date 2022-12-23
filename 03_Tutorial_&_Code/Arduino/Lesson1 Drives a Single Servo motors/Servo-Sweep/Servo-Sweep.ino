#include <Servo.h>

Servo servo_14;

void setup(){
  servo_14.attach(16);
}

void loop(){
    for(int pos=0; pos<=180; pos++)
    {
      servo_14.write(pos);
      delay(15);
    }
    for(int pos=180; pos>=0; pos--)
    {
      servo_14.write(pos);
      delay(15);
    }
}
