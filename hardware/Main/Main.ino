#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//LED pins
//first circle
#define LED_PIN13 22
//second circle
#define LED_PIN8 9
#define LED_PIN9 10
#define LED_PIN12 13
#define LED_PIN14 23
#define LED_PIN17 26
#define LED_PIN18 27
//third circle
#define LED_PIN3 4
#define LED_PIN4 5
#define LED_PIN5 6
#define LED_PIN7 8
#define LED_PIN10 11
#define LED_PIN11 12
#define LED_PIN15 24
#define LED_PIN16 25
#define LED_PIN19 28
#define LED_PIN21 30
#define LED_PIN22 31
#define LED_PIN23 32
//forth circle
#define LED_PIN1 2
#define LED_PIN2 3
#define LED_PIN6 7
#define LED_PIN20 29
#define LED_PIN24 33
#define LED_PIN25 34

//
#define LED_COUNT 40

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN1, NEO_GRB + NEO_KHZ800);

int brightness = 50;
int wait = 1000;
int buttonState = 0;

//For multithreading
long lastTime = 0;
int beatIndex = 0;

// beat array
long beat[] = {17, 484, 810, 2683, 4180, 4738, 6003, 6359, 6721, 7952, 8488,
               8809, 10204, 10698, 11459, 13676, 14699, 15996, 16768, 18746,
               20710, 22732, 23948, 24740, 26720, 27457, 29703, 30743, 31993,
               32771, 34720, 36717, 38087, 38702, 40486, 40796, 42714, 43461,
               44281, 45696, 46721, 47996, 48778, 50719, 52715, 54094, 54729,
               55946, 56719, 58734, 59457, 60227, 61719, 63985, 64789, 66706,
               68703, 70102, 70739, 71958, 72726, 74709, 75455, 76235, 77706,
               80682, 82678, 84685, 86694, 88692, 90674, 91495, 92213, 93699,
               94738, 95745, 96477, 96780, 97082, 98677, 100680, 102682, 103943,
               104470, 106687, 107498, 108237, 109702, 110692, 112674, 113444,
               114672, 116679, 117460, 118699, 119945, 120696, 121470, 122671,
               124225, 124695, 125463, 126687, 127979, 128670, 129438, 130667,
               132689, 133460, 34685, 135948, 136674, 137471, 138670, 140246,
               140685, 141458, 142686, 143996, 144682, 145456, 146672, 148687,
               149472, 150693, 152704, 154682
              };

long beat1[] = {15, 728, 1459, 2207, 2966, 3671, 4467, 4850, 5217, 5973, 6679,
                7489, 7861, 8968, 9672, 10463, 10850, 11954, 12664, 13490, 13850,
                14233, 14966, 15656, 16474, 16856, 17979, 18649, 19483, 19844,
                20218, 20978, 21673, 22469, 22854, 23221, 23978, 24659, 25475,
                25859, 26956, 27657, 28480, 28829, 29229, 29979, 30649, 31464,
                31844, 32239, 32967, 33668, 34480, 34843, 35982, 36671, 37489,
                37848, 38211, 38980, 39638, 40467, 40855, 41964, 42656, 43476,
                43853, 44229, 44966, 45674, 46479, 46837, 47222, 47982, 48655,
                49482, 49844, 51865, 52523, 53230, 54755, 55294, 55952, 56688,
                57791, 58151, 58512, 59201, 60748, 61283, 61983, 63903, 65244,
                66738, 67302, 67969, 68654, 69748, 70168, 71234, 72730, 73285,
                73950, 74964, 75669, 76469, 76852, 77209, 77976, 78664, 79451,
                79849, 80970, 81662, 82501, 82838, 83963, 84682, 85476, 85833,
                86219, 86973, 87650, 88457, 88842, 89966, 90673, 91472, 91855,
                92210, 92966, 93678, 94481, 94853, 95222, 95973, 96654, 97477,
                97842, 98964, 99660, 100468, 100852, 101232, 101971, 102655,
                103456, 103843, 104983, 105661, 106467, 106865, 107967, 108670,
                109476, 109847, 110216, 110962, 111648, 112486, 112848, 113982,
                114658, 115484, 115868, 116220, 116961, 117666, 118483, 118829,
                119981, 120657, 121483, 121846, 123889, 124506, 125253, 126768,
                127300, 127948, 129872
               };



void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.show();
  strip.setBrightness(brightness);

  Serial.begin(9600);
  //  pinMode(BUTTON_PIN, INPUT);
}

// from rasberry pi to arduino
String data;

void loop() {
  // put your main code here, to run repeatedly:
  long elapsedTime = millis() - lastTime;
  lastTime = lastTime + elapsedTime;

  if (data == "blink") {
    // --- need update ---
    strip.setPin(3);
    blink(elapsedTime);
    strip.setPin(4);
    blink(elapsedTime);
    strip.setPin(5);
    blink(elapsedTime);
  }
  else if (data == "CenterRipple") {
    // single colour ripple
    CenterRipple(elapsedTime);
  }
  else if (data == "Rainbow") {
    // --- [ just a placeholder ] ---
    strip.setPin(2);
    rainbow(elapsedTime);
    strip.setPin(3);
    rainbow(elapsedTime);
    strip.setPin(4);
    rainbow(elapsedTime);
  }
  else {
    data = "";
  }
}







// function      -------- blink -------- 

void blink(long elapsedTime) {
  static long blinkTime = 0;
  static long ledTime = 0;
  ledTime += elapsedTime;

  if (ledTime >= beat1[beatIndex]) {
    if (blinkTime <= 60) {
      if (blinkTime <= 10) {
        strip.setBrightness(blinkTime * 20);
      }      else {
        strip.setBrightness(200 - (blinkTime * 4));
      }
      strip.fill(strip.Color(38, 38, 38));
      strip.show();
      blinkTime += 1;
      Serial.println("b" + String(blinkTime));
    }    else {
      strip.clear();
      strip.show();
      blinkTime = 0;
      beatIndex += 1;
    }
  }  else {
    strip.clear();
    strip.show();
  }
}


// function    -------- CenterRipple -------- 

void TurnOn(int pin, long elapsedMS) {
  strip.setPin(pin);
  strip.fill(strip.Color(0, 0, 127));
  strip.show();

  static long ledTime = 0;
  ledTime += elapsedMS;
  if (ledTime >= wait) {
    strip.clear();
    strip.show();
    ledTime -= wait;
  }
}

void CenterRipple(long elapsedTime) {
  static long ledTime = 0;
  ledTime += elapsedTime;

  if (ledTime <= 1000) {
    TurnOn(LED_PIN13, elapsedTime);
  };
  if (ledTime >= 1000 && ledTime < 2000) {
    TurnOn(LED_PIN8, elapsedTime);
    TurnOn(LED_PIN9, elapsedTime);
    TurnOn(LED_PIN12, elapsedTime);
    TurnOn(LED_PIN14, elapsedTime);
    TurnOn(LED_PIN17, elapsedTime);
    TurnOn(LED_PIN18, elapsedTime);
  };
  if (ledTime >= 2000 && ledTime < 3000) {
    TurnOn(LED_PIN3, elapsedTime);
    TurnOn(LED_PIN4, elapsedTime);
    TurnOn(LED_PIN5, elapsedTime);
    TurnOn(LED_PIN7, elapsedTime);
    TurnOn(LED_PIN10, elapsedTime);
    TurnOn(LED_PIN11, elapsedTime);
    TurnOn(LED_PIN15, elapsedTime);
    TurnOn(LED_PIN16, elapsedTime);
    TurnOn(LED_PIN19, elapsedTime);
    TurnOn(LED_PIN21, elapsedTime);
    TurnOn(LED_PIN22, elapsedTime);
    TurnOn(LED_PIN23, elapsedTime);
  };
  if (ledTime >= 3000 && ledTime < 4000) {
    TurnOn(LED_PIN1, elapsedTime);
    TurnOn(LED_PIN2, elapsedTime);
    TurnOn(LED_PIN6, elapsedTime);
    TurnOn(LED_PIN20, elapsedTime);
    TurnOn(LED_PIN24, elapsedTime);
    TurnOn(LED_PIN25, elapsedTime);
  };
  if (ledTime >= 4000) {
    ledTime -= 4000;
  }
}



// function     -------- rainbow -------- 

void rainbow(long elapsedTime) {      //wait = 20
  static int i = 0;
  static int j = 0 ;
  static long RainbowTime = 0;
  RainbowTime += elapsedTime;

  if (RainbowTime >= 100) {
    if (i <= strip.numPixels()) {
      i++;
    }
    else if (i >= strip.numPixels()) {
      j++;
    }
    if (i + j >= 256) {
      i = 0;
      j = 0;
    }
    strip.fill(Wheel((i + j)));
    strip.show();

    RainbowTime -= 100;
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
