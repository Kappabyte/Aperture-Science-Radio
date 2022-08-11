#include <btAudio.h>
#include <Wire.h>
#include "SH1106Wire.h"
#include "SPIFFS.h"

// PIN DEFINITIONS
#define DIS_SDA SDA // 21
#define DIS_SCL SCL // 22
#define AMP_LRC 27
#define AMP_BCLK 26
#define AMP_DIN 25
#define LED_R 23
#define LED_G 19
#define LED_B 18
#define SWH_MODE 14
#define SWH_ENABLED 4

// Physical Devices
btAudio audio = btAudio("Aperture Science™ Radio");
SH1106Wire display(0x3c, SDA, SCL);

// Files
File radio_loop;
File connect_sound;
File disconnect_sound;

uint8_t original_buffer[8192];
uint8_t expanded_buffer[32768];

// Status
boolean mode = false;
boolean enabled = false;

void setup() {
  //Enabled for DEBUG
  Serial.begin(9600);

  //Setup pin in/out
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(SWH_MODE, INPUT);
  pinMode(SWH_ENABLED, INPUT);

  //Set the light to red
  statusLED(128, 0, 0);

  //Load the Radio Loop file
  startFS();
  loadRadioLoop();
  loadConnectSound();
  loadDisconnectSound();

  //Set the audio output pins
  audio.I2S(AMP_BCLK, AMP_DIN, AMP_LRC);

  //Initalize the display
  display.init();
  display.flipScreenVertically();

  //Wait for the voltages on the input pins to stabilize
  delay(2000);
}

void loop() {
  //Read switches and determine if the radio is enabled and which mode is currently selected
  boolean newEnabled = analogRead(SWH_ENABLED) > 4000;
  boolean newMode = analogRead(SWH_MODE) > 4000;

  //If changed to enabled, activate the correct mode
  if(newEnabled && !enabled) {
    if(newMode) {
      bluetooth_setup();
    } else {
      other_setup();
    }
    mode = newMode;
  }

  //If changed to disabled, deactivate the correct mode
  if(!newEnabled && enabled) {
    if(newMode) {
      bluetooth_shutdown();
    } else {
      other_shutdown();
    }
  }

  //Update the enabled state
  enabled = newEnabled;

  if(enabled) {
    //If the mode changed to bluetooth, turn off the loop mode, and enable beluetooth
    if(newMode && !mode) {
      other_shutdown();
      bluetooth_setup();
    }
    //If the mode changed to loop, turn off bluetooth, and enable the loop mode
    if(!newMode && mode) {
      bluetooth_shutdown();
      other_setup();
    }

    //Update the current mode
    mode = newMode;

    // Run current mode update function
    if(mode) {
      bluetooth_update();
    } else {
      other_update(); 
    }
  } else {
    //Turn off the LED
    statusLED(0, 0, 0);
  }
}
