#define BLYNK_PRINT Serial  // Blynk serial
#include <ESP8266WiFi.h>    // wifi
#include <BlynkSimpleEsp8266.h> // ไลบลารี่ Blynk app esp8266
#include <TridentTD_LineNotify.h> // line notify
#define LINE_TOKEN "Tc7MhOtauvqaGDoyJ0QOvXlNErqk05BPCbNvLa5DxFs"
#define INPUT_PIN 13


WidgetLED led1(V1);

int mVperAmp = 66; // use 100 for 20A Module and 66 for 30A Module
int RawValue= 0;
int ACSoffset = 2525;
double Voltage = 0;
double Amps = 0;

int ldr;

//float vi;
//float ii;
//float pi;
//float ei;
//int ledlevelA;

float VRMS = 0;
float vol = 0;
float am = 0;
float wat = 0;
float bath = 0;
float total = 0;

//timer
int Ti;

String eiei = "ค่าไฟที่ตอนนี้ คือ ";

BLYNK_WRITE(V5){
Ti = param.asInt();    
}


BlynkTimer timer;
//char auth[] = "3Yk6obm-Iao-N7aF-52YLPTRCPN2hTXX";
char auth[] = "yUacjJGl6a1_YMlliconUg3JFxiBgjMe";
//char ssid[] = "SASASASAS";
//char password[] = "cc445566";
char ssid[] = "iSr4p";
char password[] = "sr12345678";


void sendSensor(){
 Blynk.virtualWrite(V2,wat);
 Blynk.virtualWrite(V3,bath);
 Blynk.virtualWrite(V10,vol);
 Blynk.virtualWrite(V11,am);
 Blynk.virtualWrite(V12,wat);
}

void noti(){
  LINE.notify("ค่าไฟที่ตอนนี้ คือ "+String(bath,3)+" Bath");
}



void setup() {
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

Serial.println(LINE.getVersion());
  LINE.setToken(LINE_TOKEN);
  Blynk.begin(auth, ssid, password);
//  pzem.setAddress(ip);
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(60000L, noti);

  pinMode(0,OUTPUT); //relay
//  pinMode(5, INPUT); //pir
pinMode(INPUT_PIN, INPUT_PULLUP);
  pinMode(16, INPUT); //ldr
  pinMode(14, INPUT_PULLUP); //sw
}

void loop() {
  Blynk.run();
  timer.run();
 
//analogWrite(4, 10); levellight
//0=มีแสง 1=ไม่มี
//0*ไม่เจอ 1*เจอ

ldr = digitalRead(16); 
int pir = digitalRead(INPUT_PIN);
int swauto = digitalRead(14);
int swlight = digitalRead(15);
RawValue = analogRead(A0);

//Voltage = (RawValue/1024.0)*5000; // Gets you mV

Voltage = (5*RawValue)/1023;
VRMS = (Voltage/2) *0.707;
Amps = (VRMS * 1000)/mVperAmp;

//Amps = ((Voltage - ACSoffset) / mVperAmp);
//Amps = (((RawValue-511)*5)/1023)/0.0645;

//for(int i = 0; i < 1000; i++) {
//Voltage = (Voltage + (.0049 * analogRead(A0)));
//delay(1);
//}
//Voltage = Voltage/1000;
//Amps = (Voltage-2.5)/0.066;

// if(watt )

if(swauto == 1){
  ldr = 1;
   if(ldr == 1 && pir == 1   ){
 digitalWrite(0,LOW);
  led1.on();
   vol = Voltage;
 am = Amps;
wat = (Voltage*Amps);
total = total + wat;
float demo = (Voltage*Amps)/3600;
float bathdemo = ((demo/1000) * 0.5);
bath = (bathdemo*3.222)+bath;
   } 
   else {
   led1.off();
   ldr = 0;
digitalWrite(0,HIGH);

      vol = 0;
 am = 0;
wat = 0;
total = total + 0;
   }
} 

else {
  if(swlight == 1 || Ti == 1){
   led1.on();
  digitalWrite(0,LOW);
   vol = Voltage;
 am = Amps;
wat = (Voltage*Amps);
total = total + wat;
float demo = (Voltage*Amps)/3600;
float bathdemo = ((demo/1000) * 0.5);
bath = (bathdemo*3.222)+bath;
   }

   
   
   else {
    led1.off();
    digitalWrite(0,HIGH);
      vol = 0;
 am = 0;
wat = 0;
total = total + 0;
   }
}


 
 Serial.print("pir : ");
 Serial.print(pir);
 Serial.print(" || ldr : ");
 Serial.println(ldr); 

Serial.print("Raw Value = " ); // shows pre-scaled value
Serial.print(RawValue);
Serial.print("\t mV = "); // shows the voltage measured
Serial.print(Voltage,3); // the '3' after voltage allows you to
Serial.print("\t Amps = "); // shows the voltage measured
Serial.println(Amps,3);

Serial.print("amp = "); // shows the voltage measured
Serial.print(am,3); // the '3' after voltage allows you to
Serial.print(" watt = "); // shows the voltage measured
Serial.println(wat,3);
 delay(1000);
}
