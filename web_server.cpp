#include "web_server.h"
#include <Arduino.h>

void WebServerHandler::begin(Cache* cachePtr, Config* cfgPtr, LEDControl* ledPtr) {
  cacheRef = cachePtr;
  cfg = cfgPtr;
  leds = ledPtr;

  // Root page: web interface
  server.on("/", [this]() {
    String page = R"rawliteral(
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>ESP32 Cache Simulator</title>
<style>
  body{font-family:Arial;display:flex;align-items:center;justify-content:center;height:100vh;background:#f7fafc}
  .card{background:#fff;padding:20px;border-radius:8px;box-shadow:0 6px 18px rgba(0,0,0,0.08);width:320px;text-align:center}
  h1{font-size:18px;margin:0 0 10px;color:#0f172a}
  .stat{font-size:20px;margin:8px 0}
  .small{font-size:12px;color:#64748b}
  button{margin:6px;padding:8px 12px;border-radius:6px;border:0;background:#2563eb;color:#fff;cursor:pointer}
  input,select{width:100%;padding:8px;margin:6px 0;border-radius:6px;border:1px solid #e2e8f0}
</style>
</head>
<body>
<div class="card">
  <h1>ESP32 Cache Simulator</h1>
  <div class="stat">Hits: <span id="hits">0</span></div>
  <div class="stat">Misses: <span id="misses">0</span></div>
  <div class="small">LED (hit = green, miss = red)</div>
  <hr>
  <div>
    <button onclick="resetStats()">Reset Stats</button>
    <button onclick="simulateOne()">Simulate One</button>
  </div>
  <hr>
  <div>
    <label>Cache Size (lines)</label>
    <input id="cacheSize" type="number" min="1">
    <label>Associativity</label>
    <input id="assoc" type="number" min="1">
    <label>Policy</label>
    <select id="policy"><option>LRU</option><option>FIFO</option></select>
    <label>Sim Interval (ms)</label>
    <input id="interval" type="number" min="10">
    <button onclick="saveConfig()">Save Config</button>
  </div>
  <div class="small" id="msg"></div>
</div>
<script>
async function fetchJson(path, opts){ const r = await fetch(path, opts); return r.json(); }

async function update() {
  try {
    const s = await fetchJson('/stats');
    document.getElementById('hits').textContent = s.hits;
    document.getElementById('misses').textContent = s.misses;
  } catch(e){ console.log(e); }
}

async function resetStats() {
  await fetch('/reset', {method:'POST'});
  update();
}

async function simulateOne(){
  await fetch('/simulate', {method:'POST'});
  update();
}

async function loadConfig(){
  const c = await fetchJson('/config');
  document.getElementById('cacheSize').value = c.cacheSize;
  document.getElementById('assoc').value = c.associativity;
  document.getElementById('policy').value = c.replacementPolicy;
  document.getElementById('interval').value = c.simIntervalMs;
}

async function saveConfig(){
  const cfg = {
    cacheSize: parseInt(document.getElementById('cacheSize').value),
    associativity: parseInt(document.getElementById('assoc').value),
    replacementPolicy: document.getElementById('policy').value,
    simIntervalMs: parseInt(document.getElementById('interval').value)
  };
  const res = await fetch('/config', {
    method: 'POST',
    headers: {'Content-Type':'application/json'},
    body: JSON.stringify(cfg)
  });
  const j = await res.json();
  document.getElementById('msg').textContent = j.message || 'Saved';
  setTimeout(()=>document.getElementById('msg').textContent='',2000);
}

setInterval(update, 1000);
update();
loadConfig();
</script>
</body>
</html>
)rawliteral";
    server.send(200, "text/html", page);
  });

  // Return current stats
  server.on("/stats", HTTP_GET, [this]() {
    String json = "{\"hits\":" + String(cacheRef->getHits()) +
                  ",\"misses\":" + String(cacheRef->getMisses()) + "}";
    server.send(200, "application/json", json);
  });

  // Reset statistics
  server.on("/reset", HTTP_POST, [this]() {
    cacheRef->resetStats();
    server.send(200, "application/json", "{\"message\":\"reset\"}");
  });

  // Simulate one access
  server.on("/simulate", HTTP_POST, [this]() {
    cacheRef->access(random(0, 1024));
    server.send(200, "application/json", "{\"message\":\"simulated\"}");
  });

  // Get current config
  server.on("/config", HTTP_GET, [this]() {
    String json = "{\"cacheSize\":" + String(cfg->cacheSize) +
                  ",\"associativity\":" + String(cfg->associativity) +
                  ",\"replacementPolicy\":\"" + cfg->replacementPolicy + "\"" +
                  ",\"simIntervalMs\":" + String(cfg->simIntervalMs) + "}";
    server.send(200, "application/json", json);
  });

  // Save config
  server.on("/config", HTTP_POST, [this]() {
    if (!server.hasArg("plain")) {
      server.send(400, "application/json", "{\"error\":\"no body\"}");
      return;
    }
    String body = server.arg("plain");

    auto getInt = [&](const String& key, int def) -> int {
      int idx = body.indexOf("\"" + key + "\"");
      if (idx < 0) return def;
      int colon = body.indexOf(':', idx);
      if (colon < 0) return def;
      int comma = body.indexOf(',', colon);
      String sub = (comma < 0) ? body.substring(colon + 1) : body.substring(colon + 1, comma);
      sub.trim();
      return sub.toInt();
    };

    auto getStr = [&](const String& key, const String& def) -> String {
      int idx = body.indexOf("\"" + key + "\"");
      if (idx < 0) return def;
      int colon = body.indexOf(':', idx);
      int quote1 = body.indexOf('"', colon + 1);
      if (quote1 < 0) return def;
      int quote2 = body.indexOf('"', quote1 + 1);
      if (quote2 < 0) return def;
      return body.substring(quote1 + 1, quote2);
    };

    cfg->cacheSize = getInt("cacheSize", cfg->cacheSize);
    cfg->associativity = getInt("associativity", cfg->associativity);
    cfg->replacementPolicy = getStr("replacementPolicy", cfg->replacementPolicy);
    cfg->simIntervalMs = getInt("simIntervalMs", cfg->simIntervalMs);

    cfg->save();
    cacheRef->begin(*cfg);
    server.send(200, "application/json", "{\"message\":\"config saved\"}");
  });

  // ✅ Start server
  server.begin();
  Serial.println("✅ Web server started");
}



void WebServerHandler::handleClient() {
  server.handleClient();
}
