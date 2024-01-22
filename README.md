# Temperador
Temperador is an ESP32 HTTP air measurement server.

### Sensors
Temperador supports AHT20 and BMP280 sensors.
That means that it can measure air temperature, humidity and pressure.
Two ports are implemented. This allows sensors placement in different places (e. g. outside and inside house).

### Configuration
Before Temperador may be compiled, WiFi and pin configuration must be done with `idf.py menuconfig`.

### Measurement
When HTTP request to http://temperador.ip/external (or http://temperador.ip/internal) is made, Temperador performs measurement and returns lightweight JSON response.
Example of Temperador response:
```json
{
    "aht20": {
        "temperature": -5.4,
        "humidity": 68.4
    },
    "bmp280": {
        "temperature": -6.2,
        "pressure": 1012
    }
}
```
