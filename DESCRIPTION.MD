# 💀 ESP32 BITCHES ON YOU v4.0 - Penjelasan Kode Lengkap

Dokumen ini menjelaskan secara **detail struktur dan fungsi** dari kode utama proyek ESP32 Real Hack Lab v4.0. Cocok untuk **tugas informatika** atau **presentasi ke guru**.

---

## 📋 Struktur Kode

Kode dibagi menjadi **5 bagian utama**:

### 1. Include & Definisi
```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <FS.h>
#include <SPIFFS.h>

#define AP_SSID "Redzskid_Lab"
#define AP_PASS "HackThePlanet!"
#define LED_PIN 2
#define SCRIPT_FILE "/scripts.txt"

Include: Library bawaan ESP32 untuk WiFi, web server, dan file system
Define: Konstanta untuk nama WiFi, password, pin LED, dan nama file script
2. Global Variables
