#include "led.hpp"

namespace kc {

const std::unique_ptr<Led> Led::Instance(new Led);

Led::Led()
{
    ledc_timer_config_t timerConfig = {};
    timerConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    timerConfig.timer_num = LEDC_TIMER_0;
    timerConfig.duty_resolution = LEDC_TIMER_13_BIT;
    timerConfig.freq_hz = 1000;
    timerConfig.clk_cfg = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&timerConfig));

    ledc_channel_config_t greenChannelConfig = {};
    greenChannelConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    greenChannelConfig.channel = GreenChannel;
    greenChannelConfig.timer_sel = LEDC_TIMER_0;
    greenChannelConfig.intr_type = LEDC_INTR_DISABLE;
    greenChannelConfig.gpio_num = CONFIG_STATUS_LED_GREEN_CHANNEL_PIN;
    greenChannelConfig.duty = 0;
    greenChannelConfig.hpoint = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&greenChannelConfig));

    ledc_channel_config_t blueChannelConfig = {};
    blueChannelConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    blueChannelConfig.channel = BlueChannel;
    blueChannelConfig.timer_sel = LEDC_TIMER_0;
    blueChannelConfig.intr_type = LEDC_INTR_DISABLE;
    blueChannelConfig.gpio_num = CONFIG_STATUS_LED_BLUE_CHANNEL_PIN;
    blueChannelConfig.duty = 0;
    blueChannelConfig.hpoint = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&blueChannelConfig));
}

void Led::setDuty(ledc_channel_t channel, int duty)
{
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, channel));
}

void Led::setColor(Color color)
{
    setDuty(GreenChannel, 0b1111111111111 * (color.green / 255.0));
    setDuty(BlueChannel, 0b1111111111111 * (color.blue / 255.0));
}

void Led::threadFunction(Settings settings)
{
    {
        std::lock_guard lock(m_mutex);
        m_threadStatus = ThreadStatus::Running;
    }
    
    while (true)
    {
        double transitionCycles = std::round(settings.transitionDuration * 100);
        double currentGreen = 0.0, currentBlue = 0.0;
        double greenStep = settings.color.green / transitionCycles, blueStep = settings.color.blue / transitionCycles;
        for (int transitionCycle = 0; transitionCycle < transitionCycles; ++transitionCycle)
        {
            setColor({ static_cast<uint8_t>(std::round(currentGreen)), static_cast<uint8_t>(std::round(currentBlue)) });
            currentGreen += greenStep;
            currentBlue += blueStep;
            Utility::Sleep(0.01);
        }
        setColor({ static_cast<uint8_t>(std::round(currentGreen)), static_cast<uint8_t>(std::round(currentBlue)) });

        Utility::Sleep(settings.glowDuration);
        for (int transitionCycle = 0; transitionCycle < transitionCycles; ++transitionCycle)
        {
            setColor({ static_cast<uint8_t>(std::round(currentGreen)), static_cast<uint8_t>(std::round(currentBlue)) });
            currentGreen -= greenStep;
            currentBlue -= blueStep;
            Utility::Sleep(0.01);
        }
        setColor({ static_cast<uint8_t>(std::round(currentGreen)), static_cast<uint8_t>(std::round(currentBlue)) });

        if (settings.type == Settings::Type::Glow)
            break;
        {
            std::lock_guard lock(m_mutex);
            if (m_threadStatus == ThreadStatus::Stopped)
                break;
        }
        Utility::Sleep(settings.glowDuration);
    }

    std::lock_guard lock(m_mutex);
    m_threadStatus = ThreadStatus::Idle;
}

Led::~Led()
{
    stopThread();
}

void Led::glow(Color color, double glowDuration, double transitionDuration, bool shouldntBlock)
{
    if (shouldntBlock)
    {
        std::lock_guard lock(m_mutex);
        if (m_threadStatus == ThreadStatus::Running)
            return;
    }

    stopThread();
    m_thread = std::thread(&Led::threadFunction, this, Settings{ Settings::Type::Glow, color, transitionDuration, glowDuration });
}

void Led::blink(Color color, double transitionDuration)
{
    stopThread();
    m_thread = std::thread(&Led::threadFunction, this, Settings{ Settings::Type::Blink, color, transitionDuration, 0 });
}

} // namespace kc
