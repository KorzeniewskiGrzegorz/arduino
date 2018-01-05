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
  BTserial.begin(9600);

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
  sensors.begin();
}

void loop()
{
  for(int i=0;i<1000;i++){
    BTserial.write('q');
  }
  sensors.requestTemperatures(); // Send the command to get temperature readings
  float temp = sensors.getTempCByIndex(0);
  
  int var = temp * 100;
  int calosc = var / 100;
  int ulamek = var % 100;

  char hexval[5];
  sprintf(hexval, "%02X%02X", calosc, ulamek);

  delay(500);
  String s1 = String("AT+MINO0x");
  String s2 = String(hexval);

  s1 += s2;
  BTserial.print(s1);

  delay(300);

  for(int i=0;i<8;i++){
  char sl[8]={'A','T','+','S','L','E','E','P'};
    BTserial.write(sl[i]);  
  }
  delay(300);
  while(mojRead()==false){
    digitalWrite(LED_BUILTIN, HIGH);
    for(int i=0;i<8;i++){
        char sl[8]={'A','T','+','S','L','E','E','P'};
        BTserial.write(sl[i]);
    }
    delay(300);
  }
  
  for (int i = 0; i <7; i++) { 
     LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
  }
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
        str.concat(c);
        if(str.indexOf("SLEEP")){
          digitalWrite(LED_BUILTIN, LOW);
          znalezione=true;
          return true;
        }
      }
    }
  }
    return false;
}



