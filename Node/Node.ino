// Server ESP8266 code
// Made by Bart Slot in august 2019.

// This installation/game is for the Nijntje Museum in Utrecht, the Netherlands

// Step 1. Check if light sensor on server is < 50 light (this is when something is on the light sensor, hopefully the phone)
// Step 2. If yes: Cycle through color ranges of Green and Blue 0-50.
// Step 3. Check the value of the server - is the Button Pressed: Go check the current hue value
// Step 4. If color values match within the correction range (within 30hue value)
// Step 4.1. If the color values do not match: START the error routine. This will check if the button is still pressed, keep on 
//          displaying error routine.
// Step 4.2. Match: START the Win routine. This will make both LED's pulsing Green. 


// LOAD LIBRARYS
#include <SPI.h>
#include <ESP8266WiFi.h>
#include "FastLED.h"

//-----------------------------------------------------------------------------------------------------------------------------

// WIFI connection setup
char ssid[] = "__";                   // SSID of home WiFi
char pass[] = "0636303438";           // password of WiFi
IPAddress server(192,168,1,20);     // fix IP of the server
WiFiClient client;



//-----------------------------------------------------------------------------------------------------------------------------

//setup pins
#define buttonPin D5    //Button Pin input

 
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

//-----------------------------------------------------------------------------------------------------------------------------

//setup pins
 
#define LED_DT      D4
#define BRIGHTNESS  240
#define NUM_LEDS    1
#define COLOR_ORDER GRB
#define LED_TYPE    WS2812
//-----------------------------------------------------------------------------------------------------------------------------

//Variables

#define UPDATES_PER_SECOND 100

bool go = false;

// Global variables can be changed on the fly.
static uint8_t hueValue = 0; 
uint8_t previousHueValue = 0; 
uint8_t max_bright = 128;                                      // Overall brightness definition. It can be changed on the fly.
 
struct CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette(CRGB::Black);
CRGBPalette16 targetPalette(OceanColors_p);
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND  

int buttonState = 0;         // variable for reading the pushbutton status
String tempColorValue = "0";
int buttonPressed = 0;

int lives = 10;

 
//-----------------------------------------------------------------------------------------------------------------------------

void setup() 
{
  Serial.begin(74880);
  //delay(3000);
  
  LEDS.addLeds<LED_TYPE,LED_DT,COLOR_ORDER>(leds,NUM_LEDS).setCorrection(TypicalLEDStrip); //initalises the ledstrip
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
   
  pinMode(buttonPin, INPUT);     // Analog input pin that the PUSHBUTTON is attached to

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      Serial.println(".");
      FadeInOut(135, 2, 1); //Color (lightblue) / Wait / Times
  }
  FadeInOut(96, 0, 1); //Color / Wait / Times
  Serial.println("start, wifi connected");

  cleanUp(500);
  
  //currentPalette = RainbowColors_p;
  //currentBlending = LINEARBLEND;
  
  

} //-----------------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------
void cleanUp(long delayTime){
  for (int i = 0; i<NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(delayTime);
}
//---------------------------------------------------------------
void loop() 
{
  leds[0] = CHSV(hueValue,255,255); //hue, saturation, value
  
  if(hueValue = 255) { hueValue = 0; } //Reset the HueValue to 0 if 255 (max hue is reached).

  for (int i = 0; i<NUM_LEDS; i++) {
    for (hueValue = 0; hueValue < 256; hueValue++) {
        hueValue+10;
        Serial.println(hueValue);
        leds[0] = CHSV(hueValue,255,255); //hue, saturation, value
        FastLED.show();
        checkButtonState();
        checkServerValue();
        //delay(20);
        yield();
      }
  }  
  
}
//---------------------------------------------------------------

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
}

//---------------------------------------------------------------
void checkServerValue() {
  Serial.println("Check server value started");
  
  //client.connect(server, 80);             // connects to the server
  //client.flush();
  client.readStringUntil('\r') = tempColorValue;
  // int ServerColorValue = tempColorValue.toInt();
  /*
  if (ServerColorValue >= hueValue + 10 && hueValue - 10 ) // If Nodecolor value is in range of color value of Base
  {
    Serial.println("Good");
    go = true;
    String externalColor = client.readStringUntil('\r');
    client.println(hueValue + "\r");  // sends the message to the server
  } */
  Serial.println("Check server value ended");
  
}
//-----------------------------------------------------------------------------------------------------------------------------
void checkButtonState() 
{
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) 
  {
    checkServerValue();
    
    buttonPressed ++;             //add 1 time to the button is pressed
    Serial.println("Button is Pressed " + buttonPressed);
    if (buttonPressed == lives)   //if the player tried x times, game over!
    {
      gameOver();
      return;
    }
  }
}
//-----------------------------------------------------------------------------------------------------------------------------
void gameOver()
{
  Serial.println("Game Over");
      
}
