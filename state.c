#include "state.h"

void fill_state(struct State *st) {
    //inputs - multiplexing
    gpio_put(M0, false);
    busy_wait_us_32(MATRIX_SCAN_DELAY_US);
    scan_data_pins(st->header1, 0);
    gpio_put(M0, true);

    gpio_put(M1, false);
    busy_wait_us_32(MATRIX_SCAN_DELAY_US);
    scan_data_pins(st->header1, 8);
    gpio_put(M1, true);

    gpio_put(M2, false);
    busy_wait_us_32(MATRIX_SCAN_DELAY_US);
    scan_data_pins(st->header1, 16);
    gpio_put(M2, true);

    gpio_put(M3, false);
    busy_wait_us_32(MATRIX_SCAN_DELAY_US);
    scan_data_pins(st->header1, 24);
    gpio_put(M3, true);

    gpio_put(M4, false);
    busy_wait_us_32(MATRIX_SCAN_DELAY_US);
    scan_data_pins(st->header2, 0);
    gpio_put(M4, true);

    gpio_put(M5, false);
    busy_wait_us_32(MATRIX_SCAN_DELAY_US);
    scan_data_pins(st->header2, 8);
    gpio_put(M5, true);

    gpio_put(M6, false);
    busy_wait_us_32(MATRIX_SCAN_DELAY_US);
    scan_data_pins(st->header2, 16);
    gpio_put(M6, true);

    gpio_put(M7, false);
    busy_wait_us_32(MATRIX_SCAN_DELAY_US);
    scan_data_pins(st->header2, 24);
    gpio_put(M7, true);

    // pots
    st->pots[0] = get_pot_position(0);
    st->pots[1] = get_pot_position(1);
    st->pots[2] = get_pot_position(2);
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

    for (int i=0; i<NO_POTS; i++) {
        st->pots[i] = 0;
    }
}

void process_state(struct State *actual, struct State *last, SETTINGS *set, critical_section_t *cs, queue_t *buff) {
    fill_state(actual);

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

    // POTs changes
    //A1
    if (set->a1_en) {
        if (abs(actual->pots[0]-last->pots[0]) >= 2) {
            if ( send_pot_midi(0, set->a_m_ch, actual->pots[0], cs, buff) == true ) {
                last->pots[0] = actual->pots[0];
            }
        }
    }

    //A2
    if (set->a2_en) {
        if (abs(actual->pots[1]-last->pots[1]) >= 2) {
            if ( send_pot_midi(1, set->a_m_ch, actual->pots[1], cs, buff) == true ) {
                last->pots[1] = actual->pots[1];
            }
        }
    }
    
    //A3
    if (set->a3_en) {
        if (abs(actual->pots[2]-last->pots[2]) >= 2) {
            if ( send_pot_midi(2, set->a_m_ch, actual->pots[2], cs, buff) == true ) {
                last->pots[2] = actual->pots[2];
            }
        }
    }
}