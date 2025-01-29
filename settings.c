#include "settings.h"

uint8_t *flash_target_contents = (uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
uint8_t flash_buff[FLASH_PAGE_SIZE];

void save_settings(SETTINGS *set) {
    uint32_t ints = save_and_disable_interrupts();
    // Erase the last sector of the flash
    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);

    // write new settings
    memcpy(flash_buff, set, sizeof(SETTINGS));
    flash_range_program(FLASH_TARGET_OFFSET, (uint8_t *)flash_buff, FLASH_PAGE_SIZE);
    restore_interrupts (ints);
}

void load_settings(SETTINGS *set) {
    memcpy(set, flash_target_contents, sizeof(SETTINGS));

    // validatin of magic numbers
    if ( (set->magic_1 != MAGIC_1)\
        || (set->magic_2 != MAGIC_2)\
        || (set->magic_3 != MAGIC_3)\
        || (set->magic_4 != MAGIC_4) ) {
            set->magic_1 = MAGIC_1;
            set->magic_2 = MAGIC_2;
            set->magic_3 = MAGIC_3;
            set->magic_4 = MAGIC_4;
            set->fast_midi = FAST_MIDI_DEF;
            set->in1_m_ch = IN1_M_CH_DEF;
            set->in1_m_type = IN1_M_TYPE_DEF;
            set->in1_inv = IN1_INV_DEF;
            set->in1_base_m = IN1_BASE_M_DEF;
            set->in2_m_ch = IN2_M_CH_DEF;
            set->in2_m_type = IN2_M_TYPE_DEF;
            set->in2_inv = IN2_INV_DEF;
            set->in2_base_m = IN2_BASE_M_DEF;

            save_settings(set);
    }
}