#include <Ultrasonic.h>
#include <Servo.h>

//Ultrasonic Sensor
Ultrasonic ultrasonic(9, 10);
int distance;

//Servo Setup
Servo servoLamp;
Servo servoScythe;
//Servo servoJaw;

//Piezo Buzzer setup
const int buzzerPin = 7;

//Digital Output
const int ledPinL = 12;
const int ledPinR = 13;

//Completed Variables
bool scythe = false;
bool jaw = false;
bool chest = false;
bool lamp = false;

//Timer Setup
unsigned long currentTimeSens;
unsigned long previousTimeSens = 0;
unsigned long paused = 0;
int timerLengthSens = 1000;
int resetLength = 10000;

//Setup for Outputs
void setup() {
  servoLamp.attach(3);
  servoScythe.attach(5);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPinL, OUTPUT);
  pinMode(ledPinR, OUTPUT);
  Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  currentTimeSens = millis();
  //Timer for the Ultrasonic to read a distance every second
  if(currentTimeSens - previousTimeSens >= timerLengthSens)
  {
    previousTimeSens = currentTimeSens;
    distance = ultrasonic.read();
    Serial.print("Distance in CM: "); //Debugging
    Serial.println(distance); //Debugging
    //If the distance is the closest it can be, make the model swing arm.
    if(distance <= 50 && !scythe && jaw)
    {
      //Swing Scythe
      armSwing();
      scythe = true;
      Serial.println("Scythe WORKING");
    } 
    //If the distance is the penultimate distance, Make the screaming noise
    else if(distance <= 65 && !jaw && lamp) 
    {
      //Talking + mouth move
      mouthNoises();
      jaw = true;
      Serial.println("JAW WORKING");
      delay(1000);
    } 
    //If the distance is the second furthest distance, raise the lamp arm up
    else if(distance <= 80 && !lamp && chest) 
    {
      //arm raise
      servoLamp.write(180);
      delay(500);
      lamp = true;
      Serial.println("LAMP WORKING");
    } 
    //If the distance is within a good range, the lights turn on, signifying the user is close enough
    else if(distance <= 100 && !chest) 
    {
      //Chest lights up
      digitalWrite(ledPinL, HIGH);
      digitalWrite(ledPinR, HIGH);
      Serial.println("CHEST WORKING");
      delay(1000);
      chest = true;
    }
    //If everything has run once and the user steps outside the initial distance, have the model return to its default state.
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
    //update the timer for resets.
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
  //Update: jaw code was never created as there was limited time and hardware to create the movement of the jaw
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
