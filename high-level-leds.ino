#if IS_LED_STRIP

void setLightOn(){
  FastLED.setBrightness(brightness);
}

void setLightOff(){
  FastLED.setBrightness(BRIGHTNESS_ZERO);
}

void setAllWhite(){
  setAllOneColor(CRGB::Blue);
  setAllOneColor(CRGB::White);
}

//получение номера пикселя по кординатам
uint32_t getByXY(uint32_t x, uint32_t y){
  return horizontal_line ? _getByXY(x, y) : _getByXY(y,x);
}

//вспомогательная функция для получения номера пикселя
uint32_t _getByXY(uint32_t x, uint32_t y){
      if (x > MATRIX_HEIGHT){
          x = MATRIX_HEIGHT;
      }
      if(y > MATRIX_WIDTH){
        y = MATRIX_WIDTH;
      }
      return (y%2 == 0) ? MATRIX_HEIGHT * y - x : ( y -  1 ) * MATRIX_HEIGHT + x - 1;
}

//Установить цвет отдельного пикселя классом CRGB
void setColorToPixel(uint32_t x, uint32_t y, CRGB color){
  matrix[getByXY(x,y)] = color;
}

//Установить цвет отдельного пикселя поцветно
void setColorToPixel(uint32_t x, uint32_t y, byte red, byte green, byte blue){
  setColorToPixel(x,y, CRGB(red,green,blue));
}

//установить цвет на линию классом CRGB
void setColorToLine(uint32_t line, CRGB color){
  uint32_t columns = horizontal_line ? MATRIX_WIDTH : MATRIX_HEIGHT;
  for(uint32_t column = 1; column <= columns; column++){
    setColorToPixel(line, column, color);
  }
}

//установить цвет на линию поцветно
void setColorToLine(uint32_t line, byte red, byte green, byte blue){

  setColorToLine(line, CRGB(red,green,blue));

}

//Установить всё одиним цветом по классу CRGB
void setAllOneColor(CRGB color){
  for (uint32_t pixel; pixel<MATRIX_NUM_LEDS; pixel++){
    matrix[pixel] = color;
  }
}

//установить всё одним цветом поцветно
void setAllOneColor(byte red, byte green, byte blue){
  setAllOneColor(CRGB(red,green,blue));
}

byte filteredColor(int32_t line, int32_t evolved_line){
  int32_t ans = (MAP_RADIUS)*abs(line-evolved_line)+ENVOLVED_MIMIMUM;
  return (ans > MAX_VALUE) ? MAX_VALUE : ans;
}

void setEvolvedLine(uint32_t evolved_line){
  uint32_t lines = horizontal_line ? MATRIX_HEIGHT : MATRIX_WIDTH;

  for (uint32_t line = 1; line  <= lines; line++){


    CRGB color = CRGB(0,0,0);

    if (colorState == GREEN){
      color = CRGB(MAX_VALUE, filteredColor(line, evolved_line),filteredColor(line, evolved_line));
      
    }else if(colorState == BLUE){
      color = CRGB(filteredColor(line, evolved_line), filteredColor(line, evolved_line), MAX_VALUE);
    }else{
      color = CRGB(filteredColor(line, evolved_line), MAX_VALUE, filteredColor(line, evolved_line));
    }
    setColorToLine(line, color);

  }

}

#endif
