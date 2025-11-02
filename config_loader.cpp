#include "config_loader.h"
#include <Preferences.h>

bool Config::load() {
  Preferences prefs;
  if (!prefs.begin("cache_sim", true)) { // read-only okay but begin returns true even if empty
    Serial.println("Prefs begin failed");
    return false;
  }

  // If no saved fields exist, keep defaults
  if (!prefs.isKey("ssid")) {
    prefs.end();
    Serial.println("No saved config - using defaults.");
    return false;
  }

  ssid = prefs.getString("ssid", ssid);
  password = prefs.getString("password", password);
  hitLedPin = prefs.getUShort("hitLedPin", hitLedPin);
  missLedPin = prefs.getUShort("missLedPin", missLedPin);
  cacheSize = prefs.getUInt("cacheSize", cacheSize);
  blockSize = prefs.getUInt("blockSize", blockSize);
  associativity = prefs.getUInt("associativity", associativity);
  replacementPolicy = prefs.getString("replacementPolicy", replacementPolicy);
  simIntervalMs = prefs.getUShort("simIntervalMs", simIntervalMs);

  prefs.end();
  Serial.println("Loaded config from NVS.");
  return true;
}

bool Config::save() const {
  Preferences prefs;
  if (!prefs.begin("cache_sim", false)) {
    Serial.println("Prefs begin failed for save");
    return false;
  }

  prefs.putString("ssid", ssid);
  prefs.putString("password", password);
  prefs.putUShort("hitLedPin", hitLedPin);
  prefs.putUShort("missLedPin", missLedPin);
  prefs.putUInt("cacheSize", cacheSize);
  prefs.putUInt("blockSize", blockSize);
  prefs.putUInt("associativity", associativity);
  prefs.putString("replacementPolicy", replacementPolicy);
  prefs.putUShort("simIntervalMs", simIntervalMs);

  prefs.end();
  Serial.println("Saved config to NVS.");
  return true;
}
