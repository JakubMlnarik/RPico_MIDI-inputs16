#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "pico/binary_info.h"
#include "bsp/board.h"
#include "tusb.h"
#include "pico/sync.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"

////////////////////////
// GPIO
////////////////////////

// WiFi button
#define WIFI_BUT 22

// ADC
#define ADC_1 26
#define ADC_2 27
#define ADC_3 28

// Tones multiplex
#define M0 10
#define M1 11
#define M2 12
#define M3 13
#define M4 14
#define M5 15
#define M6 16
#define M7 17
#define D0 2
#define D1 3
#define D2 4
#define D3 5
#define D4 6
#define D5 7
#define D6 8
#define D7 9

////////////////////////
// STATE
////////////////////////
// global state structures of inputs - actual state, last state to identify changes
#define NO_HEADER_INPUTS 32
#define NO_POTS 3

struct State {
    bool header1[NO_HEADER_INPUTS];
    bool header2[NO_HEADER_INPUTS];
    uint8_t pots[NO_POTS];
};

////////////////////////
// UART
////////////////////////
#define UART_ID uart0
#define STANDARD_UART_BAUD_RATE 31250
#define FAST_UART_BAUD_RATE 115200
#define UART_DATA_BITS 8
#define UART_STOP_BITS 1
#define UART_PARITY    UART_PARITY_NONE

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

////////////////////////
// WORKFLOW
////////////////////////
// timer to launch main inputs scanner
#define INPUTS_SCANNER_TIMER_MS 2

// delay of reading tone inputs after a matrix output change (us)
#define MATRIX_SCAN_DELAY_US 150

// POTs exponential moving avergae koef
#define POT_MA_FACTOR 0.1f

// timer to launch main inputs scanner
#define SHARED_BUFF_LENGTH 256

// When a message is transmitted the LED should blick for a while. LED_ON_TIME is based on number of passes through the input_scanner function.
#define LED_ON_TIME 10 // =desired time/INPUTS_SCANNER_TIMER_MS


#endif