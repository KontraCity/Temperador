#pragma once

// FreeRTOS modules
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace kc {

namespace Utility
{
    /// @brief Stop execution for some time
    /// @param seconds Amout of seconds to delay for
    void Sleep(double seconds);
}

} // namespace kc
