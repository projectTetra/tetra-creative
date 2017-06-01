#pragma once
#ifndef BEHAVIOR_TIMING_HPP
#define BEHAVIOR_TIMING_HPP

#include <tetra/behavior/Behavior.hpp>

namespace tetra
{

/**
 * This behavior merely runs for the duration and blocks all following behaviors
 * until it completes.
 */
class Delay : public Behavior
{
public:
    /**
     * Create a delay behavior.
     */
    Delay(double duration);

    /**
     * Run the behavior.
     * If the total time from runs sums to more than duration, then this
     * behavior will complete.
     */
    virtual void run(double dt) override;

    /**
     * Utility for creating an owned delay on the heap.
     */
    static std::unique_ptr<Delay> forSeconds(double duration);
private:
    const double duration;
    double elapsed;
};

} /* namespace tetra */

#endif
