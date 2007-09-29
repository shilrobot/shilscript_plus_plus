// SHilScript.g - SS++ prototype ANTLR parser/lexer grammar

header {
using antlr::Token;
}

//----------------------------------------------------------------------------
// GLOBAL SETTINGS
//----------------------------------------------------------------------------

options {
	language="Cpp";
}

//----------------------------------------------------------------------------
// PARSER
//----------------------------------------------------------------------------


class SSParser extends Parser;

options {
	k = 2;
	//buildAST = true;
}

translationUnit 
	: (classDef | structDef | enumDef)* EOF
	;
	
// TODO: delegates?
	
// TODO: Class modifiers
classDef
	: (classModifier)* "class" ID
		("extends" ID)?
		("implements" ID (COMMA ID)*)?
		LBRACE (classMember)* RBRACE
	;
	
classModifier
	: "public"
	| "private"
	| "final"
	| "abstract"
	;
	
// TODO: Access modifiers for structs?
structDef
	: "struct" ID LBRACE (variableDef)* RBRACE
	;
	
// TODO: Access modifiers for enums?
enumDef
	: "enum" ID LBRACE enumItemList RBRACE
	;
	
enumItemList
	: enumItem (COMMA enumItem)* (COMMA)?
	;
	
enumItem
	: ID (ASSIGN expr)?
	;

accessModifier
	: "public"
	| "protected"
	| "private"
	;
	
classMember
	: (funcSig) => funcDef
	| variableDef
	| eventDef
	| defaultDef
	| onDef
	;
	
eventDef
	: "event"^ ID LPAREN paramList RPAREN SEMI
	;
	
funcDef
	: funcSig (blockStmt | SEMI)
	;
	
funcSig
	: (modifier)* typeOrVoid ID LPAREN paramList RPAREN
	;
	
// TODO: Blah, unify with other modifier stuff?
modifier
	: accessModifier
	| "transient"
	| "static"
	| "config"
	| "localized"
	| "const"
	| "abstract"
	;
	
paramList
	: param (COMMA param)*
	|
	;
	
param
	: ("ref"|"out")? type ID (ASSIGN expr)?
	;
	
variableDef
	: (modifier)* type variableNameAndInit (COMMA variableNameAndInit)* SEMI
	;
	
variableNameAndInit
	: ID (ASSIGN expr)?
	;
	
defaultDef
	: ID ASSIGN expr SEMI
	;
	
onDef
	: "on" eventSpec LPAREN paramList RPAREN blockStmt
	;
	
eventSpec
	: ID (DOT ID)*
	;
	
	
expr
	: assignExpr
	;
	
assignOp
	: ASSIGN
	| PLUS_EQ
	| MINUS_EQ
	| MUL_EQ
	| DIV_EQ
	| MODULO_EQ
	| AND_EQ
	| PIPE_EQ
	| XOR_EQ
	| LOGICAL_AND_EQ
	| LOGICAL_OR_EQ
	| LSHIFT_EQ
	| RSHIFT_EQ
	;
	
assignExpr
	: ternaryExpr (assignOp assignExpr)?
	;
	
ternaryExpr
	: logicalOrExpr (QUESTION expr COLON ternaryExpr)?
	;
	
logicalOrExpr
	: logicalAndExpr (LOGICAL_OR logicalAndExpr)*
	;
	
logicalAndExpr
	: bitwiseOrExpr (LOGICAL_AND bitwiseOrExpr)*
	;
	
bitwiseOrExpr
	: bitwiseXorExpr (PIPE bitwiseXorExpr)*
	;
	
bitwiseXorExpr
	: bitwiseAndExpr (XOR bitwiseAndExpr)*
	;
	
bitwiseAndExpr
	: equalityExpr (AND equalityExpr)*
	;
	
equalityExpr
	: comparisonExpr ((EQUALS_EQUALS | NOT_EQ) comparisonExpr)*
	;
	
comparisonExpr
	: shiftExpr ((LANGLE | RANGLE | LESS_EQ | GREATER_EQ) shiftExpr)*
	;
	
shiftExpr
	: additiveExpr ((LSHIFT | RSHIFT) additiveExpr)*
	;
	
additiveExpr
	: multiplicationExpr ((PLUS | MINUS) multiplicationExpr)*
	;

multiplicationExpr
	: exponentExpr ((STAR | DIV | MODULO | "cross" | "dot") exponentExpr)*
	;
	
exponentExpr
	: prefixUnaryExpr (STAR_STAR prefixUnaryExpr)*
	;

// TODO: Just throw out C-style casts because they're annoying?
// TODO: Speed this up more cleverly somehow?
	
prefixUnaryExpr
	: (LPAREN fqType RPAREN prefixUnaryExpr)=> LPAREN fqType RPAREN prefixUnaryExpr
	| (NOT|COMPLEMENT|INCREMENT|DECREMENT|MINUS|PLUS) prefixUnaryExpr
	| postfixUnaryExpr
	;
	

postfixUnaryExpr
	: primaryExpr
		( INCREMENT
		| DECREMENT
		| (DOT ID)
		| (LPAREN expr RPAREN) // TODO: Proper parameter list behavior
		| (LBRACKET expr RBRACKET)
		)*
	;

// TODO: I think we have the order wrong for if we go (type)func(params)

primaryExpr
	: ID
	| literal
	| LPAREN expr RPAREN
	;
	
literal
	: HexIntLiteral
	| OctalIntLiteral
	| DecimalIntLiteral
	| FloatLiteral
	| (StringLiteral)+
	| CharLiteral
	| "true"
	| "false"
	| "this"
	| "null"
	| "global"
	| "super"
	;
	
// TODO: q instead of fq (we don't know if it's from root?)
fqType
	: ID (DOT ID)*
	;
	
type
	: builtinType
	| "list" LANGLE type RANGLE
	| "dict" LANGLE type COMMA type RANGLE
	| "class" (LANGLE fqType RANGLE)?
	| fqType
	;
	
typeOrVoid
	: type | "void"
	;
	
builtinType
	: "bool"
	| "byte"
	| "sbyte"
	| "ushort"
	| "short"
	| "uint"
	| "int"
	| "ulong"
	| "long"
	| "float"
	| "double"
	| "char"
	| "string"
	;
	
statement
	: (variableDefStmt) => variableDefStmt
	| returnStmt
	| breakStmt
	| continueStmt
	| switchStmt
	| forStmt
	| forEachStmt
	| whileStmt
	| doWhileStmt
	| tryStmt
	| throwStmt
	| blockStmt
	| ifStmt
	| exprStmt
	| emptyStmt
	;
	
returnStmt
	: "return" (expr)? SEMI
	;
	
breakStmt
	: "break" SEMI
	;
	
continueStmt
	: "continue" SEMI
	;
	
switchStmt
	: "switch" LPAREN expr RPAREN
		LBRACE (switchCond)* RBRACE
	;
	
switchCond
	: "case" expr COLON (statement)*
	| "default" COLON (statement)*
	;
	
forStmt
	: "for" LPAREN
		((variableDefStmt) => variableDefStmt | exprStmt | emptyStmt)
		(expr)? SEMI
		(expr)? RPAREN
		statement
	;
	
forEachStmt
	: "foreach" LPAREN type ID "in" expr RPAREN statement
	;
	
whileStmt
	: "while" LPAREN expr RPAREN statement
	;
	
doWhileStmt
	: "do" statement "while" LPAREN expr RPAREN SEMI
	;
	
// TODO: Should we replicate what's in variableDef or not?
variableDefStmt
	: variableDef
	;
	
// TODO: Why can't we use normal statements instead of block statements here?
tryStmt
	: "try" blockStmt
		("catch" catchCond blockStmt)*
		("finally" blockStmt)?
	;
	
catchCond
	: (LPAREN) => LPAREN type (ID)? RPAREN
	|
	;
	
throwStmt
	: "throw" expr SEMI
	;
	
ifStmt
	: "if" LPAREN expr RPAREN statement
	  // We know about the 'if(x) if(y) Z; else W;' ambiguity...
	  (
	  	options { warnWhenFollowAmbig = false; } :
	    "else" statement
	  )?
	 ;

blockStmt
	: LBRACE (statement)* RBRACE
	;
	
exprStmt
	: expr SEMI
	;
	
emptyStmt
	: SEMI
	;

	
// TODO: notnull modifier?

//----------------------------------------------------------------------------
// LEXICAL ANALYZER
//----------------------------------------------------------------------------

class SSLexer extends Lexer;

options {
	k=3;
	testLiterals = false;
	caseSensitiveLiterals = false;
}

Whitespace
	: (
		(' ' | '\t')
		| "\r\n" { newline(); }
		| ('\r' | '\n') { newline(); }
	  ) {  _ttype = Token::SKIP; }
	;

MultilineComment
	: "/*" 
		(
			{ LA(2) != '/' /* lol */ }? '*'
			| ("\r\n")=>"\r\n" { newline(); }
			| ('\r' | '\n') { newline(); }
			| ~('*' | '\r' | '\n')
		)*
	   "*/" { _ttype = Token::SKIP; }
	;
	
LineComment
	: "//" (~('\n' | '\r'))* { _ttype = Token::SKIP; }
	;
	
HashLineComment
	: "#" (~('\n' | '\r'))* { _ttype = Token::SKIP; }
	;

LBRACE: 		'{';
RBRACE: 		'}';
LPAREN: 		'(';
RPAREN: 		')';
LBRACKET:		'[';
RBRACKET:		']';
LANGLE: 		'<';
RANGLE: 		'>';
PLUS: 			'+';
MINUS:			'-';
STAR: 			'*';
DIV: 			'/';
MODULO: 		'%';
AND: 			'&';
PIPE: 			'|';
CARET: 			'^';
LOGICAL_AND: 	"&&";
LOGICAL_OR: 	"||";
LSHIFT: 		"<<";
RSHIFT: 		">>";
POWER: 			"**";
PLUS_EQ: 		"+=";
MINUS_EQ:		"-=";
STAR_EQ: 		"*=";
DIV_EQ: 		"/=";
MODULO_EQ: 		"%=";
AND_EQ: 		"&=";
PIPE_EQ: 		"|=";
CARET_EQ: 		"^=";
LOGICAL_AND_EQ:	"&&=";
LOGICAL_OR_EQ:	"||=";
LSHIFT_EQ: 		"<<=";
RSHIFT_EQ: 		">>=";
POWER_EQ: 		"**=";
ASSIGN: 		'=';
EQUALS_EQUALS:	"==";
NOT_EQ: 		"!=";
LESS_EQ: 		"<=";
GREATER_EQ: 	">=";
NOT: 			'!';
COMPLEMENT: 	'~';
INCREMENT: 		"++";
DECREMENT: 		"--";
QUESTION: 		'?';
COMMA: 			',';
COLON: 			':';
SEMI: 			';';

protected DOT:;

protected HexIntLiteral:;

protected OctalIntLiteral:;

protected DecimalIntLiteral:;

protected FloatLiteral:;

protected Sign: ('+'|'-');
protected Digit: ('0'..'9');
protected NonZeroDigit: ('1'..'9');
protected OctalDigit: ('0'..'7');
protected HexDigit: ('0'..'9'|'a'..'f'|'A'..'F');

protected Exponent
	: ('e'|'E') (Sign)? (Digit)+
	;

Numerical
	: ((Digit)+ '.') =>
		((Digit)+ '.' (Digit)* (Exponent)?)	{ _ttype = FloatLiteral; }
		
	| ((Digit)+ ('e'|'E')) =>
		((Digit)+ (Exponent))	{ _ttype = FloatLiteral; }
		
	| '.' { _ttype = DOT; }
	  ((Digit)+ (Exponent)? { _ttype = FloatLiteral; } )? 
	 
    | '0' (OctalDigit)* { _ttype = OctalIntLiteral; }
    
    | NonZeroDigit (Digit)* { _ttype = DecimalIntLiteral; }
    
    | '0' ('x'|'X') (HexDigit)+ { _ttype = HexIntLiteral; }
		
	;
	
ID
	options { testLiterals = true; }
	: ('a'..'z' | 'A'..'Z' | '_') ('a'..'z' | 'A'..'Z' | '_' | '0'..'9')*
	;

StringLiteral
	: '"' 
	  (('\\' Escape) | ~('\\' | '"' | '\r' | '\n'))*
	  '"'
	;
	
CharLiteral
	: '\''
	  (('\\' Escape) | ~('\\' | '\'' | '\r' | '\n'))
	  '\''
	;
	
protected Escape
	: ('t'|'T')
	| ('r'|'R')
	| ('n'|'N')
	| '"'
	| '\''
	| '\\'
	| ('x'|'X') HexDigit HexDigit
	| ('u'|'U') HexDigit HexDigit HexDigit HexDigit
	| ('0'..'3' OctalDigit OctalDigit) => '0'..'3' OctalDigit OctalDigit
	| (OctalDigit OctalDigit) => OctalDigit OctalDigit
	| OctalDigit
	;
