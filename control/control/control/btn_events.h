#ifndef GUARD_BTN_EVENTS_H
#define GUARD_BTN_EVENTS_H

#include "debounce.h"

typedef debounce_t btn_type_t;

typedef struct {
	btn_type_t press;		// 1 indicates press event
	btn_type_t hold;
	btn_type_t release;
	btn_type_t double_press;	
} btn_events_t;

void btn_getevents(debounce_t *Reads, btn_events_t *btn_events);

#endif
