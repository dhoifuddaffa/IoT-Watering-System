/*
IoT Automatic Plant Watering System using NodeMCU, Blynk App.
Created 2018
by Dhoifuddaffa Dhiaz Fairus
*/

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "AuthToken";
char ssid[] = "NetworkName";
char pass[] = "Password";

WidgetLED standbyLed(V4);
WidgetLED workingLed(V5);

//Stepper motor pins
#define step1 14 //D5
#define step2 12 //D6
#define step3 13 //D7
#define step4 15 //D8
//Pump pin
#define pump 5 //D9
//Limit switch
#define limitSwitch 4 //D10

int steps = 0 ;
int direction = 0 ;
int totalStaps = 0 ;
int stepsCount = 0 ;

//Data from Blynk App
int onOffTimer = 0 ;
int test = 0 ;
int timerState1 = 0 ;
int timerState2 = 0 ;
int pumpPower = 0 ;
int distance = 0 ;
int instantWatering = 0 ;
int limitSwitchState = 0 ;


void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  //Stepper motor
  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(step3, OUTPUT);
  pinMode(step4, OUTPUT);
  //Pump
  pinMode(pump, OUTPUT);
  //Limit switch
  pinMode(limitSwitch, INPUT);
  stopMotor();
}

void loop()
{
  Blynk.run();
  stepCalculate(distance) ;
  if (onOffTimer == 1){ //Turn on timer
    if (timerState1 == 1 || timerState2 == 1){ //Time to watering
      standbyLed.off();
      workingLed.on();
      analogWrite(pump, pumpPower);
      goWork(totalStaps);
      analogWrite(pump, 0);
      backHome();
      standbyLed.on();
      workingLed.off();

    }
  }
  if (instantWatering == 1){ //Press instantwatering button
    standbyLed.off();
    workingLed.on();
    analogWrite(pump, pumpPower);
    goWork(totalStaps);
    analogWrite(pump, 0);
    backHome();
    standbyLed.on();
    workingLed.off();
  }
  if (test == 1){ // Press test button
    standbyLed.off();
    workingLed.on();
    goWork(totalStaps);
    backHome();
    standbyLed.on();
    workingLed.off();
  }
}

BLYNK_WRITE(V0) {
  onOffTimer = param.asInt();
}
BLYNK_WRITE(V1) {
  test = param.asInt();
}
BLYNK_WRITE(V2) {
  timerState1 = param.asInt();
}
BLYNK_WRITE(V3) {
  timerState2 = param.asInt();
}
BLYNK_WRITE(V6) {
  pumpPower = param.asInt();
}
BLYNK_WRITE(V7) {
  distance = param.asInt();
}
BLYNK_WRITE(V8) {
  instantWatering = param.asInt();
}

void stepCalculate(int distance){
  totalStaps = (distance*(4076/4)) ;
}

void backHome(){
  limitSwitchState = digitalRead(limitSwitch);
  while(limitSwitchState == 0){
    stepper(0) ;
    delay(1);
    limitSwitchState = digitalRead(limitSwitch);
  }
  stopMotor() ;
  limitSwitchState = digitalRead(limitSwitch);
}

void goWork(int totalStaps){
  while(stepsCount < totalStaps){
    stepper(1);
    delay(1);
    stepsCount ++ ;
  }
  stopMotor() ;
  stepsCount = 0 ;
}

void stepper(int direction){
  for (int x=0; x<1; x++){
    Blynk.run();
    switch(steps){
      case 0:
      digitalWrite(step1, LOW);
      digitalWrite(step2, LOW);
      digitalWrite(step3, LOW);
      digitalWrite(step4, HIGH);
      break;
      case 1:
      digitalWrite(step1, LOW);
      digitalWrite(step2, LOW);
      digitalWrite(step3, HIGH);
      digitalWrite(step4, HIGH);
      break;
      case 2:
      digitalWrite(step1, LOW);
      digitalWrite(step2, LOW);
      digitalWrite(step3, HIGH);
      digitalWrite(step4, LOW);
      break;
      case 3:
      digitalWrite(step1, LOW);
      digitalWrite(step2, HIGH);
      digitalWrite(step3, HIGH);
      digitalWrite(step4, LOW);
      break;
      case 4:
      digitalWrite(step1, LOW);
      digitalWrite(step2, HIGH);
      digitalWrite(step3, LOW);
      digitalWrite(step4, LOW);
      break;
      case 5:
      digitalWrite(step1, HIGH);
      digitalWrite(step2, HIGH);
      digitalWrite(step3, LOW);
      digitalWrite(step4, LOW);
      break;
      case 6:
      digitalWrite(step1, HIGH);
      digitalWrite(step2, LOW);
      digitalWrite(step3, LOW);
      digitalWrite(step4, LOW);
      break;
      case 7:
      digitalWrite(step1, HIGH);
      digitalWrite(step2, LOW);
      digitalWrite(step3, LOW);
      digitalWrite(step4, HIGH);
      break;
      default:
      digitalWrite(step1, LOW);
      digitalWrite(step2, LOW);
      digitalWrite(step3, LOW);
      digitalWrite(step4, LOW);
      break;
    }
    if(direction == 0){
      steps++ ;
    }
    if(direction == 1){
      steps-- ;
    }
    if(steps > 7){
      steps = 0 ;
    }
    if(steps < 0){
      steps = 7 ;
    }
  }
}

void stopMotor(){
  digitalWrite(step1, LOW);
  digitalWrite(step2, LOW);
  digitalWrite(step3, LOW);
  digitalWrite(step4, LOW);
}
