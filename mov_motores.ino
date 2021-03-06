//2450   alrededor de 180 cm
#if defined(__PIC32MX__)
#include <p32xxxx.h> /* this gives all the CPU/hardware definitions */
 
#include <plib.h> /* this gives the i/o definitions */
#endif
long duration;
int distance,uno,dos,tres,cuatro,cinco;
volatile int count = 0; //esto estaba en 0
int prev_val, curr_val;
int c=0,x=1;

int MotorDerecho=500;
int MotorIzquierdo=500;

const int speed1 = 9;
const int speed2 = 10;

int dir1PinA= 38;
int dir2PinA= 39;

int dir1PinB=12;
int dir2PinB=13;

int fwd1,fwd2;
int bck1,bck2;

//echos y triggers ultrasonicos
const int trigPin1=28;
const int echoPin1=29;
const int trigPin2=4;
const int echoPin2=5;
const int trigPin3=33;
const int echoPin3=32;
const int trigPin4=34;
const int echoPin4=35;
const int trigPin5=36;
const int echoPin5=37;
                                                 
                           
void setup() 
{
  //Setup de sensores ultrasonicos
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);
  pinMode(trigPin5, OUTPUT);
  pinMode(echoPin5, INPUT);
  //****------------------------------------------------------------------------------------------------------
  Serial.begin(9600);
  //Setup de motores------------------------------------------------------------------------------------
  analogReference(DEFAULT);  // Tell the chipKIT AD converter the max analog voltage will be the default value 3.3V.
  pinMode(speed1, OUTPUT);
  pinMode(speed2,OUTPUT);

  pinMode(dir1PinA,OUTPUT);
  pinMode(dir2PinA,OUTPUT);

  pinMode(dir1PinB,OUTPUT);
  pinMode(dir2PinB,OUTPUT);

  //Timer setup---------------------------------------------------------------------------------------------------------------------------------------------------------
  OpenTimer3( T3_ON | T3_PS_1_1 | T3_SOURCE_INT, 0xFFFFFF);
  ConfigIntTimer3((T3_INT_ON | T3_INT_PRIOR_3));
  //---------------------------------------------------------------------------------------------------------------------------------------------------------
  
  
  
} 



void moveFwd()
{
  analogWrite(38, 0); 
  analogWrite(39, 1000); 

  analogWrite(12, 0); 
  analogWrite(13, 1000); 

  digitalWrite(9, 1);
  digitalWrite(10, 1);
}


void moveBck()
{
  analogWrite(38, 1000); 
  analogWrite(39, 0); 

  analogWrite(12, 1000); 
  analogWrite(13, 0); 

  digitalWrite(9, 1);
  digitalWrite(10, 1);
}


void rotateLeft()
{
  analogWrite(38, 1000); 
  analogWrite(39, 0); 

  analogWrite(12, 0); 
  analogWrite(13, 1000); 

  digitalWrite(9, 1);
  digitalWrite(10, 1);
}

void rotateRight()
{
  analogWrite(38, 0); 
  analogWrite(39, 1000); 

  analogWrite(12, 1000); 
  analogWrite(13, 0); 

  digitalWrite(9, 1);
  digitalWrite(10, 1);
}

void fullStop()
{
  analogWrite(38, 0); 
  analogWrite(39, 0); 

  analogWrite(12, 0); 
  analogWrite(13, 0); 

}


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




void medirDistanciaIzquierda(int sensor1,int sensor2,int distmin,int distmax){
  if (sensor1 > distmax && sensor2 > distmax )
  {
    MotorIzquierdo=150;
    MotorDerecho=1000;
  }
  else if (sensor1 < distmin && sensor2 < distmin )
  {
    MotorIzquierdo=1000;
    MotorDerecho=150;
  }
  else
  {
     MotorIzquierdo=1000;
     MotorDerecho=1000;  
   }
}

void medirDistanciaDerecha(int sensor1,int sensor2,int distmin,int distmax){
  
  if (sensor1 > distmax && sensor2 > distmax )
  {
   // MotorIzquierdo=150;  
   // MotorDerecho=1000;
     MotorIzquierdo=1000;  
     MotorDerecho=150;

  }
  else if (sensor1 < distmin && sensor2 < distmin )
  {
    
    MotorIzquierdo=150;
    MotorDerecho=500;
  }
  else
  {
     MotorIzquierdo=1000;
     MotorDerecho=1000;  
   }
}


void loop() 
{

 //uno=getDistance(trigPin1,echoPin1);// der  tras
 //dos = getDistance(trigPin2,echoPin2); //der del
 //tres = getDistance(trigPin3,echoPin3); //centro
 //cuatro = getDistance(trigPin4,echoPin4);//izq del
 //cinco = getDistance(trigPin5,echoPin5);//izq tras
 
  //Serial.println(uno);
  //Serial.println(dos);
  //Serial.println(tres);
  //Serial.println(cuatro); 
  //Serial.println(cinco); 
  
  Serial.println(c);
  
  switch(c){
    case 0:
          moveFwd();
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          medirDistanciaDerecha(cuatro,cinco,10,15);
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos = getDistance(trigPin2,echoPin2); //der del
          if (uno > 20 && dos > 20)//checa hueco
            c=1;
                     
          break;
    case 1:
          digitalWrite(dir1PinB, LOW);
          digitalWrite(dir2PinB, LOW);
          digitalWrite(dir1PinA, LOW);
          digitalWrite(dir2PinA, LOW);
          rotateLeft();
          count=0;
          c=2;
          break;
    case 2:
          uno=getDistance(trigPin1,echoPin1);
          if (count > 1850){
            c=3;
          }
          break;

    case 3: 
          moveFwd();
          tres = getDistance(trigPin3,echoPin3); //centro
         
          if(tres < 8)
            c=4;
          break;
    case 4:
          digitalWrite(dir1PinB, LOW);
          digitalWrite(dir2PinB, LOW);
          digitalWrite(dir1PinA, LOW);
          digitalWrite(dir2PinA, LOW);
          rotateRight();
          count=0;
          c=5;
          break;
    case 5:
//          tres = getDistance(trigPin3,echoPin3);
          if(count>2350)
            c=6;
          break;
    case 6:
          digitalWrite(dir1PinB, LOW);
          digitalWrite(dir2PinB, LOW);
          digitalWrite(dir1PinA, LOW);
          digitalWrite(dir2PinA, LOW);
          moveFwd();
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          tres = getDistance(trigPin3,echoPin3); //centro
          medirDistanciaDerecha(cuatro,cinco,10,15);//el trigger es el optico,por mientras usamos el sensor de enfrente
              
            if(tres < 20)
               //count=0; //inicializar timer
               c=7;
          break;
/*
    case 13:
          medirDistanciaDerecha(cuatro,cinco,7,13);
          if(count >= 2000)
          { tres = getDistance(trigPin3,echoPin3); //centro
            
            if(tres < 15)
              c=7;    
          }
          break;
*/

    case 7:
          fullStop();//entran rutinas del gripper recoger victima
          delay(1000);
          c=8;
          break;

    case 8:
         rotateRight();
          count=0;
          c=9;
          break;
    case 9:
          if (count > 4650)
            c=10;
          break;
          
    case 10:
          digitalWrite(dir1PinB, LOW);
          digitalWrite(dir2PinB, LOW);
          digitalWrite(dir1PinA, LOW);
          digitalWrite(dir2PinA, LOW);
          moveFwd();
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos=getDistance(trigPin2,echoPin2); //der del
          medirDistanciaIzquierda(uno,dos,10,15);
//          if(uno > 20 || dos > 20)
            if (uno > 20)
            c=11;
          break;
    case 11:
          digitalWrite(dir1PinB, LOW);
          digitalWrite(dir2PinB, LOW);
          digitalWrite(dir1PinA, LOW);
          digitalWrite(dir2PinA, LOW);
          moveFwd();
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          medirDistanciaDerecha(cuatro,cinco,10,15);
          tres = getDistance(trigPin3,echoPin3); //centro
          if (tres< 10)
            c=12;
          break;
    case 12:
          digitalWrite(dir1PinB, LOW);
          digitalWrite(dir2PinB, LOW);
          digitalWrite(dir1PinA, LOW);
          digitalWrite(dir2PinA, LOW);
          fullStop(); //entran rutinas para dejar victima en su lugar
          break;
         
    default:
          fullStop();
          break;

  }

}



#ifdef __cplusplus
extern "C" {
#endif

void __ISR(_TIMER_3_VECTOR,IPL3AUTO) comms_handler(void)
{
  mT3ClearIntFlag();  // Clear interrupt flag
  count++;
  
}
#ifdef __cplusplus
}
#endif



