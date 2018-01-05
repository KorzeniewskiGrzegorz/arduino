#include <TimerOne.h>



#define pin 4
int ticks=25;
int state=0;
int change=0;

void setup()
{
  pinMode(pin, OUTPUT); 
    Timer1.initialize(2000000);         // initialize timer1, and set a 1/2 second period

  Timer1.attachInterrupt(callback); 
}

void loop()
{
  static int counter=ticks;
  
  while(counter--)
  {
    if(!counter){
      digitalWrite(pin, (state) ? HIGH : LOW);
      state = !state;

     
      
      counter=ticks;
    }
    delay(10);
  }

}

void callback()
{
    ticks=random(5,25);
}
  





