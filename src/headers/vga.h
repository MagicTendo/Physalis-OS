#define VIDEO_MEMORY 0xb8000
#define MAX_COLUMNS 80
#define MAX_ROWS 25
#define PREFIX "physalis > "

#ifndef VGA_H
#define VGA_H

typedef enum {
    TOP,
    CENTER,
    BOTTOM,
    AUTO_ROW
} VerticalAlignment;

typedef enum {
	LEFT,
	MIDDLE,
	RIGHT,
	AUTO_COLUMN
} HorizontalAlignment;

typedef enum {
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHT_GREY,
	DARK_GREY,
	LIGHT_BLUE,
	LIGHT_GREEN,
	LIGHT_CYAN,
	LIGHT_RED,
	LIGHT_MAGENTA,
	YELLOW,
	WHITE,
	END_COLOR
} Color;
#endif

void initialise_terminal();
void clear_terminal(Color);

void display_message(const char*);
void display_message_with_jump(const char*);
void display_message_with_jumps(const char*, unsigned short);
void display_colored_message(const char*, Color, Color);
void display_positioned_message(const char*, HorizontalAlignment, VerticalAlignment);
void display_colored_positioned_message(const char*, Color, Color, HorizontalAlignment, VerticalAlignment);
void display_command_prompt();
void show_colors();

void write_sentence(const char*, unsigned short, unsigned short);
void write_relative_character(const char, unsigned short, unsigned short);
void write_absolute_character(const char, unsigned short, unsigned short, unsigned short);
void write_space(unsigned short, unsigned short);
char get_character(unsigned short, unsigned short);
unsigned short get_color(unsigned short, unsigned short);
void delete_last_character();

void jump_line();
void jump_lines(unsigned short);
void reset_position();
void scroll_up();

unsigned short calculate_color_code(Color, Color);