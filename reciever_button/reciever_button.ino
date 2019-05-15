// NeoPixel test program showing use of the WHITE channel for RGBW
// pixels only (won't look correct on regular RGB NeoPixel strips).

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define LED_PIN 3
#define LED_COUNT 228

uint8_t BRIGHTNESS = 60;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

RF24 wirelessSPI(7, 8); // CE, CSN

#define PinIRQ 2  // Arduino Uno, Mega und Nano: Pin 3
#define IRQ 0

const byte address[6] = "00001";


boolean light_on = false;
boolean mode_a_active = false;
boolean mode_b_active = false;

boolean receivedMessage = false;
int interruptcounter = 0;
int loopcounter = 0;


uint8_t settings[6] = {0, 0, 0, 0, 0, 0};                 //Saving the incoming data


void setup() {
  pinMode(6, OUTPUT);
  Serial.begin(9600);
  wirelessSPI.begin();
  wirelessSPI.setAutoAck(1);                  //new
  wirelessSPI.enableAckPayload();              //new
  wirelessSPI.maskIRQ(1,1,0);                 //new
  wirelessSPI.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  wirelessSPI.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  wirelessSPI.startListening();              //This sets the module as receiver

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  attachInterrupt(IRQ, incommingmessage, HIGH);
}

void loop()
{
  //Serial.print("Loop counts: ");
//  Serial.println(loopcounter);
while (wirelessSPI.available()) {

  wirelessSPI.read(&settings, sizeof(settings));  //Reading the data
}
  if(receivedMessage == true){
    Serial.println("messange on interupt received");
    Serial.print("interruptcounter counts: ");
    Serial.println(interruptcounter);
    Serial.println();
    Serial.print(settings[0]);
    Serial.print(settings[1]);
    Serial.print(settings[2]);
    Serial.print(settings[3]);
    Serial.print(settings[4]);
    Serial.println(settings[5]);
    receivedMessage = false;
  }

  /*
  if (wirelessSPI.available())              //Looking for the data.
  {
    radio.read(&settings, sizeof(settings));  //Reading the data

    Serial.print(settings[0]);
    Serial.print(settings[1]);
    Serial.print(settings[2]);
    Serial.print(settings[3]);
    Serial.print(settings[4]);
    Serial.println(settings[5]);

    attachSettings();

    if (button_state == HIGH)
    {
      digitalWrite(6, HIGH);
      light_on = false;
      mode_a_active = true;
      button_state = LOW;
    }
    if (button_state2 == HIGH)
    {
      digitalWrite(6, HIGH);
      light_on = true;
      mode_a_active = true;
      Serial.println(text);
      Serial.println("Scene a active");
      button_state2 = LOW;
    }
    if (button_state3 == HIGH)
    {
      digitalWrite(6, HIGH);
      light_on = true;
      mode_a_active = false;
      Serial.println(text);
      Serial.println("Scene b active");
      button_state3 = LOW;
    }
    if (light_on == true) {
      Serial.println("Light is on");
      colorWipe(strip.Color(255,   0,   0)     , 50); // Red
      colorWipe(strip.Color(  0, 255,   0)     , 50); // Green
      colorWipe(strip.Color(  0,   0, 255)     , 50); // Blue
      colorWipe(strip.Color(  0,   0,   0, 255), 50); // True white (not RGB white)


    }
  }
  */
  loopcounter++;
}

void incommingmessage()
{
  interruptcounter++;
  while (wirelessSPI.available()) {
    wirelessSPI.read(&settings, sizeof(settings));  //Reading the data

    receivedMessage = true;    // Variable setzen, dass eine neue Nachricht zur Auswertung bereit steht
  }
}

//void attachSettings(){}


void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    //delay(wait);                           //  Pause for a moment
  }
}
