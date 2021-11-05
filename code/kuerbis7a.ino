#include "Mp3Player.h"
#include "Neopixel.h"

// Pins definieren
#define PIR             12
#define LED_PIN         13 // LED
#define RX_PIN          3 // MP3
#define TX_PIN          2 // MP3 - der Pin mit Widerstand
#define busyPin         4 // MP3

// weiteres
#define LEDsize         12 // Neopixel-Ring-Größe
#define delayTime       10 // Zeit zwischen den Farben

// definition der variablen
long randNumber;
uint16_t numberTracks;
Mp3Player mp3(RX_PIN, TX_PIN, busyPin);        // create DFMiniMp3 instance
Neopixel ledring(LEDsize, LED_PIN); // number LEDS, PIN

void setup(){
  ledring.begin();
  pinMode(PIR, INPUT);

  // Wenn der analoge Eingangspin 0 nicht verbunden ist, wird zufälliges analoges
  // Rauschen mit randomSeed() benutzt, um verschiedene Seed-Zahlen bei jedem
  // Lauf des Sketches zu generieren.
  // randomSeed() ruft dann die random()-Funktion auf.
  randomSeed(analogRead(0));
  
  delay(500);
  Serial.begin(115200);    // Initialize serial communications with the PC (baud rate != 9600, because that is used by mp3 player)
  mp3.begin();
  delay(3000);
  numberTracks = mp3.getTotalTrackCount();
}


// Hauptschleife (loop)
void loop(){
  
  if(digitalRead(PIR) == HIGH)
  {
    Serial.println("motion detected");

    randNumber = random(numberTracks) + 1;
  
    //Serial.println("   play random");
    //mp3.playRandomTrackFromAll();     // Ton spielt solange bis mp3.pause()
    Serial.print("   play ");Serial.println(randNumber);
    mp3.playCommandSound(randNumber);
    Serial.println("   show led");
    ledring.rainbow(27, false);
    //ledring.theaterChase(ledring.Color(127,   20,   0), 10000); // Orange

    Serial.println("   mp3 off");
    mp3.pause();
    Serial.println("   lights off");
    ledring.off();
    //delay(10000);
    waitMilliseconds(2000);
  }
  else {
    delay(500);
    Serial.println("scanning");
  }
  /*ledring.rainbow2(20);
  mp3.playRandomTrackFromAll();// Ton
  delay(10000);*/
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
