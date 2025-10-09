# ESP32 Real Hack Lab v4.0 🛡️

Sebuah laboratorium hacking mini yang portabel dan edukatif, dibangun di atas ESP32. Proyek ini dirancang untuk demonstrasi dan pembelajaran keamanan siber secara etis dalam lingkungan jaringan lokal yang terisolasi.

![ESP32](https://img.shields.io/badge/Platform-ESP32-000000?style=for-the-badge&logo=espressif)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

## Daftar Isi

- [Fitur Utama](#fitur-utama)
- [Kebutuhan](#kebutuhan)
  - [Hardware](#hardware)
  - [Software](#software)
- [Instalasi & Setup](#instalasi--setup)
- [Cara Penggunaan](#cara-penggunaan)
- [Struktur Kode](#struktur-kode)
- [⚠️ Pertimbangan Etis & Legal](#️-pertimbangan-ethis--legal)
- [Kontribusi](#kontribusi)
- [Lisensi](#lisensi)
- [Kredit](#kredit)

---

## Fitur Utama

Proyek ini bukan sekadar simulasi; setiap fitur diimplementasikan untuk berfungsi nyata dalam batasan jaringan lokal.

- 💡 **Kontrol LED**: Nyalakan dan matikan LED pada breadboard secara langsung dari antarmuka web.
- 📡 **Network Scanner**: Pindai perangkat yang terhubung pada jaringan lokal (192.168.4.x).
- 🔓 **Port Scanner**: Periksa port umum (seperti 80, 22, 443) pada target di jaringan lokal.
- 💥 **Denial of Service (DoS)**: Kirim sejumlah request HTTP ke target lokal untuk mensimulasikan serangan DoS.
- 🧪 **Web Fuzzer**: Uji keamanan web server lokal dengan berbagai payload (XSS, SQL Injection, Path Traversal).
- 🎭 **Evil Twin Attack**: Buat Access Point (AP) tiruan dengan SSID yang dapat disesuaikan.
- ✋ **Deauth Simulation**: Putuskan sementara koneksi client dari AP Evil Twin.
- 📜 **Script Executor & Storage**: Simpan, eksekusi, dan kelola serangkaian perintah yang tersimpan di memori flash ESP32 (SPIFFS).
- 📊 **Activity Log**: Pantau semua aktivitas dan hasil serangan secara real-time melalui log pada antarmuka web.

## Kebutuhan

### Hardware

- ESP32 DevKit V1 (atau varian lain dengan built-in LED di pin 2).
- Breadboard.
- Kabel Jumper.
- LED (opsional, jika tidak menggunakan built-in LED).
- Power Bank atau kabel USB untuk daya.
- Smartphone atau laptop dengan browser web.

### Software

- [Arduino IDE](https://www.arduino.cc/en/software) dengan ESP32 Board Manager terinstal.
- Library yang digunakan adalah library bawaan ESP32 core:
  - `WiFi.h`
  - `WebServer.h`
  - `FS.h`
  - `SPIFFS.h`

## Instalasi & Setup

1.  **Clone Repository**
    ```bash
    git clone https://github.com/username/esp32-real-hack-lab.git
    cd esp32-real-hack-lab
    ```

2.  **Setup Hardware**
    - Jika menggunakan LED eksternal, hubungkan kaki positif (+) ke pin `D2` (GPIO 2) dan kaki negatif (-) ke resistor (220-330 Ohm) lalu ke GND.
    - Jika tidak, Anda dapat menggunakan built-in LED di board ESP32.

3.  **Buka di Arduino IDE**
    - Buka file `.ino` proyek ini di Arduino IDE.
    - Pastikan board yang dipilih adalah "ESP32 Dev Module" (atau yang sesuai) dan port COM terdeteksi dengan benar.

4.  **Upload Kode**
    - Klik tombol `Upload` untuk mem-flash kode ke ESP32.
    - Buka Serial Monitor (baud rate 115200) untuk melihat pesan status, seperti "✅ SPIFFS Ready" dan "✅ AP Started".

5.  **Hubungkan ke Access Point**
    - Di smartphone atau laptop Anda, cari dan sambungkan ke jaringan Wi-Fi bernama `Redzskid_Lab` dengan password `HackThePlanet!`.

6.  **Akses Antarmuka Web**
    - Buka browser web dan kunjungi alamat `http://192.168.4.1`.
    - Anda akan melihat antarmuka kontrol Real Hack Lab.

## Cara Penggunaan

Semua fitur dapat diakses melalui antarmuka web yang telah disediakan.

- **LED Control**: Klik tombol "LED ON" atau "LED OFF" untuk mengontrol LED.
- **Scanning**: Masukkan IP target (misalnya `192.168.4.2`) dan gunakan tombol "Scan Area" atau "Port Scan" untuk memulai pemindaian.
- **Attacks**:
  - **DoS**: Masukkan IP target dan klik "Launch DoS" untuk mengirim 100 request.
  - **Fuzz**: Masukkan IP target dan klik "Start Fuzzing" untuk menguji endpoint dengan payload berbahaya.
- **Evil Twin**:
  - Masukkan SSID palsu yang diinginkan di kolom input.
  - Klik "Start Evil Twin" untuk membuat AP baru. ESP32 akan restart dengan SSID baru.
  - Untuk menghentikannya, klik "Stop Evil Twin".
- **Scripting**:
  - Ketik perintah (misalnya `led on`) di kolom "Execute Command" dan klik "Execute". Perintah akan dijalankan dan disimpan.
  - Lihat semua script yang tersimpan di bagian "Saved Scripts".

## Struktur Kode

Kode diorganisir dengan baik untuk memudahkan pemahaman dan pengembangan.

1.  **Include & Definisi**: Bagian atas berisi semua library yang diperlukan dan definisi konstanta seperti `AP_SSID`, `LED_PIN`, dan `SCRIPT_FILE`.
2.  **Global State**: Inisialisasi objek `WebServer` dan variabel global untuk menyimpan status runtime (status Evil Twin, log, dll.).
3.  **Fungsi Bantuan (Utility Layer)**: Fungsi-fungsi reusable seperti `saveScript()` untuk menyimpan ke SPIFFS, `addLog()` untuk menambahkan pesan ke log, dan `loadScripts()` untuk membaca script yang tersimpan.
4.  **Web UI**: Antarmuka HTML, CSS, dan JavaScript disimpan dalam `PROGMEM` untuk menghemat RAM. UI dirancang dengan tema gelap dan responsif.
5.  **Handler**: Fungsi-fungsi yang dipanggil saat ada request ke endpoint tertentu (misalnya `/led/on`, `/scan_area`, `/evil_twin`). Ini adalah "otak" dari operasional web server.

## ⚠️ Pertimbangan Etis & Legal

Proyek ini **HANYA** ditujukan untuk tujuan **edukasi dan pembelajaran keamanan siber secara etis**.

- **LINGKUNGAN TERISOLASI**: Semua fitur dan serangan dibatasi untuk berjalan **hanya di jaringan lokal (192.168.4.x)** yang dibuat oleh ESP32 itu sendiri.
- **TIDAK ADA KONEKSI INTERNET**: ESP32 tidak terhubung ke internet, mencegah penyalahgunaan untuk menyerang target di luar jaringan lokal.
- **TANGGUNG JAWAB PENGGUNA**: Anda bertanggung jawab penuh atas penggunaan alat ini. **Penulis tidak bertanggung jawab atas penyalahgunaan proyek ini untuk tindakan ilegal atau yang melanggar hukum.** Gunakan hanya pada perangkat yang Anda miliki izin untuk uji coba.

## Kontribusi

Kontribusi Anda sangat dihargai! Jika Anda memiliki ide untuk fitur baru atau menemukan bug, silakan buat *issue* atau kirim *pull request*.

1.  Fork proyek ini.
2.  Buat branch fitur baru (`git checkout -b fitur/BaruSekali`).
3.  Commit perubahan Anda (`git commit -am 'Menambah fitur X'`).
4.  Push ke branch (`git push origin fitur/BaruSekali`).
5.  Buat Pull Request.

## Lisensi

Proyek ini dilisensikan under **MIT License** - lihat file [LICENSE](LICENSE) untuk detailnya.

## Kredit

Proyek ini dibuat oleh **Redzskid** sebagai proyek edukasi dalam bidang keamanan siber dan pengembangan embedded systems.

---

**Hack smart. Stay legal. Tetap menjadi white hat.**
