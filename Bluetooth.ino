// --------------------------
//        BLUETOOTH
// --------------------------

void bluetooth_setup() {
  // Streams audio data to the ESP32
  audio.begin();
   
  // Re-connects to last connected device
  audio.reconnect();

  //Play audio
  audio.I2S(AMP_BCLK, AMP_DIN, AMP_LRC);
  audio.volume(0.2);
}

boolean audio_played = true;

uint16_t offsetMax;

String title = "";
String artist = "";
uint16_t title_width = 0;
uint16_t artist_width = 0;

boolean update_tick = false;

void bluetooth_update() {
  if(millis() % 100 == 0) {
    audio.updateMeta();
    title = audio.title;
    artist = audio.artist;
    display.setFont(ArialMT_Plain_16);
    title_width = display.getStringWidth(title);
    artist_width = display.getStringWidth(artist);
    offsetMax = (title_width > artist_width ? title_width : artist_width) + 128;
    display.clear();
    update_tick = true;
  }

  

  String connectionState = audio.connectionState;
  
  if(connectionState == "Disconnected") {
    if(disconnect_sound.available() && !audio_played) {
      int read_size = 8192;
      if(disconnect_sound.available() < read_size) {
        read_size = disconnect_sound.available();
        audio_played = true;
      }
    
      disconnect_sound.read(original_buffer, read_size);
      for(int i = 0; i < read_size; i+= 4) {
        expanded_buffer[i * 4] = original_buffer[i];
        expanded_buffer[i * 4 + 1] = original_buffer[i + 1];
        expanded_buffer[i * 4 + 2] = original_buffer[i + 2];
        expanded_buffer[i * 4 + 3] = original_buffer[i + 3];
        expanded_buffer[i * 4 + 4] = original_buffer[i];
        expanded_buffer[i * 4 + 5] = original_buffer[i + 1];
        expanded_buffer[i * 4 + 6] = original_buffer[i + 2];
        expanded_buffer[i * 4 + 7] = original_buffer[i + 3];
        expanded_buffer[i * 4 + 8] = original_buffer[i];
        expanded_buffer[i * 4 + 9] = original_buffer[i + 1];
        expanded_buffer[i * 4 + 10] = original_buffer[i + 2];
        expanded_buffer[i * 4 + 11] = original_buffer[i + 3];
        expanded_buffer[i * 4 + 12] = original_buffer[i];
        expanded_buffer[i * 4 + 13] = original_buffer[i + 1];
        expanded_buffer[i * 4 + 14] = original_buffer[i + 2];
        expanded_buffer[i * 4 + 15] = original_buffer[i + 3];
      }
      writeAudio(expanded_buffer, read_size * 4);
    }

    if(!disconnect_sound.available() && audio_played) {
      Serial.println("Loading Disonnect Sound...");
      loadDisconnectSound();
    }
    
    statusLED(128, 0, 0);
  } else if(connectionState == "Connected") {
    if(connect_sound.available() && audio_played) {
      int read_size = 8192;
      if(connect_sound.available() < read_size) {
        read_size = connect_sound.available();
        audio_played = false;
      }
    
      connect_sound.read(original_buffer, read_size);
      for(int i = 0; i < read_size; i+= 4) {
        expanded_buffer[i * 4] = original_buffer[i];
        expanded_buffer[i * 4 + 1] = original_buffer[i + 1];
        expanded_buffer[i * 4 + 2] = original_buffer[i + 2];
        expanded_buffer[i * 4 + 3] = original_buffer[i + 3];
        expanded_buffer[i * 4 + 4] = original_buffer[i];
        expanded_buffer[i * 4 + 5] = original_buffer[i + 1];
        expanded_buffer[i * 4 + 6] = original_buffer[i + 2];
        expanded_buffer[i * 4 + 7] = original_buffer[i + 3];
        expanded_buffer[i * 4 + 8] = original_buffer[i];
        expanded_buffer[i * 4 + 9] = original_buffer[i + 1];
        expanded_buffer[i * 4 + 10] = original_buffer[i + 2];
        expanded_buffer[i * 4 + 11] = original_buffer[i + 3];
        expanded_buffer[i * 4 + 12] = original_buffer[i];
        expanded_buffer[i * 4 + 13] = original_buffer[i + 1];
        expanded_buffer[i * 4 + 14] = original_buffer[i + 2];
        expanded_buffer[i * 4 + 15] = original_buffer[i + 3];
      }
      writeAudio(expanded_buffer, read_size * 4);
    }

    if(!connect_sound.available() && !audio_played) {
      Serial.println("Loading Connect Sound...");
      loadConnectSound();
    }
    statusLED(0, 128, 0);

    if(update_tick) {
      display.setFont(ArialMT_Plain_16);
      if(title_width <= 128) {
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(64, 8, title);
      } else {
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.drawString(-((millis() / 50) % offsetMax) + 128, 8, title);
      }
      
      display.setFont(ArialMT_Plain_10);
      if(artist_width <= 128) {
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(64, 28, artist);
      } else {
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.drawString(-((millis() / 50) % offsetMax) + 128, 28, artist);
      }
    }
  }
  if(update_tick) {
    display.display();
    update_tick = false;
  }
}

void bluetooth_shutdown() {
  loadDisconnectSoundAsRadioLoop();
  audio_played = true;
  audio.end();
}
