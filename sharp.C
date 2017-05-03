
// variables sharp //
int AnPin = 2;                                    // analog pin for reading the IR sensor
int val2,i=0;             // outgoing ADC value
int distance = 0;
int totalDistance=0;
int avgDistance=0;
////




void setup() {
  Serial.begin(9600);   // start the serial port

}

void loop() {

  do{
     // read analog input:
     val2 = analogRead(AnPin);
     // Calculate linear slope
     distance = (2914 / (val2 + 5)) - 1;
     totalDistance+=distance;
     i++;
   }while (i<20);

   avgDistance= totalDistance/20;
   i=0;

//cuando llega a refugio reset a los valores
   avgDistance=0;
   totalDistance=0;


   delay(1000);
}
