

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

char c = ' ';

boolean NL = true;

void setup()
{
  Serial.begin(9600);
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);
  Serial.println(" ");

  BTserial.begin(9600);
  Serial.println("BTserial started at 9600");

  /* BTserial.print("AT+RENEW");
    BTserial.print("AT+RESET");
    delay(1000);
    BTserial.print("AT+MARJ0x0001");
    BTserial.print("AT+MINO0xFFFE");
    BTserial.print("AT+ADVIA"); // 2 SEK
    BTserial.print("AT+NAMETemp1");
    BTserial.print("AT+ADTY3");
    BTserial.print("AT+IBEA1");
    BTserial.print("AT+DELO2");

  */

  // Start up the library
  sensors.begin();
}

void loop()
{
  // Read from the Bluetooth module and send to the Arduino Serial Monitor
  while (BTserial.available())
  {
    c = BTserial.read();
    Serial.write(c);
  }

  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperature readings
  Serial.println("DONE");

  float temp =sensors.getTempCByIndex(0);


  int var = temp * 100;
  int calosc = var / 100;
  int ulamek = var % 100;

  char hexval[5];
  sprintf(hexval, "%02X%02X", calosc, ulamek);





 Serial.println(temp);
  Serial.println(hexval);

  String s1 = String("AT+MINO0x");
  String s2 = String(hexval);

  s1 += s2;
  BTserial.print(s1);


  // Read from the Serial Monitor and send to the Bluetooth module
  if (Serial.available())
  {
    c = Serial.read();

    // do not send line end characters to the HM-10
    if (c != 10 & c != 13 )
    {
      BTserial.write(c);
    }

    // Echo the user input to the main window.
    // If there is a new line print the ">" character.
    if (NL) {
      Serial.print("\r\n>");
      NL = false;
    }
    Serial.write(c);
    if (c == 10) {
      NL = true;
    }
  }


  delay(20000);
}




