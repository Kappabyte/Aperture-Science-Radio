// --------------------------
//        LOOP MODE
// --------------------------

void other_setup() {
  // Streams audio data to the ESP32
  statusLED(0, 64, 128);
}

void other_update() {
  if(!radio_loop.available()) {
    loadRadioLoop();
  }

  int read_size = 8192;
  if(radio_loop.available() < read_size) read_size = radio_loop.available();

  radio_loop.read(original_buffer, read_size);
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

  display.clear();
  
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);
  display.drawString(64, 8, "85.2 FM");
  
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(128, 54, "Other");
  
  display.display();
}

void other_shutdown() {
  audio.end();
}
