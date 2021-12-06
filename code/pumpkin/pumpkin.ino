#include "Mp3Player.h"
#include "Neopixel.h"

// Pins definieren
#define PIR             12
#define LED_PIN         13 // LED
#define RX_PIN          3 // MP3
#define TX_PIN          2 // MP3 - pin with resistor
#define busyPin         4 // MP3

// weiteres
#define LEDsize         12 // size of Neopixel ring
//#define delayTime       10 // time between color change
#define mp3Folder 1 // all soundfiles must be in folder '01' on sd card

// definition der variablen
//uint16_t numberTracks;
Mp3Player mp3(RX_PIN, TX_PIN, busyPin);        // create DFMiniMp3 instance
Neopixel ledring(LEDsize, LED_PIN);     // create neopixel instance

void setup(){
  ledring.begin();
  pinMode(PIR, INPUT);
  
  delay(500);
  Serial.begin(115200);    // Initialize serial communications with the PC (baud rate != 9600, because that is used by mp3 player)
  mp3.begin();
  delay(3000);
  mp3.setFolder(mp3Folder);  // set folder number
}


// main loop
void loop(){

  if(digitalRead(PIR) == HIGH) {
    Serial.println("motion detected");
    
    mp3.playRandom(); // play random sound from folder 01 -> mp3s must start with 4-digit number, e.g. 0001, 0002, ...
    Serial.println("   show led");
    ledring.rainbow(27, false);
    
    do {} while (mp3.isPlaying());	// wait until sound has been played to the end
    Serial.println("   mp3 off");
    mp3.pause();
    Serial.println("   lights off");
    ledring.off();
    waitMilliseconds(2000); // wait 2s
  }
  
  else {  // no motion
    delay(500);
    Serial.println("scanning");
  }
}

void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();
  
  while ((millis() - start) < msWait)
  {
    // calling mp3.loop() periodically allows for notifications 
    // to be handled without interrupts
    mp3.loop(); 
    delay(1);
  }
}
