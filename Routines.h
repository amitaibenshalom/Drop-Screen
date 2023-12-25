#ifndef ROUTINES
#define ROUTINES

#include "Consts.h"

void pulse_io(int);
void off_all_valves(int);
void on_all_valves(int);
void cycle_all_valves(int);
void valve_on(int);
void reverse_cycle_all_valves(int);
void array_to_valves(bool[], int);
int read_encoder();
void do_encoder();


#endif
