#pragma once

// STL modules
#include <cmath>

// Custom modules
#include "const.hpp"
#include "i2c.hpp"
#include "utility.hpp"

namespace kc {

namespace Sensors
{
    struct Measurement
    {
        double aht20Temperature = 0.0;      // Temperature measured by AHT20 in celsius degrees
        double aht20Humidity = 0.0;         // Relative humidity in percents
        double bmp280Temperature = 0.0;     // Temperature measured by BMP280 in celsius degrees
        double bmp280Pressure = 0.0;        // Air pressure in hectopascales
    };

    /// @brief Peform sensors measurement
    /// @param port Sensors port (where to measure)
    /// @return Sensors measurement
    Measurement Measure(i2c_port_t port);
}

} // namespace kc
