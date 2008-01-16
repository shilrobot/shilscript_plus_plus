// SHilScript.g - SS++ ANTLR parser/lexer grammar & code tree constructor

header {
	using antlr::Token;
}

header "pre_include_hpp" {

#include "../Prereqs.h"
#include "../Package.h"
#include "../Class.h"
#include "../TypeExpr.h"
#include "../Function.h"
#include "../Variable.h"
#include "../Expr.h"
#include "../BinaryExpr.h"
#include "../AssignExpr.h"
#include "../UnaryExpr.h"
#include "../LiteralExpr.h"
#include "../CallExpr.h"
#include "../CastExpr.h"
#include "../NameExpr.h"
#include "../TernaryExpr.h"
#include "../MemberExpr.h"
#include "../IndexExpr.h"
#include "../Statement.h"

// ANTLR uses some stuff that VC8 has decided is deprecated
// (e.g., stricmp.)
// #defining _CRT_SECURE_NO_DEPRECATE doesn't seem to help.
// So, we're disabling this warning entirely when including the
// ANTLR headers.
#ifdef SS_COMPILER_MSVC
	#pragma warning(push)
	#pragma warning(disable: 4996) // 'xxx' was declared deprecated
#endif

}

header "post_include_hpp" {

#ifdef SS_COMPILER_MSVC
	#pragma warning(pop)
#endif

}

header "post_include_cpp" {

#ifdef SS_COMPILER_MSVC
	#pragma warning(disable: 4101) // unreferenced local variable
#endif

}

//----------------------------------------------------------------------------
// GLOBAL SETTINGS
//----------------------------------------------------------------------------

options {
	language="Cpp";
	namespace="SS";
	namespaceStd="std";
	namespaceAntlr="antlr";
}

//----------------------------------------------------------------------------
// PARSER
//----------------------------------------------------------------------------

class SSParser extends Parser;

options {
	k = 2;
}

{
	void reportError(const antlr::RecognitionException& ex);
	void reportError(const std::string& s);
	void reportWarning(const std::string& s);
}

translationUnit[Package* pkg]
	{
		Class* c = 0;
	}
	:	(
			c=classDef { pkg->AddClass(c); }
			| SEMI
		)*
		EOF
	;
	
//----------------------------------------------------------------------------
// Class
//----------------------------------------------------------------------------

// TODO: Warnings for multiple modifiers specified (modifier system?)
classDef returns [Class* cls = new Class()]
	{
		TypeExpr* btExpr = 0;
		Function* func = 0;
		Variable* var = 0;
	}
	: (classModifier[cls])?
		"class" cname:ID
		(COLON btExpr=type)?
		
		{
			cls->SetName(cname->getText());
			cls->SetBaseExpr(btExpr);
		}
		
		LBRACE 
		
		(
			(funcSig[0]) => func=funcDef { cls->AddFunction(func); }
			| var=variableDef { cls->AddVariable(var); }
			| SEMI
		)*
		
		RBRACE		
	;
	
// TODO: Errors when multiple modifiers are used! (when we add more besides access ctrl)
classModifier[Class* cls]
	: "public" { cls->SetAccess(ACCESS_PUBLIC); }
	| "private" { cls->SetAccess(ACCESS_PRIVATE); }
	;
				
//----------------------------------------------------------------------------
// Function
//----------------------------------------------------------------------------

// TODO: Because funcsig is in a predicate, actions won't get executed!
//		 So a lot of this null behavior is unnecessary.

funcDef returns [Function* func = new Function()]
	{
		Statement* body = 0;
	}
	:	funcSig[func] (body=blockStmt | SEMI)
	{
		func->SetBody(body);
	}
	;
	
funcSig[Function* func]
	{
		TypeExpr* rtExpr = 0;
	}
	:	(funcModifier[func])* (rtExpr=type)? fname:ID LPAREN paramList[func] RPAREN
	
		{
			if(func!=0)
			{
				if(rtExpr != 0)
					func->SetReturnTypeExpr(rtExpr);
				else
					func->SetReturnTypeExpr(new BasicTypeExpr(BT_VOID));
				func->SetName(fname->getText());
			}
		}
	;
	
// TODO: Warnings for multiple modifiers specified (modifier system?)
funcModifier[Function* func]
	:	"public"		{ if(func!=0) func->SetAccess(ACCESS_PUBLIC); }
	|	"protected"		{ if(func!=0) func->SetAccess(ACCESS_PROTECTED); }
	|	"private"		{ if(func!=0) func->SetAccess(ACCESS_PRIVATE); }
	|	"static"		{ if(func!=0) func->SetStatic(true); }
	|	"virtual"		{ if(func!=0) func->SetVirtual(true); }
	|	"sealed"		{ if(func!=0) func->SetSealed(true); }
	;
	
paramList[Function* func]
	{
		Parameter* p = 0;
	}
	:	p=param 
		{ if(func!=0) func->AddParameter(p); }
		
		(
			COMMA p=param 
			{ if(func!=0) func->AddParameter(p); }
		)*  
	|
	;
	
param returns[Parameter* param = new Parameter()]
	{
		TypeExpr* typ;
	}
	:	typ=type pname:ID
	
		{
			param->SetTypeExpr(typ);
			param->SetName(pname->getText());
		}
	;
	
//----------------------------------------------------------------------------
// Member var
//----------------------------------------------------------------------------

// TODO: Multiple vars w/ one type, e.g. int a,b;
variableDef returns [Variable* var = new Variable()]
	{
		TypeExpr* typ = 0;
		Expr* initExpr = 0;
	}
	:	(varModifier[var])*
		typ=type
		vname:ID
		(ASSIGN initExpr=expr)?
		SEMI
		
		{
			var->SetName(vname->getText());
			var->SetTypeExpr(typ);
			var->SetInitExpr(initExpr);
		}		
	;
	
// TODO: Warnings for multiple modifiers specified (modifier system?)
varModifier[Variable* var]
	:	"public"		{ var->SetAccess(ACCESS_PUBLIC); }
	|	"protected"		{ var->SetAccess(ACCESS_PROTECTED); }
	|	"private"		{ var->SetAccess(ACCESS_PRIVATE); }
	|	"static"		{ var->SetStatic(true); }
	|	"const"			{ var->SetConst(true); }
	;
	
//----------------------------------------------------------------------------
// Expressions
//----------------------------------------------------------------------------

// TODO: Expr system
expr returns[Expr* expr = 0]
	:	expr=assignExpr
	;
	
assignOp returns[BinaryOp op = BINOP_NONE]
	:	ASSIGN			{ op = BINOP_NONE; }
	|	PLUS_EQ			{ op = BINOP_ADD; }
	|	MINUS_EQ		{ op = BINOP_SUB; }
	|	MUL_EQ			{ op = BINOP_MUL; }
	|	DIV_EQ			{ op = BINOP_DIV; }
	|	MODULO_EQ		{ op = BINOP_MOD; }
	|	POWER_EQ		{ op = BINOP_POW; }
	|	AND_EQ			{ op = BINOP_BIT_AND; }
	|	PIPE_EQ			{ op = BINOP_BIT_OR; }
	|	XOR_EQ			{ op = BINOP_BIT_XOR; }
	|	LOGICAL_AND_EQ	{ op = BINOP_LOG_AND; }
	|	LOGICAL_OR_EQ	{ op = BINOP_LOG_OR; }
	|	LSHIFT_EQ		{ op = BINOP_SHL; }
	|	RSHIFT_EQ		{ op = BINOP_SHR; }
	;
	
assignExpr returns[Expr* expr = 0]
	{
		BinaryOp op = BINOP_NONE;
		Expr* right = 0;
	}
	:	expr=ternaryExpr
		(
			op=assignOp right=assignExpr
			
			{ expr = new AssignExpr(op, expr, right); }			
		)?
	;
	
ternaryExpr returns[Expr* ex = 0]
	{
		Expr* ifTrue = 0;
		Expr* ifFalse = 0;
	}
	:	ex=logicalOrExpr
		(
			QUESTION ifTrue=expr COLON ifFalse=ternaryExpr
			
			{ ex = new TernaryExpr(ex, ifTrue, ifFalse); }
		)?
	;
	
logicalOrExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=logicalAndExpr
		(
			LOGICAL_OR right=logicalAndExpr
			
			{ expr = new BinaryExpr(BINOP_LOG_OR, expr, right); }
		)*
	;
	
logicalAndExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=bitwiseOrExpr
		(
			LOGICAL_AND right=bitwiseOrExpr
			
			{ expr = new BinaryExpr(BINOP_LOG_AND, expr, right); }
		)*
	;
	
bitwiseOrExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=bitwiseXorExpr
		(
			PIPE right=bitwiseXorExpr
			
			{ expr = new BinaryExpr(BINOP_BIT_OR, expr, right); }
		)*
	;
	
bitwiseXorExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=bitwiseAndExpr
		(
			XOR right=bitwiseAndExpr
			
			{ expr = new BinaryExpr(BINOP_BIT_XOR, expr, right); }
		)*
	;
	
bitwiseAndExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=equalityExpr
		(
			AND right=equalityExpr
			
			{ expr = new BinaryExpr(BINOP_BIT_AND, expr, right); }			
		)*
	;
	
equalityExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=comparisonExpr
		(
			EQUALS_EQUALS	right=comparisonExpr	{ expr = new BinaryExpr(BINOP_EQ, expr, right); }
			| NOT_EQ		right=comparisonExpr	{ expr = new BinaryExpr(BINOP_NE, expr, right); }
		)*
	;
	
comparisonExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=shiftExpr
		(
			LANGLE			right=shiftExpr		{ expr = new BinaryExpr(BINOP_LT, expr, right); }
			| RANGLE		right=shiftExpr		{ expr = new BinaryExpr(BINOP_GT, expr, right); }
			| LESS_EQ		right=shiftExpr		{ expr = new BinaryExpr(BINOP_LE, expr, right); }
			| GREATER_EQ	right=shiftExpr		{ expr = new BinaryExpr(BINOP_GE, expr, right); }
		)*
	;
	
shiftExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=additiveExpr
		(
			LSHIFT		right=additiveExpr	{ expr = new BinaryExpr(BINOP_SHL, expr, right); }
			| RSHIFT	right=additiveExpr	{ expr = new BinaryExpr(BINOP_SHR, expr, right); }
		)*
	;
	
additiveExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=multiplicationExpr
		(
			PLUS	right=multiplicationExpr	{ expr = new BinaryExpr(BINOP_ADD, expr, right); }
			| MINUS	right=multiplicationExpr	{ expr = new BinaryExpr(BINOP_SUB, expr, right); }
		)*
	;

// TODO: cross, dot?
multiplicationExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=exponentExpr
		(
			STAR		right=exponentExpr	{ expr = new BinaryExpr(BINOP_MUL, expr, right); }
			| DIV		right=exponentExpr	{ expr = new BinaryExpr(BINOP_DIV, expr, right); }
			| MODULO	right=exponentExpr	{ expr = new BinaryExpr(BINOP_MOD, expr, right); }
		)*
	;
	
exponentExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=prefixUnaryExpr
		(
			STAR_STAR right=exponentExpr
			
			{ expr = new BinaryExpr(BINOP_POW, expr, right); }
		)?
	;

// TODO: Just throw out C-style casts because they're annoying?
// TODO: Speed this up more cleverly somehow?
	
prefixUnaryExpr returns[Expr* expr=0]
	{
		TypeExpr* typ = 0;
	}
	:	(LPAREN type RPAREN prefixUnaryExpr)=> 
			LPAREN typ=type RPAREN expr=prefixUnaryExpr
			
		{ expr = new CastExpr(typ, expr); }
			
	|	NOT			expr=prefixUnaryExpr { expr = new UnaryExpr(UNOP_LOG_NOT, expr); }
	|	COMPLEMENT	expr=prefixUnaryExpr { expr = new UnaryExpr(UNOP_COMPLEMENT, expr); }
	|	INCREMENT	expr=prefixUnaryExpr { expr = new UnaryExpr(UNOP_PRE_INC, expr); }
	|	DECREMENT	expr=prefixUnaryExpr { expr = new UnaryExpr(UNOP_PRE_DEC, expr); }
	|	MINUS		expr=prefixUnaryExpr { expr = new UnaryExpr(UNOP_NEGATIVE, expr); }
	|	PLUS		expr=prefixUnaryExpr { expr = new UnaryExpr(UNOP_POSITIVE, expr); }
	
	|	expr=postfixUnaryExpr
	;	

postfixUnaryExpr returns[Expr* ex=0]
	{
		Expr* right = 0;
		CallExpr* call = 0;
		Expr* p = 0;
	}
	:	ex=primaryExpr
		(
			INCREMENT							{ ex = new UnaryExpr(UNOP_POST_INC, ex); }
			|	DECREMENT						{ ex = new UnaryExpr(UNOP_POST_DEC, ex); }
			|	(DOT id:ID)						{ ex = new MemberExpr(ex, id->getText()); }
			|	(LBRACKET right=expr RBRACKET)	{ ex = new IndexExpr(ex, right); }
			|	LPAREN
				{ call = new CallExpr(ex); }
				(
					p=expr { call->AddParameter(p); }
					(
						COMMA p=expr	{ call->AddParameter(p); }
					)*
				)?
				RPAREN
				{ ex = call; }
		)*
	;
	

// TODO: I think we have the order wrong for if we go (type)func(params)

primaryExpr returns[Expr* ex = 0]
	:	id:ID						{ ex = new NameExpr(id->getText()); }
	|	ex=literal
	|	LPAREN ex=expr RPAREN
	;
	
// TODO: Add back in support for adjacent string literals (StringLiteral)+
literal returns[Expr* expr = 0]
	:	lit:HexIntLiteral		{ expr = new IntLiteralExpr(lit->getText()); }
	|	lit1:OctalIntLiteral	{ expr = new IntLiteralExpr(lit1->getText()); }
	|	lit2:DecimalIntLiteral	{ expr = new IntLiteralExpr(lit2->getText()); }
	|	lit3:FloatLiteral		{ expr = new FloatLiteralExpr(lit3->getText()); }
	|	lit4:StringLiteral		{ expr = new StringLiteralExpr(lit4->getText()); }
	|	lit5:CharLiteral		{ expr = new CharLiteralExpr(lit5->getText()); }
	|	"true"					{ expr = new BoolLiteralExpr(true); }
	|	"false"					{ expr = new BoolLiteralExpr(false); }
	|	"this"					{ SSAssert(0); } // TODO
	|	"null"					{ expr = new NullLiteralExpr(); }
	|	"global"				{ SSAssert(0); } // TODO
	|	"super"					{ SSAssert(0); } // TODO - use 'base'?
	;
		
//----------------------------------------------------------------------------
// Statements
//----------------------------------------------------------------------------

statement returns [Statement* st=0]
	: (variableDefStmt) => st=variableDefStmt
	| st=returnStmt
	| st=breakStmt
	| st=continueStmt
	| st=switchStmt
	| st=forStmt
	| st=forEachStmt
	| st=whileStmt
	| st=doWhileStmt
	| st=tryStmt
	| st=throwStmt
	| st=blockStmt
	| st=ifStmt
	| st=exprStmt
	| st=emptyStmt
	;
	
returnStmt returns [Statement* st=0]
	{
		Expr* ex = 0;
	}
	: "return" (ex=expr)? SEMI { st = new ReturnStatement(ex); }
	;
	
breakStmt returns [Statement* st=0]
	: "break" SEMI { st = new BreakStatement(); }
	;
	
continueStmt returns [Statement* st=0]
	: "continue" SEMI { st = new BreakStatement(); }
	;
	
// TODO
switchStmt returns [Statement* st=0]
	: "switch" LPAREN expr RPAREN
		LBRACE (switchCond)* RBRACE
	;
	
// TODO
switchCond returns [Statement* st=0]
	: "case" expr COLON (statement)*
	| "default" COLON (statement)*
	;
	
forStmt returns [Statement* st=0]
	{
		Statement* init=0;
		Expr* cond=0;
		Expr* iterate=0;
		Statement* body=0;
	}
	: "for" LPAREN
		((variableDefStmt) => init=variableDefStmt | init=exprStmt | init=emptyStmt)
		(cond=expr)? SEMI
		(iterate=expr)? RPAREN
		body=statement
		
		{ st = new ForStatement(init, cond, iterate, body); }
	;
	
forEachStmt returns [Statement* st=0]
	{
		TypeExpr* typeExpr=0;
		Expr* listExpr=0;
		Statement* body=0;
	}
	: "foreach" LPAREN typeExpr=type name:ID "in" listExpr=expr RPAREN body=statement
		{ st = new ForEachStatement(typeExpr, name->getText(), listExpr, body); }
	;
	
whileStmt returns [Statement* st=0]
	{
		Expr* cond=0;
		Statement* body=0;
	}
	:	"while" LPAREN cond=expr RPAREN body=statement
		{ st = new WhileStatement(cond, body); }
	;
	
doWhileStmt returns [Statement* st=0]
	{
		Expr* cond=0;
		Statement* body=0;
	}
	:	"do" body=statement "while" LPAREN cond=expr RPAREN SEMI
		{ st = new DoWhileStatement(cond, body); }
	;
	
// TODO: Should we replicate what's in variableDef or not?
// TODO: THIS
variableDefStmt returns [Statement* st=0]
	: variableDef
	;
	
// TODO: Why can't we use normal statements instead of block statements here?
tryStmt returns [Statement* st=0]
	: "try" blockStmt
		("catch" catchCond blockStmt)*
		("finally" blockStmt)?
	;
	
catchCond
	: (LPAREN) => LPAREN type (ID)? RPAREN
	|
	;
	
throwStmt returns [Statement* st]
	{
		Expr* ex=0;
	}
	: "throw" ex=expr SEMI { st = new ThrowStatement(ex); }
	;
	
ifStmt returns [Statement* st=0]
	{
		Expr* cond=0;
		Statement* ifTrue=0;
		Statement* ifFalse=0;
	}
	: "if" LPAREN cond=expr RPAREN ifTrue=statement
	  // We know about the 'if(x) if(y) Z; else W;' ambiguity...
	  (
	  	options { warnWhenFollowAmbig = false; } :
	    "else" ifFalse=statement
	  )?
	  
	  { st = new IfStatement(cond, ifTrue, ifFalse); }
	 ;

blockStmt returns [Statement* st=0]
	{
		Statement* innerSt = 0;
		BlockStatement* block = new BlockStatement();
	}
	:	LBRACE
		(
			innerSt=statement
			
			{ block->AddStatement(innerSt); }
		)*
		RBRACE
		{ st = block; }
	;
	
exprStmt returns [Statement* st=0]
	{
		Expr* ex=0;
	}
	: ex=expr SEMI { st = new ExprStatement(ex); }
	;
	
emptyStmt returns [Statement* st=0]
	: SEMI	{ st = new EmptyStatement(); }
	;

	
//----------------------------------------------------------------------------
// Type Exprs
//----------------------------------------------------------------------------

type returns [TypeExpr* typ = 0]
	: typ=basicType
	| typ=dottedType
	;
	
basicType returns [TypeExpr* typ = 0]
	: "void" { typ = new BasicTypeExpr(BT_VOID); }
	| "byte" { typ = new BasicTypeExpr(BT_U1); }
	| "sbyte" { typ = new BasicTypeExpr(BT_S1); }
	| "ushort" { typ = new BasicTypeExpr(BT_U2); }
	| "short" { typ = new BasicTypeExpr(BT_S2); }
	| "uint" { typ = new BasicTypeExpr(BT_U4); }
	| "int" { typ = new BasicTypeExpr(BT_S4); }
	| "ulong" { typ = new BasicTypeExpr(BT_U8); }
	| "long" { typ = new BasicTypeExpr(BT_S8); }
	| "float" { typ = new BasicTypeExpr(BT_FLOAT); }
	| "double" { typ = new BasicTypeExpr(BT_DOUBLE); }
	| "char" { typ = new BasicTypeExpr(BT_CHAR); }
	| "string" { typ = new BasicTypeExpr(BT_STRING); }
	| "object" { typ = new BasicTypeExpr(BT_OBJECT); }
	;
	
dottedType returns [TypeExpr* typ = 0]
	{
		TypeExpr* left = 0;
		TypeExpr* right = 0;
		DottedTypeExpr* dtExpr = 0;
	}
	:	n:ID
		{
			typ = new NameTypeExpr();
			typ->SetName(n->getText());
		}
		
		(
			DOT right=dottedType
			{
				left = typ;
				dtExpr = new DottedTypeExpr();
				dtExpr->SetLeft(left);
				dtExpr->SetRight(right);
				typ = dtExpr;
			}
		)?
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
