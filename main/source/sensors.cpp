#include "sensors.hpp"

namespace kc {

/// @brief Perform AHT20 sensor measurement
/// @param master I2C master that the sensor is attached to
/// @return Sensor measurement
static Sensors::Aht20Measurement MeasureAht20(I2C::Master& master)
{
    master.send(0x38, { 0xAC, 0x33, 0x00 });
    Utility::Sleep(0.08);

    std::vector<uint8_t> response = master.receive(0x38, 7);
    return {
        Utility::Round((((response[3] & 0b0000'1111) << 16) | (response[4] << 8) | response[5]) / std::pow(2, 20) * 200 - 50, 1),
        Utility::Round(((response[1] << 12) | (response[2] << 4) | ((response[3] & 0b1111'0000) >> 4)) / std::pow(2, 20) * 100, 1)
    };
}

/// @brief Perform BMP280 sensor measurement
/// @param master I2C master that the sensor is attached to
/// @return Sensor measurement
static Sensors::Bmp280Measurement MeasureBmp280(I2C::Master& master)
{
    master.send(0x77, { 0xF4, 0b111'010'01 });
    Utility::Sleep(0.05);

    uint16_t calibrationValue1 = master.receiveValue<uint16_t>(0x77, 0x88, true);
    int16_t calibrationValue2 = master.receiveValue<int16_t>(0x77, 0x8A, true);
    int16_t calibrationValue3 = master.receiveValue<int16_t>(0x77, 0x8C, true);
    int32_t rawTemperature = master.receiveValue<int32_t>(0x77, 0xFA, false, 3) >> 4;

    double var1 = (rawTemperature / 16384.0 - calibrationValue1 / 1024.0) * calibrationValue2;
    double var2 = (rawTemperature / 131072.0 - calibrationValue1 / 8192.0) * (rawTemperature / 131072.0 - calibrationValue1 / 8192.0) * calibrationValue3;
    double fineTemperature = var1 + var2;

    calibrationValue1 = master.receiveValue<uint16_t>(0x77, 0x8E, true);
    calibrationValue2 = master.receiveValue<int16_t>(0x77, 0x90, true);
    calibrationValue3 = master.receiveValue<int16_t>(0x77, 0x92, true);
    int16_t calibrationValue4 = master.receiveValue<int16_t>(0x77, 0x94, true);
    int16_t calibrationValue5 = master.receiveValue<int16_t>(0x77, 0x96, true);
    int16_t calibrationValue6 = master.receiveValue<int16_t>(0x77, 0x98, true);
    int16_t calibrationValue7 = master.receiveValue<int16_t>(0x77, 0x9A, true);
    int16_t calibrationValue8 = master.receiveValue<int16_t>(0x77, 0x9C, true);
    int16_t calibrationValue9 = master.receiveValue<int16_t>(0x77, 0x9E, true);
    int32_t rawPressure = master.receiveValue<int32_t>(0x77, 0xF7, false, 3) >> 4;

    var1 = fineTemperature / 2.0 - 64000.0;
    var2 = var1 * var1 * calibrationValue6 / 32768.0;
    var2 = var2 + var1 * calibrationValue5 * 2.0;
    var2 = (var2 / 4.0) + (calibrationValue4 * 65536.0);
    var1 = (calibrationValue3 * var1 * var1 / 524288.0 + calibrationValue2 * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * calibrationValue1;
    double pressure = 1048576.0 - rawPressure;
    pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = calibrationValue9 * pressure * pressure / 2147483648.0;
    var2 = pressure * calibrationValue8 / 32768.0;
    pressure = pressure + (var1 + var2 + calibrationValue7) / 16.0;

    return { Utility::Round(fineTemperature / 5120.0, 1), Utility::Round(pressure / 100.0, 1) };
}

Sensors::Measurement Sensors::Measure(i2c_port_t port)
{
    static I2C::Master externalMaster(ExternalPort, CONFIG_I2C_EXTERNAL_PORT_SCL_PIN, CONFIG_I2C_EXTERNAL_PORT_SDA_PIN);
    static I2C::Master internalMaster(InternalPort, CONFIG_I2C_INTERNAL_PORT_SCL_PIN, CONFIG_I2C_INTERNAL_PORT_SDA_PIN);
    static bool initialized = false;
    if (!initialized)
    {
        // AHT20 initialization
        externalMaster.send(0x38, { 0xBE, 0x08, 0x00 });
        internalMaster.send(0x38, { 0xBE, 0x08, 0x00 });
        Utility::Sleep(0.01);

        // BMP280 initialization
        externalMaster.send(0x77, { 0xB6 });
        internalMaster.send(0x77, { 0xB6 });
        Utility::Sleep(0.002);

        initialized = true;
    }

    I2C::Master& master = (port == ExternalPort ? externalMaster : internalMaster);
    return { MeasureAht20(master), MeasureBmp280(master) };
}

} // namespace kc
