#include "threadloop.h"
#include <stdexcept>

Threadloop::Threadloop(): broken(true), worker() { }

Threadloop::~Threadloop()
{
    // If stop wasn't called: call it know!
    if(!this->broken)
        this->stop();

    // If destructor was called from the same thread: it need to be detached
    if(this->worker.id == std::this_thread::get_id())
        this->worker.detach();
    // Otherwise: wait for its termination
    else if(this->worker.joinable())
        this->worker.join();
}

void Threadloop::start()
{
    worker = std::thread(std::bind(&Threadloop::operator(), this));
    this->broken = false;
}

void join()
{
    this->worker.join();
}

void Threadloop::stop()
{
    this->broken = true;
}
