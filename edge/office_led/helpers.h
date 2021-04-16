void espPinMode(int pinNum, int pinDir) {
  // Enable GPIO32 or 33 as output. Doesn't seem to work though.
  // https://esp32.com/viewtopic.php?t=9151#p38282
  if (pinNum == 32 || pinNum == 33) {
    uint64_t gpioBitMask = (pinNum == 32) ? 1ULL<<GPIO_NUM_32 : 1ULL<<GPIO_NUM_33;
    gpio_mode_t gpioMode = (pinDir == OUTPUT) ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT;
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = gpioMode;
    io_conf.pin_bit_mask = gpioBitMask;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
  } else pinMode(pinNum, pinDir);
}

void gpioSetup(int gpioNum, int gpioMode, int gpioVal) {
  #if defined(ARDUINO) && ARDUINO >= 100
    espPinMode(gpioNum, gpioMode);
    digitalWrite (gpioNum, gpioVal);
  #elif defined(ESP_PLATFORM)
    gpio_num_t gpioNumNative = static_cast<gpio_num_t>(gpioNum);
    gpio_mode_t gpioModeNative = static_cast<gpio_mode_t>(gpioMode);
    gpio_pad_select_gpio(gpioNumNative);
    gpio_set_direction(gpioNumNative, gpioModeNative);
    gpio_set_level(gpioNumNative, gpioVal);
  #endif
}



int stepper = 0;
int colord = 0;


/* --------------------------------------------
    EFFECT: Random colors
   -------------------------------------------- 
*/
void randomStrands(strand_t * strands[], int numStrands, unsigned long delay_ms, unsigned long timeout_ms){
  uint32_t dimmer = 0x0F3F3F3F;
  unsigned long start_ms = millis();
  while (timeout_ms == 0 || (millis() - start_ms < timeout_ms)) {
    for (int n = 0; n < numStrands; n++) {
      strand_t * pStrand = strands[n];
      for (uint16_t i = 0; i < pStrand->numPixels; i++) {
        pStrand->pixels[i].num = (esp_random() & dimmer);
      }
    }
    digitalLeds_drawPixels(strands, numStrands);
    delay(delay_ms);
  }
}




/* --------------------------------------------
    EFFECT: Rainbow
   -------------------------------------------- 
*/
class Rainbower {
  private:
    strand_t * pStrand;
    const uint8_t color_div = 4;
    const uint8_t anim_step = 1;
    uint8_t anim_max;
    uint8_t stepVal1;
    uint8_t stepVal2;
    pixelColor_t color1;
    pixelColor_t color2;
  public:
    Rainbower(strand_t *);
    void prepareNext();
};


Rainbower::Rainbower(strand_t * pStrandIn)
{
  pStrand = pStrandIn;
  anim_max = pStrand->brightLimit - anim_step;
  stepVal1 = 0;
  stepVal2 = 0;
  color1 = pixelFromRGBW(anim_max, 0, 0, 0);
  color2 = pixelFromRGBW(anim_max, 0, 0, 0);
}


void Rainbower::prepareNext()
{
  color1 = color2;
  stepVal1 = stepVal2;
  for (uint16_t i = 0; i < pStrand->numPixels; i++) {
    pStrand->pixels[i] = pixelFromRGBW(color1.r/color_div, color1.g/color_div, color1.b/color_div, 0);
    if (i == 1) {
      color2 = color1;
      stepVal2 = stepVal1;
    }
    switch (stepVal1) {
      case 0:
      color1.g += anim_step;
      if (color1.g >= anim_max)
        stepVal1++;
      break;
      case 1:
      color1.r -= anim_step;
      if (color1.r == 0)
        stepVal1++;
      break;
      case 2:
      color1.b += anim_step;
      if (color1.b >= anim_max)
        stepVal1++;
      break;
      case 3:
      color1.g -= anim_step;
      if (color1.g == 0)
        stepVal1++;
      break;
      case 4:
      color1.r += anim_step;
      if (color1.r >= anim_max)
        stepVal1++;
      break;
      case 5:
      color1.b -= anim_step;
      if (color1.b == 0)
        stepVal1 = 0;
      break;
    }
  }
}


void rainbows(strand_t * strands[], int numStrands, unsigned long delay_ms, unsigned long timeout_ms)
{
  Rainbower * pRbow[numStrands];
  int i;
  for (i = 0; i < numStrands; i++) {
    pRbow[i] = new Rainbower(strands[i]);
  }
  unsigned long start_ms = millis();
  while (timeout_ms == 0 || (millis() - start_ms < timeout_ms)) {
    for (i = 0; i < numStrands; i++) {
      pRbow[i]->prepareNext();
    }
    digitalLeds_drawPixels(strands, numStrands);
    delay(delay_ms);
  }
  for (i = 0; i < numStrands; i++) {
    delete pRbow[i];
  }
  digitalLeds_resetPixels(strands, numStrands);
}


/* --------------------------------------------
    EFFECT: Stepper
   -------------------------------------------- 
*/
void simpleStepper(strand_t * strands [], int numStrands, unsigned long delay_ms, unsigned long timeout_ms)
{
  int highLimit = 32;
  unsigned long start_ms = millis();
  while (timeout_ms == 0 || (millis() - start_ms < timeout_ms)) {
    strand_t * strand = strands[0];
    strand->pixels[stepper] = pixelFromRGBW(colord, colord, colord, 0);

    stepper++;
    if(stepper > strand->numPixels) {
      stepper = 0;
      colord += 2;
    }
  
    if(colord > highLimit) 
      colord = 0;
    
    digitalLeds_drawPixels(strands, numStrands);
    delay(delay_ms);
  }
  digitalLeds_resetPixels(strands, numStrands);
}


/* --------------------------------------------
    CONTROL: Set white
   -------------------------------------------- 
*/
void plainWhite(strand_t * strands [], int numStrands, int bright){
  strand_t * strand = strands[0];
  for(int x = 0 ; x < strand->numPixels ; x++){

    strand->pixels[x] = pixelFromRGBW(0, 0, 0, bright);
  }
  digitalLeds_drawPixels(strands, numStrands);
}


/* --------------------------------------------
    CONTROL: Set rgbw
   -------------------------------------------- 
*/
void setRGBW(strand_t * strands [], int numStrands, int r, int g, int b, int w){
  strand_t * strand = strands[0];
  for(int x = 0 ; x < strand->numPixels ; x++){

    strand->pixels[x] = pixelFromRGBW(r,g,b,w);
  }
  digitalLeds_drawPixels(strands, numStrands);
}
