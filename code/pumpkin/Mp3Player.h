#ifndef __MP3PLAYER__
#define __MP3PLAYER__

/* https://github.com/Makuna/DFMiniMp3/wiki/API-Reference */

#include <Arduino.h>
#include <DFMiniMp3.h>
#include <SoftwareSerial.h>

// implement a notification class,
// its member methods will get called 
//
class Mp3Notify {
public:
  static void OnError(uint16_t errorCode) {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.print(errorCode);
    Serial.print(": ");
    
    switch (errorCode) {
      case DfMp3_Error_Busy: {
          Serial.print(F("1- player busy"));      // Es läuft eine Wiedergabe
          break;
        }
      case DfMp3_Error_Sleeping: {
          Serial.print(F("2- sleep"));             // Df Player im Sleepmodus
          break;
        }
      case DfMp3_Error_SerialWrongStack: {
          Serial.print(F("3- memory"));               // Fehler interer Speicher
          break;
        }
      case DfMp3_Error_CheckSumNotMatch: {
          Serial.print(F("4- hardware communication")); // Hardware Kommunikationsfehler
          break;
        }
      case DfMp3_Error_FileIndexOut: {
          Serial.print(F("5- file index out of bounds"));   // Dateinummer außerhalb des Bereiches
          break;
        }
      case DfMp3_Error_FileMismatch: {
          Serial.print(F("6- file not found"));    // Datei nicht gefunden
          break;
        }
      case DfMp3_Error_Advertise: {
          Serial.print(F("7- advert"));            // Fehler beim Abspielen Advert
          break;
        }
      case DfMp3_Error_RxTimeout: {
          Serial.print(F("rx timeout"));           // Zeitüberschreitung RX-Signal
          break;
        }
      case DfMp3_Error_PacketSize: {
          Serial.print(F("packet size"));          // Fehler Sektorgröße
          break;
        }
      case DfMp3_Error_PacketHeader: {
          Serial.print(F("packet header"));        // Fehler Header
          break;
        }
      case DfMp3_Error_PacketChecksum: {
          Serial.print(F("packet checksum"));      // Prüfsummenfehler
          break;
        }
      case DfMp3_Error_General: {
          Serial.print(F("general"));              // allgemeiner Fehler
          break;
        }
      default: {
          Serial.print(F("unknown"));              // unbekannter Fehler
          break;
        }
    }
    Serial.println(F(" error"));
  }

  static void OnPlayFinished(DfMp3_PlaySources source, uint16_t track) {
    Serial.println();
    Serial.print("Play finished for #");
    Serial.println(track);   
  }

  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action) {
    if (source & DfMp3_PlaySources_Sd) Serial.print("SD Karte ");
    if (source & DfMp3_PlaySources_Usb) Serial.print("USB ");
    if (source & DfMp3_PlaySources_Flash) Serial.print("Flash ");
    Serial.println(action);
  }

  static void OnPlaySourceOnline(DfMp3_PlaySources source) {
    PrintlnSourceAction(source, "online");
  }
  static void OnPlaySourceInserted(DfMp3_PlaySources source) {
    PrintlnSourceAction(source, "bereit");
  }
  static void OnPlaySourceRemoved(DfMp3_PlaySources source) {
    PrintlnSourceAction(source, "entfernt");
  }
};

class Mp3Player: public DFMiniMp3<SoftwareSerial, Mp3Notify> {
  private:
  SoftwareSerial mySoftwareSerial;

  public:
  uint16_t numTracksInFolder;
  uint8_t currentTrack;
  uint8_t currentFolder;
  long randNumber;
  byte busyPin;
  Mp3Player(byte rx, byte tx, byte busy) :  busyPin(busy), mySoftwareSerial(rx, tx), DFMiniMp3<SoftwareSerial, Mp3Notify>(mySoftwareSerial)
  {}

  void begin() {  // overrides begin() of base class
    Serial.println("Initialize mp3 player");
    DFMiniMp3::begin(); // caution: uses 9600 for software serial connection
    reset();
    setVolume(40);

    // Uses the unconnected input pin 0 to generate different seed numbers.
    // randomSeed() calls the random()function.
    randomSeed(analogRead(0));
  }

  void setFolder(uint8_t folder) {
    currentFolder = folder;
    numTracksInFolder = getFolderTrackCount(folder);
    currentTrack = 1;
    Serial.print("Set Folder to ");
    Serial.print(folder);
    Serial.print(" (");
    Serial.print(numTracksInFolder);
    Serial.println(" tracks in folder)!");
  }

  void play() {
    Serial.print("   play "); Serial.println(currentTrack);
    playFolderTrack16(currentFolder, currentTrack);
  }

  void playRandom() {
    randNumber = random(numTracksInFolder) + 1;
    uint16_t currentTrack = randNumber;
    play();
  }

  bool isPlaying() { 
    return !digitalRead(busyPin); 
  }

  /*// override
  void loop() {
    DFMiniMp3::loop();
    //Serial.println("Extra!");
    //listenForReply(0x00);
    uint8_t replyCommand = 0;
    uint16_t replyArg = 0;
    
  }*/
  
};
#endif
