#ifndef LOCK_H
#define LOCK_H
#include <stdio.h>

/*States for the combination lock state machine*/
typedef enum { LOCKED, GOT1, GOT2, OPEN, N_STATES } state_t; 

/*Input events that trigger state transtitions*/
typedef enum { E_RIGHT, E_WRONG, E_CLOSE, E_TIMEOUT, N_EVENTS } event_t; 

/*Output actions produced by the state stranstitions*/
typedef enum { A_UNDEFINED, A_NONE, A_UNLOCK, A_LOCK, A_BEEP, N_ACTIONS } action_t; 

/*Example of a single state transtition table*/
typedef struct { state_t next; action_t action; } transition_t; 

/*Global state transition matrix mapping*/
extern const transition_t lock_table[N_STATES][N_EVENTS];

/*Executes a single state transtition lookup based on the current event and state*/
state_t lock_step(state_t s, event_t e, action_t *action);

/*Maps an input key character to its state machine event*/
int lock_event(state_t s, char c);

/*Returns a state enum value*/
const char *state_name(state_t s); 

#endif /*LOCK_H*/