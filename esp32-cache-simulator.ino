#include <WiFi.h>
#include "web_server.h"
#include "led_control.h"
#include "config_loader.h"
#include "cache.h"
#include "memory.h"

Cache cache;
Memory memory;
LEDControl ledControl;
WebServerHandler webServer;
Config config;

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n=== ESP32 Cache Simulator (Advanced) ===");

  // Load persisted config (if any)
  config.load();

  // Initialize LED pins
  ledControl.begin(config.hitLedPin, config.missLedPin, 18);

  // Connect to WiFi
  Serial.printf("Connecting to WiFi '%s' ...\n", config.ssid.c_str());
  WiFi.begin(config.ssid.c_str(), config.password.c_str());
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 20000) {
    delay(250);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.printf("✅ Connected! IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println();
    Serial.println("⚠️ WiFi connect failed (continuing as AP mode)...");
    // Optionally, could start AP here. For now continue (you can update WiFi via web).
  }

  // Initialize cache and memory
  cache.begin(config);
  memory.begin();

  // Start web server (gives it pointer to cache and config)
  webServer.begin(&cache, &config, &ledControl);

  Serial.println("🚀 Cache simulator ready!");
}

void loop() {
  // Simulate memory access
  uint32_t addr = random(0, 64); // larger address space
  bool hit = cache.access(addr);
  if (hit) ledControl.hit();
  else ledControl.miss();

  webServer.handleClient(); // serve web requests

  // small delay controlling simulation speed
  delay(config.simIntervalMs);
}
