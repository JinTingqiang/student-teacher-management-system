#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getch.h>
#include "tool.h"
#define len_id 20
Student* stu=NULL;//在校学生指针
Quitstu* quit_stu=NULL;//离校学生指针
Teacher* Tch=NULL;//定义在职教师指针
Master* Mst=NULL;//定义校长指针
Teacher* Tch_quit=NULL;//离职教师指针
void show(void)
{
	system("clear");
	puts("——学生成绩管理系统——");
	puts("1、 学生用户");
	puts("2、 教师用户");		
	puts("3、 校长用户");
	puts("4、   退出");	
	puts("———————————————————");
}

int code_rtl(const char* password)//判断密码是否合理
{

	int len=strlen(password);
	if(len>31||len<3)//判断长度
	{
		return 0;
	}
	for(int i=0;i<len;i++)
	{
		unsigned char ch=password[i];
		if(!((ch>=33 && ch<=126)&&(isalnum(ch)||ispunct(ch))))//isalnum()判断是否为字母或数字，是返回非零，否则返回零，ispunct()判断是否为标点符号，是返回非零，否则返回零
		{
			return 0;
		}
		
	}

	return 1;
}

void code_encryption(char* password,int num)
{
	int len=strlen(password);
	for(int i=0;i<len;i++)
	{
		unsigned char ch=password[i];
		ch=(ch+(num%128))%94+33;//按照给定的num加密，范围在33-126
		password[i]=ch;
	}
}

/*void code_decryption(char* password,int num)
{
	int len=strlen(password);
	for(int i=0;i<len;i++)
	{
		unsigned char ch=password[i];
		ch=(ch+128)%128-(num%128);
		password[i]=ch;
	}
}*/
char* hide_code(void)
{
	static char p[32];
	int i=0;
	do
	{
		p[i]=getch();
		if(10==(int)p[i])//回车
		{
			p[i]='\0';
			break;
		}
		if(127==(int)p[i])//退格
		{
			if(0==i)
			{
				continue;
			}
			i=i-1;
			printf("\b \b");
		}
		else
		{
			i=i+1;
			if(32!=i)
			{
				printf("*");
			}
			else
			{
				i=i-1;
			}
		}
	}while('\n'!=p[i]&&i<32);
	puts("");
	return p;
}

void import(char* p,int num)
{
	int i=0;
	do
	{
		p[i]=getch();
		if(10==(int)p[i])//回车
		{
			p[i]='\0';
			break;
		}
		if(127==(int)p[i])//退格
		{
			if(0==i)
			{
				continue;
			}
			i=i-1;
			printf("\b \b");
		}
		else
		{
			i=i+1;
			if(num!=i)
			{
				printf("%c",p[i-1]);
			}
			else
			{
				i=i-1;
			}
			
		}
	}while('\n'!=p[i]&&i<num);
	puts("");
}

int import_id(void)
{
	int p[len_id];
	int id=0;
	int i=0;
	do
	{
		p[i]=getch();
		if(10==p[i])//回车
		{
			p[i]='\0';
			break;
		}
		if(127==p[i])//退格
		{
			if(0==i)
			{
				continue;
			}
			i=i-1;
			printf("\b \b");
			id=id/10;
		}
		else
		{
			i=i+1;
			if(len_id!=i)
			{
				printf("%d",p[i-1]-48);
				id=id*10+(p[i-1]-48);
			}
			else
			{
				i=i-1;
			}
		}
	}while('\n'!=p[i]&&i<len_id);
	puts("");
	return id;
}
void anykey(void)
{
	printf("按任意键返回");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
	getch();	
}
