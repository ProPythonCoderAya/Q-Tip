#ifndef QTIP_TIMER_H
#define QTIP_TIMER_H
#include <Q-Tip/Config.h>
#include <cstdint>

QTIP_CODE_BEGIN

class Timer {
public:
    Timer(uint64_t delay);


    uint64_t delay;
private:
    uint64_t start;

    uint64_t now();
};

QTIP_CODE_END

#endif // QTIP_TIMER_H
