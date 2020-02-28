#ifndef AST_H
#define AST_H

#include "lexer.h"

typedef struct Node_t {
	enum {
		NT_UNKNOWN = 0,
		NT_NUMBER,
		NT_STRING,
		NT_BOOL,

		NT_UNARY_MINUS,
		NT_UNARY_BITNOT,
		NT_UNARY_NOT,

		NT_BINARY_ADD,
		NT_BINARY_SUB,
		NT_BINARY_MUL,
		NT_BINARY_DIV,
		NT_BINARY_MOD
		// TODO: Add More
	} type;
	union {
		double value;
		char* string;
		int boolean;
		struct { struct Node_t *left, *right; } op;
	};
} Node;

extern Node* node_new();

typedef struct Parser_t {
	Token* tokens;
	int pos, len;
} Parser;

extern void parser_new(Parser* p, Token* tokens, int numTokens);
extern int parser_accept(Parser* p, int type, const char* lexeme);
extern int parser_expect(Parser* p, int type, const char* lexeme);
extern void parser_advance(Parser* p);
extern Token parser_current(Parser* p);

extern Node* ast_parse_atom(Parser* p);
extern Node* ast_parse_factor(Parser* p);
extern Node* ast_parse_muldiv(Parser* p);
extern Node* ast_parse_addsub(Parser* p);

extern void ast_print(Node* root, int pad);

#endif // AST_H