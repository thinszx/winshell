/*
* controlflow.c - �����������̿���
*/
#include<stdio.h>
#include<string.h>
#include "winshell.h"

enum states {NEUTRAL, WANT_THEN, THEN_BLOCK};	// �����̿��Ʒ�Ϊ��������
enum results {SUCCESS, FAIL};

static int if_state = NEUTRAL;
static int if_result = SUCCESS;
static int processed_stat = 0;	// ���̿�������н���ִ�е�״̬

// ͨ����ǰ��if_state�ж��Ƿ���Խ�������ִ������
int execute_check() {
	int rv = 0;	// �����

	if (if_state == WANT_THEN)	// if���汾Ӧ����thenȴδ����
		rv = syn_err((char*)"then expected");	// rv == -1
	else if (if_state == THEN_BLOCK && if_result == FAIL)	// δ�ɹ�ִ���������
		rv = -1;
	// �����������������ֱ��ִ������ɣ������ж�
	return rv;
}

// �ж��Ƿ�Ϊ���̿������
int is_control_command(char* s) {
	return (strcmp(s, "if") == 0
		|| strcmp(s, "then") == 0
		|| strcmp(s, "fi") == 0
		);
}

// ���ݵ�ǰif_state��״̬������������ִ��
int do_control_command(char** args) {
	char* ctrlflag = args[0];
	int rv = -1;	// �������ִ�гɹ���־��Ĭ��Ϊ���ɹ�

	if (strcmp(ctrlflag, "if") == 0) {
		if (if_state != NEUTRAL)	// ��ǰ��������������
			rv = syn_err((char*)"if unexpected");
		else {
			processed_stat = process(args + 1);	// ָ�����̿�������ĵ�һ����
			if_result = (processed_stat == 0 ? SUCCESS : FAIL);
			if_state = WANT_THEN;	// ����then��䣬shell��IF��������then���
			rv = 0;	// ���سɹ�ִ�б�־
		}
	}
	else if (strcmp(ctrlflag, "then") == 0) {
		if (if_state != WANT_THEN)	// ��ǰ����WANT_THEN������ȴ������then
			rv = syn_err((char*)"then unexpected");
		else {
			if_state = THEN_BLOCK;
			rv = 0;
		}
	}
	else if (strcmp(ctrlflag, "fi") == 0) {
		if (if_state != THEN_BLOCK)
			rv = syn_err((char*)"fi unexpected");
		else {
			if_state = NEUTRAL;
			rv = 0;
		}
	}
	else
		fatal((char*)"internal error processing: ", ctrlflag, 2);
	return rv;
}

int syn_err(char* msg) {
	if_state = NEUTRAL;
	fprintf_s(stderr, "syntax error: %s\n", msg);
	return -1;
}