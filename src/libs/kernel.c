#include "../headers/kernel.h"
#include "../headers/vga.h"

int main() {
	initialise_terminal();

	show_colors();

	display_positioned_message("Hello, World!", RIGHT, TOP);

	reset_position();

	display_message_with_jump("Welcome to Physalis OS v0.1.0!");
	display_message_with_jump("Another random hobbyist OS project!");

	display_command_prompt();

	return 0;
}