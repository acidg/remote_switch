#include "uart.h"

int main(void) {
    uart_init();
    while(1) {
        uart_print((uint8_t*) "HELLO WORLD!\n\r");
    }

    return 0;
}