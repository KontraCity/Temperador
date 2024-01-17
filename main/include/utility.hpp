#pragma once

// FreeRTOS modules
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace kc {

namespace Utility
{
    /// @brief Delay execution for some time
    /// @param seconds Amount of seconds to delay for
    void Sleep(double seconds);
}

} // namespace kc
