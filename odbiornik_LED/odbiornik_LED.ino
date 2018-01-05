

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
#define pin 4

#include <AltSoftSerial.h>
AltSoftSerial BTserial;
// https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html

char c;


boolean NL = true;

void setup()
{

  pinMode(pin, OUTPUT); 
  
  Serial.begin(9600);
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);
  Serial.println(" ");

  BTserial.begin(9600);
  Serial.println("BTserial started at 9600");
}

void loop()
{
  String str="";
  unsigned long starttick = 0;
  unsigned long curtick = 0;

  // we stay here for a set amount of time reading the software serial
  starttick = millis();
  if(BTserial.available())
  {
      while ( ( (curtick = millis() ) - starttick) < 50 ) {
  
         while (BTserial.available() != 0) {
           c = BTserial.read();
      
          str.concat(c);
      }
    }
  }

  if(str!="")
  Serial.println(str);


  if(str.equals("LEDON")){

    digitalWrite(pin, HIGH);  
  }

   if(str.equals("LEDOFF")){
    digitalWrite(pin, LOW);  
    
  }
 
    // Read from the Serial Monitor and send to the Bluetooth module
    if (Serial.available())
    {
        c = Serial.read();
 
        // do not send line end characters to the HM-10
        if (c!=10 & c!=13 ) 
        {  
             BTserial.write(c);
        }
 
        // Echo the user input to the main window. 
        // If there is a new line print the ">" character.
        if (NL) { Serial.print("\r\n>");  NL = false; }
        Serial.write(c);
        if (c==10) { NL = true; }
    }
}


  





