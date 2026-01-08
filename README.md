# <img src="https://iili.io/feXGnBj.md.png" width="40" vertical-align="middle" /> TUATHA DÉ | COMMAND CENTER v2.0

<div align="left">
  <img src="https://img.shields.io/badge/Status-GOD_MODE-blue?style=for-the-badge&logo=espressif&logoColor=white" />
  <img src="https://img.shields.io/badge/Security-ENCRYPTED-red?style=for-the-badge&logo=target&logoColor=white" />
  <img src="https://img.shields.io/badge/UX-PREMIUM-gold?style=for-the-badge&logo=air-force&logoColor=white" />
</div>

---

### 🌑 OVERVIEW
Dashboard IoT eksklusif yang dirancang untuk kontrol node cerdas. Dibangun di atas **ESP32 DevKit V1** dengan fokus pada estetika visual dan stabilitas sistem *non-blocking*.

### 🕹️ CORE COMMANDS
| Sequence | Logic Description | Vibe |
| :--- | :--- | :--- |
| **CHAOS** | Neural random glitch pattern | ⚡ Glitchy |
| **RAIN** | Sequential node drop-down | 🌧️ Smooth |
| **BREATH** | Exponential PWM fading curve | 🧬 Biological |
| **WAVE** | RGB Phase-shift color flow | 🌊 Fluid |

---

### 🔌 HARDWARE ARCHITECTURE
Visualisasi jalur kabel untuk **ESP32 30-Pin** dan **LED RGB Common Cathode**.


graph TD
    subgraph ESP32_Controller
    A[GPIO 4,5,13,14,16,17] -->|Data| B(Normal LEDs)
    C[GPIO 25, 26, 27] -->|PWM| D(RGB LED Special)
    end
    
    subgraph LED_Power
    B --> E[GND]
    D --> E
    end

    style A fill:#111,stroke:#3b82f6,stroke-width:2px
    style C fill:#111,stroke:#ef4444,stroke-width:2px

⚠️ PIN CALIBRATION
Untuk menjaga stabilitas sistem saat booting, gunakan pemetaan pin berikut:
 * ✅ Authorized Pins: 4, 5, 13, 14, 16, 17, 25, 26, 27
 * ❌ Restricted Pins: 2, 12 (Dilarang guna untuk menghindari boot-loop)
🚀 DEPLOYMENT PROTOCOL
 * Flash Code: Gunakan file .ino yang sudah dioptimasi untuk ESP32.
 * Uplink: Hubungkan ke WiFi SSID TUATHA_DE_NODE.
 * Authorization: Masuk ke dashboard via captive portal dengan passkey redzNotDev.
<div align="center">
<p><i>Developed by <b>Azuya</b> // Powered by <b>Tuatha Dé</b></i></p>
<code>SYSTEM STATUS: OPERATIONAL [100%]</code>
</div>
