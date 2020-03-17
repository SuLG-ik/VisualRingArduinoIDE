// Only PIO
#include <Arduino.h>

//Изменяемые настройки

#define DEBUG false //TRUE - включить

#define IS_LED_STRIP false   //true - используетс лента, false - лампа

#define MATRIX_PIN 13    //Пин ленты

#define MATRIX_WIDTH 3      //Высота
#define MATRIX_HEIGHT 14    //Ширина


#define RADIUS 10 
#define ENVOLVED_MIMIMUM 35

//ищзменяемые настройки

//Не изменять 

#define ZERO 0    // Нулевая константа

#define ms() (long)millis()   //внутреннее время

#define BRIGHTNESS_ZERO ZERO    //нулевая яркость

#define MAX_VALUE 255           //максимальное значение

#define MAP_RADIUS 255/RADIUS   //нужно
#define BUTTON_OUTSIDE A0       //пин звонка
#define BUTTON_INSIDE A2        //пин управленияя

#define SOFTNESS MAX_VALUE/RADIUS                    //Мягкость сглаживания линий

#define MATRIX_NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT)    //количество светодиодов

#define MATRIX_COLORS 3      //Кол-во цветов


//Режимы

  #define MODE_LIGHT_OFF 0
  #define MODE_LIGHT_ON 1
  #define MODE_RING 2
  #define MODE_CHANGE_COLOR 3
  #define MODE_CHANGE_COLOR_ANIM 4
  #define MODE_SHACAL_SETTINGS 5
  #define MODE_SHACAL_SETTINGS_ANIM 6

//Режимы
//Не изменять


#include <GyverButton.h>
#include <GyverTimer.h>

#if IS_LED_STRIP
#include <FastLED.h>
#endif
//Переменные


byte maxClickTimes = 3;
byte clickTimes = 0;
long previousShacal = 0;
long shacalFreeTime = 30000;
long shacalTimeout = 10000;
bool forceShacalBlock = false;

uint8_t mode = 0;
bool ledLight = 0;

long tmpInt1 = 0;
long tmpInt2 = 0;
long tmpInt3 = 0;
long tmpInt4 = 0;
long tmpInt5 = 0;
long tmpTimesCounter = 0;

GButton ringButton(BUTTON_OUTSIDE);
GButton controlButton(BUTTON_INSIDE);

#if IS_LED_STRIP

#define RED 0
#define GREEN 1
#define BLUE 2

boolean horizontal_line = true;   //true - горизонтальное расположение ленты, false -вертикальное

long oneTime = 500;
long ringDuration = 5000;

struct CRGB matrix [MATRIX_NUM_LEDS];   //лента
byte brightness = 15;

byte colorState = 0; // 0- red 1- green, 2 - blue

#define MATRIX_LENGTH (horizontal_line) ? MATRIX_HEIGHT : MATRIX_WIDTH

#endif
//Классы

void setup() {

  #if IS_LED_STRIP

    FastLED.addLeds<WS2811, MATRIX_PIN, RGB>(matrix,MATRIX_NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS_ZERO);

  #else
    pinMode(MATRIX_PIN, OUTPUT);
  #endif
  
  #if DEBUG == true
  Serial.begin(9600);

  while(!Serial){};

  toLog("IS_LED_STRIP:");
  toLog(IS_LED_STRIP);
  nextLog();

  
  toLog("FastLed size:");
  #if IS_LED_STRIP
  toLog(FastLED.size());
  #else
  toLog("Unavaible");
  #endif
    
  nextLog();
  nextLog();

  #endif
}

void loop() {

  buttonsTick();
  unshacaler();

  #if IS_LED_STRIP
    stripChecker();
  #else
    rgbChecker();
  #endif
}

void toLog(String message){
  #if DEBUG
  Serial.println(message);
  #endif
}

void toLog(int message){
  #if DEBUG
  Serial.println(message);
  #endif
}

void nextLog(){
  #if DEBUG
  Serial.println("+++++++++++++++++++");
  #endif
}

void buttonsTick(){

  if(!isBlocked()){
    ringButton.tick();
  }
  controlButton.tick();


  if(controlButton.isSingle()){
    if (mode == MODE_RING || mode == MODE_LIGHT_OFF){
      setLightMode(MODE_LIGHT_ON);
    }else{
      setLightMode(MODE_LIGHT_OFF);
    }
  }

  if(controlButton.isDouble()){
    setLightMode(MODE_CHANGE_COLOR);
  }

  if(controlButton.isTriple()){
    setLightMode(MODE_SHACAL_SETTINGS);
  }

  if(ringButton.isPress()){
    setLightMode(MODE_RING);
    shacalCounter();
  }

}

void setLightMode(uint8_t _mode){
  #if DEBUG
    toLog("set mode from:");
    toLog(mode);
    toLog("to:");
    toLog(_mode);
    nextLog();
  #endif
 
  cleanTmp();
  mode = _mode;
}

void cleanTmp(){
  tmpInt1 = 0;
  tmpInt2 = 0;
  tmpInt3 = 0;
  tmpInt4 = 0;
  tmpInt5 = 0;
  tmpTimesCounter = 0;
  toLog("Cleaned");
  nextLog();
}

bool isBlocked(){
  if ((ms() - previousShacal < shacalFreeTime && clickTimes > maxClickTimes) || forceShacalBlock){
    return true;
  }else{
    return false;
  }
}

void shacalCounter(){
  previousShacal = ms();
  clickTimes++;
}

void unshacaler(){
  if(ms() - previousShacal > shacalTimeout){
    clickTimes = 0;
  }
}


void forceShacalChange(){
  clickTimes = 0;
  toLog("changed from");
  toLog(forceShacalBlock);
  forceShacalBlock = !forceShacalBlock;
  toLog("to");
  toLog(forceShacalBlock);
  nextLog();
}
