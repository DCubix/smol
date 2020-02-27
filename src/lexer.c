#include "lexer.h"

#include <ctype.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>

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
	"bool"
};

int is_keyword(const char* id) {
	for (int i = 0; i < 17; i++) {
		if (strcasecmp(KEYWORDS[i], id) == 0) return 1;
	}
	return 0;
}

int _scan_identifier(char c) {
	return isalpha(c) || c == '_' || isdigit(c);
}

int _scan_number(char c) {
	return c == '.' || isdigit(c) || ishexnumber(c) || c == 'x' || c == 'X';
}

int lexer_lex(const char* input, Token* out) {
	Scanner* sc = scanner_new(input);
	int tk = 0;

#define PUSH(tp) out[tk++].type = (tp)
#define TOK() out[tk].lexeme
#define NEW_TOK() memset(TOK(), 0, LEX_MAX_LEXEME_SIZE)

	while (scanner_peek(sc) != '\0') {
		char c = scanner_peek(sc);
		if (isalpha(c) || c == '_') { // ID
			NEW_TOK();
			scanner_read(sc, _scan_identifier, TOK());

			if (is_keyword(TOK())) {
				if (strcasecmp(TOK(), "true") == 0 || strcasecmp(TOK(), "false") == 0)
					PUSH(TT_BOOL);
				else
					PUSH(TT_KEYWORD);
			} else PUSH(TT_ID);
		} else if (isdigit(c)) { // NUMBER
			NEW_TOK();
			scanner_read(sc, _scan_number, TOK());

			if (strstr(out[tk].lexeme, ".") != NULL) {
				PUSH(TT_FLOAT);
			} else {
				PUSH(TT_INTEGER);
			}
		} else if (c == '"') { // STRING
			scanner_scan(sc);
			NEW_TOK();

			int i = 0;
			while (scanner_peek(sc) != '"' && scanner_peek(sc) != '\0') {
				if (scanner_peek(sc) == '\\') {
					scanner_scan(sc);
					char es = scanner_scan(sc);
					switch (scanner_peek(sc)) {
						case 't': TOK()[i] = '\t'; break;
						case 'n': TOK()[i] = '\n'; break;
						case 'f': TOK()[i] = '\f'; break;
						case 'r': TOK()[i] = '\r'; break;
						case 'a': TOK()[i] = '\a'; break;
						case 'b': TOK()[i] = '\b'; break;
						case '\\': TOK()[i] = '\\'; break;
						case 'x': {
							char hex[2];
							hex[0] = scanner_scan(sc);
							hex[1] = scanner_scan(sc);
							int code = strtol(hex, NULL, 16);
							TOK()[i] = '\x00' + code;
						} break;
						default: TOK()[i] = es; break;
					}
				} else {
					TOK()[i] = scanner_scan(sc);
				}
				i++;
			}
			scanner_scan(sc);

			PUSH(TT_STRING);
		} else if (c == '{') {
			scanner_scan(sc);
			NEW_TOK(); PUSH(TT_LBRACE);
		} else if (c == '}') {
			scanner_scan(sc);
			NEW_TOK(); PUSH(TT_RBRACE);
		} else if (c == '(') {
			scanner_scan(sc);
			NEW_TOK(); PUSH(TT_LPAREN);
		} else if (c == ')') {
			scanner_scan(sc);
			NEW_TOK(); PUSH(TT_RPAREN);
		} else if (c == '[') {
			scanner_scan(sc);
			NEW_TOK(); PUSH(TT_LBRACKET);
		} else if (c == ']') {
			scanner_scan(sc);
			NEW_TOK(); PUSH(TT_RBRACKET);
		} else if (c == '?') {
			scanner_scan(sc);
			NEW_TOK(); PUSH(TT_QUESTION);
		} else if (c == ':') {
			scanner_scan(sc);
			NEW_TOK(); PUSH(TT_COLON);
		} else if (c == ';') {
			scanner_scan(sc);
			NEW_TOK(); PUSH(TT_SEMICOLON);
		} else if (c == '.') {
			scanner_scan(sc);
			NEW_TOK();
			if (scanner_peek(sc) == '.') {
				scanner_scan(sc);
				PUSH(TT_DOTS);
			} else PUSH(TT_POINT);
		} else if (c == '|') {
			scanner_scan(sc);
			NEW_TOK(); 
			if (scanner_peek(sc) == '|') {
				scanner_scan(sc);
				PUSH(TT_LOGICOR);
			} else PUSH(TT_BITOR);
		} else if (c == '&') {
			scanner_scan(sc);
			NEW_TOK(); 
			if (scanner_peek(sc) == '&') {
				scanner_scan(sc);
				PUSH(TT_LOGICAND);
			} else PUSH(TT_BITAND);
		} else if (c == '<') {
			scanner_scan(sc);
			NEW_TOK(); 
			if (scanner_peek(sc) == '<') {
				scanner_scan(sc);
				PUSH(TT_LSHIFT);
			} else if (scanner_peek(sc) == '=') {
				scanner_scan(sc);
				PUSH(TT_LESSEQUALS);
			} else PUSH(TT_LESS);
		} else if (c == '>') {
			scanner_scan(sc);
			NEW_TOK(); 
			if (scanner_peek(sc) == '>') {
				scanner_scan(sc);
				PUSH(TT_RSHIFT);
			} else if (scanner_peek(sc) == '=') {
				scanner_scan(sc);
				PUSH(TT_GREATEREQUALS);
			} else PUSH(TT_GREATER);
		} else if (c == '^') {
			scanner_scan(sc);
			NEW_TOK(); PUSH(TT_BITXOR);
		} else {
			scanner_scan(sc);
		}
	}

	scanner_free(sc);
	return tk;
}