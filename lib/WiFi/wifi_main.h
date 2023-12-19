#pragma once
#include "WString.h"
#include "led_matrix_main.h"

class Wifi
{
private:
    void http();

public:
    Wifi() = delete;
    Wifi(LedMatrix* ledMatrix);
    void setupWiFi();
    void loopWifi();

    bool isConnected();
    String getIpAddress();
    String getNtpTime();
    bool getNtpTime(int& h, int& m, int& s);

private:
    LedMatrix* _ledMatrix{nullptr};
    LedMatrix::Patterns _activeLedMatrixPatterns;
    // Variable to store the HTTP request
    String _header{};
};
