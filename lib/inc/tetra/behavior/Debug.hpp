#pragma once
#ifndef BEHAVIOR_DEBUG_HPP
#define BEHAVIOR_DEBUG_HPP

#include <tetra/behavior/Behavior.hpp>

namespace tetra
{

/**
 * This behavior simply dumps a message to stdout and completes without blocking.
 */
class Echo : public Behavior
{
public:
    /**
     * Create an echo behavior.
     */
    Echo(const std::string& msg);

    /**
     * Print the message and complete.
     */
    virtual void run(double dt) override;

    /**
     * This utility method creates an owned Echo on the heap.
     */
    static std::unique_ptr<Echo> message(const std::string& msg);
private:
    const std::string msg;
};

}; /* namespace tetra */

#endif
