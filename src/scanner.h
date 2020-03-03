#ifndef SCANNER_H
#define SCANNER_H

typedef struct Scanner_t {
	char* buffer;
	int pos, size;
	int line, column;
} Scanner;

extern Scanner* scanner_new(const char* buf);
extern void scanner_free(Scanner* scanner);

extern char scanner_scan(Scanner* s);
extern char scanner_peek(Scanner* s);

typedef int (*ScannerCallback)(char);

extern void scanner_read(Scanner* s, ScannerCallback whileCond, char* out);

#endif // SCANNER_H