#include "lexical_analysis.h"

/******************************************************************
* �쳣����
* stage: ����׶λ������ӽ׶�
* level�����󼶱�
* fmt�����������ʽ
* ap���ɱ�����б�
*******************************************************************/
void handle_exception(int stage, int level, char *fmt, va_list ap)
{
	char buf[1024];
	vsprintf(buf, fmt, ap);
	if (stage == STAGE_COMPILE)
	{
		if (level == LEVEL_WARNING)
			printf("%s(��%d��): ���뾯��: %s!\n", filename, line_num, buf);
		else
		{
			printf("%s(��%d��): �������: %s!\n", filename, line_num, buf);
			exit(-1);
		}
	}
	else
	{
		printf("���Ӵ���: %s!\n", buf);
		exit(-1);
	}
}

/******************************************************************
* ���뾯�洦��
* fmt�����������ʽ
* ap���ɱ�����б�
*******************************************************************/
void warning(char *fmt, ...)
{
	va_list ap = NULL;
	va_start(ap, fmt);
	handle_exception(STAGE_COMPILE, LEVEL_WARNING, fmt, ap);
	va_end(ap);
}

/******************************************************************
* ���������
* fmt�����������ʽ
* ap���ɱ�����б�
*******************************************************************/
void error(char *fmt, ...)
{
	va_list ap = NULL;
	va_start(ap, fmt);
	handle_exception(STAGE_COMPILE, LEVEL_ERROR, fmt, ap);
	va_end(ap);
}

/******************************************************************
* ���Ӵ�����
* fmt�����������ʽ
* ap���ɱ�����б�
*******************************************************************/
void link_error(char *fmt, ...)
{
	va_list ap = NULL;
	va_start(ap, fmt);
	handle_exception(STAGE_LINK, LEVEL_ERROR, fmt, ap);
	va_end(ap);
}

/******************************************************************
* ��ʾ���󣬴˴�ȱ��ĳ���﷨�ɷ�
* msg����Ҫʲô�﷨�ɷ�
*******************************************************************/
void expect(char *msg)
{
	error("ȱ��%s", msg);
}

/******************************************************************
* ���ܣ�ȡ�õ���v�������Դ���ַ���
* v�����ʱ��
*******************************************************************/
char *get_tkstr(int v)
{
	if (v > tktable.count)
		return NULL;
	else if (v >= TK_CINT && v <= TK_CSTR)
		return sourcestr.data;
	else
		return ((TkWord*)tktable.data[v])->spelling;
}