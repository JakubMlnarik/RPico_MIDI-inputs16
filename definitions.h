#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "pico/binary_info.h"
#include "bsp/board.h"
#include "tusb.h"
#include "pico/sync.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "hardware/i2c.h"

////////////////////////
// Device name, fw version
#define DEV_NAME "MIDI_inputs16"
#define FW_VERSION "1.0"
////////////////////////

////////////////////////
// GPIO
////////////////////////

// WiFi button
#define WIFI_BUT 22

// i2c
#define I2C_BUS i2c0
#define MCP_ADD 0x20

#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

#define IODIRA 0x00	//direction in/out - port A
#define IODIRB 0x01	//direction in/out - port B
#define GPPUA 0x0c	//pullup resistor
#define GPPUB 0x0d	
#define GPIOA 0x12	//read/write
#define GPIOB 0x13	

////////////////////////
// STATE
////////////////////////
// global state structures of inputs - actual state, last state to identify changes
#define NO_HEADER_INPUTS 8

struct State {
    bool header1[NO_HEADER_INPUTS];
    bool header2[NO_HEADER_INPUTS];
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

// delay of reading inputs (us)
#define SCAN_DELAY_US 150

// timer to launch main inputs scanner
#define SHARED_BUFF_LENGTH 256

// When a message is transmitted the LED should blick for a while. LED_ON_TIME is based on number of passes through the input_scanner function.
#define LED_ON_TIME 10 // =desired time/INPUTS_SCANNER_TIMER_MS


#endif