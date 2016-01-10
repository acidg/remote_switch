/**
 * Uses Timer1 and OCR1A to send data via uart
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUDRATE 9600

#define TX_PIN PB0
#define TX_DDR DDRB
#define TX_PORT PORTB

void uart_init();

void uart_transmit(uint8_t val);

void uart_print(uint8_t *txt);

#endif