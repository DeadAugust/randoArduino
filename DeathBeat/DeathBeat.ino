
/*  PulseSensor Starter Project and Signal Tester
 *  The Best Way to Get Started  With, or See the Raw Signal of, your PulseSensor.com™ & Arduino.
 *
 *  Here is a link to the tutorial
 *  https://pulsesensor.com/pages/code-and-guide
 *
 *  WATCH ME (Tutorial Video):
 *  https://www.youtube.com/watch?v=RbB8NSRa5X4
 *
 *
-------------------------------------------------------------
1) This shows a live human Heartbeat Pulse.
2) Live visualization in Arduino's Cool "Serial Plotter".
3) Blink an LED on each Heartbeat.
4) This is the direct Pulse Sensor's Signal.
5) A great first-step in troubleshooting your circuit and connections.
6) "Human-readable" code that is newbie friendly."

*/

//libraries
#include <Adafruit_NeoPixel.h>
//#include <FastLED.h>


//  Pulse Sensor Variables
int PulseSensorPurplePin = 9;        // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 9
int LED8 = 7;   //  The on-board Arduino LED
int Signal;                // holds the incoming raw data. Signal value can range from 0-1024
int Threshold = 550;            // Determine which Signal to "count as a beat", and which to ingore.

//NeoPixel set up
#define  PIXEL_PIN    10 //for led rings
#define PIXEL_COUNT   37
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN);
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);


float deaths[105];
float deathbeats[105];
unsigned long prevMillis = 0.0;
int test = 0;
//Fast LED set up
/*
#define NUM_LEDS 37
#define DATA_PIN 10
#define LED_TYPE WS2812B
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];

#define BRIGHTNESS 96
#define FRAMES_PER_SECOND 120

uint8_t hue = 0;
*/
// The SetUp Function:
void setup() {
  pinMode(LED8,OUTPUT);         // pin that will blink to your heartbeat!
  Serial.begin(9600);         // Set's up Serial Communication at certain speed.

  strip.begin();
  strip.show();

//  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
//  FastLED.setBrightness(BRIGHTNESS);

  //set up the death timer array
  for (int i = 0; i < 105; i++){
    //need to remember float math rules...
    deaths[i] = round(((60.0/105.0)*(i +1))*100.0)/100.0; //trying just one decimal place to see if better
  }
}



// The Main Loop Function
void loop() {
/*
  Signal = analogRead(PulseSensorPurplePin);  // Read the PulseSensor's value.
                                              // Assign this value to the "Signal" variable.

   Serial.println(Signal);                    // Send the Signal value to Serial Plotter.

  
   if(Signal > Threshold){                          // If the signal is above "550", then "turn-on" Arduino's on-Board LED.
//     digitalWrite(LED8,HIGH);
//      for (int i = 0; i < 255; i++){
        for (int n = 0; n < PIXEL_COUNT; n++){
          strip.setPixelColor(n, 255, 0, 0);
        }
        strip.show();
//      }
   } else {
//     digitalWrite(LED8,LOW);                //  Else, the sigal must be below "550", so "turn-off" this LED.
      for (int n = 0; n < PIXEL_COUNT; n++){
        strip.setPixelColor(n, 0, 0, 0);
      }
      strip.show();
   }


delay(10); //needed?
*/

  //need to get bpm, 105 bpm check, and separate fade thing (no protothreading??)
  //maybe only read heart beat a few times a min so it's not constantly flooding with readings?
  
  //if millis && deaths && heartbeats

  //only update the array every minute
  if ((millis() - prevMillis) >= 60000){
    //get bpm from 6 seconds of readings
    unsigned long readSecs = millis();
    int beats = 0;
    int prevSignal = 0;
    while ((millis() - readSecs) < 6000){
      Signal = analogRead(PulseSensorPurplePin);  // Read the PulseSensor's value.
                                                // Assign this value to the "Signal" variable.
//      Serial.println(Signal);                    // Send the Signal value to Serial Plotter.

      //not getting good readings...
      if((prevSignal <= Threshold)&&(Signal > Threshold)){  //toggles so should only pick up distinct beats
        beats++;
  
        digitalWrite(LED8,HIGH); // just for debugging, some visual feedback while wearing it that it's picking up a beat
  //      for (int i = 0; i < 255; i++){
  //        for (int n = 0; n < PIXEL_COUNT; n++){
  //          strip.setPixelColor(n, 255, 0, 0);
  //        }
  //        strip.show();
  //      }
       } 
      else {
       digitalWrite(LED8,LOW);                
  //      for (int n = 0; n < PIXEL_COUNT; n++){
  //        strip.setPixelColor(n, 0, 0, 0);
  //      }
  //      strip.show();
      }
      prevSignal = Signal;
    }
    Serial.println(beats);
    int bpm = beats * 10;
    if (bpm > 180) { //ugh
      bpm = 180;
    }
//    Serial.println("BPM: ");
//    Serial.println(bpm);
//    Serial.println("\n");
    float heartbeats[bpm]; //if this ends up causing memory errors, should just do an array of 105 (what about bpm over 105?)
    for (int i = 0;i < bpm; i ++){
      heartbeats[i] = round(((60.0/float(bpm))*(i+1))*100.0)/100.0;
//      Serial.println(heartbeats[i]);
    }
    //compares the two arrays and stores any matching values in another array
    for (int i = 0; i < 105; i++){ //for each element of deaths
      for (int j = 0; j < bpm; j++){ //compare to each element of heartbeats
//        Serial.println(i);
        if (deaths[i] == heartbeats[j]){ //
          deathbeats[i] = deaths[i];
//          Serial.println("DINGDINGIDNGDINIDNGI");
//          Serial.println(deathbeats[i]);
          break;
        }
        else {
          deathbeats[i] = 0.0;
        }
      }
    }
    prevMillis = millis();
   }

  //the bloom triggers
  float prevCheckTime = 0;
  while (((millis() - prevMillis) < 60000) && (prevMillis != 0.0)){
   float checkTime = (round((millis() - prevMillis) / 1000.0) * 100.0) / 100.0;
   if (prevCheckTime != checkTime){
     for (int i = 0; i < 105; i++){
       if (checkTime ==  deathbeats[i]){
          //do the thing
          Serial.println("bloom!");
          bloom();
          break;
       }
     }
     prevCheckTime = checkTime;
   }
  }
}

void bloom(){
  for (int i = 0; i < 255; i++){ //200 to just not be so bright?
    for (int n = 0; n < PIXEL_COUNT; n++){
      strip.setPixelColor(n, 0, i, 0);
    }
    strip.show();
  }
  for (int i = 255; i >= 0; i--){
    for (int n = 0; n < PIXEL_COUNT; n++){
      strip.setPixelColor(n, 0, i, 0);
    }
    strip.show();
  } 
}
