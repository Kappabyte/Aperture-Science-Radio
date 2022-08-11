
#define LED_R 23
#define LED_G 19
#define LED_B 18

void statusLED(char r, char g, char b) {
  analogWrite(LED_R, r);
  analogWrite(LED_G, g);
  analogWrite(LED_B, b);
}

void writeAudio(const uint8_t *data, uint32_t len){
  size_t i2s_bytes_write = 0; 
  int16_t* data16=(int16_t*)data;
  int16_t fy[2];
  float temp;
  
  int jump =4; //how many bytes at a time get sent to buffer
  int  n = len/jump; // number of byte chunks  
  for(int i=0;i<n;i++){
     //process left channel
     fy[0] = (int16_t)((*data16)*0.05);
     data16++;
     // process right channel
     fy[1] = (int16_t)((*data16)*0.05);
     data16++;
     i2s_write(I2S_NUM_0, fy, jump, &i2s_bytes_write,  100 ); 
  }
}

void startFS() {
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}

void loadRadioLoop() {
  radio_loop.close();
  radio_loop = SPIFFS.open("/radio_loop.raw");
  if(!radio_loop){
    Serial.println("Failed to open radio_loop.raw for reading");
    return;
  }
}

void loadConnectSound() {
  connect_sound.close();
  connect_sound = SPIFFS.open("/connect.raw");
  if(!connect_sound){
    Serial.println("Failed to open connect.raw for reading");
    return;
  }
}

void loadDisconnectSound() {
  disconnect_sound.close();
  disconnect_sound = SPIFFS.open("/disconnect.raw");
  if(!disconnect_sound){
    Serial.println("Failed to open disconnect.raw for reading");
    return;
  }
}

void loadDisconnectSoundAsRadioLoop() {
  radio_loop.close();
  radio_loop = SPIFFS.open("/disconnect.raw");
  if(!radio_loop){
    Serial.println("Failed to open disconnect.raw for reading");
    return;
  }
}
