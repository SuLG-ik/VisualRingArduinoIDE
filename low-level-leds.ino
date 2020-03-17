//Если выбрана лента
#if IS_LED_STRIP

uint16_t getAnimTimeout(){
  if (horizontal_line){
    return oneTime/MATRIX_HEIGHT;
  }else{
    return oneTime/MATRIX_WIDTH;
  }
}

void stripChecker(){
  switch(mode){

    case MODE_LIGHT_OFF:
      setAllWhite();
      setLightOff();
    break;

    case MODE_LIGHT_ON:
      setAllWhite();
      setLightOn();
    break;

    case MODE_RING:
      setLightOn();
      ringAnimation(MATRIX_LENGTH);
      autoOffByCounter(6);
    break;

    case MODE_CHANGE_COLOR:
      raiseLightColor();
      setLightMode(MODE_CHANGE_COLOR_ANIM);
    break;

    case MODE_CHANGE_COLOR_ANIM:
      setLightOn();
      ringAnimation(MATRIX_LENGTH);
      autoOffByCounter(2);
    break;

    case MODE_SHACAL_SETTINGS:
      forceShacalChange();
      setLightMode(MODE_SHACAL_SETTINGS_ANIM);
    break;
    case MODE_SHACAL_SETTINGS_ANIM:
      setLightOn();
      fadeAnimation();
      autoOffByCounter(2);
    break;
    default:
      setLightMode(MODE_LIGHT_OFF);
    break;
  };
  
  FastLED.show();
}












void autoOffAnimation(long duration){
  if (ms() - tmpInt2 > duration){
    setLightMode(MODE_LIGHT_ON);
    toLog(ms());
    toLog(tmpInt2);
    toLog(ringDuration);
  }
}

void ringAnimation(long lines){
  if(tmpInt1 == 0){
    tmpInt1 = 1;
    tmpInt2 = ms();
    tmpInt4 = 1;
    toLog(tmpInt2);
  }
  if(ms() - tmpInt3 > getAnimTimeout()){
    raiseAnimLine(lines);
    tmpInt3 = ms();
  
  }

  setEvolvedLine(tmpInt1);
}

void raiseAnimLine(long lines){
  if(tmpInt1 >= lines) {
    tmpInt4 = -1;
    countUp();
  }
  if(tmpInt1 <= 1){
    tmpInt4 = 1;
    countUp();
  }
  if(tmpInt1 < ZERO){
    setLightMode(MODE_LIGHT_ON);
    toLog("Detected bug (tmpInt1 in raiseAnimLine <= 0):");
    toLog(tmpInt1);
    toLog("!Autofixed! (go to main state)");
  }
  tmpInt1+=tmpInt4;
}

CRGB getFadedColor(){
  CRGB color = CRGB(MAX_VALUE,MAX_VALUE,tmpInt1);
  if (!forceShacalBlock){
    color[1] = tmpInt1;
  }else{
    color[0] = tmpInt1;
  }
  return color;
}

void countUp(){
  countUp(1);
}

void countUp(long x){
  toLog("counted");
  toLog(tmpTimesCounter);
  nextLog();
  tmpTimesCounter+=x;
}


void autoOffByCounter(long counts){
  if (tmpTimesCounter > counts){
    setLightMode(MODE_LIGHT_ON);
    toLog("autoOffByCounter");
  }
}

void nextFade(){
  if (tmpInt1 <= ZERO+5) {tmpInt4 = 10; countUp(); toLog(tmpInt1); nextLog();}
  if (tmpInt1 >= MAX_VALUE-5) {tmpInt4 = -10; countUp(); toLog(tmpInt1); nextLog();}
  tmpInt1+=tmpInt4;
}

void fadeAnimation(){
  if(tmpTimesCounter == ZERO){
    tmpInt1 = MAX_VALUE;
    tmpInt4 = -1;
    tmpInt2 = ms();
  }
  if(ms() - tmpInt3 > getAnimTimeout()){
    tmpInt3 = ms();
    nextFade();
  }
  
  CRGB color = getFadedColor();
  setAllOneColor(color);
}

void raiseLightColor(){
  colorState++;
  if(colorState > BLUE){
    colorState = 0;
  }
  toLog(colorState);
}
#endif
