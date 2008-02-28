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

// Sets node's file/line info to match tok's
#define SS_LOCATE(node,tok) { node->SetFile(m_fileInfo); node->SetLine(tok->getLine()); }

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
public:
	void reportError(const antlr::RecognitionException& ex);
	void reportError(const std::string& s);
	void reportWarning(const std::string& s);
	
	FileInfo* m_fileInfo;
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
			SS_LOCATE(cls, cname);
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
				SS_LOCATE(func,fname);
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
			SS_LOCATE(param, pname);
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
			SS_LOCATE(var, vname);
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
	
assignOp[int& line] returns[BinaryOp op = BINOP_NONE]
	:	op1:ASSIGN			{ op = BINOP_NONE;		line = op1->getLine(); }
	|	op2:PLUS_EQ			{ op = BINOP_ADD;		line = op2->getLine(); }
	|	op3:MINUS_EQ		{ op = BINOP_SUB;		line = op3->getLine(); }
	|	op4:MUL_EQ			{ op = BINOP_MUL;		line = op4->getLine(); }
	|	op5:DIV_EQ			{ op = BINOP_DIV;		line = op5->getLine(); }
	|	op6:MODULO_EQ		{ op = BINOP_MOD;		line = op6->getLine(); }
	|	op7:POWER_EQ		{ op = BINOP_POW;		line = op7->getLine(); }
	|	op8:AND_EQ			{ op = BINOP_BIT_AND;	line = op8->getLine(); }
	|	op9:PIPE_EQ			{ op = BINOP_BIT_OR;	line = op9->getLine(); }
	|	op10:XOR_EQ			{ op = BINOP_BIT_XOR;	line = op10->getLine(); }
	|	op11:LOGICAL_AND_EQ	{ op = BINOP_LOG_AND;	line = op11->getLine(); }
	|	op12:LOGICAL_OR_EQ	{ op = BINOP_LOG_OR;	line = op12->getLine(); }
	|	op13:LSHIFT_EQ		{ op = BINOP_SHL;		line = op13->getLine(); }
	|	op14:RSHIFT_EQ		{ op = BINOP_SHR;		line = op14->getLine(); }
	;
	
assignExpr returns[Expr* expr = 0]
	{
		BinaryOp op = BINOP_NONE;
		Expr* right = 0;
		int line;
	}
	:	expr=ternaryExpr
		(
			op=assignOp[line] right=assignExpr
			
			{
				expr = new AssignExpr(op, expr, right);
				expr->SetFile(m_fileInfo);
				expr->SetLine(line);
			}			
		)?
	;
	
ternaryExpr returns[Expr* ex = 0]
	{
		Expr* ifTrue = 0;
		Expr* ifFalse = 0;
	}
	:	ex=logicalOrExpr
		(
			qmark:QUESTION ifTrue=expr COLON ifFalse=ternaryExpr
			
			{
				ex = new TernaryExpr(ex, ifTrue, ifFalse);
				SS_LOCATE(ex, qmark);
			}
		)?
	;
	
logicalOrExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=logicalAndExpr
		(
			op:LOGICAL_OR right=logicalAndExpr
			
			{
				expr = new BinaryExpr(BINOP_LOG_OR, expr, right);
				SS_LOCATE(expr, op);
			}
		)*
	;
	
logicalAndExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=bitwiseOrExpr
		(
			op:LOGICAL_AND right=bitwiseOrExpr
			
			{
				expr = new BinaryExpr(BINOP_LOG_AND, expr, right);
				SS_LOCATE(expr, op);
			}
		)*
	;
	
bitwiseOrExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=bitwiseXorExpr
		(
			op:PIPE right=bitwiseXorExpr
			
			{
				expr = new BinaryExpr(BINOP_BIT_OR, expr, right);
				SS_LOCATE(expr, op);
			}
		)*
	;
	
bitwiseXorExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=bitwiseAndExpr
		(
			op:XOR right=bitwiseAndExpr
			
			{
				expr = new BinaryExpr(BINOP_BIT_XOR, expr, right);
				SS_LOCATE(expr, op);
			}
		)*
	;
	
bitwiseAndExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=equalityExpr
		(
			op:AND right=equalityExpr
			
			{
				expr = new BinaryExpr(BINOP_BIT_AND, expr, right);
				SS_LOCATE(expr, op);
			}			
		)*
	;
	
equalityExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=comparisonExpr
		(
			op1:EQUALS_EQUALS	right=comparisonExpr	{ expr = new BinaryExpr(BINOP_EQ, expr, right); SS_LOCATE(expr,op1); }
			| op2:NOT_EQ		right=comparisonExpr	{ expr = new BinaryExpr(BINOP_NE, expr, right); SS_LOCATE(expr,op2); }
		)*
	;
	
comparisonExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=shiftExpr
		(
			op1:LANGLE			right=shiftExpr		{ expr = new BinaryExpr(BINOP_LT, expr, right); SS_LOCATE(expr,op1); }
			| op2:RANGLE		right=shiftExpr		{ expr = new BinaryExpr(BINOP_GT, expr, right); SS_LOCATE(expr,op2); }
			| op3:LESS_EQ		right=shiftExpr		{ expr = new BinaryExpr(BINOP_LE, expr, right); SS_LOCATE(expr,op3); }
			| op4:GREATER_EQ	right=shiftExpr		{ expr = new BinaryExpr(BINOP_GE, expr, right); SS_LOCATE(expr,op4); }
		)*
	;
	
shiftExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=additiveExpr
		(
			op1:LSHIFT		right=additiveExpr	{ expr = new BinaryExpr(BINOP_SHL, expr, right); SS_LOCATE(expr, op1); }
			| op2:RSHIFT	right=additiveExpr	{ expr = new BinaryExpr(BINOP_SHR, expr, right); SS_LOCATE(expr, op2); }
		)*
	;
	
additiveExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=multiplicationExpr
		(
			op1:PLUS	right=multiplicationExpr	{ expr = new BinaryExpr(BINOP_ADD, expr, right); SS_LOCATE(expr, op1); }
			| op2:MINUS	right=multiplicationExpr	{ expr = new BinaryExpr(BINOP_SUB, expr, right); SS_LOCATE(expr, op2); }
		)*
	;

// TODO: cross, dot?
multiplicationExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=exponentExpr
		(
			op1:STAR		right=exponentExpr	{ expr = new BinaryExpr(BINOP_MUL, expr, right); SS_LOCATE(expr, op1); }
			| op2:DIV		right=exponentExpr	{ expr = new BinaryExpr(BINOP_DIV, expr, right); SS_LOCATE(expr, op2); }
			| op3:MODULO	right=exponentExpr	{ expr = new BinaryExpr(BINOP_MOD, expr, right); SS_LOCATE(expr, op3); }
		)*
	;
	
exponentExpr returns[Expr* expr = 0]
	{
		Expr* right = 0;
	}
	:	expr=prefixUnaryExpr
		(
			op:POWER right=exponentExpr
			
			{
				expr = new BinaryExpr(BINOP_POW, expr, right);
				SS_LOCATE(expr, op);
			}
		)?
	;

// TODO: Just throw out C-style casts because they're annoying?
// TODO: Speed this up more cleverly somehow?
	
prefixUnaryExpr returns[Expr* expr=0]
	{
		TypeExpr* typ = 0;
	}
	:	(LPAREN type RPAREN prefixUnaryExpr)=> 
			op1:LPAREN typ=type RPAREN expr=prefixUnaryExpr
			
		{ expr = new CastExpr(typ, expr); SS_LOCATE(expr, op1); }
			
	|	op2:NOT			expr=prefixUnaryExpr { expr = new UnaryExpr(UNOP_LOG_NOT, expr);	SS_LOCATE(expr, op2); }
	|	op3:COMPLEMENT	expr=prefixUnaryExpr { expr = new UnaryExpr(UNOP_COMPLEMENT, expr); SS_LOCATE(expr, op3); }
	|	op4:INCREMENT	expr=prefixUnaryExpr { expr = new UnaryExpr(UNOP_PRE_INC, expr);	SS_LOCATE(expr, op4); }
	|	op5:DECREMENT	expr=prefixUnaryExpr { expr = new UnaryExpr(UNOP_PRE_DEC, expr);	SS_LOCATE(expr, op5); }
	|	op6:MINUS		expr=prefixUnaryExpr { expr = new UnaryExpr(UNOP_NEGATIVE, expr);	SS_LOCATE(expr, op6); }
	|	op7:PLUS		expr=prefixUnaryExpr { expr = new UnaryExpr(UNOP_POSITIVE, expr);	SS_LOCATE(expr, op7); }
	
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
			op1:INCREMENT							{ ex = new UnaryExpr(UNOP_POST_INC, ex); SS_LOCATE(ex, op1); }
			|	op2:DECREMENT						{ ex = new UnaryExpr(UNOP_POST_DEC, ex); SS_LOCATE(ex, op2);  }
			|	(op3:DOT id:ID)						{ ex = new MemberExpr(ex, id->getText()); SS_LOCATE(ex, op3);  }
			|	(op4:LBRACKET right=expr RBRACKET)	{ ex = new IndexExpr(ex, right); SS_LOCATE(ex, op4);  }
			|	op5:LPAREN
				{ call = new CallExpr(ex); }
				(
					p=expr { call->AddParameter(p); }
					(
						COMMA p=expr	{ call->AddParameter(p); }
					)*
				)?
				RPAREN
				{ ex = call; SS_LOCATE(ex, op5); }
		)*
	;
	

// TODO: I think we have the order wrong for if we go (type)func(params)

primaryExpr returns[Expr* ex = 0]
	:	id:ID						{ ex = new NameExpr(id->getText()); SS_LOCATE(ex, id); }
	|	ex=literal
	|	LPAREN ex=expr RPAREN
	;
	
// TODO: Add back in support for adjacent string literals (StringLiteral)+
literal returns[Expr* expr = 0]
	:	lit1:HexIntLiteral		{ expr = new IntLiteralExpr(lit1->getText()); SS_LOCATE(expr, lit1); }
	|	lit2:OctalIntLiteral	{ expr = new IntLiteralExpr(lit2->getText()); SS_LOCATE(expr, lit2); }
	|	lit3:DecimalIntLiteral	{ expr = new IntLiteralExpr(lit3->getText()); SS_LOCATE(expr, lit3); }
	|	lit4:FloatLiteral		{ expr = new FloatLiteralExpr(lit4->getText()); SS_LOCATE(expr, lit4); }
	|	lit5:StringLiteral		{ expr = new StringLiteralExpr(lit5->getText()); SS_LOCATE(expr, lit5); }
	|	lit6:CharLiteral		{ expr = new CharLiteralExpr(lit6->getText()); SS_LOCATE(expr, lit6); }
	|	lit7:"true"				{ expr = new BoolLiteralExpr(true); SS_LOCATE(expr, lit7); }
	|	lit8:"false"			{ expr = new BoolLiteralExpr(false); SS_LOCATE(expr, lit8); }
	|	lit9:"this"				{ SSAssert(0); } // TODO
	|	lit10:"null"			{ expr = new NullLiteralExpr(); SS_LOCATE(expr, lit10); }
	|	lit11:"global"			{ SSAssert(0); } // TODO
	|	lit12:"super"			{ SSAssert(0); } // TODO - use 'base'?
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
	: kw:"return" (ex=expr)? SEMI { st = new ReturnStatement(ex); SS_LOCATE(st, kw); }
	;
	
breakStmt returns [Statement* st=0]
	: kw: "break" SEMI { st = new BreakStatement(); SS_LOCATE(st, kw); }
	;
	
continueStmt returns [Statement* st=0]
	: kw:"continue" SEMI { st = new BreakStatement(); SS_LOCATE(st, kw); }
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
	: kw:"for" LPAREN
		((variableDefStmt) => init=variableDefStmt | init=exprStmt | init=emptyStmt)
		(cond=expr)? SEMI
		(iterate=expr)? RPAREN
		body=statement
		
		{ st = new ForStatement(init, cond, iterate, body); SS_LOCATE(st, kw); }
	;
	
forEachStmt returns [Statement* st=0]
	{
		TypeExpr* typeExpr=0;
		Expr* listExpr=0;
		Statement* body=0;
	}
	: kw:"foreach" LPAREN typeExpr=type name:ID "in" listExpr=expr RPAREN body=statement
		{ st = new ForEachStatement(typeExpr, name->getText(), listExpr, body); SS_LOCATE(st, kw); }
	;
	
whileStmt returns [Statement* st=0]
	{
		Expr* cond=0;
		Statement* body=0;
	}
	:	kw:"while" LPAREN cond=expr RPAREN body=statement
		{ st = new WhileStatement(cond, body); SS_LOCATE(st, kw); }
	;
	
doWhileStmt returns [Statement* st=0]
	{
		Expr* cond=0;
		Statement* body=0;
	}
	:	kw:"do" body=statement "while" LPAREN cond=expr RPAREN SEMI
		{ st = new DoWhileStatement(cond, body); SS_LOCATE(st, kw); }
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
	: kw:"throw" ex=expr SEMI { st = new ThrowStatement(ex); SS_LOCATE(st, kw); }
	;
	
ifStmt returns [Statement* st=0]
	{
		Expr* cond=0;
		Statement* ifTrue=0;
		Statement* ifFalse=0;
	}
	: kw:"if" LPAREN cond=expr RPAREN ifTrue=statement
	  // We know about the 'if(x) if(y) Z; else W;' ambiguity...
	  (
	  	options { warnWhenFollowAmbig = false; } :
	    "else" ifFalse=statement
	  )?
	  
	  { st = new IfStatement(cond, ifTrue, ifFalse);  SS_LOCATE(st, kw); }
	 ;

blockStmt returns [Statement* st=0]
	{
		Statement* innerSt = 0;
		BlockStatement* block = new BlockStatement();
	}
	:	kw:LBRACE
		(
			innerSt=statement
			
			{ block->AddStatement(innerSt); }
		)*
		RBRACE
		{ st = block; SS_LOCATE(st, kw); }
	;
	
exprStmt returns [Statement* st=0]
	{
		Expr* ex=0;
	}
	: ex=expr SEMI { st = new ExprStatement(ex); st->SetFile(ex->GetFile()); st->SetLine(ex->GetLine()); }
	;
	
emptyStmt returns [Statement* st=0]
	: kw:SEMI	{ st = new EmptyStatement(); SS_LOCATE(st, kw); }
	;

	
//----------------------------------------------------------------------------
// Type Exprs
//----------------------------------------------------------------------------

type returns [TypeExpr* typ = 0]
	: typ=basicType
	| typ=dottedType
	;
	
basicType returns [TypeExpr* typ = 0]
	: kw1:"void"	{ typ = new BasicTypeExpr(BT_VOID);		SS_LOCATE(typ, kw1); }
	| kw2:"byte"	{ typ = new BasicTypeExpr(BT_U1);		SS_LOCATE(typ, kw2); }
	| kw3:"sbyte"	{ typ = new BasicTypeExpr(BT_S1);		SS_LOCATE(typ, kw3); }
	| kw4:"ushort"	{ typ = new BasicTypeExpr(BT_U2);		SS_LOCATE(typ, kw4); }
	| kw5:"short"	{ typ = new BasicTypeExpr(BT_S2);		SS_LOCATE(typ, kw5); }
	| kw6:"uint"	{ typ = new BasicTypeExpr(BT_U4);		SS_LOCATE(typ, kw6); }
	| kw7:"int"		{ typ = new BasicTypeExpr(BT_S4);		SS_LOCATE(typ, kw7); }
	| kw8:"ulong"	{ typ = new BasicTypeExpr(BT_U8);		SS_LOCATE(typ, kw8); }
	| kw9:"long"	{ typ = new BasicTypeExpr(BT_S8);		SS_LOCATE(typ, kw9); }
	| kw10:"float"	{ typ = new BasicTypeExpr(BT_FLOAT);	SS_LOCATE(typ, kw10); }
	| kw11:"double"	{ typ = new BasicTypeExpr(BT_DOUBLE);	SS_LOCATE(typ, kw11); }
	| kw12:"char"	{ typ = new BasicTypeExpr(BT_CHAR);		SS_LOCATE(typ, kw12); }
	| kw13:"string"	{ typ = new BasicTypeExpr(BT_STRING);	SS_LOCATE(typ, kw13); }
	| kw14:"object"	{ typ = new BasicTypeExpr(BT_OBJECT);	SS_LOCATE(typ, kw14); }
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
			SS_LOCATE(typ, n);
		}
		
		(
			dot:DOT right=dottedType
			{
				left = typ;
				dtExpr = new DottedTypeExpr();
				SS_LOCATE(dtExpr, dot);
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
