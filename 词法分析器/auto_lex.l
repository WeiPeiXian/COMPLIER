%{
/*******************************************************************
* IDENTIFIER： 	变量名和函数名(字符开头)
* KEYWORD:		保留字
* OPERATOR:		+ - * / % = == != <= >= > < , ; . ' ' " " //  ( ) [ ] { }
* CONSTANT: 	字符串或字符常量,数字
* NOTE: 		注释
* *****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOTE       258//注释
#define CONSTANT   259//常数
#define OPERATOR   261//运算符
#define KEYWORD    262//关键词
#define IDENTIFIER 263//标识
#define ERROR      -2
%}

space [\t \n]+
num [0-9]+\.{0,1}[0-9]*
str ["]([^\"]*|[\\]+["])*[^\\]["]
pa_note \/\*([^\*^\/]*|[\*^\/*]*|[^\**\/]*)*\*\/
line_note \/\/[^\n]*
identifier [_a-zA-z]+[a-zA-z0-9]*
operator ")"|"("|"["|"]"|"{"|"}"|";"|","|"."|"+"|"-"|"*"|">"|"<"|"=="|"&"|"&&"
keyword "abstract"|"case"|"catch"|"class"|"def"|"do"|"else"|"extends"|"false"|"final"|"finally"|"for"|"forSome"|"if"|"implicit"|"import"|"lazy"|"macro"|"match"|"new"|"null"|"object"|"override"|"package"|"private"|"protected"|"return"|"sealed"|"super"|"this"|"trait"|"throw"|"try"|"true"|"type"|"val"|"var"|"while"|"with"|"yield"|"_"|":"|"="|"=>"|"<-"|"<:"|"<%"|">:"|"#"|"@"
		
%%

{space}
{pa_note}|{line_note}     {printf("<%3d,%s>\n",NOTE,yytext); return NOTE;}
{num}|{str}               {printf("<%3d,%s>\n",CONSTANT,yytext); return CONSTANT;}		
{keyword}                 {printf("<%3d,%s>\n",KEYWORD,yytext); return KEYWORD;}
{identifier}				      {printf("<%3d,%s>\n",IDENTIFIER,yytext); return IDENTIFIER;}
{operator}					      {printf("<%3d,%s>\n",OPERATOR,yytext); return OPERATOR;}

%%
int main(int argc, char *argv[])
{
    if (argc > 1) {
       if (!(yyin = fopen(argv[1], "r"))) {   
           perror(argv[1]);
           return 1;
       }
   }
   while (yylex());
   fclose(yyin);
   return 0;
}

int yywrap()
{ 
   return(1);
} 


