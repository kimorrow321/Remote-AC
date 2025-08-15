# Remote-AC

# ESP8266 IR Blaster AC Control

Proyek ini mengontrol AC menggunakan ESP8266 dan IR Blaster, dengan antarmuka Web serta fitur timer otomatis berdasarkan waktu NTP.

## Fitur
- Kontrol AC: ON/OFF, Suhu Naik/Turun, set timer jam, menit, hari
- Akses via Web Browser
- Timer otomatis, contoh: ON (17:00) & OFF (09:00)
- Sinkronisasi waktu via NTP (GMT+7)

## Kebutuhan
- ESP8266 (NodeMCU)
- IR LED
- Arduino IDE
- Library: `ESP8266WebServer`, `IRsend`, `WiFiClientSecure`,`ESP8266HTTPClient`, `NTPClient`, `ESP8266WiFi`, `WiFiUdp`, `Timelib`, `ArduinoJson`, `EEPROM`

## Cara Pakai
1. Upload kode ke ESP8266
2. Cek IP di Serial Monitor
3. Akses via:
   - `/power`
   - `/tempup`
   - `/tempdown`

## Catatan
Gunakan kode IR mentah dari remote AC asli untuk mengganti `rawPowerOn`, `rawPowerOff`, dll.
