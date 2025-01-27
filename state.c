#include "state.h"

void fill_state(struct State *st, i2c_inst_t *i2c) {
    uint8_t byte_value_regA = 0x00;
    uint8_t byte_value_regB = 0x00;

    reg_read(i2c, MCP_ADD, GPIOA, &byte_value_regA);
    reg_read(i2c, MCP_ADD, GPIOB, &byte_value_regB);

    // covert to a vector
    for (int bit=0; bit<8; bit++) {
        uint8_t mask = 1 << bit;
        st->header1[bit] = byte_value_regA & mask;
        st->header2[bit] = byte_value_regB & mask;
    }

    busy_wait_us_32(SCAN_DELAY_US);
}

///////////////////////////////////////
// external functions
///////////////////////////////////////
// init of state structures
void init_state(struct State *st, SETTINGS *set) {
    for (int i=0; i<NO_HEADER_INPUTS; i++) {
        if (set->in1_inv) st->header1[i] = 0;
        else st->header1[i] = 1;

        if (set->in2_inv) st->header2[i] = 0;
        else st->header2[i] = 1;
    }
}

void process_state(struct State *actual, struct State *last, SETTINGS *set, critical_section_t *cs, queue_t *buff, i2c_inst_t *i2c) {
    fill_state(actual, i2c);

    // Header 1
    for (int i=0; i<NO_HEADER_INPUTS; i++) {
        if ( (actual->header1[i]==0) && (last->header1[i]==1) ) { // falling edge
            if (set->in1_inv == 0) {
                if ( send_note_on(set->in1_m_ch, set->in1_base_m, i, cs, buff) == true ) {
                    last->header1[i] = 0;
                }
            }
            else {
                if ( send_note_off(set->in1_m_ch, set->in1_base_m, i, cs, buff) == true ) {
                    last->header1[i] = 0;
                }
            }
        }
        else if ( (actual->header1[i]==1) && (last->header1[i]==0) ) { // rising edge
            if (set->in1_inv == 0) {
                if ( send_note_off(set->in1_m_ch, set->in1_base_m, i, cs, buff) == true ) {
                    last->header1[i] = 1;
                }
            }
            else {
                if ( send_note_on(set->in1_m_ch, set->in1_base_m, i, cs, buff) == true ) {
                    last->header1[i] = 1;
                }
            }
        }
    }

    // Header 2
    for (int i=0; i<NO_HEADER_INPUTS; i++) {
        if ( (actual->header2[i]==0) && (last->header2[i]==1) ) { // falling edge
            if (set->in2_inv == 0) {
                if ( send_note_on(set->in2_m_ch, set->in2_base_m, i, cs, buff) == true ) {
                    last->header2[i] = 0;
                }
            }
            else {
                if ( send_note_off(set->in2_m_ch, set->in2_base_m, i, cs, buff) == true ) {
                    last->header2[i] = 0;
                }
            }
        }
        else if ( (actual->header2[i]==1) && (last->header2[i]==0) ) { // rising edge
            if (set->in2_inv == 0) {
                if ( send_note_off(set->in2_m_ch, set->in2_base_m, i, cs, buff) == true ) {
                    last->header2[i] = 1;
                }
            }
            else {
                if ( send_note_on(set->in2_m_ch, set->in2_base_m, i, cs, buff) == true ) {
                    last->header2[i] = 1;
                }
            }
        }
    }
}