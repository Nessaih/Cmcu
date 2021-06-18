/**
 *******************************************************************************
 * @file       sources\app\iuc\iuc_dev.c
 * @brief      description
 * @record
 * Change Logs:
 * Date             Author          Notes
 * 2021/06/17       vic             First version
 * method of application：
 * (1)Define a finite state machine object
 * (2)Define and initialize the state transition table
 * (3)Define an event input table
 * (4)Call the 'fsm_init' function to initialize.
 * (5)Call the 'fsm_event_handle' function to run the state machine.
 *******************************************************************************
 */


#include "fsm.h"


/**
 *@brief   finite-state machine dispose
 *@param   pfsm             finite-state machine object
 *@param   event            input event
 *@retval  none
 *@date    2021/06/17
 */
void fsm_event_handle(fsm_t *pfsm, event_t event, param_t parm)
{
    fsmtab_t *pfsmtab = pfsm->fsmtable;
    action_t pfunction = NULL;
    state_t current_state, next_state;
    uint8_t flag = 0;

    current_state = pfsm->cstate;
    for (uint8_t i = 0; i < pfsm->fsmtab_size; i++)
    {
        if (event == pfsmtab[i].event && current_state == pfsmtab[i].cstate)
        {
            flag = 1;
            pfunction = pfsmtab[i].action;
            next_state = pfsmtab[i].nstate;
            break;
        }
    }
    if (flag)
    {
        if (pfunction != NULL)
        {
            pfunction(parm); 
        }
        pfsm->cstate = next_state;
    }
    else
    {
        // do nothing
    }
}


/**
 *@brief   finite-state machine initialization
 *@param   pfsm             finite-state machine object
 *@param   pfsmtab          state transition table
 *@param   size             state transition table size
 *@param   size             current state
 *@retval  none
 *@date    2021/06/17
 */
void fsm_init(fsm_t *pfsm, fsmtab_t *pfsmtab, fsmsz_t size, state_t current_state)
{
    pfsm->fsmtable = pfsmtab;
    pfsm->cstate = current_state;
    pfsm->fsmtab_size = size;
}
