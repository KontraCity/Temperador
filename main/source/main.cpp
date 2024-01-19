// STL modules
#include <iostream>

// Custom modules
#include "sensors.hpp"
using namespace kc;

extern "C" void app_main(void)
{
    Sensors::Measurement measurement = Sensors::Measure(Const::ExternalPort);
    std::cout << "EXTERNAL:\n";
    std::cout << "\tAHT20  TEMP:  " << measurement.aht20Temperature << " 'C  \n";
    std::cout << "\tAHT20  HUMID: " << measurement.aht20Humidity << " %  \n";
    std::cout << "\tBMP280 TEMP:  " << measurement.bmp280Temperature << " 'C  \n";
    std::cout << "\tBMP280 PRESS: " << measurement.bmp280Pressure << " hPa  \n";

    measurement = Sensors::Measure(Const::InternalPort);
    std::cout << "INTERNAL:\n";
    std::cout << "\tAHT20  TEMP:  " << measurement.aht20Temperature << " 'C  \n";
    std::cout << "\tAHT20  HUMID: " << measurement.aht20Humidity << " %  \n";
    std::cout << "\tBMP280 TEMP:  " << measurement.bmp280Temperature << " 'C  \n";
    std::cout << "\tBMP280 PRESS: " << measurement.bmp280Pressure << " hPa  \n";
}
