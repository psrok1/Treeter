#include "threadloop.h"
#include <stdexcept>

Threadloop::Threadloop(): broken(false) { }

Threadloop::~Threadloop()
{
    if(!this->broken)
        throw std::runtime_error("It is required to explicitly stop threadloop before destroying it!");
}

void Threadloop::stop()
{
    this->broken = true;
}
