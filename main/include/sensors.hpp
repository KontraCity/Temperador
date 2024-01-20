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
    /// @param port Sensors port (where to measure)
    /// @return Sensors measurement
    Measurement Measure(i2c_port_t port);
}

} // namespace kc
