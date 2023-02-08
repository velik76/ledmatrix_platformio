#pragma once

#include "FastLED.h"

#define WIDTH 16
#define HEIGHT 16
#define SEGMENTS 1
#define NUM_LEDS (WIDTH * HEIGHT)
#define BRIGHTNESS 100 // (0-255)

#define SCORE_SIZE 0                // размер символов счёта в игре. 0 - маленький для 8х8 (шрифт 3х5), 1 - большой (шрифт 5х7)
#define GLOBAL_COLOR_1 CRGB::Green  // основной цвет №1 для игр
#define GLOBAL_COLOR_2 CRGB::Orange // основной цвет №2 для игр

extern CRGBArray<NUM_LEDS> leds;

uint16_t getPixelNumber(int8_t x, int8_t y);
uint32_t getPixColor(int thisSegm);
uint32_t getPixColorXY(int8_t x, int8_t y);
void drawPixelXY(uint16_t x, uint16_t y, CRGB color);
void drawImage(const uint16_t* const* array, uint16_t arraySize, bool init);
void runningOneLed(CRGB color, bool init);
void displayScore(byte score);