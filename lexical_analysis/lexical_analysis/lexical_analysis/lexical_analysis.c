#include "lexical_analysis.h"

FILE *fin = NULL;
char *filename;
char *outfile;

/***********************************************************
* ����:	�ʷ���ɫ
**********************************************************/
void color_token(int lex_state)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	char *p;
	switch (lex_state)
	{
	case LEX_NORMAL:
	{
		if (token >= TK_IDENT)
			SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);
		else if (token >= KW_CHAR)
			SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		else if (token >= TK_CINT)
			SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN);
		else
			SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
		p = get_tkstr(token);
		printf("%s", p);
		break;
	}
	case LEX_SEP:
		printf("%c", ch);
		break;
	}
}

/***********************************************************
*  ����:	�ʷ���������
**********************************************************/
void test_lex()
{
	do
	{
		get_token();
		color_token(LEX_NORMAL);
	} while (token != TK_EOF);
	printf("\n��������: %d��\n", line_num);
}


/***********************************************************
* ����:	��ʼ��
**********************************************************/
void init()
{
	line_num = 1;
	init_lex();
}


/***********************************************************
* ����:	ɨβ������
**********************************************************/
void cleanup()
{
	int i;
	for (i = TK_IDENT; i < tktable.count; i++)
	{
		free(tktable.data[i]);
	}
	free(tktable.data);
}

/***********************************************************
* ����:	�õ��ļ���չ��
* fname:	�ļ�����
**********************************************************/
char *get_file_ext(char *fname)
{
	char *p;
	p = strrchr(fname, '.');
	return p + 1;
}
/***********************************************************
* ����:	��Դ�ļ��ж�ȡһ���ַ�
**********************************************************/
void getch()
{
	ch = getc(fin);
}


/***********************************************************
* ����:	main������
**********************************************************/
int main(int argc, char ** argv)
{

	fin = fopen(argv[1], "rb");
	system("pause");
	if (!fin)
	{
		printf("���ܴ�SCԴ�ļ�!\n");
		return 0;
	}
	init();
	getch();
	test_lex();
	cleanup();
	fclose(fin);
	printf("%s �ʷ������ɹ�!", argv[1]);
	system("pause");
	return 1;
}