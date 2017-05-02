#include <Servo.h>
#define sensor A0 // define Sharp 

const int trigPin = 33; //Definicion de ultrasonico frontal
const int echoPin = 32;
int victimaON = 0;

Servo ServoAlfa;
Servo ServoBeta;

long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}

void tomarVictima()
{
    ServoAlfa.write(44); //Bajar
    delay(500);
    ServoBeta.write(45); //Cerrar
    delay(500);
    ServoAlfa.write(50); //Subir
    delay(500);
}

void dejarVictima()
{
    ServoAlfa.write(44);  //Bajar
    delay(500);
    ServoBeta.write(105); //Abrir
    delay(500);
    ServoAlfa.write(47);  //Neutral
    delay(500);
}

void setup() {
  ServoAlfa.attach(7);  //Servo sube-baja
  ServoBeta.attach(6); //Servo accion de gripper
  Serial.begin(9600);
  ServoAlfa.write(47); // Neutral
  ServoBeta.write(105); //Abrir
}

void loop()
{
  long duration, ultrasonic;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  
  ultrasonic = microsecondsToCentimeters(duration);
  float volts = analogRead(sensor)*0.0048828125;
  int sharp = 13*pow(volts, -1);

  Serial.print("Ultrasonic: ");
  Serial.print(ultrasonic);
  Serial.print("/ Sharp: ");
  Serial.print(sharp);
  Serial.println();

  if(sharp <= 5 && victimaON == 0) //Para evitar que lo agarre antes, hay que poner un delay antes de las acciones del gripper. VictimaON es para que no se cancele la funcion antes de tiempo
  {
    if (sharp == ultrasonic || sharp == ultrasonic++ || sharp == ultrasonic-- || sharp-- == ultrasonic++ || sharp++ == ultrasonic -- || sharp++ == ultrasonic || sharp-- == ultrasonic)
    {
      Serial.println("Pared!");
    }
    else
      {
        Serial.println("Victima");
        victimaON = 1;
        tomarVictima();
        delay(3500);
        dejarVictima();
        victimaON = 0;
      }
   }
   else
   {
    Serial.println("Nada!");
   }
  delay(1000);
}
