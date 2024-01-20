#include "utility.hpp"

namespace kc {

void Utility::Sleep(double seconds)
{
    if (seconds <= 0)
        return;

    /*
    *   One FreeRTOS tick is 10ms, so 1 second is 100 ticks.
    *   Delays shorter than 1 tick should just burn CPU cycles.
    */
    if (seconds < 0.01)
        ets_delay_us(seconds * 1'000'000);
    vTaskDelay(seconds * 100);
}

double Utility::Round(double value, int decimalPlaces)
{
    double multiplier = std::pow(10, decimalPlaces);
    return std::round(value * multiplier) / multiplier;
}

} // namespace kc
