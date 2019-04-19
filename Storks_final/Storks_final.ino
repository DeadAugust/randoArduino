#include <SoftwareSerial.h>
#include <Adafruit_Thermal.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
/*
Storks Sketch
by August Luhrs and Hayley Pasley
for Foreverware 4/13/19
eternal gratitude to Adafruit, as always
*/

//printer
//#include "Adafruit_Thermal.h"
//#include "SoftwareSerial.h"
#define TX_PIN 13
#define RX_PIN 12
SoftwareSerial mySerial(RX_PIN, TX_PIN);
Adafruit_Thermal printer(&mySerial);

//neopixel

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(180, PIN, NEO_GRB + NEO_KHZ800);

//unsigned long lastDebounce = 0;
//unsigned long debounce = 1000;
//unsigned long resetTimer = 12000; //12 seconds without buttons resets

/* ---- stork variables
 *  
 *  
 */
#include "HCLogo.h"

//bab(arra)y
unsigned long babyNum = 1348923;
int babyIndex = 0;
int totalBabies = 33; //total (-1 in loop)
char* babyTypes[] = { 
  "high empathy", "mainstream artistic talent", "gluten tolerance", //3
  "high manual dexterity", "acceptance by the crab people", "avant-garde musical talent", //3
  "already having adult teeth", "no nose hair", "mantis shrimp vision", //3
  "  having a predisposition for   badminton", "being lemon scented", "an immunity to propaganda", //3
  "being bioluminescent", "having both lungs and gills", "an innate sense of geolocation", //3
  "  having the ability to accept  others' shortcomings", "a prehensile tail", "being camouflaged at all times", //3
  "having the ability to perceive  time in the 4th Dimension", "being super ripped", //2
  //20
  "laser beam eyes", "Kevlar fingernails", "A+ gut flora", //3
  "having a sidekick", "being able to digest anything", ".#%@;==?",  //3
  "magical melanin", "photo-smellic memory", "gentle intelligence", //3
  "being unfazed by tribulations", "a built-in umbrella", //2
  "being a swiss army baby", "slightly faster reaction times" //2
  //33
//"having indestructible fingernails" -one character too long
};

//button
#define butt 8

//lights
//#define boxLED 7 //press indicator
bool off = true;

int start = true;

void setup() {
  pinMode(butt, INPUT_PULLUP);
//  pinMode(boxLED, OUTPUT);
//  digitalWrite(boxLED, HIGH);
  
  // Serial.begin(9600); // for debug
  mySerial.begin(19200);
  printer.begin();
  
  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  //default lights
//  if (off){
    white();
//  }
  int babyButt = digitalRead(butt);
  
  if (babyButt == LOW){
    //lights
    colorWipe(strip.Color(0, 0, 0), 10); //testing new timing 
    // print the baby info
    printBaby();
//    off = true;
  }
}

void printBaby(){ //after press, what prints out of the printer
  printer.feed(1); //blank space at beginning
  printer.justify('C'); //center justified
  printer.boldOn(); 
  printer.doubleHeightOn();
  printer.println("Congratulations!\n"); //need the newline?
  printer.boldOff();
  printer.doubleHeightOff();

  printer.println("You are receiving baby ##");
  printer.println(babyNum);
  babyNum++; //does baby++ work?

  printer.println("\nThey are pre-vaccinated\n");
  printer.println("and designed with\n");
  printer.println("99.99% chance of \n");
  printer.underlineOn();
  if (babyIndex <  totalBabies - 1) {
    babyIndex++;
  } else {
    babyIndex = 0;
  }
  printer.println(babyTypes[babyIndex]);
  printer.underlineOff();
  
  printer.boldOn();
  printer.println("\nWelcome to the Future!");
  printer.boldOff();

//  printer.printBitmap(HCLogo_width, HCLogo_height, HCLogo_data); //HC Logo
  printer.feed(4); //empty space at end
  
  printer.sleep();
  delay(1000L);
  printer.wake();
  printer.setDefault();
}

void white(){
  uint32_t c =  strip.Color(255, 255, 255);
  for(uint16_t i = 0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  off = false;
}

void colorWipe(uint32_t c, uint8_t wait) {
//  for(uint16_t i = strip.numPixels(); i>=8; i--) { //all but last 8
  for(uint16_t i = 180; i>=8; i--) { //all but last 8
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
