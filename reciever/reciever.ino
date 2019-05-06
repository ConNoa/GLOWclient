#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*
* by Dejan Nedelkovski, www.HowToMechatronics.com
*
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define PIN 3
#define NUM_LEDS 228
#define BRIGHTNESS 100
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);


RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
const char but1_text[32] = "but_1_pressed";


void setup() {

  Serial.begin(9600);
  Serial.print("Setup started");
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  Serial.println("Setup finished");


}
void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
      if(but1_text ==  text){
        Serial.println("Action follows now");
      }
      Serial.println(text);
  }
}
