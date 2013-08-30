#ifndef GUARD_DEBOUNCE_H
#define GUARD_DEBOUNCE_H

typedef unsigned short debounce_t;

extern debounce_t debounced_state;	// only read, never write

debounce_t debounce(debounce_t new_sample);

#endif
