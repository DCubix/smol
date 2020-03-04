#include "lexer.h"

#include <ctype.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dynarray.h"

DEF_DYN_ARRAY(Token);

static const char* KEYWORDS[] = {
	"return",
	"continue",
	"break",
	"for",
	"in",
	"if",
	"else",
	"elif",
	"fun",
	"let",
	"or",
	"and",
	"int",
	"double",
	"string",
	"bool",
	"true",
	"false",
	"nil"
};

const char* TOKENS[] = {
	"TT_SEMICOLON",
	"TT_ID",
	"TT_NUMBER",
	"TT_STRING",
	"TT_BOOL",
	"TT_LBRACE",
	"TT_RBRACE",
	"TT_KEYWORD",
	"TT_LPAREN",
	"TT_RPAREN",
	"TT_RBRACKET",
	"TT_LBRACKET",
	"TT_QUESTION",
	"TT_COLON",
	"TT_DOTS",
	"TT_BITOR",
	"TT_BITAND",
	"TT_BITXOR",
	"TT_LSHIFT",
	"TT_RSHIFT",
	"TT_PLUS",
	"TT_MINUS",
	"TT_ASTERISK",
	"TT_SLASH",
	"TT_PERCENT",
	"TT_EXCLAMATION",
	"TT_BITNOT",
	"TT_COMMA",
	"TT_POINT",
	"TT_EQUALS",
	"TT_LESS",
	"TT_GREATER",
	"TT_LESSEQUALS",
	"TT_GREATEREQUALS",
	"TT_PLUSEQUALS",
	"TT_MINUSEQUALS",
	"TT_MULEQUALS",
	"TT_DIVEQUALS",
	"TT_COMPEQUALS",
	"TT_COMPNOTEQUALS",
	"TT_LOGICOR",
	"TT_LOGICAND",
	"TT_EOF"
};

int is_keyword(const char* id) {
	for (int i = 0; i < 18; i++) {
		if (strcmp(KEYWORDS[i], id) == 0) return 1; // Keywords ARE case sensitive
	}
	return 0;
}

int _scan_identifier(char c) {
	return isalpha(c) || c == '_' || isdigit(c);
}

int _scan_number(char c) {
	return c == '.' || isdigit(c) || isxdigit(c) || c == 'x' || c == 'X';
}

void print_token(Token tok) {
	if (tok.lexeme == NULL) printf("%s() ", TOKENS[tok.type]);
	else printf("%s(\'%s\') ", TOKENS[tok.type], tok.lexeme);
}

void token_init(Token* tok) {
	tok->lexeme = (char*) malloc(sizeof(char) * LEX_MAX_LEXEME_SIZE);
	tok->line = 0;
	tok->column = 0;
	memset(tok->lexeme, 0, sizeof(char) * LEX_MAX_LEXEME_SIZE);
}

int lexer_lex(const char* input, Token** out) {
	TokenArray ret;
	TokenArray_new(&ret);

	Scanner* sc = scanner_new(input);

#define PUSH(tp, tok)  tok.line = sc->line, tok.column = sc->column, tok.type = tp, TokenArray_push(&ret, tok)
#define SPUSH(tp) { Token tok; tok.lexeme = NULL; tok.type = tp; tok.line = sc->line; tok.column = sc->column; TokenArray_push(&ret, tok); }

	while (scanner_peek(sc) != '\0') {
		char c = scanner_peek(sc);
		if (isalpha(c) || c == '_') { // ID
			Token tok; token_init(&tok);
			scanner_read(sc, _scan_identifier, tok.lexeme);

			if (is_keyword(tok.lexeme)) {
				if (strcmp(tok.lexeme, "true") == 0 || strcmp(tok.lexeme, "false") == 0)
					tok.type = TT_BOOL;
				else
					tok.type = TT_KEYWORD;
			} else tok.type = TT_ID;
			TokenArray_push(&ret, tok);
		} else if (isdigit(c)) { // NUMBER
			Token tok; token_init(&tok);
			scanner_read(sc, _scan_number, tok.lexeme);
			tok.type = TT_NUMBER;
			TokenArray_push(&ret, tok);
		} else if (c == '\'') { // STRING
			scanner_scan(sc);
			
			Token tok; token_init(&tok);
			tok.type = TT_STRING;

			int i = 0;
			while (scanner_peek(sc) != '\'' && scanner_peek(sc) != '\0') {
				if (scanner_peek(sc) == '\\') {
					scanner_scan(sc);
					char es = scanner_scan(sc);
					switch (es) {
						case 't': tok.lexeme[i] = '\t'; break;
						case 'n': tok.lexeme[i] = '\n'; break;
						case 'f': tok.lexeme[i] = '\f'; break;
						case 'r': tok.lexeme[i] = '\r'; break;
						case 'a': tok.lexeme[i] = '\a'; break;
						case 'b': tok.lexeme[i] = '\b'; break;
						case '\\': tok.lexeme[i] = '\\'; break;
						case 'x': {
							char hex[2];
							hex[0] = scanner_scan(sc);
							hex[1] = scanner_scan(sc);
							int code = strtol(hex, NULL, 16);
							tok.lexeme[i] = '\x00' + code;
						} break;
						default: tok.lexeme[i] = es; break;
					}
				} else {
					tok.lexeme[i] = scanner_scan(sc);
				}
				i++;
			}
			scanner_scan(sc);
			TokenArray_push(&ret, tok);
		} else if (c == '{') {
			scanner_scan(sc);
			SPUSH(TT_LBRACE);
		} else if (c == '}') {
			scanner_scan(sc);
			SPUSH(TT_RBRACE);
		} else if (c == '(') {
			scanner_scan(sc);
			SPUSH(TT_LPAREN);
		} else if (c == ')') {
			scanner_scan(sc);
			SPUSH(TT_RPAREN);
		} else if (c == '[') {
			scanner_scan(sc);
			SPUSH(TT_LBRACKET);
		} else if (c == ']') {
			scanner_scan(sc);
			SPUSH(TT_RBRACKET);
		} else if (c == '?') {
			scanner_scan(sc);
			SPUSH(TT_QUESTION);
		} else if (c == ':') {
			scanner_scan(sc);
			SPUSH(TT_COLON);
		} else if (c == ';') {
			scanner_scan(sc);
			SPUSH(TT_SEMICOLON);
		} else if (c == '.') {
			scanner_scan(sc);
			if (scanner_peek(sc) == '.') {
				scanner_scan(sc);
				SPUSH(TT_DOTS);
			} else SPUSH(TT_POINT);
		} else if (c == '|') {
			scanner_scan(sc);
			if (scanner_peek(sc) == '|') {
				scanner_scan(sc);
				SPUSH(TT_LOGICOR);
			} else SPUSH(TT_BITOR);
		} else if (c == '&') {
			scanner_scan(sc);
			if (scanner_peek(sc) == '&') {
				scanner_scan(sc);
				SPUSH(TT_LOGICAND);
			} else SPUSH(TT_BITAND);
		} else if (c == '<') {
			scanner_scan(sc);
			if (scanner_peek(sc) == '<') {
				scanner_scan(sc);
				SPUSH(TT_LSHIFT);
			} else if (scanner_peek(sc) == '=') {
				scanner_scan(sc);
				SPUSH(TT_LESSEQUALS);
			} else SPUSH(TT_LESS);
		} else if (c == '>') {
			scanner_scan(sc);
			if (scanner_peek(sc) == '>') {
				scanner_scan(sc);
				SPUSH(TT_RSHIFT);
			} else if (scanner_peek(sc) == '=') {
				scanner_scan(sc);
				SPUSH(TT_GREATEREQUALS);
			} else SPUSH(TT_GREATER);
		} else if (c == '=') {
			scanner_scan(sc);
			if (scanner_peek(sc) == '=') {
				scanner_scan(sc);
				SPUSH(TT_COMPEQUALS);
			} else SPUSH(TT_EQUALS);
		} else if (c == '+') {
			scanner_scan(sc);
			if (scanner_peek(sc) == '=') {
				scanner_scan(sc);
				SPUSH(TT_PLUSEQUALS);
			} else SPUSH(TT_PLUS);
		} else if (c == '-') {
			scanner_scan(sc);
			if (scanner_peek(sc) == '=') {
				scanner_scan(sc);
				SPUSH(TT_MINUSEQUALS);
			} else SPUSH(TT_MINUS);
		} else if (c == '*') {
			scanner_scan(sc);
			if (scanner_peek(sc) == '=') {
				scanner_scan(sc);
				SPUSH(TT_MULEQUALS);
			} else SPUSH(TT_ASTERISK);
		} else if (c == '/') {
			scanner_scan(sc);
			if (scanner_peek(sc) == '=') {
				scanner_scan(sc);
				SPUSH(TT_DIVEQUALS);
			} else SPUSH(TT_SLASH);
		} else if (c == '^') {
			scanner_scan(sc);
			SPUSH(TT_BITXOR);
		} else if (c == ',') {
			scanner_scan(sc);
			SPUSH(TT_COMMA);
		} else if (c == '%') {
			scanner_scan(sc);
			SPUSH(TT_PERCENT);
		} else if (c == '~') {
			scanner_scan(sc);
			SPUSH(TT_BITNOT);
		} else if (c == '!') {
			scanner_scan(sc);
			if (scanner_peek(sc) == '=') {
				scanner_scan(sc);
				SPUSH(TT_COMPNOTEQUALS);
			} else SPUSH(TT_EXCLAMATION);
		} else {
			scanner_scan(sc);
		}
	}

	scanner_free(sc);

	SPUSH(TT_EOF);

	*out = ret.data;
	return ret.len;
}