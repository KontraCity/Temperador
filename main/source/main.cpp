// STL modules
#include <iostream>

// Custom modules
#include "led.hpp"
#include "utility.hpp"
using namespace kc;

extern "C" void app_main(void)
{
    Led::Instance->blink({ 0, 255 }, 0.5);
    Utility::Sleep(5);

    std::cout << "Stopping!\n";
    Led::Instance->glow({ 255, 0 }, 2);
}
