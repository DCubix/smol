#include <stdlib.h>
#include <stdio.h>

#include "ast.h"

int main(int argc, char** argv) {
	const char* code =
		"for item in ['a', 'b', 'c'] { print(item); }";
	Token* tokens;
	int tokenCount = lexer_lex(code, &tokens);

	printf("Parsed: %s\n", code);
	for (int i = 0; i < tokenCount; i++) {
		print_token(tokens[i]);
	}
	printf("\n");

	Parser p;
	parser_new(&p, tokens, tokenCount);

	Node* nd = ast_parse_program(&p);
	ast_print(nd, 0);

	free(tokens);
	node_free(nd);
	return 0;
}