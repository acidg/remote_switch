#include "uart.h"

volatile uint8_t tx_count;
uint8_t tx_data;

void uart_init(void) {
    OCR1A = TCNT1 + 1;            // force first compare
    TCCR1A = 1 << COM1A1 ^ 1 << COM1A0;        // set OC1A high, T1 mode 0
    TCCR1B = 1 << ICNC1 ^ 1 << CS10;        // noise canceler, 1>0 transition,
    // CLK/1, T1 mode 0
    TIFR = 1 << ICF1;            // clear pending interrupt
    TIMSK = 1 << TICIE1 ^ 1 << OCIE1A;        // enable tx and wait for start

    tx_count = 0;            // nothing to sent
    STXDDR |= 1 << STX;            // TX output
}

void uart_transmit(uint8_t val) {
    while (tx_count);            // until last byte finished
    tx_data = ~val;            // invert data for Stop bit generation
    tx_count = 10;            // 10 bits: Start + data + Stop
}


void uart_print(uint8_t *txt) {
    while (*txt) {
        uart_transmit(*txt++);
    }
}


ISR(SIG_OUTPUT_COMPARE1A) {
    uint8_t dout;
    uint8_t count;

    OCR1A += BIT_TIME;            // next bit slice
    count = tx_count;

    if (count) {
        tx_count = --count;        // count down
        dout = 1 << COM1A1;            // set low on next compare
        if (count != 9) {            // no start bit
            if (!(tx_data & 1))        // test inverted data
                dout = 1 << COM1A1 ^ 1 << COM1A0;    // set high on next compare
            tx_data >>= 1;            // shift zero in from left
        }
        TCCR1A = dout;
    }
};
