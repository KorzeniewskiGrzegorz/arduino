#include <LowPower.h>
#include <AltSoftSerial.h>
AltSoftSerial BTserial;
// https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
//  Arduino D8 (SS RX) - BT TX no need voltage divider
//  Arduino D9 (SS TX) - BT RX through a voltage divider (5v to 3.3v)
/********************************************************************/
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

char c = ' ';

bool mojRead(void);
void setup()
{
  Serial.begin(9600);
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);
  Serial.println(" ");

  BTserial.begin(9600);
//  Serial.println("BTserial started at 9600");
  /* BTserial.print("AT+RENEW");
    BTserial.print("AT+RESET");
    delay(1000);
    BTserial.print("AT+MARJ0x0001");
    BTserial.print("AT+MINO0xFFFE");
    BTserial.print("AT+ADVI9"); // 1 SEK
    BTserial.print("AT+NAMETemp1");
    BTserial.print("AT+ADTY3");
    BTserial.print("AT+IBEA1");
    BTserial.print("AT+DELO2");
  */
//  pinMode(LED_BUILTIN, OUTPUT);
  sensors.begin();
}

void loop()
{
//  while (BTserial.available())
//  {
//    c = BTserial.read();
//    Serial.write(c);
//  }
    for(int i=0;i<1000;i++){
      BTserial.write('q');
    }
//  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperature readings
// Serial.println("DONE");

  float temp = sensors.getTempCByIndex(0);


  int var = temp * 100;
  int calosc = var / 100;
  int ulamek = var % 100;

  char hexval[5];
  sprintf(hexval, "%02X%02X", calosc, ulamek);

//  Serial.println(temp);
//  Serial.println(hexval);
 delay(500);
  String s1 = String("AT+MINO0x");
  String s2 = String(hexval);

  s1 += s2;
  BTserial.print(s1);
//  while (BTserial.available())
//  {
//    c = BTserial.read();
//    Serial.write(c);
//  }
  delay(300);
//    while (BTserial.available())
//  {
//    c = BTserial.read();
//    Serial.write(c);
//  }
   for(int i=0;i<8;i++){
    char sl[8]={'A','T','+','S','L','E','E','P'};
    // do not send line end characters to the HM-10
      //if (sl!=10 & sl!=13 ) 
//      {  
//        Serial.write(sl[i]);
           BTserial.write(sl[i]);
//      }
   }
      delay(300);
  while(mojRead()==false){
    digitalWrite(LED_BUILTIN, HIGH);
      for(int i=0;i<8;i++){
          char sl[8]={'A','T','+','S','L','E','E','P'};
          // do not send line end characters to the HM-10
            //if (sl!=10 & sl!=13 ) 
            {  
//                     Serial.write(sl[i]);
                 BTserial.write(sl[i]);
            }
         }
         delay(300);
  }
//  Serial.print("\n");
         // delay(500);
          for (int i = 0; i <7; i++) { 
             LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
          }
//          for(int i=0;i<1000;i++){
//            BTserial.write('q');
//          }
}

bool mojRead(void){
  
   String str="";
   bool znalezione = false;
  unsigned long starttick = 0;
  unsigned long curtick = 0;
  starttick = millis();
  if(BTserial.available())
  {
      while ( ( (curtick = millis() ) - starttick) < 2000  && znalezione==false) {
  
         while (BTserial.available() != 0) {
           c = BTserial.read();
//          Serial.write(c);
          str.concat(c);
          if(str.indexOf("SLEEP")){
            digitalWrite(LED_BUILTIN, LOW);
            znalezione=true;
            return true;
          }
      }
    }
  }
  //digitalWrite(LED_BUILTIN, LOW);
    return false;
}



