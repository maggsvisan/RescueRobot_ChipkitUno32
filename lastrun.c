    //2450   alrededor de 180 cm
    #if defined(__PIC32MX__)
    #include <p32xxxx.h> /* this gives all the CPU/hardware definitions */
     
    #include <plib.h> /* this gives the i/o definitions */
    #endif

    #include <Servo.h>
    //#define sensor AD2 // define Sharp 
    int c=0;      // Victima 1 = 0
                  //victima 2 =13
                  //victima 3 = 39
                  //victima 4 = 96
    int victimaON = 0;
    Servo ServoAlfa;
    Servo ServoBeta;
    long duration;
    int distance,uno,dos,tres,cuatro,cinco;
    volatile int count = 0; //esto estaba en 0
    int prev_val, curr_val;
    int x=1;
    int q=1;
    int MotorDerecho=500; 
    int MotorIzquierdo=500;
    float volts;
    int sharp;
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
    const int trigPin4=8;
    const int echoPin4=11;
    const int trigPin5=36;
    const int echoPin5=37;

    /// validateVictim
    int AnPin = 2;
    int val2,i=0;
    int distanceSharp=0;
    int totalDistanceSharp=0;
    int avgDistanceSharp=0;
    bool victim=false;
    int distSharp=0; //valida si es una victima
    bool detect=0;
    int correccion=0;
    ////                                
                               
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
      //-------------------------------------------------------------------------------------------------------------------------------------------------------- 
      //--------POSICION ININCIAL DEL SERVO ALFA-------
       ServoAlfa.attach(7);  //Servo sube-baja
      ServoBeta.attach(6); //Servo accion de gripper
      ServoAlfa.write(44); // Neutral
      ServoBeta.write(125); //Abrir
    }
    void tomarVictima()
{
    delay(1000); //Delay para asegurar que esta esperando a que la victima este completamente dentro de la pinzas del gripper
    ServoAlfa.write(44); //Bajar
    delay(500);
    ServoBeta.write(10); //Cerrar
    delay(500);
    ServoAlfa.write(50); //Subir
    delay(500);
}

void dejarVictima()
{
    ServoAlfa.write(44);  //Bajar
    delay(500);
    ServoBeta.write(125); //Abrir
    delay(500);
    ServoAlfa.write(44);  //Neutral
    delay(500);
}
    int mideSharp(){
      do{
         val2 = analogRead(AnPin)*0.0048828125;
         distanceSharp = 13*pow(val2, -1);
         totalDistanceSharp+=distanceSharp;
         i++;
       }while (i<30);
      
       //Serial.println(avgDistanceSharp);
       avgDistanceSharp=totalDistanceSharp/30;
       if(avgDistanceSharp < 0)
        avgDistanceSharp=1000;
       totalDistanceSharp=0;
       i=0;
       return avgDistanceSharp;

    }
    bool validateVictim(){
      tres=getDistance(trigPin3,echoPin3);
      correccion=tres+2;
      distSharp=mideSharp();
      if (distSharp == correccion){
        victim=false; //es pared
        Serial.println("PARED");
      }
      else if (distSharp <= 3){
        victim=true;
        Serial.println("Victima");
      }
      else
        victim=false;
      return victim;
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
      
      if (sensor2 >distmax)
      {   
        Serial.println("s2 > distmax");
         analogWrite(10, 500); 
         analogWrite(9, 150 ); 
      }
      else if( sensor2 < distmin)
      {   
        Serial.println("s2< distmin");
         analogWrite(10, 150); 
         analogWrite(9, 500);
      }
      else if (sensor1 > distmax )
      { Serial.println("s1 > distmax");
         analogWrite(9, 500); 
         analogWrite(10, 150); 
      }
      else if (sensor1 < distmin )
      {   
        Serial.println("s1< distmin");
         analogWrite(9, 150); 
         analogWrite(10, 500); 
      }
      else
      {
         analogWrite(10, 1000); 
         analogWrite(9, 1000);  
       }
    }
    void medirDistanciaDerecha(int sensor1,int sensor2,int distmin,int distmax){
      
      if (sensor1 > distmax )
      { 
         analogWrite(9, 500); 
         analogWrite(10, 150); 
      }
      else if (sensor1 < distmin )
      {
         analogWrite(9, 150); 
         analogWrite(10, 500); 
      }
      
      else if (sensor2 >distmax)
      {
         analogWrite(9, 150); 
         analogWrite(10, 500); 
      }
      else if( sensor2 < distmin)
      {
         analogWrite(9, 500); 
         analogWrite(10, 150);
      }
      else
      {
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
              //medirDistanciaDerecha(cuatro,cinco,14,15);
              uno=getDistance(trigPin1,echoPin1);// der  tras
              dos = getDistance(trigPin2,echoPin2); //der del
              medirDistanciaIzquierda(uno,dos,12,13);
              Serial.println(uno);
              Serial.println(dos);
              if (uno > 25 && dos > 25){//checa hueco
                c=1;
              }     
              break;
        case 1:
              rotateLeft();
              count=0;
              c=2;
              break;
        case 2:
              if (count > 1320){
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
              if(count>1290)

                c=13;
              break;
        case 6:
              if(q){
                moveFwd();
                q=0;
              }
              cuatro = getDistance(trigPin4,echoPin4);//izq del
              cinco = getDistance(trigPin5,echoPin5);//izq tras
              medirDistanciaDerecha(cuatro,cinco,9,10);//el trigger es el optico,por mientras usamos el sensor de enfrente
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
              medirDistanciaDerecha(cuatro,cinco,9,10);//el trigger es el optico,por mientras usamos el sensor de enfrente
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
              medirDistanciaDerecha(cuatro,cinco,9,10);//el trigger es el optico,por mientras usamos el sensor de enfrente
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
              medirDistanciaDerecha(cuatro,cinco,9,10);//el trigger es el optico,por mientras usamos el sensor de enfrente
              detect=validateVictim();
              if(detect ){
                   q=1;
                   c=7;
                }
              break;

        case 7:
              fullStop();//entran rutinas del gripper recoger victima
              delay(1000);
              tomarVictima();
              c=8;
              break;

        case 8:
             rotateRight();
              count=0;
              c=9;
              break;
        case 9:
              if (count > 3500)
                c=10;
              break;
              
        case 10:
              if(q){
                moveFwd();
                q=0;
              }
              uno=getDistance(trigPin1,echoPin1);// der  tras
              dos=getDistance(trigPin2,echoPin2); //der del
              medirDistanciaIzquierda(uno,dos,10,11);
              if(uno > 25 || dos > 25){
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
              medirDistanciaDerecha(cuatro,cinco,8,9);
              tres = getDistance(trigPin3,echoPin3); //centro
              if (tres< 15){
                q=1;
                c=12;
              }
              break;
              
        case 12:
              fullStop(); //entran rutinas para dejar victima en su lugar
              dejarVictima();
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
              medirDistanciaDerecha(cuatro,cinco,8,9);
              if(cuatro > 25 && cinco > 25){
                q=13;
                c=17;
              }
              break;
        case 17:
             rotateRight();
              count=0;
              c=18;
              break;
        case 18:
              if (count > 2200)
                c=19;
              break;   
        case  19:
              moveFwd();
              count=0;
              c=20;
              break;
        case  20:
              if (count > 3300){
                c=21;
              } 
              break;
        case 21:
              rotateLeft();
              count=0;
              c=22;
              break;   
        case 22:
              if(count >= 1550)
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
              medirDistanciaIzquierda(uno,dos,10,11);
              detect=validateVictim();
                if(detect){
                  q=1;
                  c=24;
                }
              break;
          case 24:
              fullStop();//entran rutinas del gripper recoger victima
              delay(1000);
              tomarVictima();
              c=25;
              break;
          case 25:
              if(q){
                moveBck();
                q=0;
              }
              uno=getDistance(trigPin1,echoPin1);// der  tras
              dos=getDistance(trigPin2,echoPin2); //der del
              medirDistanciaIzquierda(uno,dos,10,11);
              if(uno > 20 && dos > 20){
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
              if(count >= 1550){
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
              if (count > 1600)
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
              medirDistanciaDerecha(cuatro,cinco,11,12);
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
              if(count >= 1500)
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
              if(count >= 1500)
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
              medirDistanciaDerecha(cuatro,cinco,13,14);
              tres = getDistance(trigPin3,echoPin3); //centro
              if (tres< 25){
                q=1;
                c=38;
              }
              break;
          case 38:
               fullStop(); /// rutina para abrir el gripper
               dejarVictima();
               delay(1000);
               c=39;
               break;
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
              medirDistanciaDerecha(cuatro,cinco,13,14);
              if(uno > 30 && dos > 30){
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
              if(count>= 1500){
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
              if(count>1550)
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
              medirDistanciaDerecha(cuatro,cinco,10,14);//el trigger es el optico,por mientras usamos el sensor de enfrente
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
              medirDistanciaDerecha(cuatro,cinco,11,13);//el trigger es el optico,por mientras usamos el sensor de enfrente
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
              if(count >= 1500){
                c=50;
              }
              break;
          case 50:
              moveFwd();
              count=0;
              c=51;
              break;
          case 51:
              if(count >=7000){
                c=52;
                count=0;
              }
              break;
          case 52:
              rotateRight();
              count=0;
              c=53;
              break;
          case 53:
              if(count >=2000)
              {
                c=54;
              }
              break;
          case 54:
              moveFwd();
              tres=getDistance(trigPin3,echoPin3);
              if(tres < 15){
                c=55;
              }
              break;
          case 55:
              rotateLeft();
              count=0;
              c=56;
              break;
          case 56:
              if(count >=2000)
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
              medirDistanciaDerecha(cuatro,cinco,13,15);//el trigger es el optico,por mientras usamos el sensor de enfrente
              sharp=mideSharp();
              Serial.print("Sharp");
              Serial.println(sharp);
              detect=validateVictim();
              tres=getDistance(trigPin3,echoPin3);
              if(detect){ 
                c=58;//58
                q=1;
              }
              if(tres <16){
                   q=1;
                   c=74;//74
                }
              break;
    //---------------------VICTIMA 3-------CASO 1
          case 58:
                fullStop();
                tomarVictima();
                delay(1000);
                c=59;
                break;
          case 59:
                rotateRight();
                count=0;
                c=60;
          case 60:
                if(count >=4700){
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
              medirDistanciaIzquierda(uno,dos,13,15);
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
                if(count >=2000){
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
              medirDistanciaIzquierda(uno,dos,12,15);
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
                if(count >=1400){
                  c=67;
                }
                break;
          case 67:
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
              if(count >= 1450)
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
                medirDistanciaDerecha(cuatro,cinco,13,15);
                tres = getDistance(trigPin3,echoPin3); //centro
                if (tres < 25){
                  q=1;
                  c=73;
                }
                break;
                
          case 73:
                fullStop(); //entran rutinas para dejar victima en su lugar
                dejarVictima();
                delay(1000);
                c=96;//----inicio de victima 4
                break;
    //------------------VICTIMA 3 CASO 2------------------------
          case 74:
               rotateLeft();
               count=0;
               c=75;
               break;
          case 75:
               if(count >= 2000)
               {
                c=76;
                q=1;
               }
               break;

          case 76:
                if(q){
                moveFwd();
                q=0;
                }
                cuatro = getDistance(trigPin4,echoPin4);//izq del
                cinco = getDistance(trigPin5,echoPin5);//izq tras
                medirDistanciaDerecha(cuatro,cinco,13,15);//el trigger es el optico,por mientras usamos el sensor de enfrente
                //sharp=mideSharp();
                //Serial.print("Sharp");
                //Serial.println(sharp);
                detect=validateVictim();
                //tres=getDistance(trigPin3,echoPin3);
                if(detect){ 
                  c=77;// rutinas de gripper
                  q=1;
                }
                
                break;
           
          case 77://rtinas para recoger victima
                fullStop(); 
                tomarVictima();
                delay(1000);
                c=78;
                break;

          case 78:
                rotateLeft();
                count=0;
                c=79;
                break;
                
          case 79:
                if(count >=3800){
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
                if(tres <=13){
                    q=1;
                    c=81;
                  }
                break;
          case 81:
                rotateRight();
                count=0;
                c=82;
          case 82:
                if(count >=2300){
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
                if(count >=1450){
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
                if(count >=1400){
                  c=89;
                }
                break;
          case 89:
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
              if(count >= 1450)
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
                delay(1000);
                dejarVictima();
                c=96; //inicio de victima 4
                break;
    //--------VICTIMA 4
           case 96:
              if(q){
                moveBck();
                q=0;
              }
              cuatro = getDistance(trigPin4,echoPin4);//izq del
              cinco = getDistance(trigPin5,echoPin5);//izq tras
              medirDistanciaDerecha(cuatro,cinco,10,11);
              if(cuatro > 20 && cinco > 20){
                q=1;
                c=97;
              }
              break;
        case 97:
             rotateRight();
              count=0;
              c=98;
              break;
        case 98:
              if (count > 1450)
                c=99;
              break;   
        case  99:
              moveFwd();
              count=0;
              c=100;
              break;
        case  100:
              if (count > 7000){
                c=101;
              } 
              break;
        case 101:
              rotateLeft();
              count=0;
              c=102;
              break;   
        case 102:
              if(count >= 2000)
              {
                c=103;
              }      
              break;
        case 103:
              if(q){
                moveFwd();
                q=0;
              }
              uno=getDistance(trigPin1,echoPin1);// der  tras
              dos=getDistance(trigPin2,echoPin2); //der del
              medirDistanciaIzquierda(uno,dos,14,15);
              detect=validateVictim();
              tres=getDistance(trigPin3,echoPin3);
              if(detect){ 
                c=104;// detecta victima
                q=1;
              }
              if(tres <30){
                   q=1;
                   c=121;//121
                }
              break;
        case 104:
              fullStop();
              delay(1000);
              tomarVictima();
              c=105;
              break;
        case 105:
             if(q){
                moveBck();
                q=0;
              }
              uno = getDistance(trigPin1,echoPin1);//izq del
              dos = getDistance(trigPin2,echoPin2);//izq tras
              medirDistanciaIzquierda(uno,dos,14,15);
              if(uno > 20 && dos > 20){
                q=1;
                c=106;
              }
              break;

          case 106:
              rotateLeft();
              c=107;
              count=0;
              break;


          case 107:
              if(count >= 2000){
                c=108;
              }
              break;
          case 108:
              moveFwd();
              count=0;
              c=109;
              break;
          case 109:
              if(count>=7000){
                c=110;
              }
              break;
          case 110:
             rotateRight();
              count=0;
              c=111;
              break;
        case 111:
              if (count > 1450)
                c=112;
              break;   
        case 112:
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
                c=113;
              }
              break;
          case 113:
              rotateLeft();
              count=0;
              c=114;
              break;   
          case 114:
              if(count >= 1400)
              {
                c=115;
              }      
              break;
          case 115: 
              moveFwd();
              tres = getDistance(trigPin3,echoPin3); //centro
              if(tres < 12)
                c=116;
              break;
          case 116:
              rotateLeft();
              count=0;
              c=117;
              break;   
          case 118:
              if(count >= 1400)
              {
                c=119;
              }      
              break;
          case 119:
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
                c=120;
              }
              break;
          case 120:
               fullStop();
               delay(1000);//deja victima
               dejarVictima();
               c=1000;
               break;

         ///// fin caso 1 VICTIMA 4

        /// INICIO CASO 2 ____ VICTIMA 4
          case 121:
             if(q){
                moveBck();
                q=0;
              }
              uno = getDistance(trigPin1,echoPin1);//izq del
              dos = getDistance(trigPin2,echoPin2);//izq tras
              medirDistanciaIzquierda(uno,dos,14,15);
              if(uno > 20 && dos > 20){
                q=1;
                c=122;
              }
              break;
          case 122:
              rotateLeft();
              c=123;
              count=0;
              break;
          case 123:
              if(count >= 2000){
                c=124;
              }
              break;
          case 124:
              moveFwd();
              count=0;
              c=125;
              break;
          case 125:
              if(count>=6800){
                c=126;
              }
              break;
          case 126:
              rotateLeft();
              c=127;
              count=0;
              break;
          case 127:
              if(count >= 1400)
              {
                c=128;
              }
              break;
         case 128: //sigue pared donde hay hueco
              if(q){
                moveFwd();
                q=0;
              }
              uno = getDistance(trigPin1,echoPin1);//izq del
              dos = getDistance(trigPin2,echoPin2);//izq tras
              medirDistanciaIzquierda(uno,dos,8,11);
              if(uno < 25 && dos < 25){
                q=1;
                c=129;
              }
              break;
        

            case 129:
             if(q){
                moveFwd();
                q=0;
              }
              cuatro = getDistance(trigPin4,echoPin4);//izq del
              cinco = getDistance(trigPin5,echoPin5);//izq tras
              medirDistanciaDerecha(cuatro,cinco,8,11);//el trigger es el optico,por mientras usamos el sensor de enfrente
              uno=getDistance(trigPin1,echoPin1);// der  tras
              dos = getDistance(trigPin2,echoPin2); //der del  
                if(uno > 20  && dos > 20 ){
                   q=1;
                   c=131;
                }
              break;
         
          case 131:
              rotateLeft();
              count=0;
              c=132;
              break;
          case 132:
              if(count >= 1400){
               // c=13; este era el error
                c=133;
              }
              break;
          case 133 :
              moveFwd();
              c=130;
              count=0;
              break;
         case 130:
                if(count >= 7000 ){
                  c=134;
                }
              break;
          
          case 134:
              rotateRight();
              count=0;
              c=135;
              break;

          case 135:
              if(count >=2000)
              {
                c=136;
              }
              break;
          case 136:
               if(q){
                moveFwd();
                q=0;
              }
              cuatro = getDistance(trigPin4,echoPin4);//izq del
              cinco = getDistance(trigPin5,echoPin5);//izq tras
              tres = getDistance(trigPin3,echoPin3); //centro
              medirDistanciaDerecha(cuatro,cinco,13,15);//el trigger es el optico,por mientras usamos el sensor de enfrente
              if(tres < 15 ){
                   q=1;
                   c=137;
                }
              break;

              case 137:
                rotateLeft();
                count=0;
                c=138;
                break;
                
              case 138:
                if(count >=2000)
                {
                  c=139;
                }
                break;
              case 139:
                 if(q){
                  moveFwd();
                  q=0;
                }
                cuatro = getDistance(trigPin4,echoPin4);//izq del
                cinco = getDistance(trigPin5,echoPin5);//izq tras
                tres = getDistance(trigPin3,echoPin3); //centro
                medirDistanciaDerecha(cuatro,cinco,13,15);//el trigger es el optico,por mientras usamos el sensor de enfrente
                
                  if(tres <= 15 ){ 
                     q=1;
                     c=140;
                  }
                break;

               case 140:
                  rotateLeft();
                  count=0;
                  c=141;
                  break;
                  
              case 141:
                  if(count > 2000)
                  {
                    c=142;
                  }
                  break;

              case 142:
                 if(q){
                  moveFwd();
                  q=0;
                }
                cuatro = getDistance(trigPin4,echoPin4);//izq del
                cinco = getDistance(trigPin5,echoPin5);//izq tras
                
                medirDistanciaDerecha(cuatro,cinco,13,15);//el trigger es el optico,por mientras usamos el sensor de enfrente
                
                tres=getDistance(trigPin3,echoPin3);
                if(tres < 25){ 
                  c=143; // detecta pared 
                  q=1;
                }
                   
                break;

              case 143:
                  rotateLeft();
                  count=0;
                  c=144;
                  break;

              case 144:
                  if (count >= 2200)
                    c=145;
                  break;        
               
              case 145:
                if(q){
                moveFwd();
                q=0;
                }
                cuatro = getDistance(trigPin4,echoPin4);
                cinco = getDistance(trigPin5,echoPin5);
                medirDistanciaDerecha(cuatro,cinco,19,20);
                detect=validateVictim();
                
                if(detect){ 
                  c=146;// rutinas de gripper
                  q=1;
                }
                break;
          
              //// correccion de cases
              case 146:
                  fullStop();
                  delay(1000);
                  tomarVictima();
                  c=147;
                  break;

              case 147:
                  rotateLeft();
                  count=0;
                  c=148;
                  break;

              case 148:
                  if(count >= 4700)
                     c=149;
                  break;

            case 149:
                  if(q){
                  moveFwd();
                  q=0;
                }
                uno=getDistance(trigPin1,echoPin1);
                dos=getDistance(trigPin2,echoPin2);
                medirDistanciaIzquierda(uno,dos,18,20);
                tres=getDistance(trigPin3,echoPin3);
                if(tres <= 15){ 
                  c=150;  
                  q=1;
                }
                break;

              case 150:
                  rotateRight();
                  count=0;
                  c=151;
                  break;

              case 151:
                  if(count >= 2000)
                    c=152;
                  break;

              case 152:
                  if(q){
                  moveFwd();
                  q=0;
                }
                uno=getDistance(trigPin1,echoPin1);// der  tras
                dos=getDistance(trigPin2,echoPin2); //der del
                tres=getDistance(trigPin3,echoPin3);
                medirDistanciaIzquierda(uno,dos,13,15);
                if(tres < 15){
                  q=1;
                  c=153;
                }
                break;

              case 153:
                    rotateRight();
                    count=0;
                    c=154;
                    break;

              case 154:
                    if(count >=2000){
                      c=155;
                    }
                  break;

              case 155:
                  if(q){
                  moveFwd();
                  q=0;
                }
                cuatro=getDistance(trigPin4,echoPin4);// der  tras
                cinco=getDistance(trigPin5,echoPin5); //der del
                tres=getDistance(trigPin3,echoPin3);
                medirDistanciaDerecha(cuatro,cinco,13,15);
                if(tres < 15){
                  q=1;
                  c=156;
                }
                break;

            case 156:
                    rotateRight();
                    count=0;
                    c=157;
                    break;

              case 157:
                    if(count >=2000){
                      c=158;
                    }
                    break;

            case 158:
               if(q){
                  moveFwd();
                  q=0;
                }
                uno=getDistance(trigPin1,echoPin1);// der  tras
                dos = getDistance(trigPin2,echoPin2); //der del
                medirDistanciaDerecha(uno,dos,13,15);  
                  if(uno > 20  && dos > 20 ){
                     q=1;
                     c=159;
                  }
                break;

            case 159:
                  rotateLeft();
                  c=160;
                  count=0;
                  break;

            case 160:
                  if(count >=2000){
                    c=161;
                  }
                  break;

            case 161:
                moveFwd();
                count=0;
                c=162;
                break;

            case 162:
                if(count >=7000){
                  c=163;
                }
                break;

            case 163:
                rotateRight();
                count=0;
                c=164;
                break;

            case 164:
                if(count >= 1450)
                {
                  c=165;
                }
                break;
         
            case 165:
              if(q){
                moveFwd();
                q=0;
              }
              uno=getDistance(trigPin1,echoPin1);// der  tras
              dos=getDistance(trigPin2,echoPin2); //der del
              medirDistanciaIzquierda(uno,dos,11,13);
              if(uno > 20 && dos > 20){
                q=1;
                c=166;
              }
              break;

          case 166:
              rotateLeft();
              count=0;
              c=167;
              break;

          case 167:
              if(count >= 1400)
              {
                c=168;
              }      
              break;

          case 168: 
              moveFwd();
              tres = getDistance(trigPin3,echoPin3); //centro
              if(tres < 13)
                c=169;
              break;
          
          case 169:
              rotateLeft();
              count=0;
              c=170;
              break;  

          case 170:
              if(count >= 1400)
              {
                c=171;
              }      
              break;

          case 171:
              if(q){
                moveFwd();
                q=0;
              }
              cuatro = getDistance(trigPin4,echoPin4);//izq del
              cinco = getDistance(trigPin5,echoPin5);//izq tras
              medirDistanciaDerecha(cuatro,cinco,11,13);
              tres = getDistance(trigPin3,echoPin3); //centro
              if (tres < 25){
                q=1;
                c=172;
              }
              break;

          case 172:
               fullStop();
               delay(1000);
               dejarVictima();
              // c=39;
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
  