#include <stdio.h>

#include "lexer.h"

int main(int argc, char** argv) {
	const char* code = "for i in [0, 1, 2, 3] { print(i); }";
	Token* tokens;
	int tokenCount = lexer_lex(code, &tokens);

	for (int i = 0; i < tokenCount; i++) {
		print_token(tokens[i]);
	}
	printf("\n");

	return 0;
}