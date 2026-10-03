#include "../headers/io.h"
#include "../headers/vga.h"

uint8_t inb(uint16_t port) {
    uint8_t ret;

	__asm__ volatile ( "inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");

	return ret;
}

void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

void enable_cursor() {
	outb(0x3d4, 0x0a);
	outb(0x3d5, (inb(0x3d5) & 0xc0) | 15);

	outb(0x3d4, 0x0b);
	outb(0x3d5, (inb(0x3d5) & 0xe0) | 15);
}

void disable_cursor() {
	outb(0x3d4, 0x0a);
	outb(0x3d5, 0x20);
}

void update_cursor(unsigned short column, unsigned short row) {
	write_space(column, row);

	uint16_t pos = row * MAX_COLUMNS + column;

	outb(0x3d4, 0x0f);
	outb(0x3d5, (uint8_t) (pos & 0xff));
	outb(0x3d4, 0x0e);
	outb(0x3d5, (uint8_t) ((pos >> 8) & 0xff));
}