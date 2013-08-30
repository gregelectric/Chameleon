// debounce.c
/*
2-bit vertical counters courtesy of:
http://www.dattalo.com/technical/software/pic/debounce.html
With the vertical counter approach, the bits are efficiently stored in memory AND all eight counters are operated on in parallel.
Here eight 2-bit counters are used to count the number of samples that have occurred since the last switch transition was detected.
It's a decrementing counter that terminates when zero is reached. The counting sequence is 3,2,1,0,0,0,....

Limitations are:
only 2-bit counts (0 to 4)
therefore debouce time is, a very coarse, 4x polling loop period (ie. polling every 5ms gives 20ms debounce time)
size of the data type holding the counts must fit in a single register (I think)
*/
#include "debounce.h"

static debounce_t clock_A = 0;
static debounce_t clock_B = 0;
debounce_t debounced_state = 0;

debounce_t debounce(debounce_t new_sample)
{
  debounce_t delta;
  debounce_t changes;

  delta = new_sample ^ debounced_state;   //Find all of the changes

  clock_A ^= clock_B;                     //Increment the counters
  clock_B  = ~clock_B;

  clock_A &= delta;                       //Reset the counters if no changes
  clock_B &= delta;                       //were detected.

  // when the count is at zero, (clock_A & clock_B) will be true indicating that the counter has just terminated or is terminated
  // to find out if we just debounced a switch, we check to see if that counter was already in a debounced state so
  // delta & (clock_A & clock_B)
  // once the bit in new_sample matches the bit in debounced_state, the counters are reset.
  // so this actually debounces a change in debounced_state
  changes = ~(~delta | clock_A | clock_B);

 /*
	press = ~changes & debounced_state;	// before changes are applied to debounced_state
	release = changes & ~debounced_state;
 */
  debounced_state ^= changes;

  /*
	hold = ~press & debounced_state;	// after changes have been applied to debounced_state
  */
  return changes;
}

