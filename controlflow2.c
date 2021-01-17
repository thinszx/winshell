#include <stdio.h>
#include "winshell2.h"

/* 
 * if_mode  - 用来进行语法上的if、else、then的控制
 * laststat - 用来进行逻辑上的判定，通过语法检查+条件的判定，返回是否执行的标志给主程序
 */
static int if_mode = NO_IF;	// 初始的if状态
static int laststat;		// 用来存储上个命令的执行状态

/* 接下来是维护的if状态栈的定义 */
#define maxn 10	// 支持的嵌套最大值
typedef struct {
	int if_mode_store;
	int laststat_store;
}env;			// 环境变量

typedef struct {
	int top;    // 栈顶的索引
	int fi_empty;	// 用来判断当前栈是否还有未弹出的fi
	env elements[maxn];
}Stack;

Stack stack = { 0, 0 ,NULL };	// 声明栈

int if_check(char *cmd) {
	char* cp;
	
	// 当前读取进来的命令为空，抛出文件尾异常
	if (cmd == NULL) {
		if (stack.fi_empty !=0)	// 证明当前还有fi未弹出栈
			syn_err((char*)"lack of fi");
		if (if_mode != NO_IF)
			syn_err((char*)"EOF unexpected");
		return DONT_EXEC;
	}

	// 检测if
	if (strncmp(cmd, "if", 2) == 0 && is_blank(cmd[2])) {	// 确保指令是'if '的格式，防止if后直接跟指令
		if (save_env() != 0) {
			syn_err((char*)"to many if levels(more than 10)");	// if嵌套语法错误
			return SYN_ERR;
		}
		if (if_mode != SAW_THEN && laststat != 0)
			return DONT_EXEC;
		if (if_mode != NO_IF) {	// 当前if成功执行
			set_ifenv();	// 开始进行嵌套if的执行
			// return DONT_EXEC;
		}
		// 开始执行if后面的语句，检查返回结果
		for (cp = cmd + 3; is_blank(*cp); cp++);	// 将指针移到if后面的首个非空位置

		if (!*cp) {
			syn_err((char*)"null command for if");
			return DONT_EXEC;
		}
		laststat = process(cp);
		if_mode = SAW_IF;
		return DONT_EXEC;
	}

	// 检测then else fi
	// TODO: 检测elif
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
		// 当前fi全部清除完后，无论如何都该将if_mode设为初值
		/*if (stack.fi_empty == 0)
			if_mode = NO_IF;*/
		return DONT_EXEC;
	}

	// 当前在if块当中
	if (if_mode == SAW_IF) {
		laststat = process(cmd);
		return DONT_EXEC;
	}

	// 当前并非流程控制语句，开始根据状态执行代码
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

// 保护当前的if环境，即当前的if_mode和laststat的值
// 实质上等同于压栈操作
int save_env() {
	if (stack.top == maxn - 1)
		// fatal("If syntax error:", "To many if levels(more than 10)", 2);	// if嵌套层数过多
		return -1;	// if嵌套层数过多
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

// 恢复原有的if环境
// 实质上等同于出栈操作
int restore_env() {
	if (stack.top == 0)
		return -1;	// fi和原来的数量不匹配，比原来的要多
	if_mode = stack.elements[--stack.top].if_mode_store;
	laststat = stack.elements[stack.top].laststat_store;
	stack.fi_empty--;
	return 0;
}

void set_ifenv() {
	if_mode = SAW_IF;
	laststat = 0;
}