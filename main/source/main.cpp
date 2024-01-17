// STL modules
#include <iostream>
#include <cmath>

// Custom modules
#include "const.hpp"
#include "i2c.hpp"
#include "utility.hpp"
using namespace kc;

extern "C" void app_main(void)
{
    /*
    *   AHT20 measurement test code
    */

    // Initialize sensor
    I2C::Master master(Const::ExternalSensorPort, Const::ExternalSensorSCLPin, Const::ExternalSensorSDAPin);
    master.send(0x38, { 0xBE, 0x08, 0x00 });
    Utility::Sleep(0.01);

    // Send measurement command
    master.send(0x38, { 0xAC, 0x33, 0x00 });
    Utility::Sleep(0.08);

    // Receive and decode measurement
    std::vector<uint8_t> response = master.receive(0x38, 7);
    std::cout << static_cast<float>((((response[3] & 0b0000'1111) << 16) | (response[4] << 8) | response[5]) / std::pow(2, 20) * 200 - 50) << "'C\n";
    std::cout << static_cast<float>(((response[1] << 12) | (response[2] << 4) | ((response[3] & 0b1111'0000) >> 4)) / std::pow(2, 20) * 100) << "%\n";
}
