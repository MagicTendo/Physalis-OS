#include "../headers/io.h"
#include "../headers/string.h"
#include "../headers/vga.h"

unsigned char* screen = (unsigned char*) VIDEO_MEMORY;
unsigned short current_row = 0;
unsigned short current_column = 0;
unsigned short last_relative_row = 0;
unsigned short last_relative_column = 0;
unsigned short default_text_color = BROWN;
unsigned short default_background_color = BLACK;

void initialise_terminal() {
	outb(0x3f8, 0x00);
	enable_cursor();

	clear_terminal(default_background_color);
}

void clear_terminal(Color background_color) {
	for (unsigned short row = 0; row < MAX_ROWS; row++) {
		for (unsigned short column = 0; column < MAX_COLUMNS; column++) {
			write_space(column, row);
		}
	}

	reset_position();

	default_background_color = background_color;
}

void display_message(const char* sentence) {
	display_positioned_message(sentence, AUTO_COLUMN, AUTO_ROW);
}

void display_message_with_jump(const char* sentence) {
	display_message_with_jumps(sentence, 1);
}

void display_message_with_jumps(const char* sentence, unsigned short amount) {
	display_message(sentence);
	jump_lines(amount);
}

void display_colored_message(const char* sentence, Color text_color, Color background_color) {
	display_colored_positioned_message(sentence, text_color, background_color, AUTO_COLUMN, AUTO_ROW);
}

void display_positioned_message(const char* sentence, HorizontalAlignment horizontal_position, VerticalAlignment vertical_position) {
	display_colored_positioned_message(sentence, default_text_color, default_background_color, horizontal_position, vertical_position);
}

void display_colored_positioned_message(const char* sentence, Color text_color, Color background_color, HorizontalAlignment horizontal_position, VerticalAlignment vertical_position) {
	unsigned short color_code = calculate_color_code(text_color, background_color);
	unsigned short isPositioned = 1;
	unsigned short column = 0;
	unsigned short row = 0;

	switch (horizontal_position) {
		case LEFT:
			column = 0;
		break;

		case MIDDLE:
			column = (MAX_COLUMNS / 2) - (string_length(sentence) / 2);
		break;

		case RIGHT:
			column = MAX_COLUMNS - string_length(sentence);
		break;

		case AUTO_COLUMN:
			column = current_column;
		break;
	}

	switch (vertical_position) {
		case TOP:
			row = 0;
		break;

		case CENTER:
			row = MAX_ROWS / 2;
		break;

		case BOTTOM:
			row = MAX_ROWS - 1;
		break;

		case AUTO_ROW:
			row = current_row;
		break;
	}

	if (horizontal_position == AUTO_COLUMN && vertical_position == AUTO_ROW)
		isPositioned = 0;

	current_row = row;
	current_column = column;

	write_sentence(sentence, color_code, isPositioned);
}

void display_command_prompt() {
	jump_line();
	display_message(PREFIX);
}

void show_colors() {
	for (unsigned short i = 0; i < END_COLOR; i++) {
		write_absolute_character(' ', i, MAX_ROWS - 1, calculate_color_code(default_background_color, i));
	}
}

void write_sentence(const char* sentence, unsigned short color_code, unsigned short isPositioned) {
	unsigned int total_length = string_length(sentence);

	for (unsigned int i = 0; i < total_length; i++) {
		write_relative_character(sentence[i], color_code, isPositioned);
	}
}

void write_relative_character(const char character, unsigned short color_code, unsigned short isPositioned) {
	if (current_column >= MAX_COLUMNS)
		jump_line();
	if (!isPositioned && current_row + 1 >= MAX_ROWS)
		scroll_up();

	write_absolute_character(character, current_column, current_row, color_code);
	update_cursor(current_column + 1, current_row);

	current_column++;
}

void write_absolute_character(const char character, unsigned short column, unsigned short row, unsigned short color_code) {
	screen[2 * (row * MAX_COLUMNS + column)] = character;
	screen[2 * (row * MAX_COLUMNS + column) + 1] = color_code;
}

void write_space(unsigned short column, unsigned short row) {
	write_absolute_character(' ', column, row, calculate_color_code(default_text_color, default_background_color));
}

char get_character(unsigned short column, unsigned short row) {
	return screen[2 * (row * MAX_COLUMNS + column)];
}

unsigned short get_color(unsigned short column, unsigned short row) {
	return screen[2 * (row * MAX_COLUMNS + column) + 1];
}

void delete_last_character() {
	if (current_column > 5) {
		current_column--;

		write_space(current_column, current_row);
	}
}

void jump_line() {
	jump_lines(1);
}

void jump_lines(unsigned short amount) {
	current_row += amount;
	current_column = 0;
}

void reset_position() {
	current_row = 0;
	current_column = 0;

	update_cursor(current_column, current_row);
}

void scroll_up() {
	for (unsigned short i = 0; i < MAX_COLUMNS; i++) {
		for (unsigned short j = 1; j < MAX_ROWS - 1; j++) {
			write_absolute_character(get_character(i, j), i, j - 1, get_color(i, j));
		}
	}

	current_row--;
}

unsigned short calculate_color_code(Color text_color, Color background_color) {
	return (background_color << 4) | (text_color & 0x0f);
}