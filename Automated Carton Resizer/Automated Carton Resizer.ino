/*Servo Motor*/
#include <Servo.h>
Servo servoMotor; //servo motor instance

/*Relay pins*/
const int dcMotorOut = 2; //relay 2 on dc motor out
const int dcMotorIn = 3;  //relay 3 on dc motor in
const int ssrMotorControl = 4; //relay 4 ssr

/*Ultrasonic*/
#define TRIGPIN 11 //trigger pin for theh ultrasonic
#define ECHOPIN 10 //echo pin for teh ultrasonic

/*Utilities*/
bool isScanned = false; 
int sensorCounter = 0;
int sensorThreshold = 3;
const int distanceSensor = 400; //in millimeter, change this depending on the sensor value you want. refer to the print statement below 
unsigned long scanDelay = 1000; //scan delay time for the sensor 
unsigned long dcMotorInDuration = 5000; //dc motor in duration
unsigned long dcMotorOutDuration = 3000; //dc motor out duration
unsigned long ssrMotorControlDuration = 3000; //ssr motor duration

float duration, distance;

void setup() 
{
  Serial.begin(9600); //initialize Serial
  servoMotor.attach(9);  // initialize the servo to pin 9 
  servoMotor.write(90);   // Move the servo to its default position (0 degrees)
  pinMode(ECHOPIN, INPUT); //initialize echo pin
  pinMode(TRIGPIN, OUTPUT); //initialize trigger pin 
  delay(1000); //initialize first 
}

void loop() 
{

  if(!isScanned) //initial state, or if there is nothing being scanned
  {
   ScanObject();
  }
  else
  {
    StartProcess();
  }

  //NOTE: Before uncommenting line 59 and 60, comment first from line 49 to line 56
  //ScanObject(); //uncomment this line if you want to check for the scanning 
  //StartProcess(); //uncomment thhis line if you want to check for theh process happening 
}

void ScanObject()
{
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(20);
  digitalWrite(TRIGPIN, LOW);
  duration = pulseIn(ECHOPIN, HIGH);
  // Determine distance from duration. Use 343 metres per second as speed of sound Divide by 1000 as we want millimeters
 
  distance = (duration / 2) * 0.343;
 
 //change operation to <= or >=, depending on what you need. if the distance scanned is greater or less than and equal to the scanned distance
  if(distance <= distanceSensor) 
  {
    sensorCounter++;
  } 

  if(sensorCounter >= sensorThreshold) 
  {
    isScanned = true; 
    Serial.println("Scanned");
  }
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.println("sensorCounter: " + sensorCounter);
  Serial.println("isScanned: " + isScanned);
  delay(scanDelay); //scanning delay, change the value on how fast thhe values are scanned, 

}

void StartDCMotorIn(int seconds)
{
  Serial.println("Start DC Motor In: ON");
  digitalWrite(dcMotorIn, LOW);
  delay(seconds);
  digitalWrite(dcMotorIn, HIGH);
  Serial.println("Start DC Motor IN: OFF");
}

void StartDCMotorOut(int seconds)
{
  Serial.println("Start DC Motor OUT: ON");
  digitalWrite(dcMotorOut, LOW);
  delay(seconds);
  digitalWrite(dcMotorOut, HIGH);
  Serial.println("Start DC Motor OUT: OFF");
}

void StartSSR(int seconds)
{
  Serial.println("Start SSR Motor: ON");
  digitalWrite(ssrMotorControl, LOW);
  delay(seconds);
  digitalWrite(ssrMotorControl, HIGH);
  Serial.println("Start SSR Motor: OFF");
}

void StartProcess()
{
  /*actual process happening,*/
  servoMotor.write(0); //Servo motor will turn from 90deg as its initial position to 0deg
  StartDCMotorIn(dcMotorInDuration); //Turn on relay 3 for 5 secs and turn it off
  StartSSR(ssrMotorControlDuration); //Turn on relay 4 for 5 seconds and turn it off 
  servoMotor.write(90); //Servo motor will turn back to 90deg from 0deg
  StartDCMotorOut(dcMotorOutDuration); //Turn on relay 2 for 5secs
  isScanned = false; //reset the state to scanning state
  sensorCounter = 0; //reset the counter to zero for fresh start
}

