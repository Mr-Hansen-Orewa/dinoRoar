/*

  This is an example MP3 trigger sketch for the SparkFun MP3 Shield.
  Pins 0, 1, 5, 10, A0, A1, A2, A3, and A4 are setup to trigger tracks
  "track001.mp3", "track002.mp3", etc. on an SD card loaded into
  the shield. Whenever any of those pins are shorted to ground,
  their respective track will start playing.

  When a new pin is triggered, any track currently playing will
  stop, and the new one will start.

  A5 is setup to globally STOP playing a track when triggered.

  Much of this code was grabbed from the MP3 Shield Trigger and FilePlayer example
  included with the SFEMP3Shield library. Major thanks to Jim Lindblom Bill
  Porter and Michael Flaga, again, for this amazing library!
*/

#include <SPI.h>           // SPI library
#include <SdFat.h>         // SDFat Library
//#include <SdFatUtil.h>     // SDFat Util Library
#include <SFEMP3Shield.h>  // Mp3 Shield Library

SdFat sd; // Create object to handle SD functions

SFEMP3Shield MP3player; // Create Mp3 library object
// These variables are used in the MP3 initialization to set up
// some stereo options:
const byte VOLUME = 0; // MP3 Player volume 0=max, 255=lowest (off)
const byte MONOMODE = 1;  // Mono setting 0=off, 3=max

/* Pin setup */
//#define TRIGGER_COUNT 9
//int triggerPins[TRIGGER_COUNT] = {0, 1, 5, 10, A0, A1, A2, A3, A4};
//int stopPin = A5; // This pin triggers a track stop.
//int lastTrigger = 0; // This variable keeps track of which tune is playing


//PIR
const byte PIR_AOUT = A0;  // PIR analog output on A0
const byte PIR_DOUT = 2;   // PIR digital output on D2
const byte LED_PIN = 13;  // LED to illuminate on motion

const byte PRINT_TIME 100 // Rate of serial printouts
unsigned long lastPrint = 0; // Keep track of last serial out


const byte RMIN = 0;
const byte RMAX = 5;

void setup() {
  /* Set up all trigger pins as inputs, with pull-ups activated: */
  //  for (int i = 0; i < TRIGGER_COUNT; i++) {
  //    pinMode(triggerPins[i], INPUT_PULLUP);
  //  }
  //  pinMode(stopPin, INPUT_PULLUP);

  initSD();  // Initialize the SD card
  initMP3Player(); // Initialize the MP3 Shield


  //PIR
  // Analog and digital pins should both be set as inputs:
  pinMode(PIR_AOUT, INPUT);
  pinMode(PIR_DOUT, INPUT);

  // Configure the motion indicator LED pin as an output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Turn the LED off

  //RANDOM
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
}

// All the loop does is continuously step through the trigger
//  pins to see if one is pulled low. If it is, it'll stop any
//  currently playing track, and start playing a new one.
void loop() {

 //RANDOM
  // print a random number from 0 to 4
  randNumber = random(RMIN, RMAX);
  Serial.println(randNumber);
  
      lastTrigger = i + 1; // Update lastTrigger variable to current trigger
      /* If another track is playing, stop it: */
      if (MP3player.isPlaying()) {
        MP3player.stopTrack();
      }
      /* Use the playTrack function to play a numbered track: */
      uint8_t result = MP3player.playTrack(lastTrigger);
      // An alternative here would be to use the
      //  playMP3(fileName) function, as long as you mapped
      //  the file names to trigger pins.

      if (result == 0) { // playTrack() returns 0 on success
        // Success
      }
      else { // Otherwise there's an error, check the code
        // Print error code somehow, someway
      }
 
  // After looping through and checking trigger pins, check to
  //  see if the stopPin (A5) is triggered.
  if (digitalRead(stopPin) == LOW) {
    // lastTrigger = 0; // Reset lastTrigger
    // If another track is playing, stop it.
    if (MP3player.isPlaying()) {
      MP3player.stopTrack();
    }
  }

  //PIR
  // Read OUT pin, and set onboard LED to mirror output
  readDigitalValue();

  // Read A pin, print that value to serial port:
  printAnalogValue();

 

}
/*
  initSD() initializes the SD card and checks for an error
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
  initMP3Player() sets up all of the initialization for the
  MP3 Player Shield. It runs the begin() function, checks
  for errors, applies a patch if found, and sets the volume/stero mode
*/
void initMP3Player() {
  byte result = MP3player.begin();
  if (result != 0) {
    Serial.println("Error initialising the MP3 Shield");
  }
  MP3player.setVolume(VOLUME, VOLUME);
  MP3player.setMonoMode(MONOMODE);
}


//PIR
void readDigitalValue()
{
  // The OpenPIR's digital output is active high
  int motionStatus = digitalRead(PIR_DOUT);

  if (motionStatus == HIGH) {
    Serial.println("Digital motion is HIGH");
    digitalWrite(LED_PIN, HIGH);
  } else {
    Serial.println("Digital motion is HIGH");
    digitalWrite(LED_PIN, LOW);
  }
}


//PIR
void printAnalogValue()
{
  if ( (lastPrint + PRINT_TIME) < millis() )
  {
    lastPrint = millis();
    // Read in analog value:
    unsigned int analogPIR = analogRead(PIR_AOUT);
    // Convert 10-bit analog value to a voltage
    // (Assume high voltage is 5.0V.)
    float voltage = (float) analogPIR / 1024.0 * 5.0;
    // Print the reading from the digital pin.
    // Mutliply by 5 to maintain scale with AOUT.
    Serial.print(5 * digitalRead(PIR_DOUT));
    Serial.print(',');    // Print a comma
    Serial.print(2.5);    // Print the upper limit
    Serial.print(',');    // Print a comma
    Serial.print(1.7);    // Print the lower limit
    Serial.print(',');    // Print a comma
    Serial.print(voltage); // Print voltage
    Serial.println();
  }
}

void roarChoice(byte choice) {
  // The OpenPIR's digital output is active high
  int motionStatus = digitalRead(PIR_DOUT);

  if (motionStatus == HIGH) {
    Serial.println("Roar");
    switch (choice) {
      case 0:
        Serial.println("Roar 0");
        //more track playing code here
        break;
      case 1:
        Serial.println("Roar 1");
        //more track playing code here
        break;
      case 2:
        Serial.println("Roar 2");
        //more track playing code here
        break;
      case 3:
        Serial.println("Roar 3");
        //more track playing code here
        break;
      case 4:
        Serial.println("Roar 4");
        //more track playing code here
        break;
      default:
        Serial.println("Random picked a roar that doesn't exist");
        break;
    }
  } else {
    Serial.println("don't Roar");
    //if a track is playing stop it
    if (MP3player.isPlaying()) {
      MP3player.stopTrack();
    }
  }
}
