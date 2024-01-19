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

        /// @brief Receive value from I2C bus
        /// @tparam Value Type of value to receive
        /// @param address Address of device to receive from
        /// @param firstRegister First value register
        /// @param reverse Whether or not to receive bytes in reverse order
        /// @param lengthLimit Value length limit
        /// @return Received value
        template <typename Value>
        inline Value receiveValue(uint8_t address, uint8_t firstRegister, bool reverse = false, int lengthLimit = -1)
        {
            send(address, { firstRegister });
            std::vector<uint8_t> response = receive(address, (lengthLimit == -1 ? sizeof(Value) : lengthLimit));

            Value value = 0;
            if (reverse)
            {
                for (int index = 0, offset = 0; index < response.size(); ++index, offset += 8)
                    value |= response[index] << offset;
                return value;
            }

            for (int index = response.size() - 1, offset = 0; index >= 0; --index, offset += 8)
                value |= response[index] << offset;
            return value;
        }
    };
}

} // namespace kc
