#include "scanner.h"

#include <stdlib.h>
#include <string.h>
#include <memory.h>

Scanner* scanner_new(const char* buf) {
	Scanner* scan = (Scanner*) malloc(sizeof(Scanner));
	scan->size = strlen(buf);
	scan->buffer = (char*) malloc(sizeof(char) * scan->size);
	scan->pos = 0;
	scan->line = 0;
	scan->column = 0;
	strcpy(scan->buffer, buf);
	return scan;
}

void scanner_free(Scanner* scanner) {
	scanner->pos = 0;
	free(scanner);
}

char scanner_scan(Scanner* s) {
	if (s->pos >= s->size) return '\0';
	if (s->buffer[s->column++] == '\n') {
		s->line++;
		s->column = 0;
	}
	return s->buffer[s->pos++];
}

char scanner_peek(Scanner* s) {
	if (s->pos >= s->size) return '\0';
	return s->buffer[s->pos];
}

void scanner_read(Scanner* s, ScannerCallback whileCond, char* out) {
	int i = 0;
	while (scanner_peek(s) != '\0' && whileCond(scanner_peek(s))) {
		out[i++] = scanner_scan(s);
	}
}