#include <stdio.h>
#include "winshell2.h"

/* 
 * if_mode  - ���������﷨�ϵ�if��else��then�Ŀ���
 * laststat - ���������߼��ϵ��ж���ͨ���﷨���+�������ж��������Ƿ�ִ�еı�־��������
 */
static int if_mode = NO_IF;	// ��ʼ��if״̬
static int laststat;		// �����洢�ϸ������ִ��״̬

/* ��������ά����if״̬ջ�Ķ��� */
#define maxn 10	// ֧�ֵ�Ƕ�����ֵ
typedef struct {
	int if_mode_store;
	int laststat_store;
}env;			// ��������

typedef struct {
	int top;    // ջ��������
	int fi_empty;	// �����жϵ�ǰջ�Ƿ���δ������fi
	env elements[maxn];
}Stack;

Stack stack = { 0, 0 ,NULL };	// ����ջ

int if_check(char *cmd) {
	char* cp;
	
	// ��ǰ��ȡ����������Ϊ�գ��׳��ļ�β�쳣
	if (cmd == NULL) {
		if (stack.fi_empty !=0)	// ֤����ǰ����fiδ����ջ
			syn_err((char*)"lack of fi");
		if (if_mode != NO_IF)
			syn_err((char*)"EOF unexpected");
		return DONT_EXEC;
	}

	// ���if
	if (strncmp(cmd, "if", 2) == 0 && is_blank(cmd[2])) {	// ȷ��ָ����'if '�ĸ�ʽ����ֹif��ֱ�Ӹ�ָ��
		if (save_env() != 0) {
			syn_err((char*)"to many if levels(more than 10)");	// ifǶ���﷨����
			return SYN_ERR;
		}
		if (if_mode != SAW_THEN && laststat != 0)
			return DONT_EXEC;
		if (if_mode != NO_IF) {	// ��ǰif�ɹ�ִ��
			set_ifenv();	// ��ʼ����Ƕ��if��ִ��
			// return DONT_EXEC;
		}
		// ��ʼִ��if�������䣬��鷵�ؽ��
		for (cp = cmd + 3; is_blank(*cp); cp++);	// ��ָ���Ƶ�if������׸��ǿ�λ��

		if (!*cp) {
			syn_err((char*)"null command for if");
			return DONT_EXEC;
		}
		laststat = process(cp);
		if_mode = SAW_IF;
		return DONT_EXEC;
	}

	// ���then else fi
	// TODO: ���elif
	if (strncmp(cmd, "then", 4) == 0) {	// then
		if (if_mode != SAW_IF)
			syn_err((char*)"\'then\' unexpected");
		else
			if_mode = SAW_THEN;
		return DONT_EXEC;
	}
	if (strncmp(cmd, "else", 4) == 0) {	// else
		if (if_mode != SAW_THEN)
			syn_err((char*)"\'else\' unexpectd");
		else
			if_mode = SAW_ELSE;
		return DONT_EXEC;
	}
	if (strncmp(cmd, "fi", 2) == 0) {	// fi
		if (if_mode != SAW_THEN && if_mode != SAW_ELSE)
			syn_err((char*)"\'fi\' unexpected");
		else
			restore_env();
		// ��ǰfiȫ��������������ζ��ý�if_mode��Ϊ��ֵ
		/*if (stack.fi_empty == 0)
			if_mode = NO_IF;*/
		return DONT_EXEC;
	}

	// ��ǰ��if�鵱��
	if (if_mode == SAW_IF) {
		laststat = process(cmd);
		return DONT_EXEC;
	}

	// ��ǰ�������̿�����䣬��ʼ����״ִ̬�д���
	if (if_mode == NO_IF
		|| if_mode == SAW_THEN && laststat == 0
		|| if_mode == SAW_ELSE && laststat != 0
		)
		return OK_EXEC;

	return DONT_EXEC;
}

int syn_err(char* msg) {
	if_mode = NO_IF;
	fprintf_s(stderr, "syntax error: %s\n", msg);
	return -1;
}

// ������ǰ��if����������ǰ��if_mode��laststat��ֵ
// ʵ���ϵ�ͬ��ѹջ����
int save_env() {
	if (stack.top == maxn - 1)
		// fatal("If syntax error:", "To many if levels(more than 10)", 2);	// ifǶ�ײ�������
		return -1;	// ifǶ�ײ�������
	stack.elements[stack.top].if_mode_store = if_mode;
	stack.elements[stack.top].laststat_store = laststat;
	stack.top++;
	stack.fi_empty++;
	return 0;
}

int check_inside(char* cmd) {
	if (strncmp(cmd, "echo ", 5) == 0)
		return 1;
	else
		return 0;
}

// �ָ�ԭ�е�if����
// ʵ���ϵ�ͬ�ڳ�ջ����
int restore_env() {
	if (stack.top == 0)
		return -1;	// fi��ԭ����������ƥ�䣬��ԭ����Ҫ��
	if_mode = stack.elements[--stack.top].if_mode_store;
	laststat = stack.elements[stack.top].laststat_store;
	stack.fi_empty--;
	return 0;
}

void set_ifenv() {
	if_mode = SAW_IF;
	laststat = 0;
}