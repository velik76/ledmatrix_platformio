#pragma once
#include <map>

#include "timer.h"
class LedMatrix
{
public:
    enum DrawingPattern
    {
        OneLine = 0,
        RunningText,
        RoutineSparkles,
        RoutineStarsFall,
        RoutineFire,
        RoutineSnow,
        RoutineMatrix,
        RoutineBall,
        RoutineBalls,
        RoutineBrightness,
        RoutineColors,
        NoiseRainbow,
        NoiseMadness,
        NoiseRainbowStripe,
        NoiseZebra,
        NoiseForest,
        NoiseOcean,
        NoisePlasma,
        NoiseCloud,
        NoiseLava,
        GameTetris,
        GameRunner,
        GameFlappy,
        GameArkanoid,
        GameMaze,
        GameSnake,
        ImageMario,
        ImageFlame,
        ImageGhost,
        ImageIronMan,
        ImageCharacters,
        Last
    };

    LedMatrix();
    void setup();
    void loop();
    void switchToNextEnabledPattern();
    void switchToPattern(DrawingPattern pattern);
    void switchToRandomEnabledPattern();
    enum DrawingPattern getActivePattern() const;
    String getPatternName();
    void setRunningText(const String& text);

private:
    bool isPatternActive(const DrawingPattern pattern);

private:
    std::map<DrawingPattern, String> _patternNames;
    DrawingPattern _drawState{DrawingPattern::OneLine};
    Timer _timer;
    String _runningText{"Привет Крис:-)"};
};
