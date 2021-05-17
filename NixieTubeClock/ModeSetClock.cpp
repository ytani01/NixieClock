/**
 * (c) Yoichi Tanibayashi
 */
#include "ModeSetClock.h"

/**
 *
 */
ModeSetClock::ModeSetClock(NixieArray *nxa): ModeBase::ModeBase(nxa,
                                                                "SetClock",
                                                                ModeSetClock::TICK_MS) {
} // ModeSetClock::ModeSetClock()

