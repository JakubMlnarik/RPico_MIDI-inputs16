// Settings.h
// shared app data for multi-thread sharing
#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hardware/sync.h>

#include "pico/stdlib.h"
#include "hardware/flash.h"


// Last sector of Flash
#define FLASH_TARGET_OFFSET (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE)

typedef struct SETTINGS_ {
    // sequence of magic numbers to verify a valid settings were in the flash (first boot)
    uint8_t magic_1;
    uint8_t magic_2;
    uint8_t magic_3;
    uint8_t magic_4;

    uint8_t fast_midi;

    uint8_t in1_m_ch;
    uint8_t in1_inv;
    uint8_t in1_base_m;
    
    uint8_t in2_m_ch;
    uint8_t in2_inv;
    uint8_t in2_base_m;
} SETTINGS;

// default values
#define MAGIC_1 1
#define MAGIC_2 2
#define MAGIC_3 3
#define MAGIC_4 4
#define FAST_MIDI_DEF 0
#define IN1_M_CH_DEF 0
#define IN1_INV_DEF 0
#define IN1_BASE_M_DEF 36
#define IN2_M_CH_DEF 0
#define IN2_INV_DEF 0
#define IN2_BASE_M_DEF 68

extern void load_settings(SETTINGS *set);
extern void save_settings(SETTINGS *set);

#endif
