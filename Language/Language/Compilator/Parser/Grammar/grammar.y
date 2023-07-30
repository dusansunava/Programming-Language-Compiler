%{
#include <stdio.h>

extern int yylineno;    // Bison-generated global variable

%}

%error-verbose

%parse-param { int currentMode }
%parse-param { FILE * fp }

%union
{
	int numval;
	char* idval;
	tree* tval;
}

%token tINCREMENT
%token tDECREMENT
%token tASSIGN
%token tGREATEREQUAL
%token tLESSEQUAL
%token tDO
%token tWHILE
%token tIF
%token tTHEN
%token tELSE
%token tFUNCTION
%token tPRINTSTR
%token tPRINTVAR
%token tGETINPUT
%token tGLOBALVAR
%token tLOCALVAR
%token tRETURN
%token tEQUAL
%token tNOTEQUAL

%token <idval> tIDENTIFIER
%token <idval> tSTRING
%token <numval> tNUMBER

%type <tval> statement statements expression parameters_pop parameters_push

%start custom

%left '>' '<' tGREATEREQUAL tLESSEQUAL tEQUAL tNOTEQUAL
%left '+' '-'
%left '*' '/'

%right tTHEN tELSE
%right tINCREMENT tDECREMENT

%%

custom:
	statements { proceed($1, currentMode, fp); }

statement:
	tIDENTIFIER tASSIGN expression { $$ = mknode(t_assign, 0, mkid($1), $3); }
	| tIF '[' expression ']' tTHEN '{' statements '}' { $$ = mknode(t_if, 0, $3, mknode(t_then, 0, $7, NULL)); }
	| tIF '[' expression ']' tTHEN '{' statements '}' tELSE '{' statements '}' { $$ = mknode(t_if, 0, $3, mknode(t_then, 0, $7, $11)); }
	| tDO '{' statements '}' tWHILE '[' expression ']' { $$ = mknode(t_loop, 0, $3, $7); }
	| tWHILE '[' expression ']' tDO '{' statements '}' { $$ = mknode(t_loop_while, 0, $3, $7); }
	| tFUNCTION tIDENTIFIER '(' parameters_pop ')' '{' statements '}' { $$ = mknode(t_function, 0, mknode(t_function_def, 0, mkid($2), $4), $7); }
	| tFUNCTION tIDENTIFIER '(' ')' '{' statements '}' { $$ = mknode(t_function, 0, mknode(t_function_def, 0, mkid($2), NULL), $6); }
 	| tIDENTIFIER '(' parameters_push ')' { $$ = mknode(t_function_call, 0, mkid($1), $3); }
 	| tIDENTIFIER '(' ')' { $$ = mknode(t_function_call, 0, mkid($1), NULL); }
 	| tIDENTIFIER tASSIGN tIDENTIFIER '(' parameters_push ')' { $$ = mknode(t_function_call_w_ret, 0, mkid($1),  mknode(t_function_call, 0, mkid($3), $5)); }
 	| tPRINTSTR '(' tSTRING ')' { $$ = mknode(t_print_str, 0, mkid($3), NULL); }
 	| tPRINTVAR '(' tIDENTIFIER ')' { $$ = mknode(t_print_var, 0, mkid($3), NULL); }
 	| tIDENTIFIER tASSIGN tGETINPUT '(' ')' { $$ = mknode(t_assign, 0, mkid($1), mknode(t_get_input, 0, NULL, NULL)); }
 	| expression tINCREMENT { $$ = mknode(t_op, a_increment, $1, NULL); }
    | tINCREMENT expression { $$ = mknode(t_op, z_increment, NULL, $2); }
    | expression tDECREMENT { $$ = mknode(t_op, a_decrement, $1, NULL); }
    | tDECREMENT expression { $$ = mknode(t_op, z_decrement, NULL, $2); }
    | tGLOBALVAR tIDENTIFIER { $$ = mknode(t_global_var, 0, mkid($2), NULL); }
    | tLOCALVAR tIDENTIFIER { $$ = mknode(t_local_var, 0, mkid($2), NULL); }
    | tRETURN tIDENTIFIER { $$ = mknode(t_function_ret, 0, mkid($2), NULL); }
    | tRETURN tNUMBER { $$ = mknode(t_function_ret, 0, mknum($2), NULL); }
;


parameters_pop:
    tIDENTIFIER { $$  = mknode(t_parameter_pop, 0, mkid($1), NULL); }
    | tIDENTIFIER ';' parameters_pop { $$ = mknode(t_parameter_pop, 0, mkid($1), $3); }
;

parameters_push:
    tIDENTIFIER { $$  = mknode(t_parameter_push, 0, mkid($1), NULL); }
    | tIDENTIFIER ';' parameters_push { $$ = mknode(t_parameter_push, 0, mkid($1), $3); }
;

statements:
	statement  '$' { $$ = mknode(t_join, 0, $1, NULL); }
	| statement '$' statements { $$ = mknode(t_join, 0, $1, $3); }
;

expression:
	tNUMBER { $$ = mknum($1); }
	| tIDENTIFIER { $$ = mkid($1); }
	| expression '+' expression { $$ = mknode(t_op, plus, $1, $3); }
	| expression '-' expression { $$ = mknode(t_op, minus, $1, $3); }
	| expression '*' expression { $$ = mknode(t_op, multiply, $1, $3); }
    | expression '/' expression { $$ = mknode(t_op, divison, $1, $3); }
	| expression '>' expression { $$ = mknode(t_op, greater, $1, $3); }
	| expression tGREATEREQUAL expression { $$ = mknode(t_op, greater_equal, $1, $3); }
	| expression '<' expression { $$ = mknode(t_op, less, $1, $3); }
	| expression tLESSEQUAL expression { $$ = mknode(t_op, less_equal, $1, $3); }
	| expression tEQUAL expression { $$ = mknode(t_op, equal, $1, $3); }
	| expression tNOTEQUAL expression { $$ = mknode(t_op, not_equal, $1, $3); }

    | '(' expression ')' { $$ = $2; }
;

%%

#include "lex.yy.c"

int yyerror(int cm, FILE * fp, const char* msg)
{
    fprintf(stderr, "Syntax error: %s\n", msg);
    exit(1);
}
