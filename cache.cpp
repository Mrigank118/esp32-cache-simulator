#include "cache.h"
#include <Arduino.h>

void Cache::begin(const Config& cfg) {
  cacheSize = cfg.cacheSize;
  blockSize = cfg.blockSize;
  associativity = cfg.associativity;
  replacementPolicy = cfg.replacementPolicy;
  hits = 0;
  misses = 0;
  tick = 0;

  // compute number of sets = (cacheSize / associativity)
  uint16_t numSets = 1;
  if (associativity == 0) associativity = 1;
  numSets = (cacheSize + associativity - 1) / associativity;
  sets.clear();
  sets.resize(numSets);
  for (auto &s : sets) s.resize(associativity);

  Serial.printf("Cache init: size=%u assoc=%u sets=%u policy=%s\n",
                cacheSize, associativity, (unsigned)numSets, replacementPolicy.c_str());
}

bool Cache::access(uint32_t address) {
  tick++;
  // Simple mapping: tag = address / blockSize, set = tag % numSets
  uint32_t tag = address / (blockSize ? blockSize : 1);
  uint16_t numSets = sets.size();
  uint32_t setIndex = (numSets > 0) ? (tag % numSets) : 0;
  auto &set = sets[setIndex];

  // Search for tag in set
  for (auto &line : set) {
    if (line.valid && line.tag == tag) {
      // Hit: update age for LRU
      line.age = tick;
      hits++;
      Serial.printf("HIT addr=%u tag=%u set=%u\n", address, tag, setIndex);
      return true;
    }
  }

  // Miss: choose a victim (invalid preferred)
  misses++;
  Serial.printf("MISS addr=%u tag=%u set=%u\n", address, tag, setIndex);

  int replaceIndex = -1;
  // find invalid slot first
  for (int i = 0; i < (int)set.size(); ++i) {
    if (!set[i].valid) { replaceIndex = i; break; }
  }

  if (replaceIndex == -1) {
    // all valid => apply replacement policy
    if (replacementPolicy == "FIFO") {
      unsigned long oldest = ULONG_MAX;
      for (int i = 0; i < (int)set.size(); ++i) {
        if (set[i].inserted < oldest) { oldest = set[i].inserted; replaceIndex = i; }
      }
    } else { // default LRU
      unsigned long oldestAge = ULONG_MAX;
      for (int i = 0; i < (int)set.size(); ++i) {
        if (set[i].age < oldestAge) { oldestAge = set[i].age; replaceIndex = i; }
      }
    }
  }

  if (replaceIndex < 0) replaceIndex = 0;

  // Replace
  set[replaceIndex].tag = tag;
  set[replaceIndex].valid = true;
  set[replaceIndex].age = tick;
  set[replaceIndex].inserted = tick;

  return false;
}
