// ===================================================================================================
// ESP32 Real Hack Lab v4.0 - Deep Dive Kode (Description.md)
// ===================================================================================================
//
// Bayangin punya lab hacking mini yang bisa dibawa ke mana aja — cuma modal ESP32, breadboard, dan HP.
// Itu yang gue bangun di sini. Tapi ini bukan cuma “tampilan keren”. Setiap baris kode dirancang supaya
// semua fitur benar-benar jalan, offline, dan legal — karena semuanya cuma berjalan di jaringan lokal 
// yang dibuat oleh ESP32 sendiri (192.168.4.x).
//
// ===================================================================================================
// 1. Include & Definisi
// ===================================================================================================
// Semua library yang dipakai adalah bawaan ESP32 core — gak perlu tambahan eksternal.
// AP_SSID dan AP_PASS bikin ESP32 jadi hotspot sendiri.
// LED_PIN 2 dipilih karena itu built-in LED di kebanyakan ESP32 DevKit V1.
// SCRIPT_FILE adalah path file di SPIFFS — sistem file internal ESP32.
//
// #include <WiFi.h>
// #include <WebServer.h>
// #include <WiFiClient.h>
// #include <FS.h>
// #include <SPIFFS.h>
//
// #define AP_SSID "Redzskid_Lab"
// #define AP_PASS "HackThePlanet!"
// #define LED_PIN 2
// #define SCRIPT_FILE "/scripts.txt"
//
// ===================================================================================================
// 2. Global State
// ===================================================================================================
// WebServer server(80) bikin ESP32 jadi web server beneran di port 80.
// Variabel global dipakai buat simpan status runtime: apakah Evil Twin aktif, SSID-nya apa, dan log aktivitas terkini.
//
// WebServer server(80);
// String currentFakeSSID = "";
// bool evilTwinActive = false;
// String logBuffer = "";
//
// ===================================================================================================
// 3. Fungsi Bantuan (Utility Layer)
// ===================================================================================================
// saveScript() nyimpen script ke file SPIFFS dalam mode append — jadi script lama gak ilang.
// addLog() nyimpen riwayat aktivitas dengan batas 1000 karakter biar gak memory leak.
// initSpiffs() inisialisasi sistem file internal ESP32.
// loadScripts() membaca semua script dari file dan mengembalikannya dalam format HTML yang aman.
//
// void saveScript(String script) {
//   File file = SPIFFS.open(SCRIPT_FILE, "a");
//   file.println(script);
//   file.close();
// }
//
// void addLog(String msg) {
//   logBuffer = "> " + msg + "\n" + logBuffer;
//   if (logBuffer.length() > 1000) logBuffer = logBuffer.substring(0, 1000);
// }
//
// void initSpiffs() {
//   if (!SPIFFS.begin(true)) {
//     Serial.println("❌ SPIFFS Mount Failed");
//     return;
//   }
//   Serial.println("✅ SPIFFS Ready");
// }
//
// String loadScripts() {
//   if (!SPIFFS.exists(SCRIPT_FILE)) return "<i>Belum ada script.</i>";
//   
//   File file = SPIFFS.open(SCRIPT_FILE, "r");
//   String content = "";
//   while (file.available()) {
//     String line = file.readStringUntil('\n');
//     line.trim();
//     if (line.length() > 0) {
//       line.replace("<", "<");
//       line.replace(">", ">");
//       content = "<div style='padding:4px;border-bottom:1px solid #00331a;'>• " + line + "</div>" + content;
//     }
//   }
//   file.close();
//   return content == "" ? "<i>Belum ada script.</i>" : content;
// }
//
// ===================================================================================================
// 4. Web UI – HTML dalam Flash
// ===================================================================================================
// HTML UI disimpan langsung di flash memory pake PROGMEM dan R"rawliteral(...)" — hemat RAM.
// Desainnya dark mode, responsif, dan interaktif — kayak tools beneran, bukan demo murahan.
//
// const char MAIN_PAGE[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// ...
// )rawliteral";
//
// ===================================================================================================
// 5. Handler – Otak Operasional
// ===================================================================================================
// Setiap tombol di web punya handler spesifik:
// - /led/on → digitalWrite(LED_PIN, HIGH) → LED di breadboard nyala.
// - /scan_area → loop dari 192.168.4.1 sampai .10, coba koneksi ke port 80/8080.
// - /port_scan → coba 8 port umum (80, 22, 21, dll) pake WiFiClient.connect().
// - /dos → kirim 100 request HTTP GET ke target lokal.
// - /fuzz → lempar 7 payload keamanan (XSS, path traversal, SQLi) ke endpoint /.
// - /evil_twin → WiFi.softAP(ssid, "12345678") → bikin fake hotspot.
// - /deauth → matikan AP sebentar, lalu nyalain lagi → simulasi disconnect client.
// - /exec → eksekusi command (misal led on) dan simpan ke SPIFFS.
//
// void handleLedOn() { digitalWrite(LED_PIN, HIGH); ... }
// void handleScanArea() { ... }
// void handlePortScan() { ... }
// void handleDoS() { ... }
// void handleFuzz() { ... }
// void handleEvilTwin() { ... }
// void handleDeauth() { ... }
// void handleExec() { ... }
// void handleScripts() { ... }
//
// ===================================================================================================
// ⚡ Cara Kerja Nyata (Bukan Simulasi)
// ===================================================================================================
// - DoS Attack: Bukan cuma tampilin teks. ESP32 benar-benar kirim 100 request HTTP ke target lokal.
// - Port Scanner: Bukan daftar statis. ESP32 coba koneksi TCP beneran ke setiap port.
// - Evil Twin: Bukan ilusi. ESP32 beneran jadi access point dengan SSID bebas.
// - Script Storage: Data disimpan di flash internal via SPIFFS. Bahkan setelah ESP32 direstart, semua script tetap ada.
//
// ===================================================================================================
// 🛡️ Legal & Etika
// ===================================================================================================
// Semua fitur dibatasi hanya untuk jaringan lokal:
// - Target IP harus dimulai dengan 192.168.4. → artinya cuma bisa nyerang perangkat yang sengaja lu sambungin ke hotspot ESP32.
// - Tidak ada koneksi ke internet.
// - Tidak ada scanning di luar subnet lokal.
//
// Ini bukan alat buat nyerang — tapi alat edukasi buat ngerti cara kerja serangan, supaya bisa dipertahankan.
//
// ===================================================================================================
// 💡 Kenapa Ini Beda?
// ===================================================================================================
// - Bukan demo: Semua fitur nyata, tapi dikurung di lingkungan aman.
// - Portabel: Cuma butuh power bank + HP buat demo.
// - Self-contained: Gak butuh laptop, internet, atau server eksternal.
// - White hat by design: Dibangun dari prinsip ethical hacking sejak awal.
//
// Dibikin buat tugas informatika, tapi levelnya udah kayak proyek cybersecurity beneran. Cocok buat demo ke guru, ikut lomba, atau jadi fondasi eksperimen lanjutan.
//
// Hack smart. Stay legal. Dan tetap jadi white hat.
