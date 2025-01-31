#include "definitions.h"
#include "state.h"
#include "status_dispatcher.h"
#include "access_point.h"
#include "settings.h"
#include "pico/cyw43_arch.h"

////////////////////////////
// global variables
////////////////////////////
SETTINGS main_settings;

i2c_inst_t *i2c = I2C_BUS;

struct State act_state;
struct State last_state;

int led_on_timer = 0;

// critical code protection from concruency
critical_section_t cs_lock;

// shared output buffer
queue_t shared_buff;

// USB callbacks
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
}

// timer interrupt callback
bool scan_timer_callback(struct repeating_timer *t) {
    process_state(&act_state, &last_state, &main_settings, &cs_lock, &shared_buff, i2c);

    // incremetning the LED timer every pass until the limit
    led_on_timer++;
    if (led_on_timer > LED_ON_TIME) led_on_timer = LED_ON_TIME;
    return true;
}

// RX interrupt - incomming MIDI
// RX interrupt handler
void on_uart_rx() { 
    while (uart_is_readable(UART_ID)) {
        uint8_t byte = uart_getc(UART_ID);
        status_dispatcher(&byte, &cs_lock, &shared_buff);
    }
}

void general_init() {
    board_init();
    tusb_init();
    cyw43_arch_init();
    load_settings(&main_settings);

    // WIFI button pin
    gpio_init(WIFI_BUT);
    gpio_set_dir(WIFI_BUT, GPIO_IN);

    // This will use i2c0 on the default SDA and SCL pins (GP4, GP5 on a Pico)
    i2c_init(i2c, 100 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
}

void midi_init() {
    init_mcp(i2c, MCP_ADD);

    init_state(&act_state, &main_settings);
    init_state(&last_state, &main_settings);

    // init critical section lock
    critical_section_init(&cs_lock);

    // shared output buffer
    queue_init(&shared_buff, sizeof(uint8_t), SHARED_BUFF_LENGTH);

    // UART
    // Set up our UART with a basic baud rate.
    if (main_settings.fast_midi) uart_init(UART_ID, FAST_UART_BAUD_RATE);
    else uart_init(UART_ID, STANDARD_UART_BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, UART_DATA_BITS, UART_STOP_BITS, UART_PARITY);

    // Turn on FIFO's
    uart_set_fifo_enabled(UART_ID, true);

    // set RX interrupt
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);
}

int main() {

    general_init();

    // WIFI AP point mode enter
    if (!gpio_get(WIFI_BUT)) {
        wifi_ap_proc(&main_settings);
    }

    // normal working MIDI mode
    midi_init();

    // Blink LED
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(50);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(50);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(50);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

    // set timer to scan peripherals periodically
    struct repeating_timer timer;
    add_repeating_timer_ms(INPUTS_SCANNER_TIMER_MS, scan_timer_callback, NULL, &timer);

    // Loop forever
    while (true) {
        tud_task(); // tinyusb device task

        // The MIDI interface always creates input and output port/jack descriptors
        // regardless of these being used or not. Therefore incoming traffic should be read
        // (possibly just discarded) to avoid the sender blocking in IO
        while (tud_midi_available()) {
            uint8_t byte;
            tud_midi_stream_read(&byte, 1);
            led_on_timer = 0;
        }

        // send all midi from the safe queue to tx
        while (!queue_is_empty(&shared_buff)) {
            uint8_t byte;
            queue_try_remove(&shared_buff, &byte);
            uart_putc_raw(UART_ID, byte);
            tud_midi_n_stream_write(0, 0, &byte, 1);
            led_on_timer = 0;
        }

        // handling MIDI messages signal LED
        if (led_on_timer < LED_ON_TIME) cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        else cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    }
}