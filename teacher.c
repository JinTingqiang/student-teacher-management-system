#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getch.h>
#include "tool.h"
#include "teacher.h"
extern int num_tch_in;
int cnt_stu;//学生人数
int cnt_quit_stu;//退学学生人数
void teacher(void)
{//登录教师端
	//使用帐号登录
	int id;
	char code[32]={};
	int i;
	printf("请输入工号：");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
	id=import_id();//输入帐号
	FILE* frp = fopen("insch_tch.txt","r");
	if(NULL==frp)
	{
		perror("fopen");
		return;
	}
	for(i=0;i<num_tch_in;i++)
	{
		fscanf(frp,"%s %s %d %s %d %d %d",Tch[i].name,Tch[i].sex,&Tch[i].id,Tch[i].code,&Tch[i].first_logon,&Tch[i].lock,&Tch[i].code_cnt);
	}	
	fclose(frp);
	for(i=0;i<num_tch_in;i++)
	{	
		if(id==Tch[i].id)
		{
			break;
		}
	}
	if(i==num_tch_in)
	{
		printf("帐号不存在！\n");
		anykey();
		return ;
	}
	if(1==Tch[i].lock)
	{
		printf("您的帐号已上锁，请找校长解锁\n");
		return ;
	}

	//密码判断
	printf("请输入密码：");
	for(;;)
	{
		stdin->_IO_read_ptr=stdin->_IO_read_end;
		strcpy(code,hide_code());
		code_encryption(code,Tch[i].id);
		if(!strcmp(code,Tch[i].code))
		{
			Tch[i].code_cnt=0;
			printf("登录成功！\n");
			break;
		}
		else 
		{
			Tch[i].code_cnt++;
		}
		if(1==Tch[i].code_cnt)
		{
			printf("输入错误，还有两次机会！\n");
		}
		else if(2==Tch[i].code_cnt)
		{
			printf("输入错误，还有一次机会！\n");
		}
		else if(3==Tch[i].code_cnt)
		{
			Tch[i].lock=1;
			printf("密码输入错误三次，帐号已锁定，解锁请校长解锁！\n");
			return ;
		}
	}
	//是否初次登录
	if (0==Tch[i].first_logon)
	{
		printf("初次登录请修改密码！\n");
		Tch[i].first_logon=1;
		printf("请输入新密码：");
		char newcode[20];
		scanf("%s",newcode);
		if(!(code_rtl(newcode)))//判断是否合法
		{
			printf("密码不合法！\n");
			anykey();
			return;
		}	
		code_encryption(newcode,Tch[i].id);
		strcpy(Tch[i].code,newcode);
		printf("修改成功！\n");
	}
	FILE* fwp = fopen("insch_tch.txt","w");
	if(NULL==fwp)
	{
		perror("fopen");
		return;
	}
	for(i=0;i<num_tch_in;i++)
	{
		fprintf(fwp,"%s %s %d %s %d %d %d\n",Tch[i].name,Tch[i].sex,Tch[i].id,Tch[i].code,Tch[i].first_logon,Tch[i].lock,Tch[i].code_cnt);	
	}
	fclose(fwp);		
	for(;;)
	{
		//显示主界面
		switch(menu_tch())
		{
			case '1': add_stu(); break;
			case '2': del_stu(); break;
			case '3': find_stu(); break;
			case '4': rev_stu(); break;
			case '5': input_stu(); break;
			case '6': reset_code_stu(); break;
			case '7': unlock_stu(); break;
			case '8': show_insch_stu(); break;
			case '9': show_quitsch_stu(); break;
			case '0': return ;
		}	
	}

}

//添加学生
void add_stu(void) 
{
	printf("请选择添加学生的方式：\n");
	printf("1、单个添加学生信息\n");
	printf("2、批量导入学生信息\n");
	switch(getch())
	{
		case '1':add_single_stu();break; 	//按1进入单个添加功能
		case '2':add_batch_stu();break;		//按2进入批量导入功能
	}	
}
 
//单个添加
void add_single_stu(void)
{
	int id_max=10000;
	quit_stu_read();
	for(int i=0;i<cnt_quit_stu;i++)
	{
		if(id_max<quit_stu[i].id)
		{
			id_max=quit_stu[i].id;
		}
	}
	stu_read();
	stu = realloc(stu,(cnt_stu+1)*sizeof(Student));
	printf("请输入新学生的姓名:");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
  	import(stu[cnt_stu].name,20); 
	char sex[4]={};
	printf("请输入新学生的性别:");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
	scanf("%s",sex);
	if(!strcmp(sex,"男")||!strcmp(sex,"女"))//判断是否合法
	{
		strcpy(stu[cnt_stu].sex,sex);
	}
	else
	{
		printf("性别不合法\n");
		anykey();
		return;
	}
	for(int i=0;i<cnt_stu;i++)
	{
		if(id_max<stu[i].id)
		{
			id_max=stu[i].id;
		}
	}
	stu[cnt_stu].id=id_max+1;
	char a[]="123456";
	code_encryption(a,id_max);
	strcpy(stu[cnt_stu].code,a);		//学生初始密码为123456
	stu[cnt_stu].first_logon=0;
	stu[cnt_stu].code_cnt=0;
	stu[cnt_stu].lock=0;
	cnt_stu++;
	
	stu_save();
	printf("添加成功\n");
	anykey();
}
 
//批量导入
void add_batch_stu(void)
{
	typedef struct Addstu
	{
		char name[20];
		char sex[4];
	}Addstu;
	Addstu* add_stu=NULL;
	char file_add[32]={};
	printf("请输入要导入的学生名单文件:");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
  	import(file_add,32);  
	FILE* frp=fopen(file_add,"r");	
	if(NULL==frp)
	{
		printf("未找到该文件\n");
		anykey();
		return;
	}
	char r[sizeof(Addstu)];
	int cnt_add_stu=0;
	while(NULL!=fgets(r,sizeof(Addstu),frp))
	{
		add_stu = realloc(add_stu,(cnt_add_stu+1)*sizeof(Addstu));
		sscanf(r,"%s %s\n",add_stu[cnt_add_stu].name,add_stu[cnt_add_stu].sex);
		cnt_add_stu++;
	}
	fclose(frp);
	frp=NULL;
	int id_max=10000;
	quit_stu_read();
	for(int i=0;i<cnt_quit_stu;i++)
	{
		if(id_max<quit_stu[i].id)
		{
			id_max=quit_stu[i].id;
		}
	}
	stu_read();
	for(int i=0;i<cnt_stu;i++)
	{
		if(id_max<stu[i].id)
		{
			id_max=stu[i].id;
		}
	}
	stu = realloc(stu,(cnt_stu+cnt_add_stu)*sizeof(Student));
	for(int i=cnt_stu,j=0;i<cnt_stu+cnt_add_stu;i++)
	{
		strcpy(stu[i].name,add_stu[j].name);
		if(!strcmp(add_stu[j].sex,"男")||!strcmp(add_stu[j].sex,"女"))//判断是否合法
		{
			strcpy(stu[i].sex,add_stu[j].sex);
		}
		else
		{
			printf("性别不合法，\n");
			anykey();
			return;
		}
		stu[i].id=id_max+1+j;
		char code[]="123456";
		code_encryption(code,stu[i].id);
		strcpy(stu[i].code,code);
		stu[i].first_logon=0;
		stu[i].code_cnt=0;
		stu[i].lock=0;
		j++;
	}
	cnt_stu+=cnt_add_stu;
	stu_save();
	printf("添加成功！\n");
	anykey();
}
 
//删除学生
void del_stu(void)
{
	int del_id,del_id1;
	int flag=0;
	printf("请输入要删除的学生学号:");
	scanf("%d",&del_id);
	printf("请再次输入要删除的学生学号:");
	scanf("%d",&del_id1);
	if(del_id!=del_id1)
	{
		printf("输入学号不一致\n");
		anykey();
		return;
	}
	stu_read();
	quit_stu_read();
	for(int i=0;i<cnt_stu;i++)
	{
		if(stu[i].id==del_id)
		{
			flag=1;
			quit_stu = realloc(quit_stu,(cnt_quit_stu+1)*sizeof(Quitstu));
			strcpy(quit_stu[cnt_quit_stu].name,stu[i].name);
			strcpy(quit_stu[cnt_quit_stu].sex,stu[i].sex);
			quit_stu[cnt_quit_stu].id=stu[i].id;
			quit_stu[cnt_quit_stu].Chinese=stu[i].Chinese;
			quit_stu[cnt_quit_stu].Math=stu[i].Math;
			quit_stu[cnt_quit_stu].English=stu[i].English;
			cnt_quit_stu++;
			quit_stu_save();
			strcpy(stu[i].sex,"0");
			break;
		}
	}
	stu_save();
	if(flag==0)
	{
		printf("未找到该学生\n");
	}
	else if(flag==1)
	{
		printf("已删除该学生\n");
	}
	anykey();
}
 
//查找学生
void find_stu(void)
{
	printf("请选择功能：\n");
	printf("1、按姓名查找学生\n");
	printf("2、按学号查找学生\n");
	switch(getch())
	{
		case '1':find_name_stu();break;
		case '2':find_id_stu();break;
	}	
}

//按姓名查找
void find_name_stu(void)
{
	char find_name[20]={};
	int flag=0;
	printf("请输入要查找的学生姓名:");
	scanf("%s",find_name);
	stu_read();
	printf("姓名\t性别\t学号\t语文\t数学\t英语\n");
	for(int i=0;i<cnt_stu;i++)
	{
		if(strstr(stu[i].name,find_name)!=NULL)
		{
			flag=1;
			printf("%s\t%s\t%d\t%g\t%g\t%g\n",stu[i].name,stu[i].sex,stu[i].id,
			stu[i].Chinese,stu[i].Math,stu[i].English);
		}
	}
	if(flag==0)
	{
		printf("未找到该学生\n");
	}
	else if(flag==1)
	{
		printf("以上为查到的学生信息\n");
	}
	anykey();
}

//按学号查找
void find_id_stu(void)
{
	int find_id;
	int flag=0;
	printf("请输入要查找的学生学号:");
	scanf("%d",&find_id);
	stu_read();
	printf("姓名\t性别\t学号\t语文\t数学\t英语\n");
	for(int i=0;i<cnt_stu;i++)
	{
		if(stu[i].id==find_id)
		{
			flag=1;
			printf("%s\t%s\t%d\t%g\t%g\t%g\n",stu[i].name,stu[i].sex,stu[i].id,
			stu[i].Chinese,stu[i].Math,stu[i].English);
			break;
		}
	}
	if(flag==0)
	{
		printf("未找到该学生，\n");
	}
	else if(flag==1)
	{
		printf("以上为查到的学生信息\n");
	}
	anykey();
}

//修改学生信息
void rev_stu(void)
{
	printf("请选择功能：\n");
	printf("1、修改学生基础信息\n");
	printf("2、修改学生成绩\n");
	switch(getch())
	{
		case '1':rev_base_stu();break;
		case '2':rev_score_stu();break;
	}	
}
 
//修改学生基础信息
void rev_base_stu(void)
{
	int find_id;
	int flag=0;
	char rev_name[20]={};
	char rev_sex[4]={};
	int i;
	printf("请输入要修改的学生学号:");
	scanf("%d",&find_id);
	stu_read();
	printf("姓名\t性别\t学号\n");
	for(i=0;i<cnt_stu;i++)
	{
		if(stu[i].id==find_id)
		{
			flag=1;
			printf("%s\t%s\t%d\n",stu[i].name,stu[i].sex,stu[i].id);
			break;
		}
	}
	if(flag==0)
	{
		printf("未找到该学生\n");
		anykey();
	}
	else if(flag==1)
	{
		printf("以上为要修改的学生的基础信息，请输入新的基础信息（姓名，性别）\n");
		scanf("%s %s",rev_name,rev_sex);
		strcpy(stu[i].name,rev_name);
		if(!strcmp(rev_sex,"男")||!strcmp(rev_sex,"女"))//判断是否合法
		{
			strcpy(stu[i].sex,rev_sex);
		}
		else
		{
			printf("修改性别不合法\n");
			anykey();
			return;
		}
		stu_save();
		printf("修改成功\n");
		anykey();
	}
}
	
//修改学生成绩
void rev_score_stu(void)
{
	int find_id;
	int flag=0;
	float rev_Chinese;
	float rev_Math;
	float rev_English;
	int i;
	printf("请输入要修改的学生学号:");
	scanf("%d",&find_id);
	stu_read();
	printf("姓名\t语文\t数学\t英语\n");
	for(i=0;i<cnt_stu;i++)
	{
		if(stu[i].id==find_id)
		{
			flag=1;
			printf("%s\t%g\t%g\t%g\n",stu[i].name,stu[i].Chinese,stu[i].Math,stu[i].English);
			break;
		}
	}
	if(flag==0)
	{
		printf("未找到该学生\n");
		anykey();
	}
	else if(flag==1)
	{
		printf("以上为要修改的学生的成绩信息，请输入新的成绩（语文，数学，英语）\n");
		scanf("%f %f %f",&rev_Chinese,&rev_Math,&rev_English);
		if(rev_Chinese>=0 && rev_Chinese<=150)//判断是否合法
		{
			stu[i].Chinese=rev_Chinese;
		}
		else
		{
			printf("成绩不合法\n");
			anykey();
			return;
		}
		if(rev_Math>=0 && rev_Math<=150)//判断是否合法
		{
			stu[i].Math=rev_Math;
		}
		else
		{
			printf("成绩不合法\n");
			anykey();
			return;
		}
		if(rev_English>=0 && rev_English<=150)//判断是否合法
		{
			stu[i].English=rev_English;
		}
		else
		{
			printf("成绩不合法\n");
			anykey();
			return;
		}
		stu_save();
		printf("修改成功\n");
		anykey();
	}

}
 
//录入学生成绩
void input_stu(void) 
{
	printf("请选择录入成绩的方式：\n");
	printf("1、单个录入\n");
	printf("2、批量导入\n");
	switch(getch())
	{
		case '1':input_single_stu();break;
		case '2':input_batch_stu();break;
	}
}
 
//单个录入
void input_single_stu(void)
{
	int find_id;
	int flag=0;
	float input_Chinese;
	float input_Math;
	float input_English;
	int i;
	printf("请输入要录入成绩的学生学号:");
	scanf("%d",&find_id);
	stu_read();
	for(i=0;i<cnt_stu;i++)
	{
		if(stu[i].id==find_id)
		{
			flag=1;
			break;
		}
	}
	if(flag==0)
	{
		printf("未找到该学生\n");
		anykey();
	}
	else if(flag==1)
	{
		printf("请输入要录入的成绩（语文，数学，英语）\n");
		scanf("%f %f %f",&input_Chinese,&input_Math,&input_English);
		if(input_Chinese>=0 && input_Chinese<=150)//判断是否合法
		{
			stu[i].Chinese=input_Chinese;
		}
		else
		{
			printf("成绩不合法\n");
			anykey();
			return;
		}
		if(input_Math>=0 && input_Math<=150)//判断是否合法
		{
			stu[i].Math=input_Math;
		}
		else
		{
			printf("成绩不合法\n");
			anykey();
			return;
		}
		if(input_English>=0 && input_English<=150)//判断是否合法
		{
			stu[i].English=input_English;
		}
		else
		{
			printf("成绩不合法\n");
			anykey();
			return;
		}
		stu_save();
		printf("录入成功\n");
		anykey();
	}
	
}
 
//批量导入
void input_batch_stu(void)
{	
	typedef struct Inputstu
	{
		char name[20];
		int id;
		float Chinese;
		float Math;
		float English;
	}Inputstu;
	Inputstu* input_stu=NULL;
	char file_input[32]={};
	printf("请输入要导入的学生成绩文件:");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
  	import(file_input,32); 
	FILE* frp=fopen(file_input,"r");	
	if(NULL==frp)
	{
		printf("未找到该文件");
		anykey();
		return;
	}
	char r[sizeof(Inputstu)];
	int cnt_input_stu=0;
	while(NULL!=fgets(r,sizeof(Inputstu),frp))
	{
		input_stu = realloc(input_stu,(cnt_input_stu+1)*sizeof(Inputstu));
		sscanf(r,"%s %d %f %f %f\n",input_stu[cnt_input_stu].name,&input_stu[cnt_input_stu].id,&input_stu[cnt_input_stu].Chinese,&input_stu[cnt_input_stu].Math,&input_stu[cnt_input_stu].English);
		cnt_input_stu++;
	}
	fclose(frp);
	frp=NULL;
	stu_read();
	for(int i=0;i<cnt_input_stu;i++)
	{
		for(int j=0;j<cnt_stu;j++)
		{
			if(input_stu[i].id==stu[j].id)
			{
				if(input_stu[i].Chinese>=0 && input_stu[i].Chinese<=150)//判断是否合法
				{
					stu[j].Chinese=input_stu[i].Chinese;
				}
				else
				{
					printf("成绩不合法\n");
					anykey();
					return;
				}
				if(input_stu[i].Math>=0 && input_stu[i].Math<=150)//判断是否合法
				{
					stu[j].Math=input_stu[i].Math;
				}
				else
				{
					printf("成绩不合法\n");
					anykey();
					return;
				}
				if(input_stu[i].English>=0 && input_stu[i].English<=150)//判断是否合法
				{
					stu[j].English=input_stu[i].English;
				}
				else
				{
					printf("成绩不合法\n");
					anykey();
					return;
				}
			}
		}
	}
	stu_save();
	printf("导入成功！\n");
	anykey();
}
 
//重置学生密码
void reset_code_stu(void)
{
	int find_id;
	int flag=0;
	int i;
	printf("请输入要重置密码的学生学号:");
	scanf("%d",&find_id);
	stu_read();
	for(i=0;i<cnt_stu;i++)
	{
		if(stu[i].id==find_id)
		{
			flag=1;
			break;
		}
	}
	if(flag==0)
	{
		printf("未找到该学生\n");
		anykey();
	}
	else if(flag==1)
	{
		char code[]="123456";
		code_encryption(code,stu[i].id);
		strcpy(stu[i].code,code);
		stu[i].first_logon=0;
		stu_save();
		printf("重置成功\n");
	}
	anykey();
}

//解锁学生帐号
void unlock_stu(void) 
{
	int find_id;
	int flag=0;
	int i;
	printf("请输入要解锁帐号的学生学号:");
	scanf("%d",&find_id);
	stu_read();
	for(i=0;i<cnt_stu;i++)
	{
		if(stu[i].id==find_id)
		{
			flag=1;
			break;
		}
	}
	if(flag==0)
	{
		printf("未找到该学生\n");
		anykey();
	}
	else if(flag==1)
	{
		stu[i].lock=0;
		stu[i].code_cnt=0;
		stu_save();
		printf("解锁成功\n");
		anykey();
	}
}

//显示所有在校学生信息
void show_insch_stu(void) 
{
	stu_read();
	printf("姓名\t性别\t学号\t语文\t数学\t英语\n");
	//printf("%d",cnt_stu);
	for(int i=0;i<cnt_stu;i++)
	{
		printf("%s\t%s\t%d\t%g\t%g\t%g\n",stu[i].name,stu[i].sex,stu[i].id,
		stu[i].Chinese,stu[i].Math,stu[i].English);
	}
	anykey();
}
 
//显示所有退学学生信息
void show_quitsch_stu(void)
{
	quit_stu_read();
	printf("姓名\t性别\t学号\t语文\t数学\t英语\n");
	for(int i=0;i<cnt_quit_stu;i++)
	{
		printf("%s\t%s\t%d\t%g\t%g\t%g\n",quit_stu[i].name,quit_stu[i].sex,quit_stu[i].id,
		quit_stu[i].Chinese,quit_stu[i].Math,quit_stu[i].English);
	}
	anykey();
}

// 教师端主菜单
int menu_tch(void)
{
	system("clear");
	puts("—————————教师管理界面————————");
	puts("      1.添加学生");
	puts("      2.删除学生");
	puts("      3.查找学生");
	puts("      4.修改学生信息");
	puts("      5.录入学生成绩");
	puts("      6.重置学生密码");
	puts("      7.解锁学生帐号");
	puts("      8.显示所有在校学生信息");
	puts("      9.显示所有退学学生信息");
	puts("      0.退出系统");
	puts("—————————————————————————————");
	printf("请输入指令：");
	char cmd=getch();
	printf("%c\n",cmd);
	return cmd;
}

//读文件student.txt
void stu_read(void)
{	
	FILE* frp=fopen("student.txt","r");	
	if(NULL==frp)
	{
		perror("fopen");
		return;
	}
	char row[sizeof(Student)];
	cnt_stu=0;
	while(NULL!=fgets(row,sizeof(Student),frp))
	{
		stu = realloc(stu,(cnt_stu+1)*sizeof(Student));
		sscanf(row,"%s %s %d %s %f %f %f %d %d %d\n",
		stu[cnt_stu].name,stu[cnt_stu].sex,&stu[cnt_stu].id,
		stu[cnt_stu].code,&stu[cnt_stu].Chinese,&stu[cnt_stu].Math,
		&stu[cnt_stu].English,&stu[cnt_stu].first_logon,
		&stu[cnt_stu].code_cnt,&stu[cnt_stu].lock);
		cnt_stu++;
	}
	fclose(frp);
	frp=NULL;
}

//保存文件student.txt
void stu_save(void)
{
	FILE* fwp=fopen("student.txt","w");
	for(int i=0;i<cnt_stu;i++)
	{
		if(!strcmp(stu[i].sex,"男")||!strcmp(stu[i].sex,"女"))
		{
			fprintf(fwp,"%s %s %d %s %g %g %g %d %d %d\n",
			stu[i].name,stu[i].sex,stu[i].id,
			stu[i].code,stu[i].Chinese,stu[i].Math,
			stu[i].English,stu[i].first_logon,
			stu[i].code_cnt,stu[i].lock);
		}
	}
	fclose(fwp);
	fwp=NULL;
}

//读文件quit_stu.txt
void quit_stu_read(void)
{	
	FILE* frp=fopen("quit_stu.txt","r");	
	if(NULL==frp)
	{
		perror("fopen");
		return;
	}
	char row[sizeof(Quitstu)];
	cnt_quit_stu=0;
	while(NULL!=fgets(row,sizeof(Quitstu),frp))
	{
		quit_stu = realloc(quit_stu,(cnt_quit_stu+1)*sizeof(Quitstu));
		sscanf(row,"%s %s %d %f %f %f\n",
		quit_stu[cnt_quit_stu].name,quit_stu[cnt_quit_stu].sex,&quit_stu[cnt_quit_stu].id,
		&quit_stu[cnt_quit_stu].Chinese,&quit_stu[cnt_quit_stu].Math,&quit_stu[cnt_quit_stu].English);
		cnt_quit_stu++;
	}
	fclose(frp);
	frp=NULL;
}

//保存文件quit_stu.txt
void quit_stu_save(void)
{
	FILE* fwp=fopen("quit_stu.txt","w");
	for(int i=0;i<cnt_quit_stu;i++)
	{
		if(!strcmp(quit_stu[i].sex,"男")||!strcmp(quit_stu[i].sex,"女"))
		{
			fprintf(fwp,"%s %s %d %g %g %g\n",quit_stu[i].name,quit_stu[i].sex,quit_stu[i].id,
			quit_stu[i].Chinese,quit_stu[i].Math,quit_stu[i].English);
		}
	}
	fclose(fwp);
	fwp=NULL;
}
