#include <Servo.h>
int data_x = 0;
int data_y = 0;
int data[1];
Servo myservo_x;
Servo myservo_y;// create servo object to control a servo

void setup() {
  Serial.begin(9600);
  myservo_x.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo_y.attach(10);
  myservo_x.write(90);
  myservo_y.write(90);
}

void loop() {
  while (Serial.available() >= 2) {
    for (int i = 0; i < 2; i++) {
      data[i] = Serial.read();
    }

    myservo_x.write(data[0]);
    myservo_y.write(data[1]);

    Serial.println(data[0]);
    Serial.println(data[1]);
  }  
}
