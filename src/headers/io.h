#include "stdint.h"

uint8_t inb(uint16_t);
void outb(uint16_t, uint8_t);
void enable_cursor();
void disable_cursor();
void update_cursor(unsigned short, unsigned short);