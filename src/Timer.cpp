#include <Q-Tip/Timer.h>
#include <SDL3/SDL.h>

QTIP_CODE_BEGIN

Timer::Timer(uint64_t d) {
    start = SDL_GetTicks();
    delay = d;
}

uint64_t Timer::now() {
    return 0;
}

QTIP_CODE_END
