#include "utils.h"

// Write 1 byte to the specified register
int reg_write(  i2c_inst_t *i2c,
                const uint addr,
                const uint8_t reg,
                uint8_t value) {
    uint8_t msg[2];

    msg[0] = reg;
    msg[1] = value;

    // Write data to register(s) over I2C
    return i2c_write_blocking(i2c, addr, msg, 2, false);
}

// Read byte from the register
int reg_read(  i2c_inst_t *i2c,
                const uint addr,
                const uint8_t reg,
                uint8_t *byte) {
    // Read data from register(s) over I2C
    i2c_write_blocking(i2c, addr, &reg, 1, true);

    return i2c_read_blocking(i2c, addr, byte, 1, false);
}

int init_mcp(i2c_inst_t *i2c, const uint addr) {
    //port A as inputs
    reg_write(i2c, addr, IODIRA, 0xff);

    //input pullups
    reg_write(i2c, addr, GPPUA, 0xff);

    //port B as inputs
    reg_write(i2c, addr, IODIRB, 0xff);

    //input pullups
    reg_write(i2c, addr, GPPUB, 0xff);
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

bool send_prg_change(uint8_t channel, uint8_t midi_base, int input, critical_section_t *cs, queue_t *buff) {
    uint8_t msg[2];
    msg[0] = 0xC0 | channel; // Program Change
    msg[1] = midi_base + input;

    return send_midi_msg(msg, 2, cs, buff);
}

