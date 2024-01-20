#pragma once

// STL modules
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

// ESP drivers
#include <driver/ledc.h>

// Custom modules
#include "const.hpp"
#include "utility.hpp"

namespace kc {

class Led
{
public:
    struct Color
    {
        uint8_t green = 0.0;
        uint8_t blue = 0.0;
    };

    enum class ThreadStatus
    {
        Idle,
        Running,
        Stopped,
    };

    struct Settings
    {
        enum class Type
        {
            Glow,
            Blink,
        };

        Type type;
        Color color;
        double transitionDuration;
        double glowDuration;
    };

public:
    // Singleton instance
    static const std::unique_ptr<Led> Instance;

private:
    std::mutex m_mutex;
    std::thread m_thread;
    ThreadStatus m_threadStatus;

private:
    /// @brief Initialize LED controller
    Led();

    /// @brief Set LEDC PWM duty
    /// @param channel LEDC channel where to set duty
    /// @param duty The duty to set
    void setDuty(ledc_channel_t channel, int duty);

    /// @brief Set LED color
    /// @param color Color to set
    void setColor(Color color);

    /// @brief Thread implementation
    /// @param settings Thread settings
    void threadFunction(Settings settings);

    /// @brief Stop thread
    inline void stopThread()
    {
        {
            std::lock_guard lock(m_mutex);
            if (m_threadStatus == ThreadStatus::Running)
                m_threadStatus = ThreadStatus::Stopped; 
        }
        
        if (m_thread.joinable())
            m_thread.join();
    }

public:
    ~Led();

    /// @brief Make LED glow
    /// @param color Color to glow
    /// @param glowDuration LED glow duration in seconds
    /// @param transitionDuration Duration of state transition in seconds
    /// @param shouldntBlock Whether or not should return as fast as possible
    void glow(Color color, double glowDuration, double transitionDuration = 0.3, bool shouldntBlock = false);

    /// @brief Make LED blink
    /// @param color Color to blink
    /// @param transitionDuration Duration of state transition in seconds
    void blink(Color color, double transitionDuration = 0.3);
};

} // namespace kc
