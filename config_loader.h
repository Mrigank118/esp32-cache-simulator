#pragma once
#include <Arduino.h>

struct Config {
  // defaults
  String ssid = "OnePlus 9R";
  String password = "mrigank@11";
  uint8_t hitLedPin = 4;   // D4
  uint8_t missLedPin = 5;  // D5

  // cache params
  uint16_t cacheSize = 16;       // number of lines (for direct-mapped) OR total lines
  uint16_t blockSize = 1;        // block size (words) — unused in demo
  uint16_t associativity = 1;    // 1 => direct-mapped, >1 => set-assoc
  String replacementPolicy = "LRU"; // "LRU" or "FIFO"

  uint16_t simIntervalMs = 1000; // simulation interval between accesses

  // persistence API
  bool load();   // load from Preferences; returns true if loaded
  bool save() const; // save to Preferences
};
