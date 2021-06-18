#include <stdio.h>
#include "fsm.h"

#define event1 1
#define event2 2
#define event3 3
#define event4 4

#define state1 1
#define state2 2
#define state3 3
#define state4 4
#define state5 5


void state1_act(void * event)
{
    printf("State 1!  event = %d \n",*(int *)event);

}
void state2_act(void * event)
{
    printf("State 2!  event = %d \n",*(int *)event);

}
void state3_act(void * event)
{
    printf("State 3!  event = %d \n",*(int *)event);

}
void state4_act(void * event)
{
    printf("State 4!  event = %d \n",*(int *)event);

}
void state5_act(void * event)
{
    printf("State 5!  event = %d \n",*(int *)event);

}

fsm_t task_sm;
fsmtab_t task_sm_tab[]= {
        {event1,state1,state1,state1_act,},
        {event2,state1,state2,state2_act,},
        {event3,state2,state3,state3_act,},
        {event4,state3,state4,state4_act,},
        {event2,state4,state5,state5_act,},
        {event1,state5,state1,state1_act,},
};
int event_tab[] = {event1,event2,event3,event4,event2,event1};


int main() {
    fsm_init(&task_sm,task_sm_tab,6,state1);
    for (int i = 0; i < 6; ++i) {
        fsm_event_handle(&task_sm, event_tab[i],(void *)&event_tab[i]);
    }
    return 0;
}
