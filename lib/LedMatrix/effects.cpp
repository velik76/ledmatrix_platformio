#include <Arduino.h>
#include <pgmspace.h>
#include "led_matrix_utilities.h"
#include "FastLED.h"

// **************** НАСТРОЙКИ ЭФФЕКТОВ ****************
// эффект "шарики"
#define BALLS_AMOUNT 3 // количество "шариков"
#define CLEAR_PATH 1   // очищать путь
#define BALL_TRACK 1   // (0 / 1) - вкл/выкл следы шариков
#define TRACK_STEP 70  // длина хвоста шарика (чем больше цифра, тем хвост короче)

// эффект "квадратик"
#define BALL_SIZE 2    // размер квадрата
#define RANDOM_COLOR 1 // случайный цвет при отскоке

// эффект "огонь"
#define SPARKLES 1 // вылетающие угольки вкл выкл
#define HUE_ADD 0  // добавка цвета в огонь (от 0 до 230) - меняет весь цвет пламени

// эффект "кометы"
#define TAIL_STEP 100  // длина хвоста кометы
#define SATURATION 150 // насыщенность кометы (от 0 до 255)
#define STAR_DENSE 60  // количество (шанс появления) комет

// эффект "конфетти"
#define DENSE 3        // плотность конфетти
#define BRIGHT_STEP 70 // шаг уменьшения яркости

// эффект "снег"
#define SNOW_DENSE 10 // плотность снегопада

extern CRGBArray<NUM_LEDS> leds;

// --------------------- ДЛЯ РАЗРАБОТЧИКОВ ----------------------
// *********** "дыхание" яркостью ***********
byte _breathBrightness; // Яркость эффекта "Дыхание"

boolean brightnessDirection = true;
void brightnessRoutine()
{
  if (brightnessDirection)
  {
    _breathBrightness += 2;
    if (_breathBrightness > BRIGHTNESS - 2)
    {
      _breathBrightness = BRIGHTNESS;
      brightnessDirection = false;
    }
  }
  else
  {
    _breathBrightness -= 2;
    if (_breathBrightness < 2)
    {
      _breathBrightness = 2;
      brightnessDirection = true;
    }
  }
  FastLED.setBrightness(_breathBrightness);
  FastLED.show();
}

// *********** смена цвета активных светодиодов (рисунка) ***********
byte hue;
void colorsRoutine()
{
  hue += 4;
  for (int i = 0; i < NUM_LEDS; i++)
  {
    if (getPixColor(i) > 0)
      leds[i] = CHSV(hue, 255, 255);
  }
  FastLED.show();
}

// *********** снегопад 2.0 ***********
void snowRoutine()
{
  // сдвигаем всё вниз
  for (byte x = 0; x < WIDTH; x++)
  {
    for (byte y = 0; y < HEIGHT - 1; y++)
    {
      drawPixelXY(x, y, getPixColorXY(x, y + 1));
    }
  }

  for (byte x = 0; x < WIDTH; x++)
  {
    // заполняем случайно верхнюю строку
    // а также не даём двум блокам по вертикали вместе быть
    if (getPixColorXY(x, HEIGHT - 2) == 0 && (random(0, SNOW_DENSE) == 0))
      drawPixelXY(x, HEIGHT - 1, 0xE0FFFF - 0x101010 * random(0, 4));
    else
      drawPixelXY(x, HEIGHT - 1, 0x000000);
  }
  FastLED.show();
}

// ***************************** БЛУДНЫЙ КУБИК *****************************
int coordB[2];
int8_t vectorB[2];
CRGB ballColor;

void ballRoutine(bool init)
{
  if (init)
  {
    for (byte i = 0; i < 2; i++)
    {
      coordB[i] = WIDTH / 2 * 10;
      vectorB[i] = random(8, 20);
      ballColor = CHSV(random(0, 9) * 28, 255, 255);
    }
  }
  for (byte i = 0; i < 2; i++)
  {
    coordB[i] += vectorB[i];
    if (coordB[i] < 0)
    {
      coordB[i] = 0;
      vectorB[i] = -vectorB[i];
      if (RANDOM_COLOR)
        ballColor = CHSV(random(0, 9) * 28, 255, 255);
      // vectorB[i] += random(0, 6) - 3;
    }
  }
  if (coordB[0] > (WIDTH - BALL_SIZE) * 10)
  {
    coordB[0] = (WIDTH - BALL_SIZE) * 10;
    vectorB[0] = -vectorB[0];
    if (RANDOM_COLOR)
      ballColor = CHSV(random(0, 9) * 28, 255, 255);
    // vectorB[0] += random(0, 6) - 3;
  }
  if (coordB[1] > (HEIGHT - BALL_SIZE) * 10)
  {
    coordB[1] = (HEIGHT - BALL_SIZE) * 10;
    vectorB[1] = -vectorB[1];
    if (RANDOM_COLOR)
      ballColor = CHSV(random(0, 9) * 28, 255, 255);
    // vectorB[1] += random(0, 6) - 3;
  }
  FastLED.clear();
  for (byte i = 0; i < BALL_SIZE; i++)
    for (byte j = 0; j < BALL_SIZE; j++)
      leds[getPixelNumber(coordB[0] / 10 + i, coordB[1] / 10 + j)] = ballColor;
  FastLED.show();
}

// *********** радуга заливка ***********
void rainbowRoutine()
{
  hue += 3;
  for (byte i = 0; i < WIDTH; i++)
  {
    CRGB thisColor = CHSV((byte)(hue + i * float(255 / WIDTH)), 255, 255);
    for (byte j = 0; j < HEIGHT; j++)
      drawPixelXY(i, j, thisColor); // leds[getPixelNumber(i, j)] = thisColor;
  }
  FastLED.show();
}

// *********** радуга дигональная ***********
void rainbowDiagonalRoutine()
{
  hue += 3;
  for (byte x = 0; x < WIDTH; x++)
  {
    for (byte y = 0; y < HEIGHT; y++)
    {
      CRGB thisColor = CHSV((byte)(hue + (float)(WIDTH / HEIGHT * x + y) * (float)(255 / max(WIDTH, HEIGHT))), 255, 255);
      drawPixelXY(x, y, thisColor); // leds[getPixelNumber(i, j)] = thisColor;
    }
  }
  FastLED.show();
}

// *********** радуга активных светодиодов (рисунка) ***********
void rainbowColorsRoutine()
{
  hue++;
  for (byte i = 0; i < WIDTH; i++)
  {
    CRGB thisColor = CHSV((byte)(hue + i * float(255 / WIDTH)), 255, 255);
    for (byte j = 0; j < HEIGHT; j++)
      if (getPixColor(getPixelNumber(i, j)) > 0)
        drawPixelXY(i, j, thisColor);
  }
}

// ********************** огонь **********************
unsigned char matrixValue[8][16];
unsigned char line[WIDTH];
int pcnt = 0;

// these values are substracetd from the generated values to give a shape to the animation
const unsigned char valueMask[8][16] PROGMEM = {
    {32, 0, 0, 0, 0, 0, 0, 32, 32, 0, 0, 0, 0, 0, 0, 32},
    {64, 0, 0, 0, 0, 0, 0, 64, 64, 0, 0, 0, 0, 0, 0, 64},
    {96, 32, 0, 0, 0, 0, 32, 96, 96, 32, 0, 0, 0, 0, 32, 96},
    {128, 64, 32, 0, 0, 32, 64, 128, 128, 64, 32, 0, 0, 32, 64, 128},
    {160, 96, 64, 32, 32, 64, 96, 160, 160, 96, 64, 32, 32, 64, 96, 160},
    {192, 128, 96, 64, 64, 96, 128, 192, 192, 128, 96, 64, 64, 96, 128, 192},
    {255, 160, 128, 96, 96, 128, 160, 255, 255, 160, 128, 96, 96, 128, 160, 255},
    {255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255}};

// these are the hues for the fire,
// should be between 0 (red) to about 25 (yellow)
const unsigned char hueMask[8][16] PROGMEM = {
    {1, 11, 19, 25, 25, 22, 11, 1, 1, 11, 19, 25, 25, 22, 11, 1},
    {1, 8, 13, 19, 25, 19, 8, 1, 1, 8, 13, 19, 25, 19, 8, 1},
    {1, 8, 13, 16, 19, 16, 8, 1, 1, 8, 13, 16, 19, 16, 8, 1},
    {1, 5, 11, 13, 13, 13, 5, 1, 1, 5, 11, 13, 13, 13, 5, 1},
    {1, 5, 11, 11, 11, 11, 5, 1, 1, 5, 11, 11, 11, 11, 5, 1},
    {0, 1, 5, 8, 8, 5, 1, 0, 0, 1, 5, 8, 8, 5, 1, 0},
    {0, 0, 1, 5, 5, 1, 0, 0, 0, 0, 1, 5, 5, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0}};

// Randomly generate the next line (matrix row)
static void generateLine()
{
  for (uint8_t x = 0; x < WIDTH; x++)
  {
    line[x] = random(64, 255);
  }
}

// shift all values in the matrix up one row
static void shiftUp()
{
  for (uint8_t y = HEIGHT - 1; y > 0; y--)
  {
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      uint8_t newX = x;
      if (x > 15)
        newX = x % 16;
      if (y > 7)
        continue;
      matrixValue[y][newX] = matrixValue[y - 1][newX];
    }
  }

  for (uint8_t x = 0; x < WIDTH; x++)
  {
    uint8_t newX = x;
    if (x > 15)
      newX = x % 16;
    matrixValue[0][newX] = line[newX];
  }
}

// draw a frame, interpolating between 2 "key frames"
// @param pcnt percentage of interpolation
static void drawFrame(int pcnt)
{
  int nextv;

  // each row interpolates with the one before it
  for (unsigned char y = HEIGHT - 1; y > 0; y--)
  {
    for (unsigned char x = 0; x < WIDTH; x++)
    {
      uint8_t newX = x;
      if (x > 15)
        newX = x % 16;
      if (y < 8)
      {
        nextv =
            (((100.0 - pcnt) * matrixValue[y][newX] + pcnt * matrixValue[y - 1][newX]) / 100.0) - pgm_read_byte(&(valueMask[y][newX]));

        CRGB color = CHSV(
            HUE_ADD + pgm_read_byte(&(hueMask[y][newX])), // H
            255,                                          // S
            (uint8_t)max(0, nextv)                        // V
        );

        leds[getPixelNumber(x, y)] = color;
      }
      else if (y == 8 && SPARKLES)
      {
        if (random(0, 20) == 0 && getPixColorXY(x, y - 1) != 0)
          drawPixelXY(x, y, getPixColorXY(x, y - 1));
        else
          drawPixelXY(x, y, 0);
      }
      else if (SPARKLES)
      {

        // старая версия для яркости
        if (getPixColorXY(x, y - 1) > 0)
          drawPixelXY(x, y, getPixColorXY(x, y - 1));
        else
          drawPixelXY(x, y, 0);
      }
    }
  }

  // first row interpolates with the "next" line
  for (unsigned char x = 0; x < WIDTH; x++)
  {
    uint8_t newX = x;
    if (x > 15)
      newX = x % 16;
    CRGB color = CHSV(
        HUE_ADD + pgm_read_byte(&(hueMask[0][newX])),                                  // H
        255,                                                                           // S
        (uint8_t)(((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0) // V
    );
    // leds[getPixelNumber(newX, 0)] = color; // На форуме пишут что это ошибка - вместо newX должно быть x, иначе
    leds[getPixelNumber(x, 0)] = color; // на матрицах шире 16 столбцов нижний правый угол неработает
  }
}

void fireRoutine(bool init)
{
  if (init)
  {
    generateLine();
    memset(matrixValue, 0, sizeof(matrixValue));
  }
  if (pcnt >= 100)
  {
    shiftUp();
    generateLine();
    pcnt = 0;
  }
  drawFrame(pcnt);
  pcnt += 30;
  FastLED.show();
}

// **************** МАТРИЦА *****************
void matrixRoutine()
{
  for (byte x = 0; x < WIDTH; x++)
  {
    // заполняем случайно верхнюю строку
    uint32_t thisColor = getPixColorXY(x, HEIGHT - 1);
    if (thisColor == 0)
      drawPixelXY(x, HEIGHT - 1, 0x00FF00 * (random(0, 10) == 0));
    else if (thisColor < 0x002000)
      drawPixelXY(x, HEIGHT - 1, 0);
    else
      drawPixelXY(x, HEIGHT - 1, thisColor - 0x002000);
    FastLED.show();
  }

  // сдвигаем всё вниз
  for (byte x = 0; x < WIDTH; x++)
  {
    for (byte y = 0; y < HEIGHT - 1; y++)
    {
      drawPixelXY(x, y, getPixColorXY(x, y + 1));
    }
  }
  FastLED.show();
}

// ********************************* ШАРИКИ *********************************
int coord[BALLS_AMOUNT][2];
int8_t vector[BALLS_AMOUNT][2];
CRGB ballColors[BALLS_AMOUNT];

static void fadePixel(byte i, byte j, byte step)
{ // новый фейдер
  int pixelNum = getPixelNumber(i, j);
  if (getPixColor(pixelNum) == 0)
    return;

  if (leds[pixelNum].r >= 30 ||
      leds[pixelNum].g >= 30 ||
      leds[pixelNum].b >= 30)
  {
    leds[pixelNum].fadeToBlackBy(step);
  }
  else
  {
    leds[pixelNum] = 0;
  }
}

// функция плавного угасания цвета для всех пикселей
static void fader(byte step)
{
  for (byte i = 0; i < WIDTH; i++)
  {
    for (byte j = 0; j < HEIGHT; j++)
    {
      fadePixel(i, j, step);
    }
  }
}

void ballsRoutine(bool init)
{
  if (init)
  {
    for (byte j = 0; j < BALLS_AMOUNT; j++)
    {
      int sign;

      // забиваем случайными данными
      coord[j][0] = WIDTH / 2 * 10;
      random(0, 2) ? sign = 1 : sign = -1;
      vector[j][0] = random(4, 15) * sign;
      coord[j][1] = HEIGHT / 2 * 10;
      random(0, 2) ? sign = 1 : sign = -1;
      vector[j][1] = random(4, 15) * sign;
      ballColors[j] = CHSV(random(0, 9) * 28, 255, 255);
    }
  }

  if (!BALL_TRACK)   // если режим БЕЗ следов шариков
    FastLED.clear(); // очистить
  else
  { // режим со следами
    fader(TRACK_STEP);
  }

  // движение шариков
  for (byte j = 0; j < BALLS_AMOUNT; j++)
  {

    // движение шариков
    for (byte i = 0; i < 2; i++)
    {
      coord[j][i] += vector[j][i];
      if (coord[j][i] < 0)
      {
        coord[j][i] = 0;
        vector[j][i] = -vector[j][i];
      }
    }

    if (coord[j][0] > (WIDTH - 1) * 10)
    {
      coord[j][0] = (WIDTH - 1) * 10;
      vector[j][0] = -vector[j][0];
    }
    if (coord[j][1] > (HEIGHT - 1) * 10)
    {
      coord[j][1] = (HEIGHT - 1) * 10;
      vector[j][1] = -vector[j][1];
    }
    leds[getPixelNumber(coord[j][0] / 10, coord[j][1] / 10)] = ballColors[j];
  }
  FastLED.show();
}

// ********************* ЗВЕЗДОПАД ******************
void starfallRoutine()
{
  // заполняем головами комет левую и верхнюю линию
  for (byte i = HEIGHT / 2; i < HEIGHT; i++)
  {
    if (getPixColorXY(0, i) == 0 && (random(0, STAR_DENSE) == 0) && getPixColorXY(0, i + 1) == 0 && getPixColorXY(0, i - 1) == 0)
      leds[getPixelNumber(0, i)] = CHSV(random(0, 200), SATURATION, 255);
  }

  for (byte i = 0; i < WIDTH / 2; i++)
  {
    if (getPixColorXY(i, HEIGHT - 1) == 0 && (random(0, STAR_DENSE) == 0) && getPixColorXY(i + 1, HEIGHT - 1) == 0 && getPixColorXY(i - 1, HEIGHT - 1) == 0)
      leds[getPixelNumber(i, HEIGHT - 1)] = CHSV(random(0, 200), SATURATION, 255);
  }

  // сдвигаем по диагонали
  for (byte y = 0; y < HEIGHT - 1; y++)
  {
    for (byte x = WIDTH - 1; x > 0; x--)
    {
      drawPixelXY(x, y, getPixColorXY(x - 1, y + 1));
    }
  }

  // уменьшаем яркость левой и верхней линии, формируем "хвосты"
  for (byte i = HEIGHT / 2; i < HEIGHT; i++)
  {
    fadePixel(0, i, TAIL_STEP);
  }
  for (byte i = 0; i < WIDTH / 2; i++)
  {
    fadePixel(i, HEIGHT - 1, TAIL_STEP);
  }
  FastLED.show();
}

// рандомные гаснущие вспышки
void sparklesRoutine()
{
  for (byte i = 0; i < DENSE; i++)
  {
    byte x = random(0, WIDTH);
    byte y = random(0, HEIGHT);
    if (getPixColorXY(x, y) == 0)
      leds[getPixelNumber(x, y)] = CHSV(random(0, 255), 255, 255);
  }
  fader(BRIGHT_STEP);
  FastLED.show();
}
