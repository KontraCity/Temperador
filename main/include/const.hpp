#pragma once

// ESP drivers
#include <driver/i2c.h>

namespace kc {

namespace Const
{
    constexpr i2c_port_t ExternalPort = I2C_NUM_0;
    constexpr int ExternalSCLPin = 26;
    constexpr int ExternalSDAPin = 25;

    constexpr i2c_port_t InternalPort = I2C_NUM_1;
    constexpr int InternalSCLPin = 33;
    constexpr int InternalSDAPin = 32;
}

} // namespace kc
