
#include <stdexcept>
#include <thread>
#include <chrono>

#include "TimedDoor.h"

DoorTimerAdapter::DoorTimerAdapter(TimedDoor & door_) : door(door_) {}

void DoorTimerAdapter::Timeout()
{
    door.throwState();
}

TimedDoor::TimedDoor(int timeout_) : iTimeout(timeout_), isOpened(false)
{
    adapter = new DoorTimerAdapter(*this);
}

bool TimedDoor::isDoorOpened()
{
    return isOpened;
}

void TimedDoor::unlock()
{
    isOpened = true;
    Timer timer;
    timer.tregister(getTimeOut(), adapter);
}

void TimedDoor::lock()
{
    isOpened = false;
}

int TimedDoor::getTimeOut() const
{
    return iTimeout;
}

void TimedDoor::throwState()
{
   if (isOpened) throw std::runtime_error("Time's up!");
   else throw std::runtime_error("Door is closed!");
}

void Timer::sleep(int time_)
{
    std::this_thread::sleep_for(std::chrono::seconds(time_));
}

void Timer::tregister(int time_, TimerClient* client_)
{
    sleep(time_);
    client_->Timeout();
}
