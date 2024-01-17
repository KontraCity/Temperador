#pragma once

// STL modules
#include <vector>

// ESP drivers
#include <driver/i2c.h>

namespace kc {

namespace I2C
{
    class Master
    {
    private:
        i2c_port_t m_port;

    public:
        /// @brief Initialize I2C master
        /// @param port ESP32 I2C port to use
        /// @param sclPin I2C bus SCL pin
        /// @param sdaPin I2C bus SDA pin
        Master(i2c_port_t port, int sclPin, int sdaPin);

        ~Master();

        /// @brief Send data to I2C bus
        /// @param address Address of device that data is sent for
        /// @param data Data to send
        void send(uint8_t address, const std::vector<uint8_t> data);

        /// @brief Receive data from I2C bus
        /// @param address Address of device to receive from
        /// @param length Length of data to receive
        /// @return Received data
        std::vector<uint8_t> receive(uint8_t address, int length);
    };
}

} // namespace kc
