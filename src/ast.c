#include "ast.h"

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

static const char* AST_TYPES[] = {
	"NT_UNKNOWN",
	"NT_NUMBER",
	"NT_STRING",
	"NT_BOOL",

	"NT_UNARY_MINUS",
	"NT_UNARY_BITNOT",
	"NT_UNARY_NOT",

	"NT_BINARY_ADD",
	"NT_BINARY_SUB",
	"NT_BINARY_MUL",
	"NT_BINARY_DIV",
	"NT_BINARY_MOD"
};

Node* node_new() {
	Node* nd = (Node*) malloc(sizeof(Node));
	nd->type = NT_UNKNOWN;
	nd->string = NULL;
	nd->op.left = NULL;
	nd->op.right = NULL;
	return nd;
}

void parser_new(Parser* p, Token* tokens, int numTokens) {
	p->tokens = tokens;
	p->pos = 0;
	p->len = numTokens;
}

int parser_accept(Parser* p, int type, const char* lexeme) {
	if (p->pos >= p->len) return 0;
	if (parser_current(p).type == type) {
		if (lexeme != NULL) {
			return strcasecmp(parser_current(p).lexeme, lexeme) == 0;
		} else {
			return 1;
		}
	}
	return 0;
}

int parser_expect(Parser* p, int type, const char* lexeme) {
	if (parser_accept(p, type, lexeme)) {
		return 1;
	}
	printf("Expected a %s, got a %s.\n", TOKENS[type], TOKENS[parser_current(p).type]);
	return 0;
}

void parser_advance(Parser* p) {
	if (p->pos >= p->len) return;
	p->pos++;
}

Token parser_current(Parser* p) {
	return p->tokens[p->pos];
}

void _printpad(int pad) {
	for (int i = 0; i < pad; i++) printf(" ");
}

void ast_print(Node* root, int pad) {
	if (root == NULL) return;
	_printpad(pad);
	printf("%s {\n", AST_TYPES[root->type]);
	switch (root->type) {
		case NT_NUMBER: _printpad(pad + 2); printf("value = %f\n", root->value); break;
		case NT_STRING: _printpad(pad + 2); printf("value = %s\n", root->string); break;
		case NT_BOOL: _printpad(pad + 2); printf("value = %s", root->boolean ? "true" : "false"); break;
		case NT_UNARY_MINUS:
		case NT_UNARY_NOT:
		case NT_UNARY_BITNOT: ast_print(root->op.right, pad + 2); break;
		case NT_BINARY_ADD:
		case NT_BINARY_DIV:
		case NT_BINARY_MOD:
		case NT_BINARY_MUL:
		case NT_BINARY_SUB: ast_print(root->op.left, pad + 2); ast_print(root->op.right, pad + 2); break;
		default: break;
	}
	_printpad(pad);
	printf("}\n");
}

Node* ast_parse_atom(Parser* p) {
	if (parser_accept(p, TT_NUMBER, NULL)) {
		Node* nd = node_new();
		nd->type = NT_NUMBER;
		nd->value = strtod(parser_current(p).lexeme, NULL);
		parser_advance(p);
		return nd;
	} else if (parser_accept(p, TT_LPAREN, NULL)) {
		parser_advance(p);
		Node* nd = ast_parse_addsub(p);
		if (parser_expect(p, TT_RPAREN, NULL)) {
			parser_advance(p);
			return nd;
		}
	}
	// TODO: Other types...
	return NULL;
}

Node* ast_parse_factor(Parser* p) {
	if (parser_accept(p, TT_MINUS, NULL)) {
		parser_advance(p);
		Node* nd = node_new();
		nd->type = NT_UNARY_MINUS;
		nd->op.right = ast_parse_atom(p);
		return nd;
	} else if (parser_accept(p, TT_BITNOT, NULL)) {
		parser_advance(p);
		Node* nd = node_new();
		nd->type = NT_UNARY_BITNOT;
		nd->op.right = ast_parse_atom(p);
	} else if (parser_accept(p, TT_EXCLAMATION, NULL)) {
		parser_advance(p);
		Node* nd = node_new();
		nd->type = NT_UNARY_NOT;
		nd->op.right = ast_parse_atom(p);
	}
	
	return ast_parse_atom(p);
}

Node* ast_parse_muldiv(Parser* p) {
	Node* left = ast_parse_factor(p);
	if (parser_accept(p, TT_ASTERISK, NULL)) {
		parser_advance(p);
		Node* right = ast_parse_muldiv(p);
		Node* nd = node_new();
		nd->type = NT_BINARY_MUL;
		nd->op.left = left;
		nd->op.right = right;
		return nd;
	} else if (parser_accept(p, TT_SLASH, NULL)) {
		parser_advance(p);
		Node* right = ast_parse_muldiv(p);
		Node* nd = node_new();
		nd->type = NT_BINARY_DIV;
		nd->op.left = left;
		nd->op.right = right;
		return nd;
	} else if (parser_accept(p, TT_PERCENT, NULL)) {
		parser_advance(p);
		Node* right = ast_parse_muldiv(p);
		Node* nd = node_new();
		nd->type = NT_BINARY_MOD;
		nd->op.left = left;
		nd->op.right = right;
		return nd;
	}
	return left;
}

Node* ast_parse_addsub(Parser* p) {
	Node* left = ast_parse_muldiv(p);
	if (parser_accept(p, TT_PLUS, NULL)) {
		parser_advance(p);
		Node* right = ast_parse_addsub(p);
		Node* nd = node_new();
		nd->type = NT_BINARY_ADD;
		nd->op.left = left;
		nd->op.right = right;
		return nd;
	} else if (parser_accept(p, TT_MINUS, NULL)) {
		parser_advance(p);
		Node* right = ast_parse_addsub(p);
		Node* nd = node_new();
		nd->type = NT_BINARY_SUB;
		nd->op.left = left;
		nd->op.right = right;
		return nd;
	}
	return left;
}