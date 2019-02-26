#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include <serial.h>
#include <type_others.h>
#include <vt100.h>

void serial_init(uint32_t baudrate);
void serial_putchar(volatile char c);
void serial_puts(const volatile char *s);

/* -1 if none */
signed char serial_get_last_char(void);

#endif /* SERIAL_H_ */
