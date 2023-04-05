#ifndef ADMIN_H
#define ADMIN_H
#include <stdbool.h>
void show(void);

//判断密码是否合法
int code_rtl(const char* password);

//密码加密显示
void code_encryption(char* password,int num);

//密码解密
//void code_decryption(char* password,int num);

//隐藏输入密码
char* hide_code(void);

//任意键返回
void anykey(void);

void import(char* p,int num);
int import_id(void);

typedef struct Student
{
	char name[20];//姓名
	char sex[4];//性别
	int id;//学号
	char code[32];//密码
	float Chinese;//语文成绩
	float Math;//数学成绩
	float English;//英语成绩
	int first_logon;//判断是否第一次登录，0为第一次登录
	int code_cnt;//密码输入次数
	int lock;//判断帐号是否锁定，0为没有锁定
}Student;



typedef struct Quitstu
{
	char name[20];//姓名
	char sex[4];//性别
	int id;//学号
	float Chinese;//语文成绩
	float Math;//数学成绩
	float English;//英语成绩
}Quitstu;


typedef struct Teacher
{
	char name[20];//教师姓名
	char sex[4];//教师性别
	int id;//工号
	char code[32];//教师密码
	int first_logon;//是否第一次登录，0为第一次
	int lock;//是否锁定，0为未锁定
	int code_cnt;//记录密码输入错误次数
	
}Teacher;

typedef struct Master
{
	char name[20];//校长姓名
	char sex[4];//校长性别
	char code[32];//校长密码
	int num_tch_in;//在职教师人数
	int num_tch_quit;//离职教师人数
	int first_logon;//是否第一次登录，0为第一次

}Master;
extern Student* stu;//在校学生指针
extern Quitstu* quit_stu;//离校学生指针
extern Teacher* Tch;//定义在职教师指针
extern Master* Mst;//定义校长指针
extern Teacher* Tch_quit;//离职教师指针
#endif//ADMIN_H
