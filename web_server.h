#pragma once
#include <WebServer.h>
#include "cache.h"
#include "config_loader.h"
#include "led_control.h"

class WebServerHandler {
public:
  void begin(Cache* cachePtr, Config* cfgPtr, LEDControl* ledPtr);
  void handleClient();  // 👈 make sure this line exists

private:
  WebServer server{80};
  Cache* cacheRef;
  Config* cfg;
  LEDControl* leds;
};
