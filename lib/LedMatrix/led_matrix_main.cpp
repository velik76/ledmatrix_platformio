#include "led_matrix_main.h"

#include <Arduino.h>

#include "FastLED.h"
#include "bitmap_characters.h"
#include "bitmap_flame.h"
#include "bitmap_ghost.h"
#include "bitmap_ironman.h"
#include "bitmap_mario.h"
#include "effects.h"
#include "g_arkanoid.h"
#include "g_flappy.h"
#include "g_maze.h"
#include "g_runner.h"
#include "g_snake.h"
#include "g_tetris.h"
#include "led_matrix_utilities.h"
#include "noise_effects.h"
#include "running_text.h"

#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define DATA_PIN 27
#define VOLTS 5
#define MAX_MA 8000

CRGBArray<NUM_LEDS> leds;

LedMatrix::LedMatrix()
{
    _timer.setInterval(10);
    _patternNames[OneLine] = "OneLine";
    _patternNames[RunningText] = "RunningText";
    _patternNames[GameTetris] = "GameTetris";
    _patternNames[GameRunner] = "GameRunner";
    _patternNames[GameFlappy] = "GameFlappy";
    _patternNames[GameArkanoid] = "GameArkanoid";
    _patternNames[GameMaze] = "GameMaze";
    _patternNames[GameSnake] = "GameSnake";
    _patternNames[RoutineSparkles] = "RoutineSparkles";
    _patternNames[RoutineStarsFall] = "RoutineStarsFall";
    _patternNames[RoutineFire] = "RoutineFire";
    _patternNames[RoutineSnow] = "RoutineSnow";
    _patternNames[RoutineMatrix] = "RoutineMatrix";
    _patternNames[RoutineBall] = "RoutineBall";
    _patternNames[RoutineBalls] = "RoutineBalls";
    _patternNames[RoutineBrightness] = "RoutineBrightness";
    _patternNames[RoutineColors] = "RoutineColors";
    _patternNames[NoiseRainbow] = "NoiseRainbow";
    _patternNames[NoiseMadness] = "NoiseMadness";
    _patternNames[NoiseRainbowStripe] = "NoiseRainbowStripe";
    _patternNames[NoiseZebra] = "NoiseZebra";
    _patternNames[NoiseForest] = "NoiseForest";
    _patternNames[NoiseOcean] = "NoiseOcean";
    _patternNames[NoisePlasma] = "NoisePlasma";
    _patternNames[NoiseCloud] = "NoiseCloud";
    _patternNames[NoiseLava] = "NoiseLava";
    _patternNames[ImageMario] = "ImageMario";
    _patternNames[ImageFlame] = "ImageFlame";
    _patternNames[ImageGhost] = "ImageGhost";
    _patternNames[ImageIronMan] = "ImageIronMan";
    _patternNames[ImageCharacters] = "ImageCharacters";
}

void LedMatrix::setup()
{
    FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    FastLED.show();
}

bool LedMatrix::isPatternActive(const DrawingPattern pattern)
{
    bool result = false;
    switch(pattern)
    {
        case DrawingPattern::OneLine:
            break;
        case DrawingPattern::RunningText:
            result = true;
            break;
        case DrawingPattern::RoutineSparkles:
            result = true;
            break;
        case DrawingPattern::RoutineStarsFall:
            result = true;
            break;
        case DrawingPattern::RoutineFire:
            result = true;
            break;
        case DrawingPattern::RoutineSnow:
            result = true;
            break;
        case DrawingPattern::RoutineMatrix:
            break;
        case DrawingPattern::RoutineBall:
            result = true;
            break;
        case DrawingPattern::RoutineBalls:
            result = true;
            break;
        case DrawingPattern::RoutineBrightness:
            break;
        case DrawingPattern::RoutineColors:
            break;
        case DrawingPattern::NoiseRainbow:
            result = true;
            break;
        case DrawingPattern::NoiseMadness:
            result = true;
            break;
        case DrawingPattern::NoiseRainbowStripe:
            result = true;
            break;
        case DrawingPattern::NoiseZebra:
            break;
        case DrawingPattern::NoiseForest:
            break;
        case DrawingPattern::NoiseOcean:
            result = true;
            break;
        case DrawingPattern::NoisePlasma:
            result = true;
            break;
        case DrawingPattern::NoiseCloud:
            result = true;
            break;
        case DrawingPattern::NoiseLava:
            break;
        case DrawingPattern::GameTetris:
            break;
        case DrawingPattern::GameRunner:
            break;
        case DrawingPattern::GameFlappy:
            break;
        case DrawingPattern::GameArkanoid:
            break;
        case DrawingPattern::GameMaze:
            break;
        case DrawingPattern::GameSnake:
            break;
        case DrawingPattern::ImageMario:
            result = true;
            break;
        case DrawingPattern::ImageFlame:
            break;
        case DrawingPattern::ImageGhost:
            break;
        case DrawingPattern::ImageIronMan:
            break;
        case DrawingPattern::ImageCharacters:
            break;
        default:
            break;
    }
    return result;
}

void LedMatrix::loop()
{
    if(!_timer.isReady())
        return;

    switch(_drawState)
    {
        case DrawingPattern::OneLine:
            runningOneLed(random16() * random8(), false);
            break;
        case DrawingPattern::GameTetris:
            tetrisRoutine(false);
            break;
        case DrawingPattern::GameRunner:
            runnerRoutine(false);
            break;
        case DrawingPattern::GameFlappy:
            flappyGame();
            break;
        case DrawingPattern::GameArkanoid:
            arkanoidRoutine(false);
            break;
        case DrawingPattern::GameSnake:
            snakeGame(false);
            break;
        case DrawingPattern::GameMaze:
            mazeRoutine(false);
            break;
        case DrawingPattern::RoutineBrightness:
            brightnessRoutine();
            break;
        case DrawingPattern::RoutineColors:
            colorsRoutine();
            break;
        case DrawingPattern::RoutineSparkles:
            sparklesRoutine();
            break;
        case DrawingPattern::RoutineStarsFall:
            starfallRoutine();
            break;
        case DrawingPattern::RoutineBalls:
            ballsRoutine(false);
            break;
        case DrawingPattern::RoutineFire:
            fireRoutine(false);
            break;
        case DrawingPattern::RoutineMatrix:
            matrixRoutine();
            break;
        case DrawingPattern::RoutineSnow:
            snowRoutine();
            break;
        case DrawingPattern::RoutineBall:
            ballRoutine(false);
            break;
        case DrawingPattern::RunningText:
            runningString("", CRGB::HTMLColorCode::Black, false);
            break;
        case DrawingPattern::NoiseRainbow:
            rainbowNoise(false);
            break;
        case DrawingPattern::NoiseMadness:
            madnessNoise(false);
            break;
        case DrawingPattern::NoiseRainbowStripe:
            rainbowStripeNoise(false);
            break;
        case DrawingPattern::NoiseZebra:
            zebraNoise(false);
            break;
        case DrawingPattern::NoiseForest:
            forestNoise(false);
            break;
        case DrawingPattern::NoiseOcean:
            oceanNoise(false);
            break;
        case DrawingPattern::NoisePlasma:
            plasmaNoise(false);
            break;
        case DrawingPattern::NoiseCloud:
            cloudNoise(false);
            break;
        case DrawingPattern::NoiseLava:
            lavaNoise(false);
            break;
        case DrawingPattern::ImageMario:
            drawImage(mario_array, sizeof(mario_array) / sizeof(mario_array[0]), false);
            break;
        case DrawingPattern::ImageFlame:
            drawImage(flame_array, sizeof(flame_array) / sizeof(flame_array[0]), false);
            break;
        case DrawingPattern::ImageGhost:
            drawImage(ghost_array, sizeof(ghost_array) / sizeof(ghost_array[0]), false);
            break;
        case DrawingPattern::ImageIronMan:
            drawImage(ironMan_array, sizeof(ironMan_array) / sizeof(ironMan_array[0]), false);
            break;
        case DrawingPattern::ImageCharacters:
            drawImage(characters_array, sizeof(characters_array) / sizeof(characters_array[0]), false);
            break;
        default:
            break;
    }
}

void LedMatrix::switchToNextEnabledPattern()
{
    int t = static_cast<int>(_drawState);
    bool active = true;
    do
    {
        if(++t >= DrawingPattern::Last)
            t = 0;
        active = isPatternActive(static_cast<DrawingPattern>(t));
    } while(!active);
    switchToPattern(static_cast<DrawingPattern>(t));
}

void LedMatrix::switchToRandomEnabledPattern(void)
{
    bool active = true;
    long next;
    do
    {
        next = random(0, static_cast<long>(LedMatrix::DrawingPattern::Last) - 1);
        active = isPatternActive(static_cast<DrawingPattern>(next));
    } while(!active);
    switchToPattern(static_cast<LedMatrix::DrawingPattern>(next));
}

void LedMatrix::switchToPattern(DrawingPattern pattern)
{
    const uint32_t timeoutText = 100;
    const uint32_t timeoutGame = 100;
    const uint32_t timeoutNoise = 100;
    const uint32_t timeoutBitmap = 200;
    const uint32_t timeoutRoutines = 100;

    // Recover original brightness
    if(_drawState == DrawingPattern::RoutineBrightness)
    {
        FastLED.setBrightness(BRIGHTNESS);
    }

    _drawState = pattern;
    switch(_drawState)
    {
        case DrawingPattern::OneLine:
            FastLED.clear();
            _timer.setInterval(timeoutNoise);
            break;

        case DrawingPattern::GameTetris:
            FastLED.clear();
            _timer.setInterval(timeoutGame);
            tetrisRoutine(true);
            break;

        case DrawingPattern::GameRunner:
            FastLED.clear();
            _timer.setInterval(timeoutGame);
            runnerRoutine(true);
            break;

        case DrawingPattern::GameFlappy:
            FastLED.clear();
            _timer.setInterval(timeoutGame);
            break;

        case DrawingPattern::GameArkanoid:
            FastLED.clear();
            _timer.setInterval(timeoutGame / 5);
            arkanoidRoutine(true);
            break;

        case DrawingPattern::GameSnake:
            FastLED.clear();
            _timer.setInterval(timeoutGame);
            snakeGame(true);
            break;

        case DrawingPattern::GameMaze:
            FastLED.clear();
            _timer.setInterval(timeoutGame);
            mazeRoutine(true);
            break;

        case DrawingPattern::RoutineBrightness:
            _timer.setInterval(timeoutRoutines);
            break;

        case DrawingPattern::RoutineColors:
            _timer.setInterval(timeoutRoutines);
            break;

        case DrawingPattern::RoutineSparkles:
            FastLED.clear();
            _timer.setInterval(timeoutRoutines);
            break;

        case DrawingPattern::RoutineStarsFall:
            FastLED.clear();
            _timer.setInterval(timeoutRoutines);
            break;

        case DrawingPattern::RoutineBalls:
            FastLED.clear();
            _timer.setInterval(timeoutRoutines);
            ballsRoutine(true);
            break;

        case DrawingPattern::RoutineFire:
            FastLED.clear();
            _timer.setInterval(timeoutRoutines);
            fireRoutine(true);
            break;

        case DrawingPattern::RoutineMatrix:
            FastLED.clear();
            _timer.setInterval(timeoutRoutines);
            break;

        case DrawingPattern::RoutineSnow:
            FastLED.clear();
            _timer.setInterval(timeoutRoutines);
            break;

        case DrawingPattern::RoutineBall:
            FastLED.clear();
            _timer.setInterval(timeoutRoutines);
            ballRoutine(true);
            break;

        case DrawingPattern::RunningText:
            //            runningString(_runningText, CRGB::HTMLColorCode::Aqua, true);
            runningString(_runningText, static_cast<CRGB::HTMLColorCode>(1), true);
            _timer.setInterval(timeoutText);
            break;

        case DrawingPattern::NoiseRainbow:
            rainbowNoise(true);
            _timer.setInterval(timeoutNoise);
            break;

        case DrawingPattern::NoiseMadness:
            madnessNoise(true);
            _timer.setInterval(timeoutNoise);
            break;

        case DrawingPattern::NoiseRainbowStripe:
            rainbowStripeNoise(true);
            _timer.setInterval(timeoutNoise);
            break;

        case DrawingPattern::NoiseZebra:
            zebraNoise(true);
            _timer.setInterval(timeoutNoise);
            break;

        case DrawingPattern::NoiseForest:
            forestNoise(true);
            _timer.setInterval(timeoutNoise);
            break;

        case DrawingPattern::NoiseOcean:
            FastLED.clear();
            oceanNoise(true);
            _timer.setInterval(timeoutNoise);
            break;

        case DrawingPattern::NoisePlasma:
            FastLED.clear();
            plasmaNoise(true);
            _timer.setInterval(timeoutNoise);
            break;

        case DrawingPattern::NoiseCloud:
            FastLED.clear();
            cloudNoise(true);
            _timer.setInterval(timeoutNoise);
            break;

        case DrawingPattern::NoiseLava:
            FastLED.clear();
            lavaNoise(true);
            _timer.setInterval(timeoutNoise);
            break;

        case DrawingPattern::ImageMario:
            FastLED.clear();
            drawImage(nullptr, 0, true);
            _timer.setInterval(timeoutBitmap);
            break;

        case DrawingPattern::ImageFlame:
            FastLED.clear();
            drawImage(nullptr, 0, true);
            _timer.setInterval(timeoutBitmap);
            break;

        case DrawingPattern::ImageGhost:
            FastLED.clear();
            drawImage(nullptr, 0, true);
            _timer.setInterval(timeoutBitmap);
            break;

        case DrawingPattern::ImageIronMan:
            FastLED.clear();
            drawImage(nullptr, 0, true);
            _timer.setInterval(timeoutBitmap);
            break;

        case DrawingPattern::ImageCharacters:
            FastLED.clear();
            drawImage(nullptr, 0, true);
            _timer.setInterval(timeoutBitmap);
            break;
        default:
            break;
    }
    Serial.println(getPatternName());
}

enum LedMatrix::DrawingPattern LedMatrix::getActivePattern() const
{
    return _drawState;
}

String LedMatrix::getPatternName()
{
    return _patternNames[_drawState];
}

void LedMatrix::setRunningText(const String& text)
{
    if(_runningText != text)
    {
        runningStringSetText(text);
        _runningText = text;
    }
}
