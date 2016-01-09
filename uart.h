#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define TX_PIN     PB0     // OC1A on Mega8
#define TX_DDR  DDRB

#define BIT_TIME    (uint16_t)((F_CPU + BAUD/2) / BAUD)

void uart_init(uint32_t baud);

void uart_transmit(uint8_t val);

void uart_print(uint8_t *txt);

#endif