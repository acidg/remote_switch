#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <avr/io.h>
#include <util/atomic.h>

//USI ports and pins
#define SPI_DDR_PORT DDRB
#define SCK_DD_PIN DDB2
#define SDO_DD_PIN DDB1
#define SDI_DD_PIN DDB0

//SPI data modes
#define SPI_MODE0 0x00
#define SPI_MODE1 0x04

void spi_init(void);

void spi_setDataMode(uint8_t spiDataMode);

uint8_t spi_transfer(uint8_t spiData);

void spi_end(void);

#endif