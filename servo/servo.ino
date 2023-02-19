#include <Servo.h>
// #define CONTROL_PIN 5

int servo_pin = 5;
int control_pin = A5;

Servo servo_1;
float i = 0;

void setup(){
    
    servo_1.attach(servo_pin);
    pinMode(control_pin, INPUT);
    Serial.begin(9600);

}


void loop(){

    
    // float angle = sin(i * PI / 360.0) * 90 + 90;
    // Serial.println(angle);
    // servo_1.write(angle);
    // i += 1;
    int val = analogRead(control_pin);
    float angle = map(val, 0, 1024, 0, 180);
    servo_1.write(angle);

    
    delay(10);

}