#ifndef THREADLOOP_H
#define THREADLOOP_H

#include <atomic>
#include <thread>
#include <memory>
#include <functional>

template<class DerivedClass>
class Threadloop
{
public:
    typedef std::shared_ptr<DerivedClass> Reference;
private:
    std::thread worker;
public:
    Threadloop() {}
    virtual ~Threadloop() {}
    virtual void operator()(Reference) = 0;

    // Copying isn't allowed
    Threadloop(const Threadloop&) = delete;
    Threadloop& operator=(const Threadloop&) = delete;

    std::thread& createThread(Reference ref) {
        worker = std::thread(std::bind(&DerivedClass::operator(), static_cast<DerivedClass*>(this), std::placeholders::_1), ref);
        return worker;
    }

    void detachThread() {
        if(this->worker.joinable())
            this->worker.detach();
    }

    void joinThread() {
        if(this->worker.joinable())
            this->worker.join();
    }

    virtual void stopThread() = 0;
};

#endif // THREADLOOP_H
