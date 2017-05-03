#include <Servo.h>
#define sensor A0 // define Sharp

const int trigPin = 33; //Definicion de ultrasonico frontal
const int echoPin = 32;


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

/// validateVictim
bool victim=false;
int distSharp=0; //valida si es una victima
////

int mideSharp(){
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

   return avgDistance;

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

bool validateVictim(int tres){
  distSharp = mideSharp();

  if (distSharp == (tres+2))
    victim=false; //es pared

  else if (sharp <= 6)
    victim=true;

  else
    victim=false;
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

}
