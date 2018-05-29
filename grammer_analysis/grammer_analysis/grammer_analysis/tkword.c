#include "grammar_analysis.h"

TkWord* tk_hashtable[MAXKEY];	// ���ʹ�ϣ��
DynArray tktable;		// ���ʱ�

/***********************************************************
* ����:	�����ϣ��ַ
* key:��ϣ�ؼ���
* MAXKEY:��ϣ����
**********************************************************/
int ELFHash(char *str)
{
	int hash = 0;
	int x = 0;

	while (*str)
	{
		hash = (hash << 4) + *str++;
		x = hash & 0xf0000000;
		if (x)
			hash ^= x >> 24;
		hash &= ~x;
	}
	return hash % MAXKEY;
}

/***********************************************************
* ����: ����������ؼ��֡�����ֱ�ӷ��뵥�ʱ�
* tp:	 ����ָ��
**********************************************************/
TkWord* tkword_direct_insert(TkWord* tp)
{
	int keyno;
	dynarray_add(&tktable, tp);
	keyno = ELFHash(tp->spelling);
	tp->next = tk_hashtable[keyno];
	tk_hashtable[keyno] = tp;
	return tp;
}

/***********************************************************
* ����:	 �ڵ��ʱ��в��ҵ���
*  p:		�����ַ���ָ��
*  keyno:	���ʵĹ�ϣֵ
**********************************************************/
TkWord* tkword_find(char *p, int keyno)
{
	TkWord *tp = NULL, *tp1;
	for (tp1 = tk_hashtable[keyno]; tp1; tp1 = tp1->next)
	{
		if (!strcmp(p, tp1->spelling))
		{
			token = tp1->tkcode;
			tp = tp1;
		}
	}
	return tp;
}

/***********************************************************
* ����:	������ڴ沢�����ݳ�ʼ��Ϊ'0'
* size: �����ڴ��С
**********************************************************/
void *mallocz(int size)
{
	void *ptr;
	ptr = malloc(size);
	if (!ptr && size)
		error("�ڴ����ʧ��");
	memset(ptr, 0, size);
	return ptr;
}

/***********************************************************
* ����:	��ʶ�����뵥�ʱ��Ȳ��ң����Ҳ����ٲ��뵥�ʱ�
* p:		�����ַ���ָ��
**********************************************************/
TkWord* tkword_insert(char *p)
{
	TkWord *tp;
	int keyno;
	char *s;
	char *end;
	int length;

	keyno = ELFHash(p);
	tp = tkword_find(p, keyno);
	if (tp == NULL)
	{
		length = strlen(p);
		tp = (TkWord*)mallocz(sizeof(TkWord) + length + 1);
		tp->next = tk_hashtable[keyno];
		tk_hashtable[keyno] = tp;
		dynarray_add(&tktable, tp);
		tp->tkcode = tktable.count - 1;
		s = (char*)tp + sizeof(TkWord);
		tp->spelling = (char *)s;
		for (end = p + length; p < end;)
		{
			*s++ = *p++;
		}
		*s = (char)'\0';
	}
	return tp;
}