/* 为shell添加变量支持 */

#define VARLEN 20
// int	value_environ2table(char**);
// int	value_export(char*);
char* value_lookup(char* name);
int	value_store(char* name, char* val);
int is_an_idchar(char ch, int pos);
char* getvar(char** srcp);
void split(char* src, const char* separator, char** dest, int* num);
// char** value_table2environ();
// int	value_environ2table(char**);
int eval(char* src, char* dest, int len);
int value_assign(char* cmd);