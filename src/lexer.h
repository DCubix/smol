#ifndef LEXER_H
#define LEXER_H

#include "scanner.h"

enum TokenType {
	TT_SEMICOLON = 0,
	TT_ID,
	TT_NUMBER,
	TT_STRING,
	TT_BOOL,
	TT_LBRACE,
	TT_RBRACE,
	TT_KEYWORD,
	TT_LPAREN,
	TT_RPAREN,
	TT_RBRACKET,
	TT_LBRACKET,
	TT_QUESTION,
	TT_COLON,
	TT_DOTS,
	TT_BITOR,
	TT_BITAND,
	TT_BITXOR,
	TT_LSHIFT,
	TT_RSHIFT,
	TT_PLUS,
	TT_MINUS,
	TT_ASTERISK,
	TT_SLASH,
	TT_PERCENT,
	TT_EXCLAMATION,
	TT_BITNOT,
	TT_COMMA,
	TT_POINT,
	TT_EQUALS,
	TT_LESS,
	TT_GREATER,
	TT_LESSEQUALS,
	TT_GREATEREQUALS,
	TT_PLUSEQUALS,
	TT_MINUSEQUALS,
	TT_MULEQUALS,
	TT_DIVEQUALS,
	TT_COMPEQUALS,
	TT_COMPNOTEQUALS,
	TT_LOGICOR,
	TT_LOGICAND,
	TT_EOF
};

extern const char* TOKENS[];

#define LEX_MAX_LEXEME_SIZE 65536
typedef struct Token_t {
	char* lexeme;
	int type;
	int line, column;
} Token;

extern void token_init(Token* tok);

extern void print_token(Token tok);
extern int lexer_lex(const char* input, Token** out);

#endif // LEXER_H