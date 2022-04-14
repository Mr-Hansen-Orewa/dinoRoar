/*
  Much of this code was grabbed from the MP3 Shield Trigger and FilePlayer example
  included with the SFEMP3Shield library. Major thanks to Jim Lindblom Bill
  Porter and Michael Flaga, again, for this amazing library!
*/
#include <SPI.h>           // SPI library
#include <SdFat.h>         // SDFat Library
#include <SFEMP3Shield.h>  // Mp3 Shield Library

//MP3
SdFat sd; // Create object to handle SD functions
SFEMP3Shield MP3player; // Create Mp3 library object

byte volume = 128; // MP3 Player volume 0=max, 255=lowest (off)
const byte MONOMODE = 1;  // Mono setting 0=off, 3=max


//PIR
const byte PIRAOUT = A1;  // PIR analog output on A1
const byte PIRDOUT = 2;   // PIR digital output on D2
const byte LEDPIN = 13;  // LED to illuminate on motion

const byte PRINTTIME 100 // Rate of serial printouts
unsigned long lastPrint = 0; // Keep track of last serial out


//Random number generation
const byte RSEED = A0;  // unconnected analog output to seed random number
const byte RMIN = 0;
const byte RMAX = 5;

void setup() {
  pinMode(PIRAOUT, INPUT);
  pinMode(PIRDOUT, INPUT);

  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW); // Turn the LED off

  Serial.begin(9600);
  initSD();  // Initialize the SD card
  initMP3Player(); // Initialize the MP3 Shield

  randomSeed(analogRead(RSEED));  //seed the random number generator

  Serial.println("Setup finished");
}

/*
  pick a random number, call the roarChoice method, wait for 3 seconds
 */
void loop() {
  // print a random number from 0 to 4
  randNumber = random(RMIN, RMAX);
  Serial.println("Random number chosen is " + String(randNumber));

  roarChoice(randNumber);
  // wait for the roar to finish
  delay(3000);
}

/*
  initialise the SD card, brief error checks
*/
void initSD() {
  //Initialize the SdCard.
  if (!sd.begin(SD_SEL, SPI_HALF_SPEED)) {
    sd.initErrorHalt();
    Serial.println("Error initialising the SD Card");
  }
  if (!sd.chdir("/")) {
    sd.errorHalt("sd.chdir");
    Serial.println("Error setting the root directory");
  }
}

/*
  initialise the MP3 player, brief error check, set volume for L and R and mono
*/
void initMP3Player() {
  byte result = MP3player.begin();
  if (result != 0) {
    Serial.println("Error initialising the MP3 Shield");
    Serial.println("Error code is " + String(result));
    /*
      0 OK
      1 *Failure of SdFat to initialize physical contact with the SdCard
      2 *Failure of SdFat to start the SdCard's volume
      3 *Failure of SdFat to mount the root directory on the volume of the SdCard
      4 Other than default values were found in the SCI_MODE register.
      5 SCI_CLOCKF did not read back and verify the configured value.
      6 Patch was not loaded successfully. This may result in playTrack errors
    */
  }
  MP3player.setVolume(VOLUME, VOLUME);
  MP3player.setMonoMode(MONOMODE);
}

/*
  check for motion, set the volume based on the Analog PIR output, call the playTrack method, brief error check
 */
void roarChoice(byte choice) {
  // The OpenPIR's digital output is active high
  int motionStatus = digitalRead(PIRDOUT);

  if (motionStatus == HIGH) {
    Serial.println("Motion detected");

  // MP3 Player volume 0=max, 255=lowest (off)
  volume = map(analogRead(PIRAOUT), 0, 1023, 255, 0);
  MP3player.setVolume(volume, volume);
    
    Serial.print("choice is ");
    switch (choice) {
      case 0:
        Serial.println("Roar 0");
        playTrack("track000.mp3");
        break;
      case 1:
        Serial.println("Roar 1");
        playTrack("track001.mp3");
        break;
      case 2:
        Serial.println("Roar 2");
        playTrack("track002.mp3");
        break;
      case 3:
        Serial.println("Roar 3");
        playTrack("track003.mp3");
        break;
      case 4:
        Serial.println("Roar 4");
        playTrack("track004.mp3");
        break;
      default:
        Serial.println("");
        Serial.println("choice picked a roar that doesn't exist");
        break;
    }
  }
//  } else {
//    Serial.println("No motion detected");
//    //if a track is playing stop it
//    if (MP3player.isPlaying()) {
//      MP3player.stopTrack();
//    }
//  }
}

/*
  stop the track if its still playing, play the fileName mentioned, brief error check
 */
void playTrack(String fileName) {
  //if a track is playing stop it
  if (MP3player.isPlaying()) {
    MP3player.stopTrack();
  }
  //play the named track and check if it works
  byte result = MP3player.playMP3(fileName);
  if (result == 0) {
    Serial.println(fileName + " is playing");
  } else {
    Serial.println("Error the track isn't playing for some reason");
    Serial.println("Error code is " + String(result));
    /*
      0 OK
      1 Already playing track
      2 File not found
      3 indicates that the VSdsp is in reset.
    */
  }
}
