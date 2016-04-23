#ifndef THREADLOOP_H
#define THREADLOOP_H

#include <atomic>

class Threadloop
{
    std::atomic<bool> broken;
public:
    // Copying isn't allowed
    Threadloop(const Threadloop&) = delete;
    Threadloop& operator=(const Threadloop&) = delete;

    Threadloop();
    virtual ~Threadloop();

    virtual void operator()() = 0;
    virtual void stop();
};

#endif // THREADLOOP_H
