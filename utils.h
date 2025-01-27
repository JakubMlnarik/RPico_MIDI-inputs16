#ifndef UTILS_H
#define UTILS_H

#include "definitions.h"
#include "settings.h"

extern int init_mcp(i2c_inst_t *i2c, const uint addr);

extern int reg_read(  i2c_inst_t *i2c,
                const uint addr,
                const uint8_t reg,
                uint8_t *byte);

extern int get_channel();

extern bool send_note_on(uint8_t channel, uint8_t midi_base, int tone, critical_section_t *cs, queue_t *buff);

extern bool send_note_off(uint8_t channel, uint8_t midi_base, int tone, critical_section_t *cs, queue_t *buff);
#endif