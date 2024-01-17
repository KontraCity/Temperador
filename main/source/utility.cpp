#include "utility.hpp"

namespace kc {

void Utility::Sleep(double seconds)
{
    /*
    *   One FreeRTOS tick is 10ms, so 1 second is 100 ticks.
    *   Delays shorter than 1 tick can't be requested.
    */
    if (seconds < 0.01)
        seconds = 0.01;
    vTaskDelay(seconds * 100);
}

} // namespace kc
