# 💀 ESP32 BITCHES ON YOU v4.0 - Penjelasan Kode Lengkap

Dokumen ini menjelaskan secara **detail struktur dan fungsi** dari kode utama proyek ESP32 Real Hack Lab v4.0. Cocok untuk **tugas informatika** atau **presentasi ke guru**.

Bayangin punya **lab hacking mini** yang bisa lu bawa ke mana aja — cuma modal ESP32, breadboard, dan HP. Itu yang gue bangun di sini. Tapi ini bukan cuma “tampilan keren”. Setiap baris kode dirancang supaya **semua fitur benar-benar jalan**, **offline**, dan **legal** — karena semuanya cuma berjalan di jaringan lokal yang dibuat oleh ESP32 sendiri (`192.168.4.x`).

---

## 🔧 Struktur Inti

Kode ini dibangun dari **5 lapisan utama** yang saling terhubung:

### 1. **Include & Definisi**
```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <FS.h>
#include <SPIFFS.h>

#define AP_SSID "Redzskid_Lab"
#define AP_PASS "HackThePlanet!"
#define LED_PIN 2
#define SCRIPT_FILE "/scripts.txt"```
Semua library yang dipakai adalah bawaan ESP32 core — gak perlu tambahan eksternal.
AP_SSID dan AP_PASS bikin ESP32 jadi hotspot sendiri.
LED_PIN 2 dipilih karena itu built-in LED di kebanyakan ESP32 DevKit V1.
SCRIPT_FILE adalah path file di SPIFFS — sistem file internal ESP32.

##Global State
```cpp
WebServer server(80);
String currentFakeSSID = "";
bool evilTwinActive = false;
String logBuffer = "";
