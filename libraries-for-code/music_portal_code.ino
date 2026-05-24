#include<Wire.h>
#include <DFRobot_MAX98357A.h>
#include <radio.h>
#include <RDA5807M.h>
const int volume_p = 4;
const int station_p = 5;
const int blue = 2;
const int  sleepp = 0;
const int radioo = 15;
const int din = 25;
const int LR = 26;
const int BC = 27;
int vol = 0;
int freq = 0;
const char* music_portal;
DFRobot_MAX98357A amplifier; 
RDA5807M rad;
void setup() {
  Serial.begin(115200);
  pinMode(volume_p, INPUT);
  pinMode(station_p, INPUT);
  pinMode(blue , INPUT_PULLDOWN);
  pinMode(sleepp , INPUT_PULLDOWN);
  pinMode(radioo, INPUT_PULLDOWN);
  amplifier.begin(music_portal, BC, LR , din);
}

void loop() {
  if(digitalRead(sleepp) == HIGH){
   esp_err_t rtc_gpio_pulldown_en(GPIO_NUM_0);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);
    esp_deep_sleep_start();
  }else if(digitalRead(blue)== HIGH){
   amplifier.initBluetooth(music_portal);
   while(digitalRead(blue)== HIGH){
     vol = map(analogRead(volume_p),0,4095,0,9);
     amplifier.setVolume(vol);
   }
   }else if (digitalRead(radioo) == HIGH){
    rad.setMono(true);
    rad.setMute(false);
    rad.setup(RADIO_FMSPACING, RADIO_FMSPACING_100);
    rad.setup(RADIO_DEEMPHASIS, RADIO_DEEMPHASIS_50);
    if(!rad.initWire(Wire)){
      delay(1000);
      ESP.restart();
    }
    while(digitalRead(radioo) == HIGH){
      vol = map(analogRead(volume_p),0,4095,0,9);
      freq = map(analogRead(station_p),0,4095, 8750,10800);
      rad.setBandFrequency( RADIO_BAND_FM, freq);
      rad.setVolume(vol);
    }
   }
  }
