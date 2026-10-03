#include "../headers/string.h"

unsigned int string_length(const char* string) {
	unsigned int count = 0;

	while (string[count] != '\0') {
		count++;
	}

	return count;
}