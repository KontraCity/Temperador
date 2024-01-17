#pragma once

// ESP drivers
#include <driver/i2c.h>

namespace kc {

namespace Const
{
    constexpr i2c_port_t ExternalSensorPort = I2C_NUM_0;
    constexpr int ExternalSensorSCLPin = 22;
    constexpr int ExternalSensorSDAPin = 21;

    constexpr i2c_port_t InternalSensorPort = I2C_NUM_1;
    constexpr int InternalSensorSCLPin = 25;
    constexpr int InternalSensorSDAPin = 26;
}

} // namespace kc
