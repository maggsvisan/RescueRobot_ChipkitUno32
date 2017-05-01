long duration;
int distance,uno,dos,tres,cuatro,cinco;
volatile int count = 0;
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
const int trigPin2=31;
const int echoPin2=30;
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
/*
  //Timer setup---------------------------------------------------------------------------------------------------------------------------------------------------------
  OpenTimer3( T3_ON | T3_PS_1_1 | T3_SOURCE_INT, 0xFFFFFF);
  ConfigIntTimer3((T3_INT_ON | T3_INT_PRIOR_3));
  //---------------------------------------------------------------------------------------------------------------------------------------------------------
}
*/
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, LOW);

   digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, LOW);
}



/////////////////////////////////////// MOTORES ///////////////////////////////

/////////////////////////////////  Motor A /////////////////////////////////
void moveMotorAFwd(int motor, int speed,int dir1, int dir2){ // se va hacia adelante el motorA
    analogWrite(motor, speed);
    digitalWrite(dir1, LOW);
    digitalWrite(dir2, HIGH);
}

void moveMotorABck(int motor, int speed,int dir1, int dir2){ // se va hacia atras el motorA
    analogWrite(motor, speed);
    digitalWrite(dir1, HIGH);
    digitalWrite(dir2, LOW);
}
////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////  Motor B /////////////////////////////////
void moveMotorBFwd(int motor, int speed,int dir1, int dir2){ // se va hacia adelante el motorB
    analogWrite(motor, speed);
    digitalWrite(dir1, LOW);
    digitalWrite(dir2, HIGH);
}

void moveMotorBBck(int motor, int speed,int dir1, int dir2){ // se va hacia atras el motorA
    analogWrite(motor, speed);
    digitalWrite(dir1, HIGH);
    digitalWrite(dir2, LOW);
}
////////////////////////////////////////////////////////////////////////////////////

void stopMotor(int motor,int dir1, int dir2){
    analogWrite(motor, 0);
    digitalWrite(dir1, LOW);
    digitalWrite(dir2, LOW);
}
void fullStop(){
    analogWrite(speed1, 0);
    digitalWrite(dir2PinA, HIGH);
    digitalWrite(dir1PinA, LOW);
    analogWrite(speed2, 0);
    digitalWrite(dir2PinB, HIGH );
    digitalWrite(dir1PinB, LOW);
}

void moveFwd(){
   moveMotorAFwd(speed2,MotorDerecho,dir1PinA,dir2PinA);
   moveMotorBFwd(speed1,MotorIzquierdo,dir1PinB,dir2PinB);
 }

void moveBck(){ //ya funciona
   moveMotorABck(speed2,MotorDerecho,dir1PinA,dir2PinA);
   moveMotorBBck(speed2,MotorDerecho,dir1PinB,dir2PinB);   // motor A va para atrás
}

void rotatRight(){
  moveMotorAFwd(speed2,MotorDerecho,dir1PinA,dir2PinA);
 moveMotorBBck(speed2,MotorDerecho,dir1PinB,dir2PinB);
}

void rotateLeft(){
  moveMotorBFwd(speed1,MotorIzquierdo,dir1PinB,dir2PinB);
  moveMotorABck(speed2,MotorDerecho,dir1PinA,dir2PinA);
    
}


void loop()
{
  //se mueve para atrás
  //  moveBck();

    // se mueve hacia adelante
    // moveFwd();
    
    rotateLeft();


//Rutinas de moviento de los motores
  // moveMotorAFwd(speed2,MotorDerecho,dir1PinA,dir2PinA); // este motorA se va para adelante
//     moveMotorABck(speed2,MotorDerecho,dir1PinA,dir2PinA);   // motor A va para atrás

      //moveMotorBFwd(speed1,MotorIzquierdo,dir1PinB,dir2PinB); // este motorB se va para adelante
     //  moveMotorBBck(speed2,MotorDerecho,dir1PinB,dir2PinB);   // motor B va para atrás

}
