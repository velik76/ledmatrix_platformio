#include <led_matrix_main.h>
#include <wifi_main.h>

#include <Arduino.h>
#include <DS3231.h>
#include <Wire.h>

#include "timer.h"

#define LED_GPIO 2
#define BUTTON_GPIO 23
#define TIME_NEXT_PATTERN (60 * 1000)

LedMatrix ledMatrix;
Wifi wifi{&ledMatrix};
Timer timerSwitchNextPattern;
DS3231 rtc;
bool initialRunningTextActive = true;

bool buttonAction()
{
    static int previousState = 0;
    static Timer timer(20);

    if(!timer.isReady())
        return false;

    bool result = false;
    const int state = digitalRead(BUTTON_GPIO);

    if(state == 0 && previousState == 1)
        result = true;

    previousState = state;
    return result;
}

void setupRtc(int h, int m, int s)
{
    bool h12Flag, pmFlag;

    const int isH = rtc.getHour(h12Flag, pmFlag);
    const int isM = rtc.getMinute();

    if(isH != h || isM != m)
    {
        rtc.setHour(h);
        rtc.setMinute(m);
        rtc.setSecond(s);
        Serial.println("Setup RTC to: " + String(isH) + ":" + String(isM));
    }
    else
    {
        Serial.println("RTC time is Ok. Time: " + String(isH) + ":" + String(isM));
    }
}

String getTime()
{
    const unsigned long RtcUpdateTimeout_ms = 500;
    static unsigned long previousRtcUpdate = 0;
    static String timeRtc = "-";

    unsigned long ms = millis();
    unsigned long tDiff = ms - previousRtcUpdate;
    if(previousRtcUpdate == 0 || tDiff > RtcUpdateTimeout_ms)
    {
        bool h12Flag;
        bool pmFlag;
        previousRtcUpdate = ms;

        int h = rtc.getHour(h12Flag, pmFlag);
        int m = rtc.getMinute();
        String hStr = h < 10 ? "0" + String(h) : String(h);
        String mStr = m < 10 ? "0" + String(m) : String(m);
        timeRtc = hStr + ":" + mStr;
    }
    return timeRtc;
}

void setup()
{
    initialRunningTextActive = true;

    // safety startup delay
    delay(1000);

    Serial.begin(460800);
    timerSwitchNextPattern.setInterval(TIME_NEXT_PATTERN);

    pinMode(LED_GPIO, OUTPUT);
    pinMode(BUTTON_GPIO, INPUT);

    // Start the I2C interface for RTC
    Wire.begin();

    ledMatrix.setup();
    wifi.setupWiFi();
    if(wifi.isConnected())
    {
        int h, m, s;
        if(wifi.getNtpTime(h, m, s))
        {
            setupRtc(h, m, s);
        }

        Serial.println("wifi connected");
        ledMatrix.setRunningText(wifi.getIpAddress());
    }
    else
    {
        Serial.println("wifi NOT connected");
        ledMatrix.setRunningText("Привет");
    }
    ledMatrix.switchToPattern(LedMatrix::DrawingPattern::RunningText);
}

void loop()
{
    if(initialRunningTextActive && ledMatrix.getActivePattern() != LedMatrix::DrawingPattern::RunningText)
    {
        initialRunningTextActive = false;
    }

    if(!initialRunningTextActive && ledMatrix.getActivePattern() == LedMatrix::DrawingPattern::RunningText)
    {
        auto time = getTime();
        ledMatrix.setRunningText(time);
    }

    ledMatrix.loop();

    wifi.loopWifi();

    if(buttonAction())
    {
        ledMatrix.switchToNextEnabledPattern();
        timerSwitchNextPattern.reset();
    }

    if(timerSwitchNextPattern.isReady())
    {
        ledMatrix.switchToRandomEnabledPattern();
    }
}
