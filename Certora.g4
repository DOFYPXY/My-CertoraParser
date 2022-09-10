grammar Certora;

root: methods definition* rulespec*;

type:
	'uint256'
	| 'int256'
	| 'address'
	| 'bool'
	| 'env'
	| 'string'
	| 'bytes32'
	| 'mathint';
methods: METHODS '{' methodSpec* '}';
methodSpec:
	ID '(' param_types = types? ')' (
		RETURNS '(' ret_types = types ')'
	)? ENVFREE?;
types: type (',' type)*;

definition:
	'definition' ID ('(' params? ')')? RETURNS type '=' expr ';';

rulespec: RULE ID '(' params? ')' '{' stmt* '}';
param: type ID;
params: param (',' param)*;

stmt:
	type ID ('=' expr)? ';'
	| REQUIRE '(' expr ')' ';'
	| ASSERT '(' expr (',' STRING)? ')' ';'
	| lhs '=' expr ';'
	| fnCall ';';

lhs: ID (',' ID)*;

expr:
	CONST
	| ID
	| '(' prth_expr = expr ')'
	| obj_expr = expr '.' ID
	| fnCall
	| UOP expr
	| le = expr OP_POW re = expr
	| le = expr OP_MUL re = expr
	| le = expr OP_ADD re = expr
	| le = expr OP_SH re = expr
	| le = expr OP_REL re = expr
	| le = expr OP_EQ re = expr
	| le = expr OP_BIT re = expr
	| le = expr OP_LOGIC re = expr
	| le = expr OP_INF re = expr
	| cond = expr '?' te = expr ':' fe = expr;

fnCall: ID ('@' method_tag = ID)? '(' exprs? ')';
exprs: expr (',' expr)*;

// lexer
METHODS: 'methods';
RETURNS: 'returns';
ENVFREE: 'envfree';
REQUIRE: 'require';
ASSERT: 'assert';
RULE: 'rule';

NEWLINE: ('\r'? '\n' | '\r')+ -> skip;

WHITESPACE: ' ' -> skip;

ID: [a-zA-Z_][a-zA-Z_0-9]*;

fragment DECIMAL: [1-9][0-9]*;
fragment OCTAL: '0' [0-7]*;
fragment HEXADECIMAL: '0' [xX][0-9a-fA-F]+;
CONST: DECIMAL | OCTAL | HEXADECIMAL;

BLOCK_COMMENT: '/*' .*? '*/' -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;
fragment ESC: '\\"' | '\\\\';
STRING: '"' ( ESC | ~[\\"\r\n])* '"';

// UOP: '~' | '-' | '!';
UOP: '~' | '!';
OP_POW: '^';
OP_MUL: '*' | '/' | '%';
OP_ADD: '+' | '-';
OP_SH: '<<' | '>>' | '>>>';
OP_REL: '<' | '>' | '<=' | '>=';
OP_EQ: '==' | '!=';
OP_BIT: '&' | '|' | 'xor';
OP_LOGIC: '&&' | '||';
OP_INF: '=>' | '|=>';