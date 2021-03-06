
#include <Servo.h> // the servo library

/// variables servo code //
Servo myservo; // create servo object
//int potpin = 0;  // analog pin used to connect the potentiometer
int servoAngle;               // variable to read the value from the analog pin 
int servoPin = 9;
int val=0;
//////

// variables sharp //
int AnPin = 0;                                    // analog pin for reading the IR sensor
int val2,i=0;             // outgoing ADC value
int distance = 0;
int totalDistance=0;
int avgDistance=0;
////


/// variables ultrasonico //
const int echoPin1 = 32;
const int trigPin1 = 33;
////

int getDistance(int trigPin, int echoPin){
  long duration=0;  
  int distance=0;
  
  for(int i=0;i<=20;i++){
  
    digitalWrite(trigPin,HIGH);
    delay(2);
    digitalWrite(trigPin,LOW);
    duration += pulseIn(echoPin, HIGH); 
  }
  distance = duration*0.034/2/20;
  return distance;
}


void setup() {
  Serial.begin(9600);   // start the serial port
  myservo.attach(servoPin);
  pinMode(echoPin1,INPUT);
  pinMode(trigPin1,OUTPUT);
}

void loop() {

  do{
     // read analog input:
     val2 = analogRead(AnPin); 
     // Calculate linear slope 
     distance = (2914 / (val2 + 5)) - 1;
     totalDistance+=distance;
     i++;
   }while (i<30);
   
   avgDistance= totalDistance/20;
   i=0;
   
   // parametros de distancias para ultrasonicos
   int dist1;
   dist1=getDistance(trigPin1,echoPin1);//izq frontal
  /// 
    
   Serial.print("Analog Value: ");
   Serial.println (avgDistance);
   Serial.print("Ultra: ");
   Serial.println(dist1);
   avgDistance=0;
   totalDistance=0;
   
   // wait 10ms for ADC to reset before next reading:
   delay(1000);  
}







