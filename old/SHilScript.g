// Quick simple ANTLR 3 grammar

grammar SHilScript;

options
{
	output=AST;
}

tokens
{
	FILE;
	CLASS;
	BASES;
	MODIFIERS;
}

file	:	classDef* EOF -> ^(FILE classDef*)
	;
	
classDef
scope
{
	String nameText;
}	:	modifier*
		'class' name=ID
		(':' bases+=ID (',' bases+=ID)*)?
		'{'
		'}'
		{$classDef::nameText = $name.text;}
		-> ^(CLASS $name ^(BASES $bases*) ^(MODIFIERS modifier*))
	;
	
modifier:	'public'
	|	'private'
	|	'protected'
	|	'sealed'
	|	'native'
	;
	
literal : STRING_LITERAL
	| CHAR_LITERAL
	| INTEGER_LITERAL
	| FLOAT_LITERAL
	;
	
literalTest 
	: INTEGER_LITERAL+ EOF
	;
	
//----------------------------------------------------------------------------
// SCANNER
//----------------------------------------------------------------------------
		
// An identifier (normal C rules)
ID	:	Letter (Digit|Letter)*
	;

// Simple integer literal
INTEGER_LITERAL
	:	Digit+
	;
	
// Floating point literal
FLOAT_LITERAL
	:	Digit+ '.' Digit* Exponent?
	|	'.' Digit+ Exponent?
	| 	Digit+ Exponent
	;
	
fragment
Exponent:	('e'|'E') ('+'|'-')? Digit+
	;

// String literal, C++ style
STRING_LITERAL
	:	DblQuote 
		((Backslash Escape) | ~(Backslash | DblQuote | '\r' | '\n'))*
		DblQuote
	;
	
// Character literal, like C++
CHAR_LITERAL
	:	SingleQuote 
		((Backslash Escape) | ~(Backslash | SingleQuote | '\r' | '\n'))
		SingleQuote
	;		
	
// Multiline comment
// Greediness turned off so we terminate as soon as possible.
// I'm not 100% sure this is necessary, however!
MULTILINE_COMMENT
	//:   '/*' (options {greedy=false;} : .)* '*/' {$channel=HIDDEN;}
	: '(' .* ')' {$channel = HIDDEN;}
   	;
   		
// Line comments can either start with // or #
// # has a proud tradition as a comment in scripts :>
LINE_COMMENT	: ('//'|'#') ~('\n'|'\r')* {$channel = HIDDEN;}
		;
	
// Eat some whitespace.
// TODO: Anything special to do for newlines?
WS	:	(' '|'\n'|'\r'|'\t')* {$channel = HIDDEN;}
	;
	
// Part of the escape sequence after the backslash
fragment Escape
	: ('t'|'T')
	| ('r'|'R')
	| ('n'|'N')
	| DblQuote
	| SingleQuote
	| Backslash
	| ('x'|'X') HexDigit HexDigit
	| ('u'|'U') HexDigit HexDigit HexDigit HexDigit
	| ('0'..'3' OctalDigit OctalDigit) => '0'..'3' OctalDigit OctalDigit
	| (OctalDigit OctalDigit) => OctalDigit OctalDigit
	| OctalDigit
	;
	
// Easier-to-read (arguably) aliases
fragment DblQuote : '\"';
fragment SingleQuote : '\'';
fragment Backslash : '\\';

// Easier-to-type aliases
fragment Letter: ( 'a'..'z'|'A'..'Z'|'_');
fragment Digit: ('0'..'9');
fragment NonZeroDigit: ('1'..'9');
fragment OctalDigit: ('0'..'7');
fragment HexDigit: ('0'..'9'|'a'..'f'|'A'..'F');
