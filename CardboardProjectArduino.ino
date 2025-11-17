#include <Ultrasonic.h>

//Ultrasonic Sensor
Ultrasonic ultrasonic(9, 10);
int distance;

//Digital Output
const int ledPinL = 12;
const int ledPinR = 13;

//Setup for Outputs
void setup() {
  // put your setup code here, to run once:
  pinMode(ledPinL, OUTPUT);
  pinMode(ledPinR, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  distance = ultrasonic.read();

  Serial.print("Distance in CM: "); //Debugging
  Serial.println(distance); //Debugging

  if(distance < 75)
  {
    //Arm swing
  }
  else if(distance < 250)
  {
    //Talking + mouth move
  }
  else if(distance < 325)
  {
    //arm drops
  }
  else if(distance < 500)
  {
    //Chest lights up
  }
  else
  {
    //Do nothing?
  }
}

void armSwing()
{
  //Servo code for the arm swinging will be used in shortest distance. Make arm move back and forth while holding the scythe
}

void mouthMovement();
{
  //Servo code for the upper and lower mouth to move. Probably just going to be the lower jaw moving as it makes noises.
}


