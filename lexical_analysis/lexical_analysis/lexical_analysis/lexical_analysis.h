#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define MAXKEY	1024				// ��ϣ������
#define CH_EOF   (-1)				// �ļ�β��ʶ


/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*��̬�ַ�������*/
typedef struct DynString
{
	int count; //�ַ�������
	int capacity; //�������ַ����Ļ���������
	char *data; //ָ���ַ�����ָ��
}DynString;
void dynstring_init(DynString *pstr, int initsize);//��ʼ����̬�ַ����洢����������dynstring_reset()
void dynstring_free(DynString *pstr);//�ͷŶ�̬�ַ���ʹ�õ��ڴ�ռ�,����dynstring_reset()
void dynstring_realloc(DynString *cstr, int new_size);//���·����ַ�������
void dynstring_chcat(DynString *cstr, int ch);//�ַ���������ַ�
void dynstring_reset(DynString *cstr);//���ö�̬�ַ���

////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////

/*��̬���鶨��*/
typedef struct DynArray
{
	int count;//��̬����Ԫ�ظ���
	int capacity;//��̬���黺��������
	void **data;//ָ������ָ�������
}DynArray;

void dynarray_realloc(DynArray *parr, int new_size);//���·��䶯̬��������,����dynarray_add()����
void dynarray_add(DynArray *parr, void *data);//׷�Ӷ�̬����Ԫ��
void dynarray_init(DynArray *parr, int initsize);//��ʼ����̬����洢����
void dynarray_free(DynArray *parr);//�ͷŶ�̬����ʹ�õ��ڴ�ռ�
int dynarray_search(DynArray *parr, int key);//��̬����Ԫ�ز���

///////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////

/* ���󼶱� */
enum e_ErrorLevel
{
	LEVEL_WARNING,
	LEVEL_ERROR,
};

/* �����׶� */
enum e_WorkStage
{
	STAGE_COMPILE,
	STAGE_LINK,
};

void handle_exception(int stage, int level, char *fmt, va_list ap);//�쳣������Ϊ���뾯�桢��������Ӵ�����õĹ��ܺ���
void warning(char *fmt, ...);//���뾯�洦��
void error(char *fmt, ...);//������
void expect(char *msg);//��ʾ���󣬴˴�ȱ��ĳ���﷨�ɷ�
void link_error(char *fmt, ...);//���Ӵ�����
void *get_tkstr(int v);//ȡ�õ���v�������Դ���ַ���

////////////////////////////////////////////////////////////////////////////////////////////////////

/*���ʴ洢�ṹ����*/
typedef struct TkWord
{
	int tkcode;//���ʱ���
	struct TkWord *next; //ָ���ϣ��ͻ��ͬ���
	char *spelling; //�����ַ���
}TkWord;

int ELFHash(char *str);//�����ϣ��ַ
TkWord* tkword_direct_insert(TkWord* tp);//����������ؼ��֡�����ֱ�ӷ��뵥�ʱ�
TkWord* tkword_find(char *p, int keyno);//�ڵ��ʱ��в��ҵ���,����tkword_insert()
void *mallocz(int size);//������ڴ沢�����ݳ�ʼ��Ϊ'0',����tkword_insert()
TkWord* tkword_insert(char *p);//��ʶ�����뵥�ʱ��Ȳ��ң����Ҳ����ٲ��뵥�ʱ�

/////////////////////////////////////////////////////////////////////////////////////////////////////

/* ���ʱ��� */
enum e_TokenCode
{
	/* ��������ָ��� */
	TK_PLUS,		// + �Ӻ�
	TK_MINUS,		// - ����
	TK_STAR,		// * �Ǻ�
	TK_DIVIDE,		// / ����
	TK_MOD,			// % ���������
	TK_EQ,			// == ���ں�
	TK_NEQ,			// != �����ں�
	TK_LT,			// < С�ں�
	TK_LEQ,			// <= С�ڵ��ں�
	TK_GT,			// > ���ں�
	TK_GEQ,			// >= ���ڵ��ں�
	TK_ASSIGN,		// = ��ֵ����� 
	TK_POINTSTO,	// -> ָ��ṹ���Ա�����
	TK_DOT,			// . �ṹ���Ա�����
	TK_AND,         // & ��ַ�������
	TK_OPENPA,		// ( ��Բ����
	TK_CLOSEPA,		// ) ��Բ����
	TK_OPENBR,		// [ ��������
	TK_CLOSEBR,		// ] ��Բ����
	TK_BEGIN,		// { �������
	TK_END,			// } �Ҵ�����
	TK_SEMICOLON,	// ; �ֺ�    
	TK_COMMA,		// , ����
	TK_ELLIPSIS,	// ... ʡ�Ժ�
	TK_EOF,			// �ļ�������

	/* ���� */
	TK_CINT,		// ���ͳ���
	TK_CCHAR,		// �ַ�����
	TK_CSTR,		// �ַ�������								

	/* �ؼ��� */
	KW_CHAR,		// char�ؼ���
	KW_SHORT,		// short�ؼ���
	KW_INT,			// int�ؼ���
	KW_VOID,		// void�ؼ���  
	KW_STRUCT,		// struct�ؼ���   
	KW_IF,			// if�ؼ���
	KW_ELSE,		// else�ؼ���
	KW_FOR,			// for�ؼ���
	KW_CONTINUE,	// continue�ؼ���
	KW_BREAK,		// break�ؼ���   
	KW_RETURN,		// return�ؼ���
	KW_SIZEOF,		// sizeof�ؼ���

	KW_ALIGN,		// __align�ؼ���	
	KW_CDECL,		// __cdecl�ؼ��� standard c call
	KW_STDCALL,     // __stdcall�ؼ��� pascal c call

	/* ��ʶ�� */
	TK_IDENT
};

/* �ʷ�״̬ */
enum e_LexState
{
	LEX_NORMAL,
	LEX_SEP
};

void init_lex();//�ʷ�������ʼ��
void skip_white_space();//���Կո�,TAB���س�
void parse_comment();//����ע��
void preprocess();//Ԥ�������Էָ�����ע��
int is_nodigit(char c);//�ж�c�Ƿ�Ϊ��ĸ(a-z,A-Z)���»���(-)
int is_digit(char c);//�ж�c�Ƿ�Ϊ����
TkWord* parse_identifier();//������ʶ��
void parse_num();//�������ͳ���
void parse_string(char sep);//�����ַ��������ַ�������
void get_token();//ȡ����

//////////////////////////////////////////////////////////////////////////////////////////////////////

void getch();//��Դ�ļ��ж�ȡһ���ַ�

extern TkWord* tk_hashtable[MAXKEY];// ���ʹ�ϣ��
extern DynArray tktable;			// ���ʶ�̬����
extern DynString tkstr;		//�����ַ���
extern DynString sourcestr;	//����Դ���ַ���

extern char ch;		//��ǰȡ����Դ���ַ�
extern int token;		//���ʱ���
extern int tkvalue;	//����ֵ
extern int line_num;	//�к�

extern FILE *fin;
extern char *filename;
extern char *outfile;