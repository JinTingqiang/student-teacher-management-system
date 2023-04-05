#ifndef HEADMASTER_H
#define HEADMASTER_H
void master(void);
void reset_code_mst(void);//重置校长密码
void reset_code_tch(void);//重置教师密码
void add_tch(void);//增加教师
void del_tch(void);//删除教师
void show_insch_tch(void);//显示在职教师
void show_quitsch_tch(void);//显示离职教师
void unlock_tch(void);//解锁教师帐号
void change_code_new(char* code);//改密码
void show_mst(void);//显示界面
int log_on_mst(void);//第一次登录判断
extern char code_mst[20];//密码
extern char mst_name[20];//名字
#endif//HEADMASTER_H
