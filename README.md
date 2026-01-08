# ⚡ TUATHA DÉ | NODE INTERFACE v2.0
**Author:** Azuya (Stage Name: Tuatha Dé / redzskid)  
**Status:** God Mode Calibrated  
**Hardware:** ESP32 DevKit V1 (30 Pins)

---

## 🛠 Project Overview
Sistem kontrol lampu cerdas (IoT) berbasis ESP32 yang mengintegrasikan dashboard web premium dengan fitur keamanan **Auth-Wipe Sequence**. Sistem ini dirancang untuk mengontrol LED standar dan LED RGB Common Cathode melalui protokol HTTP Async.

## 🚀 Key Features
* **Encrypted Access:** Proteksi password sebelum masuk ke dashboard.
* **Wipe Animation:** Efek transisi sistem bergaya terminal hacker.
* **Live Pulse Monitor:** Visualisasi aktivitas sistem secara real-time.
* **8+ Sequence Modes:** Termasuk Chaos, Rain, Breath, dan Wave.
* **RGB Engine:** Menggunakan PWM (LEDC) untuk transisi warna yang smooth (Bukan On/Off biasa).
* **Captive Portal:** Otomatis membuka dashboard saat terhubung ke WiFi tanpa ketik IP.

## 🔌 Pin Mapping (ESP32 DevKit V1)

| Component | Pin (GPIO) | Notes |
|-----------|-----------|-------|
| LED 1 | 4 | Normal Pin |
| LED 2 | 5 | Normal Pin |
| LED 3 | 13 | Normal Pin |
| LED 4 | 14 | Normal Pin |
| LED 5 | 16 | Normal Pin |
| LED 6 | 17 | Normal Pin |
| **RGB RED** | **25** | **PWM Channel 0** |
| **RGB GREEN**| **26** | **PWM Channel 1** |
| **RGB BLUE** | **27** | **PWM Channel 2** |

> **Warning:** Jangan menggunakan GPIO 2 atau 12 untuk menghindari boot-failure pada ESP32.

## ⚙️ Installation
1.  Buka file `.ino` di Arduino IDE.
2.  Install library berikut melalui Library Manager:
    * `ESPAsyncWebServer`
    * `AsyncTCP`
3.  Pilih board **DOIT ESP32 DEVKIT V1**.
4.  Klik **Upload**.

## 📱 How to Use
1.  Nyalakan ESP32.
2.  Cari WiFi bernama `TUATHA_DE_NODE` di HP/Laptop lo.
3.  Konek (Tanpa password WiFi).
4.  Tunggu pop-up muncul atau buka browser ke `192.168.4.1`.
5.  Masukkan Passkey: `redzNotDev`.
6.  **Enjoy God Mode.**

---

## 📜 Dev Notes
- **Non-Blocking Logic:** Menggunakan `millis()` untuk memastikan UI tetap responsif saat animasi berjalan.
- **Safety Whitelist:** Endpoint `/update` telah diproteksi agar tidak bisa memanipulasi pin sistem yang sensitif.
- **Visuals:** UI menggunakan font 'Inter' dan skema warna Deep Space (Tailwind Palette).

"THIS IS NOT A TOY PROJECT"
