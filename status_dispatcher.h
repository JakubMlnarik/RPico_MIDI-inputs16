#ifndef STATUS_DISPATCHER_H
#define STATUS_DISPATCHER_H

#include "definitions.h"
#include "utils.h"

extern void status_dispatcher(unsigned char *byte, critical_section_t *cs, queue_t *buff);

#endif