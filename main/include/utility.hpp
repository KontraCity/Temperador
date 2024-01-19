#pragma once

// STL modules
#include <cmath>

// FreeRTOS modules
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// ROM modules
#include <rom/ets_sys.h>

namespace kc {

namespace Utility
{
    /// @brief Delay execution for some time
    /// @param seconds Amount of seconds to delay for
    void Sleep(double seconds);

    /// @brief Round value to decimal places
    /// @param value Value to round
    /// @param decimalPlaces Decimal places to round to
    /// @return Rounded value
    double Round(double value, int decimalPlaces);
}

} // namespace kc
