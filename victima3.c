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
int q=1;
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
const int trigPin1=2;
const int echoPin1=3;
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

  analogWrite(9, 1000); 
  analogWrite(10, 1000);
}


void moveBck()
{
  analogWrite(38, 1000); 
  analogWrite(39, 0); 

  analogWrite(12, 1000); 
  analogWrite(13, 0); 

  analogWrite(9, 1000); 
  analogWrite(10, 1000);
}


void rotateLeft()
{
  analogWrite(38, 1000); 
  analogWrite(39, 0); 

  analogWrite(12, 0); 
  analogWrite(13, 1000); 

  analogWrite(9, 1000); 
  analogWrite(10, 1000);

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
  {  //Serial.println("lejos");
     analogWrite(9, 150); //Jony lo había puesto en 500
     analogWrite(10, 1000); 
  }
  else if (sensor1 < distmin && sensor2 < distmin )
  {
     //Serial.println("cerca");
     analogWrite(9, 1000); 
     analogWrite(10, 150); 
  }
  else
  {
   //  Serial.println("dentro");
     analogWrite(9, 1000); 
     analogWrite(10, 1000);  
   }
}



void medirDistanciaDerecha(int sensor1,int sensor2,int distmin,int distmax){
  
  if (sensor1 > distmax && sensor2 > distmax )
  { // Serial.println("lejos");
     analogWrite(9, 1000); 
     analogWrite(10, 150); 
  }
  else if (sensor1 < distmin && sensor2 < distmin )
  {
     //Serial.println("cerca");
     analogWrite(9, 150); 
     analogWrite(10, 1000); 
  }
  else
  {
    // Serial.println("dentro");
     analogWrite(9, 1000); 
     analogWrite(10, 1000);  
   }
}



void loop() 
{

 //uno=getDistance(trigPin1,echoPin1);// der  tras
 //dos = getDistance(trigPin2,echoPin2); //der del
 //tres = getDistance(trigPin3,echoPin3); //centro
 //cuatro = getDistance(trigPin4,echoPin4);//izq del
 //cinco = getDistance(trigPin5,echoPin5);//izq tras
 // Serial.print("uno : ");
 // Serial.println(uno);
 // Serial.print("dis : ");
 // Serial.println(dos);
 // Serial.print("cuatro : ");
 //Serial.println(tres);
 // Serial.println(cuatro);
 //Serial.print("cinco : "); 
  //Serial.println(cinco); 
 
  Serial.println(c);
  
  switch(c){
    case 0:
          if(q){
            q=0;
            moveFwd();
          }
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          medirDistanciaDerecha(cuatro,cinco,8,11);
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos = getDistance(trigPin2,echoPin2); //der del
          Serial.println(uno);
          Serial.println(dos);
          if (uno > 20 && dos > 20){//checa hueco
            c=1;
            q=1;
          }
                     
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
          if (count > 1820){
            c=3;
          }
          break;

    case 3: 
          moveFwd();
          tres = getDistance(trigPin3,echoPin3); //centro
         
          if(tres < 12)
            c=4;
          break;
    case 4:
          rotateRight();
          count=0;
          c=5;
          break;
    case 5:
          if(count>1820)
            c=13;
          break;
    case 6:
          if(q){
            moveFwd();
            q=0;
          }
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          medirDistanciaDerecha(cuatro,cinco,10,15);//el trigger es el optico,por mientras usamos el sensor de enfrente
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos = getDistance(trigPin2,echoPin2); //der del    
            if(uno >25  && dos >25 ){
               q=1;
               c=13;
            }
          break;
     case 13:
         if(q){
            moveFwd();
            q=0;
          }
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          medirDistanciaDerecha(cuatro,cinco,8,11);//el trigger es el optico,por mientras usamos el sensor de enfrente
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos = getDistance(trigPin2,echoPin2); //der del  
            if(uno <25  && dos <25 ){
               q=1;
               c=14;
            }
          break;
     case 14:
          if(q){
            moveFwd();
            q=0;
          }
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          tres = getDistance(trigPin3,echoPin3); //centro
          medirDistanciaDerecha(cuatro,cinco,8,11);//el trigger es el optico,por mientras usamos el sensor de enfrente
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos = getDistance(trigPin2,echoPin2); //der del 
            if(uno >25  && dos >25 ){
               q=1;
               c=15;
            }
          break;
    case 15:
          if(q){
            moveFwd();
            q=0;
          }
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          tres = getDistance(trigPin3,echoPin3); //centro
          medirDistanciaDerecha(cuatro,cinco,8,11);//el trigger es el optico,por mientras usamos el sensor de enfrente
          Serial.print("Sensor 3: ");
          Serial.println(tres);    
            if(tres < 20){
               q=1;
               c=7;
            }
          break;

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
          if (count > 3810)
            c=10;
          break;
          
    case 10:
          if(q){
            moveFwd();
            q=0;
          }
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos=getDistance(trigPin2,echoPin2); //der del
          medirDistanciaIzquierda(uno,dos,8,10);
          if(uno > 20 || dos > 20){
              q=1;
              c=11;
            }
          break;
    case 11:
          if(q){
            moveFwd();
            q=0;
          }
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          medirDistanciaDerecha(cuatro,cinco,8,11);
          tres = getDistance(trigPin3,echoPin3); //centro
          if (tres< 12){
            q=1;
            c=12;
          }
          break;
          
    case 12:
          fullStop(); //entran rutinas para dejar victima en su lugar
          c=16;
          break;
//victima 2----------------------------
    case 16:
          if(q){
            moveBck();
            q=0;
          }
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          medirDistanciaDerecha(cuatro,cinco,8,11);
          if(cuatro > 20 && cinco > 20){
            q=1;
            c=17;
          }
          break;
    case 17:
         rotateRight();
          count=0;
          c=18;
          break;
    case 18:
          if (count > 1850)
            c=19;
          break;   
    case  19:
          moveFwd();
          count=0;
          c=20;
          break;
    case  20:
          if (count > 3500){
            c=21;
          } 
          break;
    case 21:
          rotateLeft();
          count=0;
          c=22;
          break;   
    case 22:
          if(count >= 1850)
          {
            c=23;
          }      
          break;
    case 23:
          if(q){
            moveFwd();
            q=0;
          }
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos=getDistance(trigPin2,echoPin2); //der del
          medirDistanciaIzquierda(uno,dos,10,12);
          tres=getDistance(trigPin3,echoPin3);
          if(tres < 15){
              q=1;
              c=24;
            }
          break;
      case 24:
          fullStop();//entran rutinas del gripper recoger victima
          delay(1000);
          c=25;
          break;
      case 25:
          if(q){
            moveBck();
            q=0;
          }
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos=getDistance(trigPin2,echoPin2); //der del
          medirDistanciaIzquierda(uno,dos,8,10);
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          if(cuatro > 20 && cinco > 20){
            q=1;
            c=26;
          }
          break;
      case 26:
          rotateLeft();
          count=0;
          c=27;
          break;
      case 27:
          if(count >= 1850){
            c=28;
          }
          break;
      case 28:
          moveFwd();
          tres = getDistance(trigPin3,echoPin3); //centro
          if(tres < 12)
            c=29;
          break;
      case 29:
         rotateRight();
          count=0;
          c=30;
          break;
    case 30:
          if (count > 1850)
            c=31;
          break;   
    case 31:
          if(q){
            moveFwd();
            q=0;
          }
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos=getDistance(trigPin2,echoPin2); //der del
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          medirDistanciaDerecha(cuatro,cinco,10,12);
          if(uno > 20 && dos > 20){
            q=1;
            c=32;
          }
          break;
      case 32:
          rotateLeft();
          count=0;
          c=33;
          break;   
      case 33:
          if(count >= 1850)
          {
            c=34;
          }      
          break;
      case 34: 
          moveFwd();
          tres = getDistance(trigPin3,echoPin3); //centro
          if(tres < 12)
            c=35;
          break;
      case 35:
          rotateLeft();
          count=0;
          c=36;
          break;   
      case 36:
          if(count >= 1850)
          {
            c=37;
          }      
          break;
      case 37:
          if(q){
            moveFwd();
            q=0;
          }
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          medirDistanciaDerecha(cuatro,cinco,11,13);
          tres = getDistance(trigPin3,echoPin3); //centro
          if (tres< 25){
            q=1;
            c=38;
          }
          break;
      case 38:
           fullStop();
           c=39;
//VICTIMA 3---------------------------------------------------------------------      
      case 39:
            if(q){
            moveBck();
            q=0;
          }
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos=getDistance(trigPin2,echoPin2); //der del
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          medirDistanciaDerecha(cuatro,cinco,11,13);
          if(uno > 20 && dos > 20){
            q=1;
            c=40;
          }
          break;
      case 40:
          rotateLeft();
          c=41;
          count=0;
          break;
      case 41:
          if(count>= 1850){
            c=42;
          }
          break;
      case 42:
          moveFwd();
          tres = getDistance(trigPin3,echoPin3); //centro
          if(tres < 12)
            c=43;
          break;
      case 43:
          rotateRight();
          count=0;
          c=44;
          break;
      case 44:
          if(count>1820)
            c=45;
          break;
      case 45:
          if(q){
            moveFwd();
            q=0;
          }
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          medirDistanciaDerecha(cuatro,cinco,10,15);//el trigger es el optico,por mientras usamos el sensor de enfrente
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos = getDistance(trigPin2,echoPin2); //der del    
            if(uno >25  && dos >25 ){
               q=1;
               c=46;
            }
          break;
     case 46:
         if(q){
            moveFwd();
            q=0;
          }
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          medirDistanciaDerecha(cuatro,cinco,8,11);//el trigger es el optico,por mientras usamos el sensor de enfrente
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos = getDistance(trigPin2,echoPin2); //der del  
            if(uno <25  && dos <25 ){
               q=1;
               c=47;
            }
          break;
     case 47:
          if(q){
            moveFwd();
            q=0;
          }
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          tres = getDistance(trigPin3,echoPin3); //centro
          medirDistanciaDerecha(cuatro,cinco,8,11);//el trigger es el optico,por mientras usamos el sensor de enfrente
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos = getDistance(trigPin2,echoPin2); //der del 
            if(uno >25  && dos >25 ){
               q=1;
               c=48;
            }
          break;
      case 48:
          rotateLeft();
          count=0;
          c=49;
          break;
      case 49:
          if(count >= 1850){
            c=50;
          }
          break;
      case 50;
          moveFwd();
          count=0;
          c=51;
          break;
      case 51:
          if(count >=6500){
            c=52;
          }
          break;
      case 52:
          rotateRight();
          count=0;
          c=53:
          break;
      case 53:
          if(count >=1850);
          {
            c=54;
          }
          break;
      case 54:
          moveFwd();
          tres=getDistance(trigPin3,echoPin3);
          if(tres <15){
            c=55;
          }
      case 55:
          rotateLeft();
          count=0;
          c=56:
          break;
      case 56:
          if(count >=1850);
          {
            c=57;
          }
          break;
//--------------------------------------VALIDACION GRIPPER-------------------------------------------------------------     
      case 57:  //RUTINA DEL GRIPPER PARA IDENTIFICAR ENTRE VICTIMA Y PARED  CASO 1->58    CASO 2->74
          if(q){
            moveFwd();
            q=0;
          }
          cuatro = getDistance(trigPin4,echoPin4);//izq del
          cinco = getDistance(trigPin5,echoPin5);//izq tras
          tres = getDistance(trigPin3,echoPin3); //centro
          medirDistanciaDerecha(cuatro,cinco,8,11);//el trigger es el optico,por mientras usamos el sensor de enfrente
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos = getDistance(trigPin2,echoPin2); //der del 
            if(uno >25  && dos >25 ){
               q=1;
               c=58;
            }
          break;
//---------------------VICTIMA 3-------CASO 1
      case 58:
            fullStop();
            c=59;
            break;
      case 59:
            rotateRight();
            count=0;
            c=60;
      case 60:
            if(count >=3810){
              c=61;
            }
            break;
      case 61:
            if(q){
            moveFwd();
            q=0;
          }
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos=getDistance(trigPin2,echoPin2); //der del
          tres=getDistance(trigPin3,echoPin3);
          medirDistanciaIzquierda(uno,dos,9,11);
          if(tres < 12){
            q=1;
            c=62;
          }
          break;
      case 62:
            rotateRight();
            count=0;
            c=63;
      case 63:
            if(count >=1850){
              c=64;
            }
            break;
      case 64:
            if(q){
            moveFwd();
            q=0;
          }
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos=getDistance(trigPin2,echoPin2); //der del
          medirDistanciaIzquierda(uno,dos,9,11);
          if(uno >20 && dos>20){
            q=1;
            c=65;
          }
          break;
      case 65:
            rotateLeft();
            count=0;
            c=66;
      case 66:
            if(count >=1850){
              c=67;
            }
            break;
      case 67;
          moveFwd();
          count=0;
          c=68;
          break;
      case 68:
          if(count >=6500){
            c=69;
          }
          break;
      case  69:
          rotateRight();
          count=0;
          c=70;
      case 70:
          if(count >= 1850)
          {
            c=71;
          }
          break;
      case 71:
        if(q){
              moveFwd();
              q=0;
            }
            uno=getDistance(trigPin1,echoPin1);// der  tras
            dos=getDistance(trigPin2,echoPin2); //der del
            medirDistanciaIzquierda(uno,dos,8,10);
            if(uno > 20 || dos > 20){
                q=1;
                c=72;
              }
            break;
      case 72:
            if(q){
              moveFwd();
              q=0;
            }
            cuatro = getDistance(trigPin4,echoPin4);//izq del
            cinco = getDistance(trigPin5,echoPin5);//izq tras
            medirDistanciaDerecha(cuatro,cinco,8,11);
            tres = getDistance(trigPin3,echoPin3); //centro
            if (tres< 12){
              q=1;
              c=73;
            }
            break;
            
      case 73:
            fullStop(); //entran rutinas para dejar victima en su lugar
            c=96;//----inicio de victima 4
            break;
//------------------VICTIMA 3 CASO 2------------------------
      case 74:
           rotateLeft();
           count=0;
           c=75;
           break;
      case 75:
           if(count >= 1850)
           {
            c=76;
           }
           break;
      case 76:
            moveFwd();
            if(sharp <4)//------optico
            {
              c=77:
            }
            break;
      case 77://rtinas para recoger victima
            fullStop();
            c=78;
            break;
      case 78:
            rotateRight();
            count=0;
            c=79;
      case 79:
            if(count >=3810){
              c=80;
            }
            break;
      case 80:
            if(q){
              moveFwd();
              q=0;
            }
            uno=getDistance(trigPin1,echoPin1);// der  tras
            dos=getDistance(trigPin2,echoPin2); //der del
            medirDistanciaIzquierda(uno,dos,8,10);
            tres= getDistance(trigPin3,echoPin3);
            if(tres <=12){
                q=1;
                c=81;
              }
            break;
      case 81:
            rotateRight();
            count=0;
            c=82;
      case 82:
            if(count >=1850){
              c=83;
            }
            break;
       case 83:
            if(q){
              moveFwd();
              q=0;
            }
            uno=getDistance(trigPin1,echoPin1);// der  tras
            dos=getDistance(trigPin2,echoPin2); //der del
            medirDistanciaIzquierda(uno,dos,8,10);
            tres= getDistance(trigPin3,echoPin3);
            if(tres <=12){
                q=1;
                c=84;
              }
            break;
        case 84:
            rotateRight();
            count=0;
            c=85;
      case 85:
            if(count >=1850){
              c=86;
            }
            break;
      case 86:
            if(q){
            moveFwd();
            q=0;
          }
          uno=getDistance(trigPin1,echoPin1);// der  tras
          dos=getDistance(trigPin2,echoPin2); //der del
          medirDistanciaIzquierda(uno,dos,9,11);
          if(uno >20 && dos>20){
            q=1;
            c=87;
          }
          break;
      case 87:
            rotateLeft();
            count=0;
            c=88;
      case 88:
            if(count >=1850){
              c=89;
            }
            break;
      case 89;
          moveFwd();
          count=0;
          c=90;
          break;
      case 90:
          if(count >=6500){
            c=91;
          }
          break;
      case  91:
          rotateRight();
          count=0;
          c=92;
      case 92:
          if(count >= 1850)
          {
            c=93;
          }
          break;
      case 93:
        if(q){
              moveFwd();
              q=0;
            }
            uno=getDistance(trigPin1,echoPin1);// der  tras
            dos=getDistance(trigPin2,echoPin2); //der del
            medirDistanciaIzquierda(uno,dos,8,10);
            if(uno > 20 || dos > 20){
                q=1;
                c=94;
              }
            break;
      case 94:
            if(q){
              moveFwd();
              q=0;
            }
            cuatro = getDistance(trigPin4,echoPin4);//izq del
            cinco = getDistance(trigPin5,echoPin5);//izq tras
            medirDistanciaDerecha(cuatro,cinco,8,11);
            tres = getDistance(trigPin3,echoPin3); //centro
            if (tres< 12){
              q=1;
              c=95;
            }
            break;
            
      case 95:
            fullStop(); //entran rutinas para dejar victima en su lugar
            c=96; //inicio de victima 4
            break;
//--------VICTIMA 4
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

