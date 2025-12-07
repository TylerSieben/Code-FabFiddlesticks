#include <Ultrasonic.h>
#include <Servo.h>

//Ultrasonic Sensor
Ultrasonic ultrasonic(9, 10);
int distance;

//Servo Setup
Servo servoLamp;
Servo servoScythe;
//Servo servoJaw;

const int buzzerPin = 7;

//Digital Output
const int ledPinL = 12;
const int ledPinR = 13;

//Timer for reset
unsigned long resetTime;
unsigned long resetPrev = 0;
int resetLength = 10000;

//Timer for Main
unsigned long currentTime;
unsigned long previousTime = 0;
int timerLength = 5000;

//Timer for second
unsigned long currentTimeSec;
unsigned long previousTimeSec = 0;
int timerLengthSec = 1000;

//Completed Variables
bool scythe = false;
bool jaw = false;
bool chest = false;
bool lamp = false;

//Timer sensor
unsigned long currentTimeSens;
unsigned long previousTimeSens = 0;
unsigned long paused = 0;
int timerLengthSens = 1000;

//Setup for Outputs
void setup() {
  servoLamp.attach(3);
  servoScythe.attach(5);
  //servoJaw.attach(6);

  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPinL, OUTPUT);
  pinMode(ledPinR, OUTPUT);
  Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  currentTimeSens = millis();
  if(currentTimeSens - previousTimeSens >= timerLengthSens)
  {
    previousTimeSens = currentTimeSens;
    distance = ultrasonic.read();
    Serial.print("Distance in CM: "); //Debugging
    Serial.println(distance); //Debugging
    if(distance <= 50 && !scythe && jaw)
    {
      //Swing Scythe
      armSwing();
      scythe = true;
      Serial.println("Scythe WORKING");
    } 
    else if(distance <= 65 && !jaw && lamp) 
    {
      //Talking + mouth move
      mouthNoises();
      jaw = true;
      Serial.println("JAW WORKING");
      delay(1000);
    } 
    else if(distance <= 80 && !lamp && chest) 
    {
      //arm raise
      servoLamp.write(180);
      delay(500);
      lamp = true;
      Serial.println("LAMP WORKING");
    } 
    else if(distance <= 100 && !chest) 
    {
      //Chest lights up
      digitalWrite(ledPinL, HIGH);
      digitalWrite(ledPinR, HIGH);
      Serial.println("CHEST WORKING");
      delay(1000);
      chest = true;
    }
    else if(distance > 100 && chest && lamp && jaw && scythe && paused != 0)
    {
      if(currentTimeSens - paused >= resetLength)
      {
        digitalWrite(ledPinL, LOW);
        digitalWrite(ledPinR, LOW);
        servoLamp.write(90);
        servoScythe.write(90);
        tone(buzzerPin, 740);
        delay(200);
        tone(buzzerPin, 500);
        delay(200);
        tone(buzzerPin, 300);
        delay(200);
        tone(buzzerPin, 100);
        delay(200);
        noTone(buzzerPin);
        Serial.println("RESTART");
        chest = false;
        lamp = false;
        jaw = false;
        scythe = false;
      }
    }
    else
    {
      paused = currentTimeSens;
    }
  }
}
  
void armSwing()
{
  //Servo code for the arm swinging will be used in shortest distance. Make arm move back and forth while holding the scythe
        servoScythe.write(0);
        delay(300);
        servoScythe.write(165);
        delay(300);
}

void mouthNoises()
{
  //Servo code for the upper and lower mouth to move. Probably just going to be the lower jaw moving as it makes noises.
  for(int i = 0; i < 3; i++)
  {
    tone(buzzerPin, 740);
    delay(50);
    tone(buzzerPin, 690);
    delay(70);
    tone(buzzerPin, 730);
    delay(60);
    tone(buzzerPin, 466);
    delay(45);
  }
  tone(buzzerPin, 293);
  delay(2000);
  noTone(buzzerPin);
}
