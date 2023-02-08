#pragma once
#include <Arduino.h>

class Timer
{
public:
  Timer();
  Timer(uint32_t interval);
  void setInterval(uint32_t interval);
  boolean isReady();
  void reset();

private:
  uint32_t _timer = 0;
  uint32_t _interval = 0;
};
