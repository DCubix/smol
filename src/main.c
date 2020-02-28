#include <stdlib.h>
#include <stdio.h>

#include "ast.h"

int main(int argc, char** argv) {
	const char* code = "2 + 5 * 1.5";
	Token* tokens;
	int tokenCount = lexer_lex(code, &tokens);

	printf("Parsed: %s\n", code);
	for (int i = 0; i < tokenCount; i++) {
		print_token(tokens[i]);
	}
	printf("\n");

	Parser p;
	parser_new(&p, tokens, tokenCount);

	Node* nd = ast_parse_addsub(&p);
	ast_print(nd, 0);

	free(tokens);
	return 0;
}