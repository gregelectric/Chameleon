#include "menu_fsm.h"

FSM_DECLARE(MenuFsm(btn_events_t *Events, eFsmState_t *result))
{
	eFsmState_t eStateCnt=(*result);
	char cSuspend=1;

	FSM_START

		FSM_STATE(STATE_SOLID)
			FsmChangeState((Events->double_press & SW_PWR), STATE_PULSE);
		FSM_STATE_END

		FSM_STATE(STATE_PULSE)
			FsmChangeState((Events->double_press & SW_PWR), STATE_SOLID);
		FSM_STATE_END

		FSM_STATE(STATE_SPECTRUM)
			FsmChangeState((Events->double_press & SW_PWR), STATE_SOLID);
		FSM_STATE_END

		FSM_STATE(STATE_FLICKER)
			FsmChangeState((Events->double_press & SW_PWR), STATE_SOLID);
		FSM_STATE_END
		
//	FSM_END	<--- FSM_END is defined as } return(FSM_ENDED); which means no further instructions can follow!
	}
	(*result) = eStateCnt;
	return(FSM_ENDED);
}