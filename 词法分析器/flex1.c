/*******************************************************************
* IDENTIFIER (标识符)： 变量名和函数名(字符开头)
* KEYWORD:保留字
* OPERATOR:+ - * / % = == != <= >= > < , ; . ' ' " " //  ( ) [ ] { }
* CONSTANT: 字符串或字符常量,数字
* NOTE: 注释
* *****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN    99//词素最大字符数

#define LETTER     0
#define DIG        1
#define SYM        2
#define UNKNOWN    999//数字 字符 未知

#define NOTE       258//注释
#define CONSTANT   259//常数
#define OPERATOR   261//运算符
#define KEYWORD    262//关键词
#define IDENTIFIER 263//标识
#define ERROR      -2



int charClass;//字符所属的类别，分为NUM DIG
int lexLen;//lexeme字符长度
char theCha;
char nextCha;
char proCha;//存放当前字符以及下一个和上一个字符
char lexeme[MAX_LEN];//存放词素的地方
int nextToken;//元素的类型
FILE *stream;//文件指针

char *keyword[] = {
	"abstract",    "case",        "catch",       "class",       "def",
	"do",          "else",        "extends",     "false",       "final",
	"finally",     "for" ,        "forSome",     "if"   ,       "implicit",
	"import"  ,    "lazy",        "macro"  ,     "match",       "new",
	"null"     ,   "object",      "override",    "package",     "private",
	"protected" ,  "return",      "sealed"  ,    "super"  ,     "this",
	"throw"     ,  "trait" ,      "try"     ,    "true"   ,     "type",
	"val"       ,  "var"   ,      "while"   ,    "with"   ,     "yield",
	"_",    ":",    "=",    "=>",    "<-",    "<:",    "<%" ,    ">:" ,   "#",    "@",
	"0",
};

int isDig()
{
	if (theCha >= '0' && theCha <= '9')
		return 1;
	else
		return 0;
}

int isLetter()
{
	if ((theCha >= 'a' && theCha <= 'z') || (theCha >= 'A' && theCha <= 'Z'))
		return 1;
	else
		return 0;
}

int isSYM()
{
	switch(theCha)
	{
		case ')':
		case '(':
		case '[':
		case ']':
		case '{':
		case '}':
		case ';':
		case ',':
		case '.':
		case '+':
		case '-':
		case '*':
		case '#':
		case ':':
		case '@':
		case '_':
		case '=':
		case '>':
		case '<':
		case '/':return 1;break;
		default: return 0;break;
	}
}

void addChar()
{
    printf("%d",lexLen);
	if (lexLen <= MAX_LEN-2)
	{
		lexeme[lexLen++] = theCha;
		lexeme[lexLen] = 0;
	}
	else
		printf("ERROR:lexeme is too long");
}

int getCha()
{
	int i = 0;//判断line读到哪里
    static int firstRun = 1;
	if(firstRun)//当第一次读时，只读两个
	{
		proCha = ' ';
		theCha = fgetc(stream);
		nextCha = fgetc(stream);
		firstRun = 0;
	}
	else
	{
		proCha = theCha;
		theCha = nextCha;
		nextCha = fgetc(stream);
	}

	if (theCha == EOF)
	{
		charClass = EOF;
		nextToken = EOF;
	}
	else//判断时字符还是数字还是其他
	{
		if(isLetter(theCha))
			charClass = LETTER;
		else if(isDig(theCha))
			charClass = DIG;
		else if(isSYM(theCha))
			charClass = SYM;
		else
			charClass = UNKNOWN;
	}
	printf("%c",theCha);
}

void getNon()
{
	while(theCha == ' '|| theCha == '\n')
		getCha();
}

int checkSymbol()
{
	switch(theCha)
	{
		case ')':
		case '(':
		case '[':
		case ']':
		case '{':
		case '}':
		case ';':
		case ',':
		case '.':
		case '+':
		case '-':
		case '*':{
			addChar();
			nextToken = OPERATOR;
			break;
		}
		case '#':
		case ':':
		case '@':
		case '_':{
			addChar();
			nextToken = OPERATOR;
			break;
		}
		case '=':{
			addChar();
			nextToken = OPERATOR;
			if (nextCha == '=')
			{
				getCha();
				addChar();
				nextToken = OPERATOR;
			}
			if (nextCha == '>')
			{
				getCha();
				addChar();
				nextToken = KEYWORD;
			}
			break;
		}
		case '>':{
			addChar();
			nextToken = OPERATOR;
			if (nextCha == ':')
			{
				getCha();
				addChar();
				nextToken = KEYWORD;
			}
			break;
		}
		case '<':{
			addChar();
			nextToken = OPERATOR;
			if ((nextCha == '-')||(nextCha == ':')||(nextCha == '%'))
			{
				getCha();
				addChar();
				nextToken = KEYWORD;
			}
			break;
		}
		// case '!':{
		// 	if (nextCha = '=')
		// 	{
		// 		getCha();
		// 		addChar();
		// 		nextToken = OPERATOR;
		// 	}
		// }
		case '/':{
		    addChar();
		    nextToken = OPERATOR;
			if (nextCha == '/')
			{
			    getCha();
				addChar();
				while(theCha != '\n' && theCha != EOF)
                {
                    addChar();
                    getCha();
                }
                nextToken = NOTE;
				break;
			}
		}
		// case '"':break;
		// case '"':break;
		case EOF:{
			addChar();
			nextToken = EOF;
		}
		default:{
			printf("ERROR:unknown charator\n");
			nextToken = ERROR;
		}
	}
	return nextToken;
}

void checkKeyword(char *pword)
{
	int i = 0;
	while(keyword[i] != "0")
	{
	    char* pkeyword = keyword[i];
		if (strcmp(pword,pkeyword) == 0)
		{
			nextToken = KEYWORD;
			return;
		}
		i++;
	}
}

int lexer()
{
	lexLen = 0;
	getNon();
	switch(charClass)
	{
		case LETTER:{
			addChar();
			getCha();
			while(charClass == LETTER || charClass == DIG || charClass == SYM)
			{
				addChar();
				getCha();
			}
			nextToken = IDENTIFIER;
			checkKeyword(lexeme);
			break;
		}
		case DIG:{
			addChar();
			getCha();
			while(charClass == DIG)
			{
				addChar();
				getCha();
			}
			nextToken = CONSTANT;
			break;
		}
		case SYM:{
			checkSymbol();
			getCha();
			break;
		}
		case EOF:{
			nextToken = EOF;
			lexeme[0] = 'E';
			lexeme[1] = 'O';
			lexeme[2] = 'F';
			lexeme[3] = 0;
			break;
		}
	}
	printf("<%d,%s>\n",nextToken,lexeme);
	return nextToken;
}

int main(int argc, char const *argv[])
{
	stream = fopen("abcd.c","r+");
	if (stream == NULL)
	{
		printf("ERROR\n");
		return 0;
	}
	getNon();
	getCha();
	while(nextToken != EOF)
        lexer();

	return 0;
}
