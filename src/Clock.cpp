#include <Q-Tip/Clock.h>
#include <SDL3/SDL.h>

QTIP_CODE_BEGIN

Clock::Clock() {
    start = SDL_GetTicks();
    lastCall = start;
}

uint64_t Clock::now() const {
    return SDL_GetTicks() - start;
}

uint64_t Clock::elapsedFromLastCall() {
    const uint64_t current = SDL_GetTicks();
    const uint64_t delta = current - lastCall;
    lastCall = current;
    return delta;
}

QTIP_CODE_END
