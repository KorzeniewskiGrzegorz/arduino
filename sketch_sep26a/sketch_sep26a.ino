

//  SerialIn_SerialOut_HM-10_01
//
//  Uses hardware serial to talk to the host computer and AltSoftSerial for communication with the bluetooth module
//
//  What ever is entered in the serial monitor is sent to the connected device
//  Anything received from the connected device is copied to the serial monitor
//  Does not send line endings to the HM-10
//
//  Pins
//  BT VCC to Arduino 5V out. 
//  BT GND to GND
//  Arduino D8 (SS RX) - BT TX no need voltage divider 
//  Arduino D9 (SS TX) - BT RX through a voltage divider (5v to 3.3v)
//
 
#include <AltSoftSerial.h>
AltSoftSerial BTserial; 
// https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
 
 /********************************************************************/
// First we include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 2 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

char c=' ';

boolean NL = true;
 
void setup() 
{
    Serial.begin(9600);
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
 
    BTserial.begin(9600);  
    Serial.println("BTserial started at 9600");

    

   
}
 
void loop()
{


    
    // Read from the Bluetooth module and send to the Arduino Serial Monitor
    if (BTserial.available())
    {
        c = BTserial.read();
        Serial.write(c);
    }

  
 
    // Read from the Serial Monitor and send to the Bluetooth module
    if (Serial.available())
    {
        c = Serial.read();
 
        // do not send line end characters to the HM-10
        if (c!=10 & c!=13 & c!='w' ) 
        {  
             BTserial.write(c);
        }

          if(c=='w'){
            for(int i=0;i<8;i++){
              char sl[8]={'A','T','+','S','L','E','E','P'};
              // do not send line end characters to the HM-10
                //if (sl!=10 & sl!=13 ) 
                {  
                     Serial.write(sl[i]);
                     BTserial.write(sl[i]);
                }
            }

//



          }
        
 
        
          
    
        
        if(c=='q'){

          
          for(int i=0;i<1000;i++){
            BTserial.write(c);
          }
         // Serial.println("juz");
        }
 
        // Echo the user input to the main window. 
        // If there is a new line print the ">" character.
        if (NL) { Serial.print("\r\n>");  NL = false; }
        Serial.write(c);
        if (c==10) { NL = true; }
    }

       if (BTserial.available())
    {
        c = BTserial.read();
        Serial.write(c);
    }

}

void ReadScan(unsigned long duration)
{
  char chr;

  // for timer
  unsigned long starttick = 0;
  unsigned long curtick = 0;

  // we stay here for a set amount of time reading the software serial
  starttick = millis();
  while ( ( (curtick = millis() ) - starttick) < duration ) {
    while (BTserial.available() != 0) {
      chr = BTserial.read();
      
        Serial.write(chr);
    
     // response.concat(chr);
    }
  }
}


