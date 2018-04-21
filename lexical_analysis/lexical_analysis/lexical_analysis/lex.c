#include "lexical_analysis.h"

DynString tkstr;		//�����ַ���
DynString sourcestr;	//����Դ���ַ���

char ch;		//��ǰȡ����Դ���ַ�
int token;		//���ʱ���
int tkvalue;	//����ֵ
int line_num;	//�к�


/***********************************************************
* ����:	�ʷ�������ʼ��
**********************************************************/
void init_lex()
{
	TkWord *tp;
	static TkWord keywords[] = {
	{ TK_PLUS,		NULL,	  "+" },
	{ TK_MINUS,		NULL,	  "-" },
	{ TK_STAR,		NULL,	  "*" },
	{ TK_DIVIDE,	NULL,	  "/" },
	{ TK_MOD,		NULL,	  "%" },
	{ TK_EQ,		NULL,	  "==" },
	{ TK_NEQ,		NULL,	  "!=" },
	{ TK_LT,		NULL,	  "<" },
	{ TK_LEQ,		NULL,	  "<=" },
	{ TK_GT,		NULL,	  ">" },
	{ TK_GEQ,		NULL,	  ">=" },
	{ TK_ASSIGN,	NULL,	  "=" },
	{ TK_POINTSTO,	NULL,	  "->" },
	{ TK_DOT,		NULL,	  "." },
	{ TK_AND,		NULL,	  "&" },
	{ TK_OPENPA,	NULL,	  "(" },
	{ TK_CLOSEPA,	NULL,	  ")" },
	{ TK_OPENBR,	NULL,	  "[" },
	{ TK_CLOSEBR,	NULL,	  "]" },
	{ TK_BEGIN,		NULL,	  "{" },
	{ TK_END,		NULL,	  "}" },
	{ TK_SEMICOLON,	NULL,	  ";" },
	{ TK_COMMA,		NULL,	  "," },
	{ TK_ELLIPSIS,	NULL,	"..." },
	{ TK_EOF,		NULL,	 "End_Of_File" },
	{ TK_CINT,		NULL,	 	"���ͳ���" },
	{ TK_CCHAR,		NULL,		"�ַ�����" },
	{ TK_CSTR,		NULL,		"�ַ�������" },
	{ KW_CHAR,		NULL,		"char" },
	{ KW_SHORT,		NULL,		"short" },
	{ KW_INT,		NULL,		"int" },
	{ KW_VOID,		NULL,		"void" },
	{ KW_STRUCT,	NULL,		"struct" },
	{ KW_IF,		NULL,		"if" },
	{ KW_ELSE,		NULL,		"else" },
	{ KW_FOR,		NULL,		"for" },
	{ KW_CONTINUE,	NULL,		"continue" },
	{ KW_BREAK,		NULL,		"break" },
	{ KW_RETURN,	NULL,		"return" },
	{ KW_SIZEOF,	NULL,		"sizeof" },
	{ KW_ALIGN,		NULL,		"__align" },
	{ KW_CDECL,		NULL,		"__cdecl" },
	{ KW_STDCALL,	NULL,		"__stdcall" },
	{ 0,			NULL,		NULL }
	};

	dynarray_init(&tktable, 8);
	for (tp = &keywords[0]; tp->spelling != NULL; tp++)
	{
		tkword_direct_insert(tp);
	}
}

/***********************************************************
*  ����:	���Կո�,TAB���س�
**********************************************************/
void skip_white_space()
{
	while (ch == ' ' || ch == '\t' || ch == '\r')
	{
		if (ch == '\r')
		{
			getch();
			if (ch != '\n')
				return;
			line_num++;
		}
		printf("%c", ch);
		getch();
	}
}

/***********************************************************
*  ����:	����ע��
**********************************************************/
void parse_comment()
{
	getch();
	do
	{
		do
		{
			if (ch == '\n' || ch == '*' || ch == CH_EOF)
				break;
			else
				getch();
		} while (1);
		if (ch == '\n')
		{
			line_num++;
			getch();
		}
		else if (ch == '*')
		{
			getch();
			if (ch == '/')
			{
				getch();
				return;
			}
		}
		else
		{
			error("һֱ���ļ�βδ������Ե�ע�ͽ�����");
			return;
		}
	} while (1);
}

/***********************************************************
*  ����:	Ԥ�������Էָ�����ע��
**********************************************************/
void preprocess()
{
	while (1)
	{
		if (ch == ' ' || ch == '\t' || ch == '\r')
			skip_white_space();
		else if (ch == '/')
		{
			//��ǰ���һ���ֽڿ��Ƿ���ע�Ϳ�ʼ�����´��˰Ѷ�����ַ��ٷŻ�ȥ
			getch();
			if (ch == '*')
			{
				parse_comment();
			}
			else
			{
				ungetc(ch, fin); //��һ���ַ��˻ص���������
				ch = '/';
				break;
			}
		}
		else
			break;
	}
}

/***********************************************************
* ����:	�ж�c�Ƿ�Ϊ��ĸ(a-z,A-Z)���»���(-)
* c:		�ַ�ֵ
**********************************************************/
int is_nodigit(char c)
{
	return (c >= 'a'  && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

/***********************************************************
* ����:	�ж�c�Ƿ�Ϊ����
* c:		�ַ�ֵ
**********************************************************/
int is_digit(char c)
{
	return c >= '0'&&c <= '9';
}

/***********************************************************
* ����:	������ʶ��
**********************************************************/
TkWord* parse_identifier()
{
	dynstring_reset(&tkstr);
	dynstring_chcat(&tkstr, ch);
	getch();
	while (is_nodigit(ch) || is_digit(ch))
	{
		dynstring_chcat(&tkstr, ch);
		getch();
	}
	dynstring_chcat(&tkstr, '\0');
	return tkword_insert(tkstr.data);
}

/***********************************************************
* ����:	�������ͳ���
**********************************************************/
void parse_num()
{
	dynstring_reset(&tkstr);
	dynstring_reset(&sourcestr);
	do {
		dynstring_chcat(&tkstr, ch);
		dynstring_chcat(&sourcestr, ch);
		getch();
	} while (is_digit(ch));
	if (ch == '.')
	{
		do {
			dynstring_chcat(&tkstr, ch);
			dynstring_chcat(&sourcestr, ch);
			getch();
		} while (is_digit(ch));
	}
	dynstring_chcat(&tkstr, '\0');
	dynstring_chcat(&sourcestr, '\0');
	tkvalue = atoi(tkstr.data);

}

/***********************************************************
* ����:	�����ַ��������ַ�������
* sep:		�ַ����������ʶΪ������(')
�ַ������������ʶΪ˫����(")
**********************************************************/
void parse_string(char sep)
{
	char c;
	dynstring_reset(&tkstr);
	dynstring_reset(&sourcestr);
	dynstring_chcat(&sourcestr, sep);
	getch();
	for (;;)
	{
		if (ch == sep)
			break;
		else if (ch == '\\')
		{
			dynstring_chcat(&sourcestr, ch);
			getch();
			switch (ch) //����ת���ַ�
			{
			case '0':
				c = '\0';
				break;
			case 'a':
				c = '\a';
				break;
			case 'b':
				c = '\b';
				break;
			case 't':
				c = '\t';
				break;
			case 'n':
				c = '\n';
				break;
			case 'v':
				c = '\v';
				break;
			case 'f':
				c = '\f';
				break;
			case 'r':
				c = '\r';
				break;
			case '\"':
				c = '\"';
				break;
			case '\'':
				c = '\'';
				break;
			case '\\':
				c = '\\';
				break;
			default:
				c = ch;
				if (c >= '!'&&c < '~')
					warning("�Ƿ�ת���ַ���\'\\%c\'", c);
				else
					warning("�Ƿ�ת���ַ���\'\\0x%x\'", c);
				break;
			}
			dynstring_chcat(&tkstr, c);
			dynstring_chcat(&sourcestr, ch);
			getch();
		}
		else
		{
			dynstring_chcat(&tkstr, ch);
			dynstring_chcat(&sourcestr, ch);
			getch();
		}
	}
	dynstring_chcat(&tkstr, '\0');
	dynstring_chcat(&sourcestr, sep);
	dynstring_chcat(&sourcestr, '\0');
	getch();
}

/***********************************************************
*  ����:	ȡ����
**********************************************************/
void get_token()
{
	preprocess();
	switch (ch)
	{
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
	case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
	case 'o': case 'p': case 'q': case 'r': case 's': case 't':
	case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
	case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
	case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
	case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
	case '_':
	{
		TkWord* tp;
		tp = parse_identifier();
		token = tp->tkcode;
		break;
	}
	case '0': case '1': case '2': case '3':
	case '4': case '5': case '6': case '7':
	case '8': case '9':
		parse_num();
		token = TK_CINT;
		break;
	case '+':
		getch();
		token = TK_PLUS;
		break;
	case '-':
		getch();
		if (ch == '>')
		{
			token = TK_POINTSTO;
			getch();
		}
		else
			token = TK_MINUS;
		break;
	case '/':
		token = TK_DIVIDE;
		getch();
		break;
	case '%':
		token = TK_MOD;
		getch();
		break;
	case '=':
		getch();
		if (ch == '=')
		{
			token = TK_EQ;
			getch();
		}
		else
			token = TK_ASSIGN;
		break;
	case '!':
		getch();
		if (ch == '=')
		{
			token = TK_NEQ;
			getch();
		}
		else
			error("�ݲ�֧��'!'(�ǲ�����)");
		break;
	case '<':
		getch();
		if (ch == '=')
		{
			token = TK_LEQ;
			getch();
		}
		else
			token = TK_LT;
		break;
	case '>':
		getch();
		if (ch == '=')
		{
			token = TK_GEQ;
			getch();
		}
		else
			token = TK_GT;
		break;
	case '.':
		getch();
		if (ch == '.')
		{
			getch();
			if (ch != '.')
				error("ʡ�Ժ�ƴд����");
			else
				token = TK_ELLIPSIS;
			getch();
		}
		else
		{
			token = TK_DOT;
		}
		break;
	case '&':
		token = TK_AND;
		getch();
		break;
	case ';':
		token = TK_SEMICOLON;
		getch();
		break;
	case ']':
		token = TK_CLOSEBR;
		getch();
		break;
	case '}':
		token = TK_END;
		getch();
		break;
	case ')':
		token = TK_CLOSEPA;
		getch();
		break;
	case '[':
		token = TK_OPENBR;
		getch();
		break;
	case '{':
		token = TK_BEGIN;
		getch();
		break;
	case ',':
		token = TK_COMMA;
		getch();
		break;
	case '(':
		token = TK_OPENPA;
		getch();
		break;
	case '*':
		token = TK_STAR;
		getch();
		break;
	case '\'':
		parse_string(ch);
		token = TK_CCHAR;
		tkvalue = *(char *)tkstr.data;
		break;
	case '\"':
	{
		parse_string(ch);
		token = TK_CSTR;
		break;
	}
	case EOF:
		token = TK_EOF;
		break;
	default:
		error("����ʶ���ַ�:\\x%02x", ch); //�����ַ�������ַ���ֻ���������Դ���ַ�������������ֵ�Դ�������λ��
		getch();
		break;
	}
}