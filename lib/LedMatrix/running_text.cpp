#include <Arduino.h>
#include <pgmspace.h>

#include "FastLED.h"
#include "fonts.h"
#include "led_matrix_utilities.h"

// **************** НАСТРОЙКИ ****************
#define MIRR_V 0 // отразить текст по вертикали (0 / 1)
#define MIRR_H 0 // отразить текст по горизонтали (0 / 1)

#define LET_WIDTH 5  // ширина буквы шрифта
#define LET_HEIGHT 7 // высота буквы шрифта
#define SPACE 1      // пробел

int _offset = WIDTH;
bool _fullTextFlag = false; // флаг: текст бегущей строки показан полностью (строка убежала)
CRGB::HTMLColorCode _color = CRGB::HTMLColorCode::Black;
String _text;

extern CRGBArray<NUM_LEDS> leds;

// интерпретатор кода символа в массиве fontHEX (для Arduino IDE 1.8.* и выше)
static uint8_t getFont(uint8_t font, uint8_t row)
{
    font = font - '0' + 16; // перевод код символа из таблицы ASCII в номер согласно нумерации массива
    if(font <= 90)
        return pgm_read_byte(&(fontHEX[font][row])); // для английских букв и символов
    else if(font >= 112 && font <= 159)
    { // и капец для русских
        return pgm_read_byte(&(fontHEX[font - 17][row]));
    }
    else if(font >= 96 && font <= 111)
    {
        return pgm_read_byte(&(fontHEX[font + 47][row]));
    }
    return 0;
}

static void drawLetter(uint8_t index, uint8_t letter, int16_t offset, uint32_t color)
{
    int8_t start_pos = 0, finish_pos = LET_WIDTH;
    int8_t LH = LET_HEIGHT;
    if(LH > HEIGHT)
        LH = HEIGHT;
    int8_t _offset_y = (HEIGHT - LH) / 2; // по центру матрицы по высоте

    CRGB letterColor;
    if(color == 1)
    {
        letterColor = CHSV(byte(offset * 10), 255, 255);
    }
    else if(color == 2)
    {
        letterColor = CHSV(byte(index * 30), 255, 255);
    }
    else
    {
        letterColor = color;
    }

    if(offset < -LET_WIDTH || offset > WIDTH)
        return;
    if(offset < 0)
        start_pos = -offset;
    if(offset > (WIDTH - LET_WIDTH))
        finish_pos = WIDTH - offset;

    for(byte i = start_pos; i < finish_pos; i++)
    {
        int thisByte;
        if(MIRR_V)
            thisByte = getFont((byte)letter, LET_WIDTH - 1 - i);
        else
            thisByte = getFont((byte)letter, i);

        for(byte j = 0; j < LH; j++)
        {
            boolean thisBit;

            if(MIRR_H)
                thisBit = thisByte & (1 << j);
            else
                thisBit = thisByte & (1 << (LH - 1 - j));

            // рисуем столбец (i - горизонтальная позиция, j - вертикальная)
            if(thisBit)
                leds[getPixelNumber(offset + i, _offset_y + j)] = letterColor;
        }
    }
}

void runningString(String text, CRGB::HTMLColorCode color, bool init)
{
    if(init)
    {
        _offset = WIDTH; // перемотка в правый край
        _text = text;
        _color = color;
        _fullTextFlag = false;
    }

    FastLED.clear();
    byte i = 0, j = 0;
    while(_text[i] != '\0')
    {
        if((byte)_text[i] > 191)
        { // работаем с русскими буквами!
            i++;
        }
        else
        {
            drawLetter(j, _text[i], _offset + j * (LET_WIDTH + SPACE), _color);
            i++;
            j++;
        }
    }
    _fullTextFlag = false;

    _offset--;
    if(_offset < -j * (LET_WIDTH + SPACE))
    {
        _offset = WIDTH + 3;
        _fullTextFlag = true;
    }

    FastLED.show();
}

void runningStringSetText(const String& text)
{
    _text = text;
}
