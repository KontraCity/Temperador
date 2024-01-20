#pragma once

// ESP drivers
#include <driver/i2c.h>
#include <driver/ledc.h>

namespace kc {

namespace Const
{
    constexpr i2c_port_t ExternalPort = I2C_NUM_0;
    constexpr int ExternalSCLPin = 26;
    constexpr int ExternalSDAPin = 25;

    constexpr i2c_port_t InternalPort = I2C_NUM_1;
    constexpr int InternalSCLPin = 33;
    constexpr int InternalSDAPin = 32;

    constexpr ledc_channel_t GreenChannel = LEDC_CHANNEL_0;
    constexpr int GreenLedPin = 12;
    constexpr ledc_channel_t BlueChannel = LEDC_CHANNEL_1;
    constexpr int BlueLedPin = 27;
}

} // namespace kc
