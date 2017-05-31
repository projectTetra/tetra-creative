#pragma once
#ifndef TIC_TOC_CLOCK_HPP
#define TIC_TOC_CLOCK_HPP

#include <chrono>
#include <thread>

namespace tetra
{

constexpr int SIXTY_HZ_MILLIS = 16;

/**
 * This class provides a simple interface for tracking frame time using a clock.
 * TimerStrategy must be a type which implements a static now() method which returns
 * a std::chrono::duration.
 *
 * EXAMPLE:
 *      auto timer = TicTocClock<std::chrono::high_resolution_clock>{};
 *
 *      timer.tic();
 *      ...
 *        do some work
 *        or something
 *      ...
 *      double time = timer.toc();
 * OR:
 *      auto timer = TicTocClock<std::chrono::high_resolution_clock>{};
 *
 *      timer.tic();
 *      timer.waitIfShorterThan(100);
 *      double time = timer.toc(); // time ~= 0.1
 */
template <class TimerStrategy>
class TicTocClock
{
public:
    using TickType = decltype(TimerStrategy::now());

    /**
     * Create a TickTocClock.
     */
    TicTocClock()
    {
        this->lastTick = TimerStrategy::now();
    }

    /**
     * Get the duration, in seconds, from the last tick().
     */
    double toc()
    {
        TickType now = TimerStrategy::now();
        std::chrono::duration<double> duration = now - this->lastTick;

        return duration.count();
    }

    /**
     * Reset the clock's reference tick().
     */
    void tic()
    {
        this->lastTick = TimerStrategy::now();
    }

    /**
     * Sleep this thread if fewer than a given amount of milliseconds have passed
     * since the last tick().
     */
    void waitIfShorterThan(int minDurationMillis = SIXTY_HZ_MILLIS)
    {
        TickType now = TimerStrategy::now();
        auto duration = now - this->lastTick;
        int millisDuration =
            std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        if (millisDuration < minDurationMillis)
        {
            std::this_thread::sleep_for(
                std::chrono::milliseconds{minDurationMillis-millisDuration}
            );
        }
    }

    /**
     * Effectively calls waitIfShorterThan, toc, tic, and returns the value of toc.
     */
    double ticToc(int minDurationMillis = SIXTY_HZ_MILLIS)
    {
        this->waitIfShorterThan(minDurationMillis);
        auto result = this->toc();
        this->tic();
        return result;
    }
private:
    TickType lastTick;
};

using HighResTicToc = TicTocClock<std::chrono::high_resolution_clock>;

}; /* namespace tetra */

#endif
