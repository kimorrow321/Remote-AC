# Remote-AC

# ESP8266 IR Blaster AC Control

Proyek ini mengontrol AC menggunakan ESP8266 dan IR Blaster, dengan antarmuka Web serta fitur timer otomatis berdasarkan waktu NTP.

## Fitur
- Kontrol AC: ON/OFF, Suhu Naik/Turun
- Akses via Web Browser
- Timer otomatis ON (06:00) & OFF (22:00)
- Sinkronisasi waktu via NTP (GMT+7)

## Kebutuhan
- ESP8266 (NodeMCU)
- IR LED + resistor
- Arduino IDE
- Library: `IRremoteESP8266`, `NTPClient`, `ESP8266WiFi`, `WiFiUdp`

## Cara Pakai
1. Upload kode ke ESP8266
2. Cek IP di Serial Monitor
3. Akses via:
   - `/power`
   - `/tempup`
   - `/tempdown`

## Catatan
Gunakan kode IR mentah dari remote AC asli untuk mengganti `rawPowerOn`, `rawPowerOff`, dll.
