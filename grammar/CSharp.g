// SHilScript.g - SS++ prototype ANTLR parser/lexer grammar

header {
	using SS.Runtime;
	using SS.AST;
	using System.Collections.Generic;
}

//----------------------------------------------------------------------------
// GLOBAL SETTINGS
//----------------------------------------------------------------------------

options {
	language="CSharp";
}

//----------------------------------------------------------------------------
// PARSER
//----------------------------------------------------------------------------

class SSParser extends Parser;

options {
	k = 2;
}

testme returns [Expr e = null]: e=expr ;

translationUnit
	returns [List<ClassDef> classes = new List<ClassDef>()]
	{ClassDef c; }
	: ( c=classDef { classes.Add(c); } )* 
	;
		
classDef returns [ClassDef cd = new ClassDef()]
	{Node m; }
	:  "class" classname:ID { cd.Name = classname.getText(); }
		("extends" basename:ID {
			NamedTypeExpr te = new NamedTypeExpr();
			te.name = new DottedName(basename.getText());
			cd.Base = te;
		 } )?
		LBRACE
		(
			m=classMember
			{ 
				if(m is MethodDef)
					cd.Methods.Add((MethodDef)m);
				else if(m is PropertyDef)
					cd.Properties.Add((PropertyDef)m);
			} 
		)*
		RBRACE
	;
	
classMember
	returns [Node n = null]
	: (funcSig[null]) => n=funcDef
	| n=variableDef
	;
		
funcDef
	returns [MethodDef meth = new MethodDef()]
	{
		Statement body;
	}
	: funcSig[meth] (blockStmt { /*meth.Body = body;*/ } | SEMI)
	;
	
funcSig[MethodDef meth]
	{
		MethodFlags flags = 0;
		/*TypeExpr returntype;*/
		List<ParamDef> paramDefs;
	}
	: (
		"static" {flags |= MethodFlags.Static; }
		| "abstract" { flags |= MethodFlags.Abstract; }
		)*
		typeOrVoid
		methname:ID
		LPAREN paramDefs=paramList RPAREN
		
		{
			if(meth != null)
			{
				meth.Flags = flags;
				/*meth.ReturnType = returntype;*/
				meth.Name = methname.getText();
				meth.Params = paramDefs;
			}
		}
	;
	
// TODO: Blah, unify with other modifier stuff?
modifier
	: "public"
	| "protected"
	| "private"
	| "transient"
	| "static"
	| "config"
	| "localized"
	| "const"
	| "abstract"
	;
	
paramList
	returns [List<ParamDef> paramList = new List<ParamDef>() ]
	{
		ParamDef p;
	}
	: p=param { paramList.Add(p); }
	  (
		COMMA param { paramList.Add(p); } 
	  )*
	|
	;
	
param
	returns [ParamDef param = new ParamDef();]
	{
		TypeExpr paramType;
	}
	: paramType=type
	  name:ID
	  {
		param.Type = paramType;
		param.Name = name.getText();
	  }
	  /*(ASSIGN expr)?*/	  
	;
	
/* TODO */
variableDef
	returns [PropertyDef def = new PropertyDef()]
	: (modifier)* type variableNameAndInit (COMMA variableNameAndInit)* SEMI
	;
	
variableNameAndInit
	: ID (ASSIGN expr)?
	;
	
//----------------------------------------------------------------------------
// Expressions
//----------------------------------------------------------------------------
	
// This is a simpler way than using 'assignExpr' in other rules...
expr
	returns [Expr e = null]
	: e=assignExpr
	;
	
// Matches +=, *=, etc. and returns the appropriate
// BinaryOp (e.g. Add, Multiply) that corresponds with
// that type of modification-and-assignment.
assignOp
	returns [BinaryOp op = BinaryOp.Assign ]
	: ASSIGN { op = BinaryOp.Assign; }
	| PLUS_EQ { op = BinaryOp.Add; }
	| MINUS_EQ { op = BinaryOp.Subtract; }
	| MUL_EQ { op = BinaryOp.Multiply; }
	| DIV_EQ { op = BinaryOp.Divide; }
	| MODULO_EQ { op = BinaryOp.Remainder; }
	| AND_EQ { op = BinaryOp.BitAnd; }
	| PIPE_EQ { op = BinaryOp.BitOr; }
	| CARET_EQ { op = BinaryOp.BitXor; }
	| LOGICAL_AND_EQ { op = BinaryOp.LogicalAnd; }
	| LOGICAL_OR_EQ { op = BinaryOp.LogicalOr; }
	| LSHIFT_EQ { op = BinaryOp.ShiftLeft; }
	| RSHIFT_EQ { op = BinaryOp.ShiftRight; }
	;
	
// Assignment expressions like a=b, a+=b, etc.
// These are different IR classes (AssignExpr and AssignOpExpr) but we can
// conveniently handle them both with the same rule.
// These are right-associative.
assignExpr
	returns [Expr e = null]
	{
		Expr right;
		BinaryOp op;
	}
	: e=trinaryExpr
		(
			op=assignOp
			right=assignExpr
			
			{
				if(op == BinaryOp.Assign)
					e = new AssignExpr(e, right);
				else
					e = new AssignOpExpr(op, e, right);
			}
		)?
	;
	
// This is the conditional operator, a ? b : c.
// It is right associative.
trinaryExpr
	returns [Expr e = null]
	{
		Expr ifTrue, ifFalse;	
	}
	: e=logicalOrExpr
	  (
		QUESTION ifTrue=expr COLON ifFalse=trinaryExpr
		{
			e = new TrinaryExpr(e, ifTrue, ifFalse);
		}
	  )?
	;
	
// The logical OR operator, a || b.
// This is left associative.
logicalOrExpr
	returns [Expr e = null]
	{
		Expr right;
	}
	: e=logicalAndExpr
	  (
	    LOGICAL_OR right=logicalAndExpr
		
		{ e = new BinaryExpr(BinaryOp.LogicalOr, e, right); }
	  )*
	;
	
// The logical AND operator, a && b.
// This is left associative.
logicalAndExpr
	returns [Expr e = null]
	{
		Expr right;
	}
	: e=bitwiseOrExpr
	  (
	    LOGICAL_AND right=bitwiseOrExpr
		
		{ e = new BinaryExpr(BinaryOp.LogicalAnd, e, right); }
	  )*
	;
	
// The bitwise OR operator, a | b.
// This is left associative.
bitwiseOrExpr
	returns [Expr e = null]
	{
		Expr right;
	}
	: e=bitwiseXorExpr
	  (
	    PIPE right=bitwiseXorExpr
		
		{ e = new BinaryExpr(BinaryOp.BitOr, e, right); }
	  )*
	;
	
// The bitwise XOR operator, a ^ b.
// This is left associative.
bitwiseXorExpr
	returns [Expr e = null]
	{
		Expr right;
	}
	: e=bitwiseAndExpr
	  (
	    CARET right=bitwiseAndExpr
		
		{ e = new BinaryExpr(BinaryOp.BitXor, e, right); }
	  )*
	;
	
// The bitwise AND operator, a & b.
// This is left associative.
bitwiseAndExpr
	returns [Expr e = null]
	{
		Expr right;
	}
	: e=equalityExpr
	  (
	    AND right=equalityExpr
		
		{ e = new BinaryExpr(BinaryOp.BitAnd, e, right); }
	  )*
	;
				
// Equality testing operators, a == b and a != b.
// These are left-associative (not that they'd get used in strings that much anyway.)
equalityExpr
	returns [Expr e = null]
	{
		BinaryOp op = BinaryOp.Equals;
		Expr right;
	}
	: e=comparisonExpr
	  (
	   (EQUALS_EQUALS { op = BinaryOp.Equals; }
	    | NOT_EQ { op = BinaryOp.NotEquals; }
	    )
		right=comparisonExpr
		
		{ e = new BinaryExpr(op, e, right); }
	  )*
	;
		
// The comparison operators, a < b, a <= b, a > b, a >= b, etc.
// These are left-associative (not that they'd get used in strings that much anyway.)
comparisonExpr
	returns [Expr e = null]
	{
		BinaryOp op = BinaryOp.Equals;
		Expr right;
	}
	: e=shiftExpr
	  (
	   (LANGLE { op = BinaryOp.LessThan; }
		| RANGLE { op = BinaryOp.GreaterThan; }
		| LESS_EQ { op = BinaryOp.LessThanEqual; }
		| GREATER_EQ { op = BinaryOp.GreaterThanEqual; }
	    )
		right=shiftExpr
		
		{ e = new BinaryExpr(op, e, right); }
	  )*
	;
		
// Bit-shift expressions, a << b and a >> b.
// Left-associative.
shiftExpr
	returns [Expr e = null]
	{
		BinaryOp op = BinaryOp.Equals;
		Expr right;
	}
	: e=additiveExpr
	  (
	   (LSHIFT { op = BinaryOp.ShiftLeft; }
		| RSHIFT { op = BinaryOp.ShiftRight; }
	    )
		right=additiveExpr
		
		{ e = new BinaryExpr(op, e, right); }
	  )*
	;
		
// Addition-level expressions, a + b and a - b.
// Left-associative.
additiveExpr
	returns [Expr e = null]
	{
		BinaryOp op = BinaryOp.Equals;
		Expr right;
	}
	: e=multiplicationExpr
	  (
	   (PLUS { op = BinaryOp.Add; }
		| MINUS { op = BinaryOp.Subtract; }
	    )
		right=multiplicationExpr
		
		{ e = new BinaryExpr(op, e, right); }
	  )*
	;
	
// Multiplication-level expressions: a * b, a / b, a % b
// Left-associative.
// TODO: Perhaps 'cross' and 'dot' can fit in here?
multiplicationExpr
	returns [Expr e = null]
	{
		BinaryOp op = BinaryOp.Equals;
		Expr right;
	}
	: e=exponentExpr
	  (
	   (STAR { op = BinaryOp.Multiply; }
		| DIV { op = BinaryOp.Divide; }
		| MODULO { op = BinaryOp.Remainder; }
	    )
		right=exponentExpr
		
		{ e = new BinaryExpr(op, e, right); }
	  )*
	;

// Exponentiation (power) operator, a ** b.
// Right-associative.
exponentExpr
	returns [Expr e = null]
	{
		Expr right;
	}
	: e=prefixUnaryExpr
	  (
	   POWER right=exponentExpr
		
		{ e = new BinaryExpr(BinaryOp.Exponent, e, right); }
	  )?
	;

// TODO: Just throw out C-style casts because they're annoying?
// TODO: Speed this up more cleverly somehow?
	
prefixUnaryExpr
	returns [Expr e = null]
	{
		UnaryOp op = UnaryOp.LogicalNot;
	}		
	: 
	
	  // TODO
	  (LPAREN fqType RPAREN prefixUnaryExpr)=> LPAREN fqType RPAREN prefixUnaryExpr
	| (
		  NOT			{ op = UnaryOp.LogicalNot; }
		  | COMPLEMENT	{ op = UnaryOp.Complement; }
		  | INCREMENT	{ op = UnaryOp.PreIncrement; }
		  | DECREMENT	{ op = UnaryOp.PreDecrement; }
		  | MINUS		{ op = UnaryOp.Negate; }
		  | PLUS		{ op = UnaryOp.Positive; }
	  )
	  e=prefixUnaryExpr
	  {
		e = new UnaryExpr(op, e);
	  } 
	|  e=postfixUnaryExpr
	;
	

// Postfix unary expressions: a++, a--, a.b, a[b], and fn(...).
// These are left associative.
postfixUnaryExpr
	returns [Expr e = null]
	{
		Expr right;
	}
	: e=primaryExpr
		( INCREMENT { e = new UnaryExpr(UnaryOp.PostIncrement, e); }
		| DECREMENT { e = new UnaryExpr(UnaryOp.PostDecrement, e); }
		| (DOT membername:ID) { e = new MemberExpr(e, membername.getText()); }
		| (LPAREN expr RPAREN) /* TODO: Proper parameter list behavior */
		| (LBRACKET right=expr RBRACKET) { e = new IndexExpr(e, right); }
		)*
	;

// TODO: I think we have the order wrong for if we go (type)func(params)

primaryExpr
	returns [Expr e = null]
	: idname:ID { e = new NameExpr(idname.getText()); }
	| e=literal
	| LPAREN e=expr RPAREN
	;
	
/* TODO !!! */
literal
	returns [Expr e = null]
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
	
//----------------------------------------------------------------------------
// Type expressions
//----------------------------------------------------------------------------

// TODO: q instead of fq (we don't know if it's from root?)
fqType
	: ID (DOT ID)*
	;
	
type
	returns [TypeExpr e = null]
	: builtinType
	| "list" LANGLE type RANGLE
	| "dict" LANGLE type COMMA type RANGLE
	| "class" (LANGLE fqType RANGLE)?
	| fqType
	;
	
typeOrVoid
	returns [TypeExpr t = null]
	: t=type |
		"void" { t = new BuiltinTypeExpr(BuiltinTypeID.Void); }
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
	
//----------------------------------------------------------------------------
// Statements
//----------------------------------------------------------------------------

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
	  ) {  _ttype = Token.SKIP; }
	;

MultilineComment
	: "/*" 
		(
			{ LA(2) != '/' /* lol */ }? '*'
			| ("\r\n")=>"\r\n" { newline(); }
			| ('\r' | '\n') { newline(); }
			| ~('*' | '\r' | '\n')
		)*
	   "*/" { _ttype = Token.SKIP; }
	;
	
LineComment
	: "//" (~('\n' | '\r'))* { _ttype = Token.SKIP; }
	;
	
HashLineComment
	: "#" (~('\n' | '\r'))* { _ttype = Token.SKIP; }
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
POWER:			"**";
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
