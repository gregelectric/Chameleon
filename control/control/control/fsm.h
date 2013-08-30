//------------------------------------------------------------------------------
/*!	
   \file Fsm.h
   \brief Include file to simplify finite state machines (FSM) implementation.

   \version rel. 1.1 06
   \date jan 2006
   \author Massimo Manca, Micron Engineering di M. Manca 2006.
   \author www.micronengineering.it
   \author massimo.manca@micronengineering.it
   \author This is completely free software.
*/
//------------------------------------------------------------------------------
#ifndef FsmH
#define FsmH

#define FIRST_STATE              -127
#define FSM_EXITED               0
#define FSM_WAITING              1
#define FSM_ENDED                2

#define BEG_STATES   static enum eFsmState {
#define END_STATES   } eStateCnt=FIRST_STATE; char cSuspend=1;
#define FSM_START    switch(eStateCnt) { 
#define END_FSM      }

#define FSM_STATES               static enum eFsmState {
#define FSM_BEGIN                END_STATES FSM_START 
#define FSM_DECLARE(name_args)   int name_args
#define FSM_END                  } return(FSM_ENDED);
#define FSM_SET(s)               s = (int)__LINE__; case __LINE__:
#define FSM_STATE(a)             case a :
#define FSM_STATE_END            break;
#define FSM_STATE_ELSE           default :


#define FsmAbort()               \
   do {                          \
      eStateCnt = FSM_EXITED;    \
      return(FSM_EXITED);        \
   } while(0)

#define FsmExit()                \
   do {                          \
      eStateCnt = FIRST_STATE;   \
      return(FSM_EXITED);        \
   } while(0)
   
#define FsmChangeState(cond, nextstate)      if(cond) eStateCnt=nextstate
#define FsmTransition(cond, exec, nextstate) \
   do {                                      \
      if(cond) {                             \
         exec;                               \
         eStateCnt=nextstate;                \
         break;                              \
      }                                      \
   } while(0)
#define FsmNextState(nextstate)           eStateCnt=nextstate
#define FsmSuccState()                    eStateCnt=eStateCnt + 1
#define FsmPrevState()                    eStateCnt=eStateCnt - 1
#define FsmRestart()                      eStateCnt=FIRST_STATE
#define FsmReturn()                       return((int)eStateCnt)
#define FsmRun(f) (f != FSM_EXITED)

#define FsmRunUpToExit(f)        \
   do {                          \
         FSM_SET(eStateCnt);		\
   } while(FsmRun(f))

#define FsmRunUntil(f, cond)     \
   do {                          \
         FSM_SET(eStateCnt);		\
   } while(FsmRun(f))
   
#define FsmSuspend()             \
   do {                          \
      cSuspend = 0;              \
      FsmWaitUntil(cSuspend);    \
   } while(0)

#define FsmSuspendUntil(cond)    \
   do {                          \
      cSuspend = 0;              \
      FsmWaitUntil(cSuspend || !(cond));    \
      cSuspend = 1;              \
   } while(0)

#define FsmWaitUntil(condition)  \
  do {				               \
    FSM_SET(eStateCnt);		      \
    if(!(condition)) 		      \
      return(FSM_WAITING);	      \
  } while(0)

#define FsmTimeoutWaitUntil(condition, timeout, nextstate)  \
  do {				               \
    FSM_SET(eStateCnt);		      \
    if(timeout)                  \
    {                            \
       eStateCnt=nextstate;      \
       return(FSM_WAITING);      \
    }                            \
    if(!(condition)) 		      \
      return(FSM_WAITING);	      \
  } while(0)

#define FsmWaitWhile(condition)  FsmWaitUntil(!(condition))
#define FsmTimeoutWaitWhile(condition)  FsmTimeoutWaitUntil(!(condition))
#define FsmWaitFsmExit(fsm)      while(FsmRun(fsm))

#define FsmDoWaitUntil(exec, condition)  \
  do {				               \
    FSM_SET(eStateCnt);		      \
    exec;                        \
    if(!(condition)) 		      \
      return(FSM_WAITING);       \
  } while(0)

#define FsmDoWaitWhile(exec, condition) FsmDoWaitUntil(!(condition))

//---------------------------------------------------------------------------
#endif

