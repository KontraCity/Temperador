#pragma once

// STL modules
#include <cmath>

// Custom modules
#include "i2c.hpp"
#include "utility.hpp"

namespace kc {

namespace Sensors
{
    constexpr i2c_port_t ExternalPort = I2C_NUM_0;
    constexpr i2c_port_t InternalPort = I2C_NUM_1;

    struct Aht20Measurement
    {
        double temperature = 0.0;   // Temperature in celsius degrees
        double humidity = 0.0;      // Relative humidity in percents
    };

    struct Bmp280Measurement
    {
        double temperature = 0.0;   // Temperature in celsius degrees
        double pressure = 0.0;      // Air pressure in hectopascales
    };

    struct Measurement
    {
        Aht20Measurement aht20;
        Bmp280Measurement bmp280;
    };

    /// @brief Peform sensors measurement
    /// @param port I2C port in which to perform measurement
    /// @return Sensors measurement
    Measurement Measure(i2c_port_t port);
}

} // namespace kc
