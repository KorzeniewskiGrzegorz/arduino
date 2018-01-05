// *****************************************************************************
// Arduino UNO Temperature and Humidity Monitor using Cypress                 **
// Solar Beacons and HM-10/HM-11 BLE board                                    **
// Software to accompany the article:                                         **
// "Build an Arduino multi-node BLE humidity and temperature sensor monitor"  **
// *****************************************************************************
// This software is offered strictly as is with no warranties whatsoever.     **
// Use it at your own risk.                                                   **
// *****************************************************************************
// SolarBeaconScannerC.ino
// Output to LCD only
//
// NOTES:
// Sensor IDs must be set for 1 (0001 - INDOOR) and (0002 - OUTDOOR)
//
// Tested with both HM10 and HM11 devices BUT...
// Software version of the HM BLE device must be 546
//
// Change the value of the variable isstale to define the time
// to assume 'stale' data
// 



// Modify below for the pins you use
#include <AltSoftSerial.h>
AltSoftSerial BTserial; 


// AT commands and responses for HM10/11
// add a AT+VERR? / HMSoft V546 to the list if desired
char* ATcmds[] = {"AT", "AT+RENEW", "AT+IMME1", "AT+ROLE1"};
char* ATresps[] = {"OK", "OK+RENEW", "OK+Set:1", "OK+Set:1"};

String response = "";

//-------------------------------------------

void setup()
{

    Serial.begin(9600);
  BTserial.begin(9600);   // initialize software serial
  ReadScan(3500); // This will effectively flush the sSerial buffer

}
//-------------------------------------------

void loop()
{
  // Once we are sure that we have set up the HM device
  // we stay in a forever while loop
  
  // send the initial AT commands to the HM10
 // if (SendCmd()) {
   // ReadScan(3500); // This will effectively flush the sSerial buffer
  //}
 // else {
    while (1) {
      // command below gets the scanned string
      BTserial.write("AT+DISI?");
      ReadScan(4000); // 4 seconds works well
      if (BTserial.overflow()) {
        // process error here if you want - but did not occur with a 4 sec scan
      }
     
    
      // check here if we have a stale sensor
      // we look for values = to isstale or 0
      // and set/clear the LCD
     /* if (s1stale==0){
        lcd.setCursor(0,0);
        lcd.print("S");
      }
      if(s1stale==isstale){
        lcd.setCursor(0,0);
        lcd.print("s");
      }
      if(s2stale==0){
        lcd.setCursor(0,1);
        lcd.print("S");
      }
      if(s2stale==isstale){
        lcd.setCursor(0,1);
        lcd.print("s");
      }*/
    }
  
}

//-------------------------------------------
// Send the AT commands to set up the HM10/11
// must return the correct responses
// ok if 0 is returned, error otherwise
int SendCmd(void)
{
  int x, ecount;

  ecount = 0;
  for (x = 0; x < 4; x++) {
    BTserial.write(ATcmds[x]);
    ReadScan(1500);
    if (response != ATresps[x]) {
      ecount++;
    }
  }
  // send ATDELO1 which does not have a response
  BTserial.write("ATDELO1");
  delay(500);
  return (ecount);
}
//-------------------------------------------
// Read the response string from the HM10/11
// stays for duration (seconds) to get a
// complete string

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
//-------------------------------------------
// search the response string for correct
// UUID and get the sensor values if found







