#include "uart.h"

#define BIT_TIME (F_CPU + BAUDRATE / 2) / BAUDRATE
#if BIT_TIME < 50
#error FCPU too slow for given baudrate
#endif

volatile uint8_t tx_count;
uint8_t tx_data;

void uart_init() {
    TX_DDR |= _BV(TX_PIN);                  // Configure TX_PIN as output
    TX_PORT |= _BV(TX_PIN);                 // Set output HIGH

    TCCR1 &= ~(_BV(COM1A1) | _BV(COM1A0));  // Disable comparator output
    TCCR1 &= 0xF0;                          // Stop counter (CS1[3:0])

    TCNT1 = 0;                              // Reset counter
    OCR1A = BIT_TIME;                       // Set bit time to compare register

    tx_count = 0;                           // reset bit counter

    TIFR = _BV(OCF1A);                      // clear pending interrupt
    TIMSK |= _BV(1 << OCIE1A);              // enable interrupt
}

void uart_transmit(uint8_t data) {
    while (tx_count);                       // Wait until last transmit finished
    tx_data = data;
    tx_count = 9;                           // 9 bits: data + stop
}


void uart_print(uint8_t *txt) {
    while (*txt) {
        uart_transmit(*txt++);
    }
}


ISR(SIG_OUTPUT_COMPARE1A) {
    uint8_t bit_index = tx_count;
    if (bit_index != 0) {
        bit_index--;
        if (bit_index == 8) {
            TX_PORT &= ~_BV(TX_PIN);            // Send start bit
        } else {
            if (tx_data & _BV(bit_index)) {
                TX_PORT &= ~_BV(TX_PIN);        // Set output LOW
            } else {
                TX_PORT |= _BV(TX_PIN);         // Set output HIGH
            }
        }
        tx_count = bit_index;
    } else {
        TX_PORT |= _BV(TX_PIN);                 // Set output HIGH
    }
};
