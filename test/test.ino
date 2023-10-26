#include <arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>

//Endre navn på bibliotek 
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;

//Variabler 
int16_t lastError = 0;
uint16_t maxSpeed = 300;    //Hastighet 
#define NUM_SENSORS 5            //Antall IR sensorer
unsigned int lineSensorValues[NUM_SENSORS]; //Verdien av avlesningen til sensorene

//Funksjonsliste

void calibrateSensors()
{
  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  for(uint16_t i = 0; i < 120; i++)
  {
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-200, 200);
    }
    
    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}

void lineFollower()
{
  int16_t position = lineSensors.readLine(lineSensorValues);

  //Beregne hvor langt unna linjen er senteret
  int16_t error = position - 2000; 
  int16_t speedDifference = error / 4 + 6 * (error - lastError);

  lastError = error;   //Deklarerer nytt navn for error

  //Sett farten til de individuelle motorene 
  int16_t leftSpeed = (int16_t)maxSpeed + speedDifference;
  int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;

  //Sett grenser til farten til motoren fra 0 - maxspeed (200)
  leftSpeed = constrain(leftSpeed, 0, (int16_t)maxSpeed);
  rightSpeed = constrain(rightSpeed, 0, (int16_t)maxSpeed);

  //Sett farten til de to motorene 
  motors.setSpeeds(leftSpeed, rightSpeed);
}

void setup() 
{
  // put your setup code here, to run once:
  lineSensors.initFiveSensors();
  buttonA.waitForButton();
  calibrateSensors();
}

void loop()
 {
  // put your main code here, to run repeatedly:
  lineFollower();

}
