#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

// --- HARDWARE CONFIG ---
const char* ssid = "TUATHA_DE_NODE";
const char* password = ""; 

// Pin Aman (Anti-Boot-Failure)
const int normalPins[] = {4, 5, 13, 14, 16, 17};
const int numNormal = sizeof(normalPins) / sizeof(normalPins[0]);

// RGB PWM Config
const int pinR = 25; const int pinG = 26; const int pinB = 27;
#define CH_R 0
#define CH_G 1
#define CH_B 2

AsyncWebServer server(80);
DNSServer dnsServer;

// --- STATE MACHINE ---
enum Mode { STANDARD, CHAOS, RAIN, BREATH, WAVE, STORM, GHOST, HEART };
Mode currentMode = STANDARD;
unsigned long lastUpdate = 0;
int waveStep = 0;

// Whitelist Filter
bool isAllowedPin(int p) {
  for(int i=0; i<numNormal; i++) if(normalPins[i] == p) return true;
  return (p == pinR || p == pinG || p == pinB);
}

// HSV Engine (Buat transisi warna OP)
void setRGB(int r, int g, int b) {
  ledcWrite(CH_R, r); ledcWrite(CH_G, g); ledcWrite(CH_B, b);
}

// --- HTML INTERFACE (PROGMEM) ---
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>TUATHA DE | COMMAND</title>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;600;800&display=swap" rel="stylesheet">
    <style>
        :root { --bg: #080809; --surface: #111113; --accent: #3b82f6; --text: #ffffff; --text-dim: #64748b; }
        * { box-sizing: border-box; -webkit-tap-highlight-color: transparent; }
        body { background: var(--bg); color: var(--text); font-family: 'Inter', sans-serif; margin: 0; display: flex; justify-content: center; align-items: center; min-height: 100vh; }
        #auth-screen { width: 100%; max-width: 380px; text-align: center; padding: 40px; }
        .logo-img { width: 140px; filter: drop-shadow(0 0 15px rgba(59, 130, 246, 0.5)); margin-bottom: 20px; }
        input[type=password] { width: 100%; padding: 18px; background: var(--surface); border: 1px solid #1f1f23; border-radius: 14px; color: white; text-align: center; outline: none; }
        .unlock-btn { width: 100%; margin-top: 15px; padding: 16px; background: var(--accent); color: white; border: none; border-radius: 14px; font-weight: 800; cursor: pointer; text-transform: uppercase; }
        #wipe-screen { display: none; width: 350px; text-align: center; }
        .progress-track { width: 100%; height: 4px; background: #1f1f23; border-radius: 10px; overflow: hidden; margin-top: 20px; }
        .progress-fill { width: 0%; height: 100%; background: var(--accent); }
        #main-dashboard { display: none; width: 100%; max-width: 420px; padding: 20px; opacity: 0; transform: translateY(20px); transition: 1s; }
        .card { background: var(--surface); padding: 20px; border-radius: 24px; border: 1px solid #1f1f23; margin-bottom: 20px; }
        .grid-modes { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; margin-bottom: 20px; }
        .mode-item { background: var(--surface); padding: 15px; border-radius: 16px; border: 1px solid #1f1f23; text-align: center; cursor: pointer; transition: 0.3s; }
        .mode-item.active { border-color: var(--accent); background: rgba(59, 130, 246, 0.1); color: var(--accent); }
        .node-row { display: flex; justify-content: space-between; align-items: center; padding: 15px 0; border-bottom: 1px solid #1f1f23; }
        .tgl { position: relative; width: 44px; height: 24px; }
        .tgl input { opacity: 0; width: 0; }
        .sld { position: absolute; top: 0; left: 0; right: 0; bottom: 0; background: #2d2d33; border-radius: 20px; cursor: pointer; transition: 0.3s; }
        .sld:before { content: ""; position: absolute; height: 18px; width: 18px; left: 3px; bottom: 3px; background: white; border-radius: 50%; transition: 0.3s; }
        input:checked + .sld { background: var(--accent); }
        input:checked + .sld:before { transform: translateX(20px); }
        .fade-in { opacity: 1 !important; transform: translateY(0) !important; }
    </style>
</head>
<body>
    <div id="auth-screen">
        <img src="https://iili.io/feXGnBj.md.png" class="logo-img">
        <h1>TUATHA DÉ</h1>
        <input type="password" id="passkey" placeholder="••••••••">
        <button class="unlock-btn" onclick="initiateSequence()">Unlock Access</button>
    </div>
    <div id="wipe-screen"><h2>AUTHORIZING</h2><div class="progress-track"><div class="progress-fill" id="p-fill"></div></div></div>
    <div id="main-dashboard">
        <div class="grid-modes">
            <div class="mode-item" onclick="setMode('chaos', this)"><h3>Chaos</h3></div>
            <div class="mode-item" onclick="setMode('rain', this)"><h3>Rain</h3></div>
            <div class="mode-item" onclick="setMode('breath', this)"><h3>Breath</h3></div>
            <div class="mode-item" onclick="setMode('wave', this)"><h3>Wave</h3></div>
        </div>
        <div class="card" id="nodes" style="padding: 0 20px;"></div>
    </div>
<script>
    function initiateSequence() {
        if(document.getElementById('passkey').value !== "redzNotDev") { alert("WRONG"); return; }
        document.getElementById('auth-screen').style.display='none';
        document.getElementById('wipe-screen').style.display='block';
        let p=0; const fill=document.getElementById('p-fill');
        const intv = setInterval(() => {
            p+=2; fill.style.width=p+"%";
            if(p>=100){ 
                clearInterval(intv); document.getElementById('wipe-screen').style.display='none';
                const d=document.getElementById('main-dashboard'); d.style.display='block';
                setTimeout(()=>d.classList.add('fade-in'),50);
            }
        }, 30);
    }
    const pins = [{p:4,n:"GPIO 4"}, {p:5,n:"GPIO 5"}, {p:13,n:"GPIO 13"}, {p:14,n:"GPIO 14"}, {p:16,n:"GPIO 16"}, {p:17,n:"GPIO 17"}, {p:25,n:"RGB-R"}, {p:26,n:"RGB-G"}, {p:27,n:"RGB-B"}];
    document.getElementById('nodes').innerHTML = pins.map(x=>`<div class="node-row"><span>${x.n}</span><label class="tgl"><input type="checkbox" id="p${x.p}" onchange="fire(${x.p},this.checked)"><span class="sld"></span></label></div>`).join('');
    function setMode(m, el) {
        document.querySelectorAll('.mode-item').forEach(i=>i.classList.remove('active'));
        el.classList.add('active');
        fetch(`/mode?type=${m}`);
    }
    function fire(p, s) { fetch(`/update?pin=${p}&state=${s?1:0}`); }
</script>
</body>
</html>
)rawliteral";

// --- SERVER HANDLERS ---
void setup() {
  Serial.begin(115200);
  for(int i=0; i<numNormal; i++) pinMode(normalPins[i], OUTPUT);
  
  ledcSetup(CH_R, 5000, 8); ledcAttachPin(pinR, CH_R);
  ledcSetup(CH_G, 5000, 8); ledcAttachPin(pinG, CH_G);
  ledcSetup(CH_B, 5000, 8); ledcAttachPin(pinB, CH_B);

  WiFi.softAP(ssid, password);
  dnsServer.start(53, "*", WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ request->send_P(200, "text/html", index_html); });

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
    int p = request->getParam("pin")->value().toInt();
    int s = request->getParam("state")->value().toInt();
    if(isAllowedPin(p)) {
       if(p >= 25) ledcWrite(p == 25 ? CH_R : (p == 26 ? CH_G : CH_B), s ? 255 : 0);
       else digitalWrite(p, s);
    }
    currentMode = STANDARD;
    request->send(200);
  });

  server.on("/mode", HTTP_GET, [](AsyncWebServerRequest *request){
    String m = request->getParam("type")->value();
    if(m == "chaos") currentMode = CHAOS;
    else if(m == "rain") currentMode = RAIN;
    else if(m == "breath") currentMode = BREATH;
    else if(m == "wave") currentMode = WAVE;
    request->send(200);
  });

  server.onNotFound([](AsyncWebServerRequest *request){ request->send_P(200, "text/html", index_html); });
  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
  unsigned long now = millis();

  if (currentMode == CHAOS && now - lastUpdate > 150) {
    lastUpdate = now;
    digitalWrite(normalPins[random(0, numNormal)], random(0, 2));
    setRGB(random(0,256), random(0,256), random(0,256));
  } 
  else if (currentMode == BREATH) {
    int val = (exp(sin(now / 2000.0 * PI)) - 0.36787944) * 108.0; // Human breathing curve
    setRGB(val, val/2, 255-val); // Smooth color shift
    lastUpdate = now;
  }
  else if (currentMode == RAIN && now - lastUpdate > 200) {
    static int rainIdx = 0;
    for(int i=0; i<numNormal; i++) digitalWrite(normalPins[i], LOW);
    digitalWrite(normalPins[rainIdx], HIGH);
    setRGB(0, (rainIdx * 40), 255);
    rainIdx = (rainIdx + 1) % numNormal;
    lastUpdate = now;
  }
  else if (currentMode == WAVE) {
    int r = (sin(now / 500.0) * 127) + 128;
    int g = (sin(now / 500.0 + 2.0) * 127) + 128;
    int b = (sin(now / 500.0 + 4.0) * 127) + 128;
    setRGB(r, g, b);
  }
}
