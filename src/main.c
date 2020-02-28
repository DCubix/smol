#include <stdlib.h>
#include <stdio.h>

#include "ast.h"

int main(int argc, char** argv) {
	const char* code = "'I am a string \\x22';";
	Token* tokens;
	int tokenCount = lexer_lex(code, &tokens);

	printf("Parsed: %s\n", code);
	for (int i = 0; i < tokenCount; i++) {
		print_token(tokens[i]);
	}
	printf("\n");

	Parser p;
	parser_new(&p, tokens, tokenCount);

	Node* nd = ast_parse_logicor(&p);
	ast_print(nd, 0);

	free(tokens);
	return 0;
}