// switches.c
#include "switches.h"
#include "PSoCGPIOINT.h"    // include the GPIO API header file 

void DebounceButton(btn_events_t *pEvents)
{
	debounce_t Reads;				// holds the flags for reading switches
	Reads = SWITCH_DEF_STATES;
/*
	Each statement says:
		if the pin bit of the port is high, we OR Reads with 0 (so do nothing)
		if the pin bit of the port is low, we OR Reads with the pin's mask (to indicate the switch is pressed)
	This is necessary because not all of the inputs are on the same port
*/
	// remember: switches are active low!
	Reads |= ((PRT3DR & PWR_BTN_MASK)  ? 0 : SW_PWR);   

	btn_getevents(&Reads, pEvents);
}


