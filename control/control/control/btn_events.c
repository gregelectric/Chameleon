#include "btn_events.h"
#include "main.h"

void btn_getevents(debounce_t *Reads, btn_events_t *btn_events)
{
	debounce_t changes;
/*
	Next we debounce what we just read
*/
	changes = debounce(*Reads);
/*
	And then check for changes
*/
	btn_events->press = changes & debounced_state;
	btn_events->hold = debounced_state & ~btn_events->press;
	btn_events->release = changes & ~debounced_state;
	
	// The button release timer is started as soon as the button is released
	// If it has not yet expired and the button is pressend then 
	// a double button press event has occured
	if(g_timer_button_release)
	{
			btn_events->double_press = changes & debounced_state;
	}	
}
