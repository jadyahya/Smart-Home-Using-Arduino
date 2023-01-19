//tinkercad link
//https://www.tinkercad.com/things/kRljL3rjznO-fantabulous-wolt/editel?sharecode=jMx4-ypADRcdr60-c69hRGRCwox0KbcSfh2cm9WbPdc
int Vout=A4;
float Rs;
float ppm;
float power;
int DigIn1 = 4;
int DigIn2 = 2;
int DigOut = 12;

void setup()
{ 
  Serial.begin(9600);
  pinMode (A0,INPUT);
  pinMode (A1,INPUT);
  pinMode (A4,INPUT);
  pinMode (A5,INPUT);
  pinMode(2, INPUT); 
  pinMode(3,OUTPUT);
  pinMode(4, INPUT);
  pinMode(12, OUTPUT);
  pinMode (13, OUTPUT);
}

void loop()
{
  //A
  // analog read gives 102 for zero and 358 for 125 degrees
  //considering other tempertures and their respective output
  //allows for regression
  //the result attained is below
  float temp=(-49.7713706+(3723.0/7639.0)*analogRead(A0)); 
  //using linear regression
  //or from data sheet
  //float temp=((5000.0*analogRead(A0)/1023.0)-500.0)/10.0;
  Serial.print("The temperature in the room is: ");
  Serial.print(temp);
  Serial.println(" degrees C");
  
  //B
  float preset=((analogRead(A1)/1023.0)*4.0+24);// scaling to 24-28
  float lower=preset-1.0;
  float upper=preset+1.0;
  // using pwm to control switch opening and closure
  //errors may be encountered
  Serial.print("The preset temperature is: ");
  Serial.print(preset);
  Serial.println(" degrees C");
    if (temp<lower){
     power=0;      
    analogWrite(3,0);
    }
  else if (temp<=30 && temp>=upper){
      power=0.3;  
      analogWrite(3,power*255);
      Serial.println("Fan Speed: 30% Maximum speed");
  }else if (temp>30 && temp<=32){
      power=0.6;
      analogWrite(3,power*255);
      Serial.println("Fan Speed: 60% Maximum speed");
  }else if (temp>32 && temp<=34){
      power=0.8;
      analogWrite(3,power*255);
      Serial.println("Fan Speed: 80% Maximum speed");
  }else if(temp>34){
      power=1;
      analogWrite(3,power*255);
      Serial.println("Fan Speed: 100% Maximum speed");
  }
  if (power==0){
    Serial.println("Fan is OFF");
  }else if(power!=0 && temp<upper){
    Serial.println("Fan Speed: 30% Maximum speed");
    Serial.println("Fan is ON");
  }else{
    Serial.println("Fan is ON");
  }
//C
// We changed our code in the overall circuit, since the former was working
// on the PIR when tested individually, but not in the wholeSmart Home project.
 int  PIRstate;
 int Buttonstate;
 PIRstate=digitalRead(2);
 Buttonstate=digitalRead(4);
 if (PIRstate==HIGH)
 {
   Serial.println("Movement detected!");
   digitalWrite(DigOut,HIGH);
  //keep LED on until button pressed
  //Please note that for the Button to work, onw has top hold for a second.
  }
   if (Buttonstate==HIGH)
  digitalWrite(DigOut,LOW);
 
 
//D
float photoR;
float brightness=0;
float outputLED=0;
  
photoR = analogRead(A5);
brightness = 0.0006575*(photoR*photoR)+0.1561*(photoR)-0.2837;
// polynomial approximation from MATLAB 
// Brightness ranging from 0 to 100

  
Serial.print("The % of perceived brightness from Photoresistor: ");
Serial.println(brightness);
  
outputLED = map(brightness,0,100,255,0);
Serial.print("The LED brightness on a scale of 0 to 255: ");
  Serial.println(outputLED);
  if (brightness>=50){
    Serial.println("Day time");
 } else{
      Serial.println("Night time");
}
// Led values from 0 to 255
// We started with 255 in line 22 since we are looking for inverse 
// relation between brightness perceived and brightness of LED.
analogWrite(13,outputLED);
//E
 Rs=((11.0*1023.0/analogRead(Vout))-11.0);
 ppm=pow(10,(log10(Rs/5.5)-1.292030)/(-0.430677));
 Serial.print("The LPG Concentration is: ");
 Serial.print(ppm);
 Serial.println(" ppm");
 Serial.println();
}