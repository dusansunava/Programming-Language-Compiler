%option noyywrap

%{
#include <string.h>
%}

ID [A-Za-z][A-Za-z0-9]*
string \"[^\"]*\"

%%
"++" { return tINCREMENT; }
"--" { return tDECREMENT; }
"=" { return tASSIGN; }
">=" { return tGREATEREQUAL; }
"<=" { return tLESSEQUAL; }
"==" { return tEQUAL; }
"!=" { return tNOTEQUAL; }
"do" { return tDO; }
"while" { return tWHILE; }
"if" { return tIF; }
"then" { return tTHEN; }
"else" { return tELSE; }
"function" { return tFUNCTION; }
"print_str" { return tPRINTSTR; }
"print_var" { return tPRINTVAR; }
"get_input" { return tGETINPUT; }
"global" { return tGLOBALVAR; }
"local" { return tLOCALVAR; }
"return" { return tRETURN; }

""

{ID} { yylval.idval = strdup(yytext); return tIDENTIFIER; }
{string} { yylval.idval = strdup(yytext); return tSTRING; }
[0-9]+ { yylval.numval = atoi(yytext); return tNUMBER; }
[ \t\n] /* ignore whitespace */
. { return *yytext; }
%%





