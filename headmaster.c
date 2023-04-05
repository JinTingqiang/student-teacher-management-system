#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <getch.h>
#include "headmaster.h"
#include "tool.h"
extern int num_tch_in;//在职教师人数
extern int num_tch_quit;//离职教师人数
void master(void)
{
	if(log_on_mst()==1)
	{
		for(;;)
		{	
			int i;
			show_mst();
			scanf("%d",&i);
			switch(i)
			{
				case 1: reset_code_mst();break;
 				case 2: reset_code_tch();break;
 				case 3: add_tch();break;
 				case 4: del_tch();break;
 				case 5: show_insch_tch();break;
 				case 6: show_quitsch_tch();break;
 				case 7: unlock_tch();break;
 				case 8:	return;
			}
		}
	
	}
}
//修改密码
void change_code_new(char* code)
{
	char new_code[3][32];
	printf("请输入原来的密码：\n");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
  	strcpy(new_code[0],hide_code()); 
	code_encryption(new_code[0],1);//密码加密
	if(strcmp(new_code[0],code)==0)//输入密码与原密码是否相同
	{
		printf("请输入新密码\n");
		stdin->_IO_read_ptr=stdin->_IO_read_end;
  		strcpy(new_code[1],hide_code()); 
		if(0==(code_rtl(new_code[1])))//判断是否合法
		{
			printf("密码不合法！\n");
			anykey();
			return;
		}
		printf("请再次输入新密码\n");
		stdin->_IO_read_ptr=stdin->_IO_read_end;
  		strcpy(new_code[2],hide_code()); 
		if(strcmp(new_code[1],new_code[2])==0)//判断第二次修改的密码是否与第一次修改的相同
		{
			printf("密码修改成功\n");
			code_encryption(new_code[1],1);//密码加密
			strcpy(code,new_code[1]);
			anykey();
			return;
		}
		else
		{
			printf("修改失败\n");	
			anykey();	
			return;
		}
	}
	else if(strcmp(new_code[0],code)!=0)
	{
		printf("修改失败\n");	
		anykey();	
		return;
	}
}

//重置自己密码
void reset_code_mst(void)
{
	FILE* frp = fopen("headmaster.txt","r+");//先读取原来的密码
	if(NULL==frp)
	{
		perror("fopen");
		return;
	}
	fscanf(frp,"%s %s %s %d %d %d",Mst->name,Mst->sex,Mst->code,&Mst->num_tch_in,&Mst->num_tch_quit,&Mst->first_logon);
	fclose(frp);
	change_code_new(Mst->code);//修改密码
	FILE* fwp = fopen("headmaster.txt","w");
	if(NULL==fwp)
	{
		perror("fopen");
		return;
	}
	fprintf(frp,"%s %s %s %d %d %d",Mst->name,Mst->sex,Mst->code,Mst->num_tch_in,Mst->num_tch_quit,Mst->first_logon);
	fclose(fwp);
}

//重置教师密码 123
void reset_code_tch(void)
{
	int id;
	int m;//教师位置
	int flag=0;//记录是否识别到教师
	printf("输入重置教师的工号:\n");
	scanf("%d",&id);
	FILE* frp = fopen("insch_tch.txt","r+");//读取教师信息
	if(NULL==frp)
	{
		perror("fopen");
		return;
	}
	
	for(m=0;m<num_tch_in;m++)
	{
		fscanf(frp,"%s %s %d %s %d %d %d",Tch[m].name,Tch[m].sex,&Tch[m].id,Tch[m].code,&Tch[m].first_logon,&Tch[m].lock,&Tch[m].code_cnt);
		if(Tch[m].id==id)
		{
			char arr[]="123";//教师的初始密码
			code_encryption(arr,id);//密码加密
			strcpy(Tch[m].code,arr);
			printf("修改成功\n");
			flag=1;
			anykey();
		}	
	}
	if(flag==0)//未找到老师直接返回
	{
		printf("未找到该老师！\n");
		anykey();
		return;
	}	
	
	fclose(frp);
	FILE* fwp = fopen("insch_tch.txt","w");
	if(NULL==fwp)
	{
		perror("fopen");
		return;
	}
	for(m=0;m<num_tch_in;m++)//存储教师信息到文本
	{
		if(strcmp(Tch[m].sex,"0")!=0)
		{	
			
			fprintf(fwp,"%s %s %d %s %d %d %d\n",Tch[m].name,Tch[m].sex,Tch[m].id,Tch[m].code,Tch[m].first_logon,Tch[m].lock,Tch[m].code_cnt);
			
		}
	}
	fclose(fwp);

}

//添加教师
void add_tch(void)
{	
	Tch=realloc(Tch,sizeof(Teacher)*(num_tch_in+1));//重新申请内存
	FILE* fwp = fopen("insch_tch.txt","a");
	if(NULL==fwp)
	{
		perror("fopen");
		return;
	}	
	printf("输入教师的姓名、性别\n");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
  	import(Tch[num_tch_in].name,20);  
  	stdin->_IO_read_ptr=stdin->_IO_read_end;
	scanf("%s",Tch[num_tch_in].sex);//输入教师信息
	Tch[num_tch_in].id=100000+num_tch_in+num_tch_quit+1;
	if(!strcmp(Tch[num_tch_in].sex,"男")||!strcmp(Tch[num_tch_in].sex,"女"))//判断是否合法
	{}
	else
	{
		printf("性别不合法\n");
		anykey();
		return;
	}
	strcpy(Tch[num_tch_in].code,"123");
	Tch[num_tch_in].first_logon=0;
	Tch[num_tch_in].lock=0;
	Tch[num_tch_in].code_cnt=0;
	code_encryption(Tch[num_tch_in].code,Tch[num_tch_in].id);//对教师密码进行加密
	fprintf(fwp,"%s %s %d %s %d %d %d\n",Tch[num_tch_in].name,Tch[num_tch_in].sex,Tch[num_tch_in].id,Tch[num_tch_in].code,Tch[num_tch_in].first_logon,Tch[num_tch_in].lock,Tch[num_tch_in].code_cnt);
	fclose(fwp);
	num_tch_in++;
	FILE* fwp1 = fopen("headmaster.txt","w");//修改校长结构体中在职教师人数
	if(NULL==fwp1)
	{
		perror("fopen");
		return;
	}
	fscanf(fwp1,"%s %s %s %d %d %d",Mst->name,Mst->sex,Mst->code,&Mst->num_tch_in,&Mst->num_tch_quit,&Mst->first_logon);
	Mst->num_tch_in=num_tch_in;
	fprintf(fwp1,"%s %s %s %d %d %d",Mst->name,Mst->sex,Mst->code,Mst->num_tch_in,Mst->num_tch_quit,Mst->first_logon);
	fclose(fwp1);
	printf("添加成功\n");
	anykey();
	

}	
	

//删除教师
void del_tch(void)
{
	
	int id;
	int m;//教师位置
	int flag=0;
	printf("输入删除教师的工号:\n");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
	id=import_id();
	FILE* frp = fopen("insch_tch.txt","r+");
	if(NULL==frp)
	{
		perror("fopen");
		return;
	}
	for(m=0;m<num_tch_in;m++)
	{
		fscanf(frp,"%s %s %d %s %d %d %d",Tch[m].name,Tch[m].sex,&Tch[m].id,Tch[m].code,&Tch[m].first_logon,&Tch[m].lock,&Tch[m].code_cnt);
		if(Tch[m].id==id)//识别教师
		{	
			Tch_quit[num_tch_quit]=Tch[m];//搜索到的信息记录在离职教师结构体中
			strcpy(Tch[m].sex,"0");//并将在职教师信息中性别改为0,以便下列存储
			flag=1;
		
		}
	}
	if(flag==0)//未找到返回
	{
		printf("未找到该老师！\n");
		anykey();
		return;
	}	

	FILE* fwp = fopen("insch_tch.txt","w");//写教师文件
	if(NULL==fwp)
	{
		perror("fopen");
		return;
	}
	for(m=0;m<num_tch_in;m++)
	{
		if(strcmp(Tch[m].sex,"0")!=0)
		{	
			
			fprintf(fwp,"%s %s %d %s %d %d %d\n",Tch[m].name,Tch[m].sex,Tch[m].id,Tch[m].code,Tch[m].first_logon,Tch[m].lock,Tch[m].code_cnt);
			
		}
	}
	num_tch_in--;	
	fclose(fwp);
	FILE* fwp1 = fopen("quitsch_tch.txt","a");
	if(NULL==fwp1)
	{
		perror("fopen");
		return;
	}	
	fprintf(fwp1,"%s %s %d %s %d %d %d\n",Tch_quit[num_tch_quit].name,Tch_quit[num_tch_quit].sex,Tch_quit[num_tch_quit].id,Tch_quit[num_tch_quit].code,Tch_quit[num_tch_quit].first_logon,Tch_quit[num_tch_quit].lock,Tch_quit[num_tch_quit].code_cnt);
	num_tch_quit++;//增加离职教师人数
	fclose(fwp1);
	fclose(frp);
	FILE* fwp2 = fopen("headmaster.txt","w");//修改校长结构体内信息
	if(NULL==fwp2)
	{
		perror("fopen");
		return;
	}
	fscanf(fwp2,"%s %s %s %d %d %d",Mst->name,Mst->sex,Mst->code,&Mst->num_tch_in,&Mst->num_tch_quit,&Mst->first_logon);
	Mst->num_tch_in=num_tch_in;
	Mst->num_tch_quit=num_tch_quit;
	fprintf(fwp2,"%s %s %s %d %d %d",Mst->name,Mst->sex,Mst->code,Mst->num_tch_in,Mst->num_tch_quit,Mst->first_logon);
	fclose(fwp2);
	printf("删除成功！\n");
	anykey();
}

//查看在职教师
void show_insch_tch(void)
{
	int i;
	FILE* frp = fopen("insch_tch.txt","r");//遍历输出
	if(NULL==frp)
	{
		perror("fopen");
		return;
	}
	for(i=0;i<num_tch_in;i++)
	{
		fscanf(frp,"%s %s %d %s %d %d %d",Tch[i].name,Tch[i].sex,&Tch[i].id,Tch[i].code,&Tch[i].first_logon,&Tch[i].lock,&Tch[i].code_cnt);
		printf("%s %s %d\n",Tch[i].name,Tch[i].sex,Tch[i].id);
	}
	fclose(frp);
	anykey();
}

//查看离职教师
void show_quitsch_tch(void)
{
	int i;
	FILE* frp = fopen("quitsch_tch.txt","r");//遍历输出
	if(NULL==frp)
	{
		perror("fopen");
		return;
	}
	for(i=0;i<num_tch_quit;i++)
	{
		fscanf(frp,"%s %s %d %s %d %d %d",Tch[i].name,Tch[i].sex,&Tch[i].id,Tch[i].code,&Tch[i].first_logon,&Tch[i].lock,&Tch[i].code_cnt);
		printf("%s %s %d\n",Tch[i].name,Tch[i].sex,Tch[i].id);
	}
	fclose(frp);
	anykey();
	
}

//解锁教师帐号
void unlock_tch(void)
{
	int id;
	int m;//教师位置
	int flag=0;
	printf("输入要解锁教师的工号:\n");
	scanf("%d",&id);
	FILE* frp = fopen("insch_tch.txt","r+");
	if(NULL==frp)
	{
		perror("fopen");
		return;
	}
	for(m=0;m<num_tch_in;m++)
	{
		fscanf(frp,"%s %s %d %s %d %d %d",Tch[m].name,Tch[m].sex,&Tch[m].id,Tch[m].code,&Tch[m].first_logon,&Tch[m].lock,&Tch[m].code_cnt);
		if(Tch[m].id==id)//搜索帐号
		{	
			Tch[m].lock=0;//锁定状态
			Tch[m].code_cnt=0;//输入错误次数
			printf("解除成功\n");
			flag=1;
		}
	}
	if(flag==0)
	{
		printf("未找到该老师！\n");
		anykey();
		return;
	}	
	FILE* fwp = fopen("insch_tch.txt","w");
	if(NULL==fwp)
	{
		perror("fopen");
		return;
	}
	for(m=0;m<num_tch_in;m++)
	{
		
		fprintf(fwp,"%s %s %d %s %d %d %d\n",Tch[m].name,Tch[m].sex,Tch[m].id,Tch[m].code,Tch[m].first_logon,Tch[m].lock,Tch[m].code_cnt);
	}
	fclose(fwp);
	fclose(frp);
	anykey();
}

void show_mst(void)//显示界面
{
	system("clear");
	puts("——————校长管理界面——————");
	puts("1、   重置自己密码");
	puts("2、   重置教师密码");		
	puts("3、    添加教师");
	puts("4、    删除教师");	
	puts("5、   查看在职教师");
	puts("6、   查看离职教师");
	puts("7、   解锁教师帐号");
	puts("8、      退出");
	puts("————————————————————————");
}



//登录帐号
int log_on_mst(void)
{
	char name[20];
	char code[32];
	FILE* frp = fopen("headmaster.txt","r+");
	if(NULL==frp)
	{
		perror("fopen");
		return 0;
	}
	fscanf(frp,"%s %s %s",Mst->name,Mst->sex,Mst->code);
	fclose(frp);
	printf("请输入帐号:\n");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
  	import(name,20); 
  	printf("%s",name);
	if(strcmp(name,Mst->name)!=0)
	{
		printf("帐号不存在！\n");
		anykey();
		return 0;	
	}
	printf("请输入密码:\n");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
	strcpy(code,hide_code());
	code_encryption(code,1);//密码加密
	if(strcmp(code,Mst->code)!=0)//对比密码
	{
		printf("密码错误！\n");
		anykey();
		return 0;	
	}
	FILE* frp1 = fopen("headmaster.txt","r+");
	if(NULL==frp)
	{
		perror("fopen");
		return 0;
	}
	fscanf(frp1,"%s %s %s %d %d %d",Mst->name,Mst->sex,Mst->code,&Mst->num_tch_in,&Mst->num_tch_quit,&Mst->first_logon);
	if(Mst->first_logon ==0)//判断是否第一次登录
	{
		printf("第一次登录请修改密码\n");
		change_code_new(Mst->code);//该密码
		Mst->first_logon=1;
	}
	fclose(frp1);
	FILE* fwp = fopen("headmaster.txt","w");
	if(NULL==fwp)
	{
		perror("fopen");
		return 0;
	}
	fprintf(fwp,"%s %s %s %d %d %d",Mst->name,Mst->sex,Mst->code,Mst->num_tch_in,Mst->num_tch_quit,Mst->first_logon);
	fclose(fwp);
	return 1;
}
