#pragma once

#include <chrono>

class Timer {
public:
    Timer()
        : m_start()
        , m_end()
    {}

    void start()
    {
        m_start = std::chrono::high_resolution_clock::now();
	}

    void stop()
    {
        m_end = std::chrono::high_resolution_clock::now();
    }

    double diff_time() const
    {
        auto duration = std::chrono::duration<double, std::milli>( m_end - m_start );
        return duration.count();
    }

private:
  std::chrono::high_resolution_clock::time_point m_start;
  std::chrono::high_resolution_clock::time_point m_end;
};
