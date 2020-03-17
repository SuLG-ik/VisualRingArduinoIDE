#if !IS_LED_STRIP

void autoOff(int times){
  if (tmpTimesCounter > times){
    setLightMode(MODE_LIGHT_ON);
  }
}

void blinking(int timeout){
  if(ms() - tmpInt1 > timeout){
    ledLight =!ledLight;
    toLog("Blink");
    nextLog();
    tmpTimesCounter++;
    tmpInt1 = ms();
  }
}

void rgbChecker(){

  switch (mode)
  {

  case MODE_LIGHT_ON:
      ledLight = 1;
    break;
  case MODE_LIGHT_OFF:
      ledLight = 0;
  break;

  case MODE_RING:
      blinking(500);
      autoOff(6);
  break;
  case  MODE_SHACAL_SETTINGS:
    forceShacalChange();
    setLightMode(MODE_SHACAL_SETTINGS_ANIM);
  break;
  case MODE_SHACAL_SETTINGS_ANIM:
      blinking(250);
      if (forceShacalBlock){
        autoOff(4);
      }else{
        autoOff(2);
      }

  break;

  default:
    setLightMode(MODE_LIGHT_OFF);
  break;

  }

  if(ledLight){
    setRGB(1);
  }else{
    setRGB(0);
  }

}

void setRGB(bool ledVal){
  digitalWrite(MATRIX_PIN, ledVal);
}

#endif
