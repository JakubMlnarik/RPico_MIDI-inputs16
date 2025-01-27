#ifndef STATE_H
#define STATE_H

#include "definitions.h"
#include "utils.h"
#include "settings.h"

extern void init_state(struct State *st, SETTINGS *set);
extern void process_state(struct State *actual, struct State *last, SETTINGS *set, critical_section_t *cs, queue_t *buff, i2c_inst_t *i2c);

#endif