#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 121

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 3
#define CLOCK_PIN 13
#define LED 2
#define JoystickLamp 27

#pragma region "Light Sides"
/*
*        S5                    S1
*|-------------------||-------------------|
*|                   ||                   |
*|                   ||                   |
*|S6               S8|| S4                | S2
*|                   ||                   |
*|       S7          ||        S3         |
*|-------------------||-------------------|
*/

//Right Side
//S8 + S2

//Left Side
//S6 + S4

#define S1s 0
#define S1e 11-1

#define S2s 11
#define S2e 11+18-1

#define S3s 11+18
#define S3e 11+18+11-1

#define S4s 11+18+11
#define S4e 11+18+11+18-1

#define S5s 11+18+11+18
#define S5e 11+18+11+18+11-1

#define S6s 11+18+11+18+11
#define S6e 11+18+11+18+11+18-1

#define S7s 11+18+11+18+11+18
#define S7e 11+18+11+18+11+18+11-1

#define S8s 11+18+11+18+11+18+11
#define S8e 11+18+11+18+11+18+11+18-1
#pragma endregion




// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potPin = 34;

// Define the array of leds
CRGB leds[NUM_LEDS];
int ButtonSwitchesID [5]={19,23,35,36,39};
//int ButtonLampsID [5]={4,5,16,17,18};
int ButtonLampsID [5]={4,18,5,16,17};
int JoystickID[4]={25,26,32,33};
static uint8_t hue = 0;
static uint8_t lastHue = 0;
int i;
int j;
int potValue;
int nBrightness=255;
boolean bDEBUG=0;

void setup() {    
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical    

  //Stayalive Signal
  pinMode(LED,OUTPUT);
  //ledcAttachPin(LED, 0);

  pinMode(ButtonLampsID[0],OUTPUT);
  pinMode(ButtonLampsID[1],OUTPUT);
  pinMode(ButtonLampsID[2],OUTPUT);
  pinMode(ButtonLampsID[3],OUTPUT);
  pinMode(ButtonLampsID[4],OUTPUT);

   //Joystick lights
  pinMode(JoystickLamp,OUTPUT);

  //Buttons
  pinMode(ButtonSwitchesID[0],INPUT_PULLUP);
  pinMode(ButtonSwitchesID[1],INPUT_PULLUP);
  pinMode(ButtonSwitchesID[2],INPUT_PULLUP);
  pinMode(ButtonSwitchesID[3],INPUT_PULLUP);
  pinMode(ButtonSwitchesID[4],INPUT_PULLUP);

  //Joystick
  pinMode(JoystickID[0],INPUT_PULLUP);
  pinMode(JoystickID[1],INPUT_PULLUP);
  pinMode(JoystickID[2],INPUT_PULLUP);
  pinMode(JoystickID[3],INPUT_PULLUP);

  //Turn the LED on, then pause
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Azure;
  leds[2] = CRGB::Blue;
  leds[3] = CRGB::Blue;
  leds[4] = CRGB::Blue;
  leds[5] = CRGB::Blue;
  leds[6] = CRGB::Blue;
  int i=0;

  for(i=0;i<=NUM_LEDS-1;i++){
    leds[i] = CRGB::Red;
  }

  //leds[3] = CRGB::BlueViolet;
  FastLED.show();
  //delay(10);

  // Now turn the LED off, then pause
  //leds[0] = CRGB::Black;
  //FastLED.show();
  
  //Serial port for debugging
  Serial.begin(115200);
  Serial.println("Restarted");
  Serial.println("");

}

//Prototypes
void fadeall();
void initialiseShow();
void AllRed(int nBrightness);
void AllGreen(int nBrightness);
void AllBlue(int nBrightness);
void AllWhite(int nBrightness);
void AllYellow(int nBrightness);
void PaintTop(int nBrightness);
void PaintDown(int nBrightness);
void PaintRightSide(int nBrightness);
void PaintLeftSide(int nBrightness);
void ReadPotentiometer();



enum eStates{ 
  eInitialise =0,
  eButtonGreen=1,
  eButtonRed =2,
  eButtonBlue=3,
  eButtonYellow=4,
  eButtonWhite=5,
  eJoystickUp=6,
  eJoystickDown=7,
  eJoystickLeft=8,
  eJoystickRight=9,
  eRandomLights=10,
  eRightLights=11,
  eLeftLights=12,
  eTop=13,
  eBottom=14,
  eLeft=15,
  eRight=16
};

enum eColor{  eGreen=0,
              eRed =1,
              eBlue =2,
              eYellow=3,
              eWhite=4};

int nState=eInitialise;



void loop() { 
  int a;
  //delay(400);
  
     

  //initialiseShow();
       
  //Infinite loop
  do {  

    //toggle light
    if ((digitalRead(LED)==LOW)){
        digitalWrite(LED, HIGH);
       
      }
      else{   
        digitalWrite(LED, LOW);
      }

   
      // if ((digitalRead(LED)==LOW)){
      //   digitalWrite(LED, HIGH);
      //   digitalWrite(ButtonLampsID[0], HIGH);
      //   digitalWrite(ButtonLampsID[1], HIGH);
      //   digitalWrite(ButtonLampsID[2], HIGH);
      //   digitalWrite(ButtonLampsID[3], HIGH);
      //   digitalWrite(ButtonLampsID[4], HIGH);
      //   digitalWrite(JoystickLamp, HIGH);
      // }
      // else{   
      //   digitalWrite(LED, LOW);
      //   digitalWrite(ButtonLampsID[0], LOW);
      //   digitalWrite(ButtonLampsID[1], LOW);
      //   digitalWrite(ButtonLampsID[2], LOW);
      //   digitalWrite(ButtonLampsID[3], LOW);
      //   digitalWrite(ButtonLampsID[4], LOW);
      //   digitalWrite(JoystickLamp, LOW);
      // }
        
      delay(500);
    //Serial.println(potValue);

    //Buttons
    for(int i=0;i<5;i++){
      if(digitalRead(ButtonSwitchesID[i])==LOW) {
        //digitalWrite(ButtonLampsID[i], HIGH);
        Serial.print("Button ");
        Serial.println(i);
        nState=i+1;
        digitalWrite(ButtonLampsID[i], HIGH);

      }
      else{
        //digitalWrite(ButtonLampsID[i], LOW);
        digitalWrite(ButtonLampsID[i], LOW);
      }
    }

    // Joystick
    // for(int i=0;i<4;i++){
    //   if(digitalRead(JoystickID[i])==LOW) {
    //     //digitalWrite(ButtonLampsID[i], HIGH);
    //     Serial.print("Joy ");
    //     Serial.println(i);
    //     nState=i+1+5;
    //   }
    //   else{
    //     //digitalWrite(ButtonLampsID[i], LOW);
    //   }
    // }



    // Serial.print("State: ");
    // Serial.println(nState);

    // switch (nState){
    //   case eInitialise:
    //    digitalWrite(ButtonLampsID[0], LOW);
    //    digitalWrite(ButtonLampsID[1], LOW);
    //    digitalWrite(ButtonLampsID[2], LOW);
    //    digitalWrite(ButtonLampsID[3], LOW);
    //    digitalWrite(ButtonLampsID[4], LOW);
    //    j=0;
    //    hue=0;
    //    nState=eRightLights;
    //   break;

    //   case eButtonGreen:
    //     //Turn button red
    //     Serial.println("Green Button");
    //     digitalWrite(ButtonLampsID[0], HIGH);
    //     AllGreen(nBrightness);
    //     nState=eInitialise;  
    //   break;

    //   case eButtonRed:
    //     //Turn button red
    //     Serial.println("Red Button");
    //     digitalWrite(ButtonLampsID[1], HIGH);
    //     AllRed(nBrightness);
    //     //Turn the leds RED
    //     nState=eInitialise;
    //   break;

    //   case eButtonBlue:
    //     //Turn button red
    //     Serial.println("Blue Button");
    //     digitalWrite(ButtonLampsID[2], HIGH);
    //     AllBlue(nBrightness);
    //     nState=eInitialise;
    //   break;

    //   case eButtonYellow:
    //     Serial.println("Yellow Button");
    //     digitalWrite(ButtonLampsID[3], HIGH);
    //     AllYellow(nBrightness);
    //     nState=eInitialise;
    //   break;

    //   case eButtonWhite:
    //     //Turn button red
    //     Serial.println("White Button");
    //     digitalWrite(ButtonLampsID[4], HIGH);
    //     AllWhite(nBrightness);
    //     nState=eInitialise;
    //   break;

    //   case eJoystickUp:
    //     Serial.println("eJoystickUp");
    //     PaintTop(nBrightness);
    //       //FastLED.show();
    //     nState=eInitialise;
    //   break;

    //   case eJoystickDown:
    //     PaintDown(nBrightness);
    //     nState=eInitialise;
    //   break;

    //   case eJoystickLeft:
    //     PaintLeftSide(nBrightness);
    //     nState=eInitialise;
    //   break;

    //   case eJoystickRight:
    //     PaintRightSide(nBrightness);
    //     nState=eInitialise;
    //   break;

    //   case eRightLights:
    //     j = j+1;      
    //     if (j< NUM_LEDS){
    //         //for(int i = 0; i < NUM_LEDS; i++) {
    //         // Set the i'th led to red 
    //         leds[j] = CHSV(hue++, 255, nBrightness);
    //         // Show the leds
    //         FastLED.show(); 
    //         // now that we've shown the leds, reset the i'th led to black
    //         // leds[i] = CRGB::Black;
    //         fadeall();
    //         // Wait a little bit before we loop around and do it again
    //         //delay(5);
    //         ReadPotionemeter();

          
    //         //turn off the lights
    //         for(int i=0;i<5;i++){
    //           digitalWrite(ButtonLampsID[i], LOW); 
    //         }
    //         a=j % 5;
    //         digitalWrite(ButtonLampsID[a], HIGH); 


    //     }
    //     else{
    //       j=NUM_LEDS;
    //       nState=eLeftLights;
    //       //Turn Off buttons
    //       for(int i=0;i<5;i++){
    //           digitalWrite(ButtonLampsID[i], LOW); 
    //       }
    //       //Turn On Joystic
    //        digitalWrite(JoystickLamp, HIGH); 

    //     }
    //     break;

          
    //   case eLeftLights:
    //     j=j-1;
    //     if (j>=0){
    //                // for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    //       // Set the i'th led to red 
    //       leds[j] = CHSV(hue++, 255, nBrightness);
    //       // Show the leds
    //       FastLED.show();
    //       // now that we've shown the leds, reset the i'th led to black
    //       // leds[i] = CRGB::Black;
    //       fadeall();
    //       // Wait a little bit before we loop around and do it again
    //       //delay(5);
    //       ReadPotionemeter();
    //     }
    //     else{
    //       j=0;
    //       nState=eRightLights;
    //       //Turn On Joystic
    //        digitalWrite(JoystickLamp, LOW); 
    //     }
    //   break;

    //   case eTop:
        
    //   break;

    //   case eBottom:
    //   break;

    //   case eRight:
    //   break;

    //    case eLeft:
    //   break;
      
    //   default:
    //   break;
    //   }

    //   FastLED.show();

    } while (true);
}




void ReadPotionemeter(){
  potValue = analogRead(potPin);
  nBrightness=potValue/16;
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(253); } }

void initialiseShow(){
  //Light up Joystick
    digitalWrite(JoystickLamp, HIGH); 
  //Light up buttons
  for(i=0;i<5;i++){
     digitalWrite(ButtonLampsID[i], HIGH);
     delay(100);
  }
  delay(100);
  for(i=0;i<5;i++){
     digitalWrite(ButtonLampsID[i], LOW); 
  }
  digitalWrite(JoystickLamp, LOW); 

  
  //Paint all and fade up
  hue=0;
  nBrightness=125;
  for(int k=0;k<NUM_LEDS;k++){
    leds[k] = CHSV(hue, 255, nBrightness);   
    delay(10);
    FastLED.show();
  }
 delay(200);
  //
  for(int l=0;l<255;l++){
    for(int k=0;k<NUM_LEDS;k++){
      leds[k] = CHSV(l, 255, nBrightness);  
    }
    delay(10);
    FastLED.show();
  }
   
 
}

void AllRed(int nBrightness){
  hue=5;
  lastHue=hue;
  for(int l=0;l<NUM_LEDS;l++){
    leds[l] = CHSV(hue, 255, nBrightness);
    
  }
   //FastLED.show(); 
}

void AllGreen(int nBrightness){
  hue=85;
  lastHue=hue;
  for(int k=0;k<NUM_LEDS;k++){
    leds[k] = CHSV(hue, 255, nBrightness);
    //leds[k] = CRGB::Green;
    
  }
   //FastLED.show(); 
}

void AllBlue(int nBrightness){
  hue=171;
  lastHue=hue;
  for(int k=0;k<NUM_LEDS;k++){
    leds[k] = CHSV(hue, 255, nBrightness);   
  }
   //FastLED.show(); 
}

void AllWhite(int nBrightness){
  hue=255;
  lastHue=hue;
  for(int k=0;k<NUM_LEDS;k++){
    leds[k] = CHSV(hue, 0, nBrightness);
  }
  // FastLED.show(); 
}

void AllYellow(int nBrightness){
  hue=55;
  lastHue=hue;
  for(int k=0;k<NUM_LEDS;k++){
    leds[k] = CHSV(hue, 255, nBrightness);
  }
   //FastLED.show(); 
}

void PaintTop(int nBrightness){
  //s3 and S7
  for(int k=0;k<NUM_LEDS;k++){
    //'Segment3
    if ((k>=S3s) && (k<=S3e)){
      leds[k] = CHSV(lastHue, 255, nBrightness);
    }
    else if((k>=S7s) && (k<=S7e)){
      //Segment7
      leds[k] = CHSV(lastHue, 255, nBrightness);
    }
    else{
      leds[k] = CHSV(lastHue, 0, 0);
    }
  }
}

void PaintDown(int nBrightness){
    for(int k=0;k<NUM_LEDS;k++){
      //'Segment3
      if ((k>=S1s) && (k<=S1e)){
        leds[k] = CHSV(lastHue, 255, nBrightness);
      }
      else if((k>=S5s) && (k<=S5e)){
        //Segment7
        leds[k] = CHSV(lastHue, 255, nBrightness);
      }
      else{
        leds[k] = CHSV(lastHue, 0, 0);
      }
  }
}

void PaintRightSide(int nBrightness){
    for(int k=0;k<NUM_LEDS;k++){
      //'Segment4
      if ((k>=S4s) && (k<=S4e)){
        leds[k] = CHSV(lastHue, 255, nBrightness);
      }
      else if((k>=S6s) && (k<=S6e)){
        //Segment6
        leds[k] = CHSV(lastHue, 255, nBrightness);
      }
      else{
        leds[k] = CHSV(lastHue, 0, 0);
      }
  }
}

void PaintLeftSide(int nBrightness){
    for(int k=0;k<NUM_LEDS;k++){
      //'Segment4
      if ((k>=S2s) && (k<=S2e)){
        leds[k] = CHSV(lastHue, 255, nBrightness);
      }
      else if((k>=S8s) && (k<=S8e)){
        //Segment6
        leds[k] = CHSV(lastHue, 255, nBrightness);
      }
      else{
        leds[k] = CHSV(lastHue, 0, 0);
      }
  }
}
