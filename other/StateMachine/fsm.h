
#ifndef _FSM_H_
#define _FSM_H_

#include <stdint.h>
#include <stddef.h>

typedef uint32_t  event_t;
typedef uint32_t  state_t;
typedef uint32_t  fsmsz_t;
typedef void *    param_t;
typedef void    (*action_t)(param_t);


typedef struct {
    event_t     event;              /* input event */
    state_t     cstate;             /* current state */
    uint8_t     nstate;             /* next state */
    action_t    action;             /* atction function*/
}fsmtab_t;

typedef struct FSM_s
{
    fsmtab_t    *fsmtable;           /* state transition table */
    fsmsz_t     fsmtab_size;         /* state transition table size*/
    state_t     cstate;              /* current state */
}fsm_t;


void fsm_init(fsm_t *pfsm, fsmtab_t *pfsmtab, fsmsz_t size, state_t current_state);
void fsm_event_handle(fsm_t *pfsm, event_t event, param_t parm);

#endif
