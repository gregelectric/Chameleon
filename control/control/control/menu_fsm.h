#ifndef MENU_FSM_H
#define MENU_FSM_H

#include "fsm.h"
#include "btn_events.h"
#include "switches.h"

typedef enum {
// FSM_STATES
	STATE_SOLID=FIRST_STATE,
	STATE_PULSE,
	STATE_SPECTRUM,
	STATE_FLICKER,
//END_STATES
} eFsmState_t;

int MenuFsm(btn_events_t *Events, eFsmState_t *result);

#endif
