#ifndef AST_H
#define AST_H

#include "lexer.h"

#define AST_NODE_CHILDREN_CAPACITY 10

typedef struct Node_t {
	enum {
		NT_UNKNOWN = 0,
		NT_NUMBER,
		NT_STRING,
		NT_BOOL,
		NT_LIST,

		NT_UNARY_MINUS,
		NT_UNARY_BITNOT,
		NT_UNARY_NOT,

		NT_BINARY_ADD,
		NT_BINARY_SUB,
		NT_BINARY_MUL,
		NT_BINARY_DIV,
		NT_BINARY_MOD,

		NT_BINARY_LSH,
		NT_BINARY_RSH,
		NT_BINARY_GREATER,
		NT_BINARY_LESS,
		NT_BINARY_GREATEREQUALS,
		NT_BINARY_LESSEQUALS,
		NT_BINARY_EQUALITY,
		NT_BINARY_INEQUALITY,

		NT_BINARY_BITAND,
		NT_BINARY_BITXOR,
		NT_BINARY_BITOR,

		NT_BINARY_LOGICAND,
		NT_BINARY_LOGICOR,

		NT_TERNARY,
		NT_ASSIGN,
		NT_ASSIGN_ADD,
		NT_ASSIGN_SUB,
		NT_ASSIGN_MUL,
		NT_ASSIGN_DIV,

		NT_IDENTIFIER,
		NT_ARGS_INIT,
		NT_FUN_ARGS,
		NT_ARGS,

		NT_LET_STMT,
		NT_FUN_DECL_STMT,

		NT_RETURN,
		NT_CONTINUE,
		NT_BREAK,

		NT_IF,
		NT_ELSE,
		NT_ELSEIF,

		NT_STMT_LIST,
		NT_BLOCK,

		NT_IF_STMT,
		NT_FOR_STMT,

		NT_LIST_ACCESS,
		NT_CALL,
		NT_FIELD_ACCESS,

		NT_TRAIL,

		NT_PROGRAM,

		NT_FUN_CALL_STMT

		// TODO: Add More
	} type;

	union {
		double value;
		char* string;
		int boolean;
	};

	struct Node_t** children;
	int childCount;
	int capacity;
} Node;

extern Node* node_new();
extern void node_free(Node* node);
extern void node_push_child(Node* root, Node* child);

typedef struct Parser_t {
	Token* tokens;
	int pos, len;
} Parser;

extern void parser_new(Parser* p, Token* tokens, int numTokens);
extern int parser_accept(Parser* p, int type, const char* lexeme);
extern int parser_expect(Parser* p, int type, const char* lexeme);
extern void parser_advance(Parser* p);
extern Token parser_current(Parser* p);

extern Node* ast_parse_trailer(Parser* p);

extern Node* ast_parse_atom(Parser* p);
extern Node* ast_parse_trail(Parser* p);
extern Node* ast_parse_factor(Parser* p);
extern Node* ast_parse_muldiv(Parser* p);
extern Node* ast_parse_addsub(Parser* p);
extern Node* ast_parse_shifts(Parser* p);
extern Node* ast_parse_comparison(Parser* p);
extern Node* ast_parse_bitand(Parser* p);
extern Node* ast_parse_bitor(Parser* p);
extern Node* ast_parse_bitxor(Parser* p);
extern Node* ast_parse_logicand(Parser* p);
extern Node* ast_parse_logicor(Parser* p);
extern Node* ast_parse_expr(Parser* p);
extern Node* ast_parse_assignment(Parser* p);

extern Node* ast_parse_arg_assign(Parser* p);
extern Node* ast_parse_identifier(Parser* p);
extern Node* ast_parse_args(Parser* p);
extern Node* ast_parse_fun_args(Parser* p);
extern Node* ast_parse_args_init(Parser* p);
extern Node* ast_parse_let_stmt(Parser* p);
extern Node* ast_parse_fun_def_stmt(Parser* p);

extern Node* ast_parse_if(Parser* p);
extern Node* ast_parse_elif(Parser* p);
extern Node* ast_parse_else(Parser* p);

extern Node* ast_parse_if_stmt(Parser* p);
extern Node* ast_parse_for_stmt(Parser* p);

extern Node* ast_parse_fun_call_stmt(Parser* p);

extern Node* ast_parse_stmt(Parser* p);
extern Node* ast_parse_stmt_list(Parser* p);
extern Node* ast_parse_block(Parser* p);

extern Node* ast_parse_program(Parser* p);

extern void ast_print(Node* root, int pad);

#endif // AST_H