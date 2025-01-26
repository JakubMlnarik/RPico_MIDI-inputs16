#include "utils.h"

uint8_t ma_pot0(uint8_t val)
{
    static double last_val;
    double calculated = POT_MA_FACTOR*val + (1-POT_MA_FACTOR)*last_val;
    last_val = calculated;
    if (calculated < 0) return (uint8_t)0;
    else if (calculated > 127) return (uint8_t)127;
    else return (uint8_t)calculated;
}
uint8_t ma_pot1(uint8_t val)
{
    static double last_val;
    double calculated = POT_MA_FACTOR*val + (1-POT_MA_FACTOR)*last_val;
    last_val = calculated;
    if (calculated < 0) return (uint8_t)0;
    else if (calculated > 127) return (uint8_t)127;
    else return (uint8_t)calculated;
}
uint8_t ma_pot2(uint8_t val)
{
    static double last_val;
    double calculated = POT_MA_FACTOR*val + (1-POT_MA_FACTOR)*last_val;
    last_val = calculated;
    if (calculated < 0) return (uint8_t)0;
    else if (calculated > 127) return (uint8_t)127;
    else return (uint8_t)calculated;
}

bool send_midi_msg(uint8_t *data, int no_bytes, critical_section_t *cs, queue_t *buff) {
    bool retval = true;
    critical_section_enter_blocking(cs);
    for (int i=0; i<no_bytes; i++) {
        retval = queue_try_add(buff, (data+i));
    }
    critical_section_exit(cs);

    return retval;
}

uint8_t get_pot_position(int adc_num) {
    adc_select_input(adc_num); 
    int pos = (int)(adc_read()*128.0f/4096.0f);

    if (adc_num == 0) return ma_pot0(pos);
    else if (adc_num == 1) return ma_pot1(pos);
    else if (adc_num == 2) return ma_pot2(pos);
}

void scan_data_pins(bool *dest, int offset) {
    *(dest + offset) = gpio_get(D0);
    *(dest + offset + 1) = gpio_get(D1);
    *(dest + offset + 2) = gpio_get(D2);
    *(dest + offset + 3) = gpio_get(D3);
    *(dest + offset + 4) = gpio_get(D4);
    *(dest + offset + 5) = gpio_get(D5);
    *(dest + offset + 6) = gpio_get(D6);
    *(dest + offset + 7) = gpio_get(D7);
}

bool send_pot_midi(int adc_num, int channel, uint8_t value, critical_section_t *cs, queue_t *buff) {
    uint8_t msg[3];
    msg[0] = 0xB0 | channel; // CC message
    msg[1] = adc_num;
    msg[2] = value;

    return send_midi_msg(msg, 3, cs, buff);
}

bool send_note_on(uint8_t channel, uint8_t midi_base, int input, critical_section_t *cs, queue_t *buff) {
    uint8_t msg[3];
    msg[0] = 0x90 | channel; // NOTE ON
    msg[1] = midi_base + input;
    msg[2] = 127;

    return send_midi_msg(msg, 3, cs, buff);
}

bool send_note_off(uint8_t channel, uint8_t midi_base, int input, critical_section_t *cs, queue_t *buff) {
    uint8_t msg[3];
    msg[0] = 0x80 | channel; // NOTE OFF
    msg[1] = midi_base + input;
    msg[2] = 127;

    return send_midi_msg(msg, 3, cs, buff);
}

