#ifndef UTILS_H
#define UTILS_H

#include "definitions.h"
#include "settings.h"

extern void scan_data_pins(bool *dest, int offset);

extern uint8_t get_pot_position(int adc_num);

extern bool send_pot_midi(int adc_num, int channel, uint8_t value, critical_section_t *cs, queue_t *buff);

extern int get_channel();

extern bool send_note_on(uint8_t channel, uint8_t midi_base, int tone, critical_section_t *cs, queue_t *buff);

extern bool send_note_off(uint8_t channel, uint8_t midi_base, int tone, critical_section_t *cs, queue_t *buff);
#endif