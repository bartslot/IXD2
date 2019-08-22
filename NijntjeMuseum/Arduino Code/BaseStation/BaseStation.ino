// Server ESP8266 code
// Made by Bart Slot in august 2019.

// This installation/game is for the Nijntje Museum in Utrecht, the Netherlands

// Step 1. Check if light sensor is < 50 light (this is when something is on the light sensor, hopefully the phone)
// Step 2. If yes: Cycle through color ranges of Green and Blue 0-50.
// Step 3. Check the value of the client - is the Button Pressed: Go check the current hue value
// Step 4. If color values match within the correction range (within 30hue value)
// Step 4.1. If the color values do not match: START the error routine. This will check if the button is still pressed, keep on 
//          displaying error routine.
// Step 4.2. Match: START the Win routine. This will make both LED's pulsing Green. 


// LOAD LIBRARYS
#include <SPI.h>
#include <ESP8266WiFi.h>
#include "FastLED.h"
#ifdef __AVR__
  #include <avr/power.h>
#endif
//-----------------------------------------------------------------------------------------------------------------------------

// AP setup
char ssid[] = "__";                   // SSID of home WiFi
char pass[] = "0636303438";           // password of WiFi
WiFiServer server(80);

IPAddress ip(192,168,1,20);       // the fixed IP address of the server (using DD-WRT - Screenshot as attachment)
IPAddress gateway(192,168,1,1);     // WiFi router's IP
IPAddress subnet(255,255,255,0);

WiFiClient client;

//-----------------------------------------------------------------------------------------------------------------------------

//setup pins
#define LDR A0         // Analog input pin that the ldr is attached to
#define ledPin1 D4     // LED1 pin input
//-----------------------------------------------------------------------------------------------------------------------------

//Variables
bool go = false;
int ldrValue = 0;        // value read from the ldr
int ColorValue = 0;        // value sent to server Just a little test message.  
int hue = 0;

//-----------------------------------------------------------------------------------------------------------------------------



//FASTLED Neopixel setup

 
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
#define LED_DT      D4
#define BRIGHTNESS  240
#define NUM_LEDS    1
#define COLOR_ORDER GRB
#define LED_TYPE    WS2812


// Global variables can be changed on the fly.
uint8_t hueValue = 128; 
uint8_t previousHueValue = 0; 
uint8_t max_bright = 128;                                      // Overall brightness definition. It can be changed on the fly.
 
struct CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette(CRGB::Black);
CRGBPalette16 targetPalette(OceanColors_p);
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND 
 
static int16_t dist;                                          // A random number for our noise generator.
uint16_t xscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint16_t yscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint8_t maxChanges = 24;                                      // Value for blending between palettes.

//-----------------------------------------------------------------------------------------------------------------------------
//SETUP
void setup() 
{

  delay(3000);

  Serial.begin(74880);
    
  pinMode(LDR, INPUT);     // Analog input pin that the ldr is attached to

  LEDS.addLeds<LED_TYPE,LED_DT,COLOR_ORDER>(leds,NUM_LEDS).setCorrection(TypicalLEDStrip); //initalises the ledstrip
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
  
  dist = random16(12345);                                     // A semi-random number for our noise generator

  client.setSync(true);   // Synchronises both server and client to have the same connection/time/speed.
    
  ServerStart();                            // starts the WiFi server
  cleanUp(500);
  
  //END SETUP
} //-----------------------------------------------------------------------------------------------------------------------------
//CLEANUP
void cleanUp(long delayTime){
  for (int i = 0; i<NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(delayTime);
  
  //END CLEANUP
}//-----------------------------------------------------------------------------------------------------------------------------
//LOOP
void loop() 
{
  int ldrValue = analogRead(LDR); //Check sensor readings
  if (ldrValue < 20) // If phone is on
  {
    Serial.println(ldrValue);
    Serial.println("Start the Game");
    //ONE TIME START GREEN
    FadeInOut(80, 2, 1); //Color (green) / Wait / Times
    go = true; //Bool to stay in game mode
    
  } else {
    Serial.println("Stop the Game");
    go = false;
    FadeInOut(192, 10, 2); //Color (purple) / Wait / Times
    
  }
  if(go == true) 
  {
      leds[0] = CHSV(hueValue,255,255); //hue, saturation, value
      FastLED.show();
    if(hueValue = 255) { hueValue = 0; } //Reset the HueValue to 0 if 255 (max hue is reached).
  
    for (int i = 0; i<NUM_LEDS; i++) 
    {
      for (hueValue = 0; hueValue < 256; hueValue++) 
      {
          Serial.println(hueValue);
          leds[0] = CHSV(hueValue,255,255); //hue, saturation, value
          FastLED.show();
          if(analogRead(LDR) >= 20) { 
            break; //get out
          }
          //do Stuff
          String externalColor = client.readStringUntil('\r');
          client.println(hueValue + "\r");  // sends the message to the server

          
          //delay(200);
          yield();
        }
    }  
  } 
  //END LOOP
  
}//-----------------------------------------------------------------------------------------------------------------------------

//FADE IN OUT
void FadeInOut(uint8_t color, uint8_t wait, uint8_t times)
{ 
  int brightness = 0;

  //Repeat the fade in out X times
  for (int i = 0; i < times; i++) 
  {
    //Fade In
    for (int y = 0; y < 256; y++) 
    {
      leds[0] = CHSV(color,255,y); //hue, saturation, value
      FastLED.show();
      delay(wait);
    }
    for (int x = 255; x > 0; x--) 
    {
        leds[0] = CHSV(color,255,x); //hue, saturation, value
        FastLED.show();
        delay(wait);
    }
    yield();
    return;
  }
  //END FADE IN OUT
  
}//-----------------------------------------------------------------------------------------------------------------------------

//SERVER START
void ServerStart() {
  
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      Serial.println(".");
      FadeInOut(135, 2, 1); //Color (lightblue) / Wait / Times
  }
  FadeInOut(96, 2, 1); //Color / Wait / Times
  Serial.println("start, wifi connected");
  server.begin();
  delay(500);
}//-----------------------------------------------------------------------------------------------------------------------------
