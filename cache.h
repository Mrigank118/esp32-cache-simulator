#pragma once
#include "config_loader.h"
#include <vector>
#include <stdint.h>

struct CacheLine {
  uint32_t tag = 0;
  bool valid = false;
  unsigned long age = 0; // for LRU (higher = older)
  unsigned long inserted = 0; // for FIFO timestamp
};

class Cache {
public:
  void begin(const Config& cfg);
  bool access(uint32_t address);

  // statistics
  int getHits() const { return hits; }
  int getMisses() const { return misses; }

  // expose cache state (read-only snapshot)
  std::vector<std::vector<CacheLine>> getCacheSets() const { return sets; }

  // reset stats (web UI)
  void resetStats() { hits = 0; misses = 0; }

private:
  uint16_t cacheSize = 16;
  uint16_t blockSize = 1;
  uint16_t associativity = 1;
  String replacementPolicy = "LRU";

  int hits = 0;
  int misses = 0;

  // representation: sets[setIndex] -> vector of CacheLine (ways)
  std::vector<std::vector<CacheLine>> sets;
  unsigned long tick = 0; // increments on each access to help LRU/FIFO
};
