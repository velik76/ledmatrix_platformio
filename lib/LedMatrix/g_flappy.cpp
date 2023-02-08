#include <Arduino.h>
#include "led_matrix_utilities.h"
#include "g_flappy.h"
#include "timer.h"
// игра "flappy bird"

// **************** НАСТРОЙКИ FLAPPY ****************
// 1 пиксель = 10 мм
#define POP_SPEED_F 200  // скорость подскока (в мм/с)
#define POP_POS_F 20     // скорость подскока (в мм/с)
#define GRAVITY_F 900    // ускорение падения (в мм/с2)
#define DT_F 20          // период интегрирования (мс)
#define OBST_SPEED_F 130 // скорость препятствий
#define MIN_GAP_F 9      // минимальное расстояние между препятствиями
#define OBST_HEIGHT_F 6  // макс. высота препятствия (размер щели)
#define DEMO_JUMP_F 6    // за какое расстояние делать прыжок в демо режиме

Timer flappyTimer(DT_F);
Timer obstfTimer(OBST_SPEED_F);

int posFlap = 0;
int velFlap = 0; // в мм и мм/с
byte prevposFlap;
byte obstCounterFlap;
int flappyScore;

static bool gameDemo = true;
static byte buttons = 4; // Управление играми: нажата кнопка: 0 - верх, 1 - право, 2 - низ, 3 - лево, 4 - не нажата
static bool reinit = false;

void flappyGame()
{
  if (reinit)
  {
    FastLED.clear();
    reinit = false;
  }

  if (gameDemo)
  {
    for (byte i = 1; i < DEMO_JUMP_F; i++)
      if (getPixColorXY(i, posFlap) == GLOBAL_COLOR_1)
        buttons = 0; // автопрыжок
    if (getPixColorXY(1, posFlap - 1) == GLOBAL_COLOR_1)
      buttons = 0; // автопрыжок
  }

  //  if (checkButtons())
  {
    if (buttons == 0)
    { // кнопка нажата
      velFlap = POP_SPEED_F;
      posFlap += POP_POS_F;
      buttons = 4;
    }
  }

  if (flappyTimer.isReady())
  {
    if (obstfTimer.isReady())
    {
      for (byte i = 0; i < WIDTH - 1; i++)
      {
        for (byte j = 0; j < HEIGHT; j++)
        {
          leds[getPixelNumber(i, j)] = getPixColorXY(i + 1, j);
        }
      }

      obstCounterFlap++;
      if (obstCounterFlap >= MIN_GAP_F)
      {
        obstCounterFlap = 0;
        flappyScore++;

        for (byte i = 0; i < HEIGHT; i++)
        {
          drawPixelXY(WIDTH - 1, i, GLOBAL_COLOR_1);
          drawPixelXY(WIDTH - 2, i, GLOBAL_COLOR_1);
        }
        byte thisHeight = random(1, HEIGHT - OBST_HEIGHT_F);
        for (byte i = thisHeight; i < thisHeight + OBST_HEIGHT_F; i++)
        {
          drawPixelXY(WIDTH - 1, i, 0);
          drawPixelXY(WIDTH - 2, i, 0);
        }
      }
      else
      {
        for (byte i = 0; i < HEIGHT; i++)
          drawPixelXY(WIDTH - 1, i, 0);
      }
    }

    velFlap -= (float)GRAVITY_F * DT_F / 1000;
    posFlap += (float)velFlap * DT_F / 1000;
    if (posFlap < 0)
    {
      posFlap = 0;
      velFlap = 0;
    }
    else if (posFlap > (HEIGHT - 1) * 10)
    {
      posFlap = (HEIGHT - 1) * 10;
      velFlap = 0;
    }
    if (getPixColorXY(0, posFlap / 10) == GLOBAL_COLOR_1 ||
        getPixColorXY(1, posFlap / 10) == GLOBAL_COLOR_1)
    {
      displayScore(flappyScore);
      flappyScore = 0;
      reinit = true;
      delay(500);
      return;
    }
    drawPixelXY(1, prevposFlap / 10, 0x000000);
    drawPixelXY(1, posFlap / 10, GLOBAL_COLOR_2);
    drawPixelXY(0, prevposFlap / 10, 0x000000);
    drawPixelXY(0, posFlap / 10, GLOBAL_COLOR_2);
    prevposFlap = posFlap;
    FastLED.show();
  }
}
