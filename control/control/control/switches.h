#ifndef GUARD_SWITCHES_H
#define GUARD_SWITCHES_H

// switches.h
#include "debounce.h"
#include "btn_events.h"

// switch masks - regardless of the hardware, a 1 indicates pressed and 0 indicates not pressed
#define SWITCH_DEF_STATES (0)
#define SW_PWR  (1 << 0)
#define SW_DN   (1 << 1)
#define SW_ENT  (1 << 2)
#define SW_UP   (1 << 3)
#define SW_KILL (1 << 4)
#define SW_DIST (1 << 5)
#define SW_USB  (1 << 6)
#define SW_ALL	(SW_PWR | SW_DN | SW_UP | SW_ENT)

void Switch_status(btn_events_t *Events);

#endif
