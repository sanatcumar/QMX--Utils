/*
Developed by Sanat (AD0BK) to use some basic CAT functions with the QMX+ and an arduino nano.
Arduino nano does not have multiple serial ports and hence using the software serial port.
The software serial port interfaces with the QMX+ Ring(RX)to 11( soft serial TX) and tip(TX) to pin 10 (soft serial rx).
Change the pins as per your layout.
Set the baudrate to 4800 in the QMX+ or change appropriately in the serial setup
One can get the frequency using the CAT IF; command that returns the full status of the transiever
I chose to use the FT; and FA/B; The CAT IF can get the frequency using just one call. It should be fairly
straightforward to code it using this an example
*/

#include <SoftwareSerial.h>
#include <math.h>
SoftwareSerial mySerial(10, 11); // RX, TX  

String cmd ;  // CAT command no "Enter" else QMX will think its a terminal
String response ; // CAT response with a ";" for termination
long num; // converts the incomming string into integers
long frequency;
double wavelength;

void setup() {

Serial.begin(9600);// start USB serial port at 9600 bps:
mySerial.begin(4800);// start software serial port at 4800 bps. 
    while (!Serial) {
    ; // do nothing while wait for serial port to connect
    }

}

/*
send_receive_cat sends the CAT command cmd and fills the response buffer
*/
void send_receive_cat(String cmd){
  while (mySerial.available() <= 0) {
    mySerial.print(cmd); // sends CAT Frequency Read command till you get a response
    delay(500); 
    }
    // once we get a response
if (mySerial.available() > 0) {
 response= mySerial.readStringUntil(';');
   }
  }

 void get_transmit_frequency(){

    cmd="FT;"; // get the active transmit  VFO A (FT0) or VFO B (FT1)
    send_receive_cat(cmd);
    response.remove(0,2);// remove the first two characters and rest are the numbers
    num =  response.toInt();

  if (num ==0)// VFO A is the active transmit
    {
    cmd="FA;"; // get the active transmit VFO A 
    send_receive_cat(cmd);
    response.remove(0,2);// remove the first two characters and rest are the numbers
    frequency =  response.toInt();

    }
  else           // VFO B is the active transmit
    {
    cmd="FB;"; // get the active transmit VFO B 
    send_receive_cat(cmd);
    response.remove(0,2);// remove the first two characters and rest are the numbers
    frequency =  response.toInt();

    }

 }


void loop() {
 
  get_transmit_frequency();
  wavelength = 299792458.0/frequency;
  Serial.println(wavelength);
 

}



