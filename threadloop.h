#ifndef THREADLOOP_H
#define THREADLOOP_H

#include <atomic>
#include <thread>

class Threadloop
{
    std::atomic<bool> broken;
    std::thread worker;
public:
    // Copying isn't allowed
    Threadloop(const Threadloop&) = delete;
    Threadloop& operator=(const Threadloop&) = delete;

    Threadloop();
    virtual ~Threadloop();

    virtual void operator()() = 0;
    virtual void start();
    virtual void join();
    virtual void stop();
};

#endif // THREADLOOP_H
