grammar smol;

program : stmtList | funDeclStmt*;

block : stmt ';' | '{' stmtList '}';
stmtList : (stmt ';')*;

stmt
	: 'return' expr?
	| 'continue'
	| 'break'
	| letStmt
	| ifStatement
	| forStatement
	;

forStatement : 'for' args 'in' expr block;

ifStatement : ifStmt elifStmt* elseStmt?;

ifStmt : 'if' expr block;

elifStmt : 'elif' expr block;

elseStmt : 'else' block;

funDeclStmt : 'fun' ID '(' argsInit ')' block;

letStmt : 'let' argsInit;

assignment : expr ASSIGN expr | expr;

expr : logicOR '?' expr ':' expr | logicOR;

logicOR : logicAND 'or' expr | logicAND;

logicAND : bitOR 'and' logicAND | bitOR;

bitOR : bitXOR '|' bitOR | bitXOR;

bitXOR : bitAND '^' bitXOR | bitAND;

bitAND : comparison '&' bitAND | comparison;

comparison : shifts COMPARE comparison | shifts;

shifts : addSub ('<<' | '>>') shifts | addSub;

addSub : mulDiv ('+' | '-') addSub | mulDiv;

mulDiv : factor ('*' | '/' | '%') mulDiv | factor;

factor : ('-' | '+' | '!' | '~')? trail;

trail : atom trailer*;

trailer
	: '[' expr? ']'
	| '(' funArgs? ')'
	| '.' ID
	;

args : ID (',' ID)*;

funArgs : expr (',' expr)*;

argAssign : ID ('=' expr)?;

argsInit : argAssign (',' argAssign)*;

atom
	: NUMBER
	| ID
	| STRING
	| BOOL
	| '(' expr ')'
	| '[' funArgs? ']'
	;

ID : [a-zA-Z_][a-zA-Z_0-9]*;

NUMBER
	: [0-9]+
	| [0-9]* '.' [0-9]+
	| ('0x' | '0X') [0-9a-fA-F]+
	| ('0o' | '0O') [0-7]+
	| ('0b' | '0B') [0-1]+
	;

BOOL : 'true' | 'false';

STRING : '\'' ( ESCAPE | ~( '\\'|'\n'|'\'' ) )* '\'';

ESCAPE : '\\' .;

WS : [ \n\r\t\f] -> skip;

ASSIGN
	: ('=' | '+=' | '-=' | '*=' | '/=')
	;

COMPARE
	: ('==' | '!=' | '>' | '<' | '>=' | '<=')
	;