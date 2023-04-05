#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getch.h>
#include "tool.h"
#include "student.h"
static int m;//定位学生位置
int cnt=0;//排名
char code2[32]={}; // 新密码
int line_number = 0;
char line[100];
FILE* temp_fp=NULL;
FILE* frp=NULL;
int cnt_stu1;
void student(void)
{
	
	int i;
	if(log_in()==1)//帐号正确进入
	{
			for(;;)
			{	
				menu_stu();
				scanf("%d",&i);
				switch(i)
				{
					case 1:
						seek();break;
					case 2:
						change();break;
					case 3:
						seek_inform();break;
					case 4:
						return ;
				}
			}
	}
}
void seek(void)//查找函数，查找自己分数、排名、各科平均分、各科最高分、各科最低分
{
	float avg1;//语文平均分
	float avg2;//数学平均分
	float avg3;//英语平均分
	float sum1=0;//语文总分
	float sum2=0;//数学总分
	float sum3=0;//英语总分
	float max1=stu[0].Chinese;//语文最高分
	float min1=stu[0].Chinese;//语文最低分
	stu_read1();
	float sum[cnt_stu1];//语数英总分
		for(int i=0;i<cnt_stu1;i++)
		{
			if(stu[i].Chinese>=max1)//语文最高分
			{
				max1=stu[i].Chinese;
			}
		
			 if(min1>=stu[i].Chinese)//语文最低分
			{
				min1=stu[i].Chinese;
			}
			sum1 += stu[i].Chinese;//语文总分
			sum2 += stu[i].Math;//数学总分
			sum3 += stu[i].English;//英语总分
		}
					
			
	float max2=stu[0].Math;//数学最高分		
	float min2=stu[0].Math;//数学最低分
		for(int i=0;i<cnt_stu1;i++)
		{
			if(stu[i].Math>=max2)//数学最高分
			{
				max2=stu[i].Math;
			}
		
			 if(min2>=stu[i].Math)//数学最低分
			{
				min2=stu[i].Math;
			}		
		}
		
		
	float max3=stu[0].English;//英语最高分		
	float min3=stu[0].English;//英语最低分
		for(int i=0;i<cnt_stu1;i++)
		{
			if(stu[i].English>=max3)//英语最高分
			{
				max3=stu[i].English;
			}
		
			 if(min3>=stu[i].English)//英语最低分
			{
				min3=stu[i].English;
			}
			
		}
		
		
	for(int i=0;i<cnt_stu1;i++)//计算个人总分
	{
		sum[i]=stu[i].Chinese+stu[i].Math+stu[i].English;
	}
	
	
	for(int i=0;i<cnt_stu1;i++)//排名（总分排序）
	{
		if(sum[m]<sum[i])
		{
			cnt++;
		}
	}
		
		avg1=sum1/cnt_stu1;//语文平均分
		avg2=sum2/cnt_stu1;//数学平均分
		avg3=sum3/cnt_stu1;//英语平均分
		
		printf("排名：%d\n",cnt+1);//输出排名
		printf("语文：%g   数学%g   英语%g\n",stu[m].Chinese,stu[m].Math,stu[m].English);//输出自己分数
		printf("语文平均分：%g   最高分：%g   最低分：%g\n",avg1,max1,min1);//输出语文平均、最低、最高分
		printf("数学平均分：%g   最高分：%g   最低分：%g\n",avg2,max2,min2);//输出数学平均、最低、最高分
		printf("英语平均分：%g   最高分：%g   最低分：%g\n",avg3,max3,min3);//输出英语平均、最低、最高分
		anykey();
		return;
}



void seek_inform(void)//查找个人信息：姓名、性别、学号
{	
	stu_read1();
	/*FILE* frp=fopen("student.txt","r");//打开文件，遍历读取，不用说了
    if(NULL==frp)
    {
        perror("fopen");
        return ;
    }
	fscanf(frp,"%s %s %d %s %f %f %f %d %d %d\n",stu[m].name,stu[m].sex,&stu[m].id,stu[m].code,&stu[m].Chinese,&stu[m].Math,&stu[m].English,&stu[m].first_logon,&stu[m].code_cnt,&stu[m].lock);*/
	printf("姓名：%s\n",stu[m].name);//输出自己姓名
	printf("学号：%d\n",stu[m].id);//输出自己学号
	printf("性别：%s\n",stu[m].sex);//输出自己性别
	printf("\n");
	anykey();
}




void change(void)//重头戏，修改密码
{
    char code1[32]={}; // 旧密码
    char code2[32]={}; // 新密码
    char code3[32]={}; // 二次确认密码
	stu_read1();
    /*frp=fopen("student.txt","r+");//r+打开文件，读取m的信息
    if(NULL==frp)
    {
        perror("fopen");
        return ;
    }
		fscanf(frp,"%s %s %d %s %f %f %f %d %d %d\n",stu[m].name,stu[m].sex,&stu[m].id,stu[m].code,&stu[m].Chinese,&stu[m].Math,&stu[m].English,&stu[m].first_logon,&stu[m].code_cnt,&stu[m].lock);*/
	
    printf("请输入旧的密码\n");
    stdin->_IO_read_ptr=stdin->_IO_read_end;
  	strcpy(code1,hide_code());
    if(0==(code_rtl(code1)))//判断是否合法
	{
		printf("密码不合法！\n");
		anykey();
		return;
	}	
    code_encryption(code1,stu[m].id);
   // printf("%s   %s   %d",code1,stu[m].code,m);
    while(strcmp(code1,stu[m].code)!=0)//输入的密码和旧的密码比较，输入错误循环输入密码，输入正确跳出
    {
        printf("输入密码错误\n");
        anykey();
        return;
    }
    
    printf("请输入新的密码\n");
    stdin->_IO_read_ptr=stdin->_IO_read_end;
  	strcpy(code2,hide_code());
    if(0==(code_rtl(code2)))
	{
		printf("密码不合法！\n");
		anykey();
		return;
	}
	//code_encryption(code2,stu[m].id);
    while(strcmp(code2,code1)==0)//输入的密码和旧的密码比较，相同则循环输入密码，不同跳出
    {
        printf("新密码不能与旧密码相同\n");
		anykey();
		return;
    }

    printf("请再次确认新密码\n");
    stdin->_IO_read_ptr=stdin->_IO_read_end;
  	strcpy(code3,hide_code()); 
    while(strcmp(code3,code2)!=0)//输入的密码和前一次输入的密码比较，输入错误循环输入密码，输入正确跳出
    {
        printf("两次密码输入不一致\n");
        anykey();
		return;
    }
    FILE* frp=fopen("student.txt","r+");//r+打开文件
    if(NULL==frp)
    {
        perror("fopen");
        return ;
    }
    	int line_number = 0;
    			char line[100];
    			
				fseek(frp, 0, SEEK_SET);
				char id[32];
				sprintf(id,"%d",stu[m].id);
				while (fgets(line, sizeof(line), frp) != NULL)
				{
					if (strstr(line,id)!=NULL )
					{
						break;
					}
					line_number++;
				}
		
				fseek(frp, 0, SEEK_SET);

				FILE* temp_fp = fopen("temp.txt", "w+");
				if(NULL == temp_fp)
				{
					perror("fopen");
					return ;
				}
				
		
			int current_line = 0;
			while (fgets(line, sizeof(line), frp) != NULL)
			{
				if (current_line != line_number)
				{
				    fputs(line, temp_fp);
				}
				else
				{
				 	code_encryption(code2,stu[m].id);
				    strcpy(stu[m].code, code2);
				    fprintf(temp_fp, "%s %s %d %s %g %g %g %d %d %d\n", stu[m].name, stu[m].sex, stu[m].id, stu[m].code, stu[m].Chinese, stu[m].Math, stu[m].English, stu[m].first_logon,stu[m].code_cnt, stu[m].lock);
				   
				}
				current_line++;
			}
		
			
			while (fgets(line, sizeof(line), frp) != NULL)
			{
				fputs(line, temp_fp);
			}

			fclose(frp);
			fclose(temp_fp);
			remove("student.txt");
			rename("temp.txt", "student.txt");

			printf("密码修改成功\n");
			anykey();
}



int log_in(void)//重头戏，登录等相关操作
{
	stu_read1();
	FILE* frp=fopen("student.txt","r+");//r+打开文件
    if(NULL==frp)
    {
        perror("fopen");
        return 0;
    }
   
    for(int i=0;i<cnt_stu1;i++)
    {
		fscanf(frp,"%s %s %d %s %f %f %f %d %d %d\n",stu[i].name,stu[i].sex,&stu[i].id,stu[i].code,&stu[i].Chinese,&stu[i].Math,&stu[i].English,&stu[i].first_logon,&stu[i].lock,&stu[i].code_cnt);
	}
	
	int account;//学生帐号
	char code[32]={};//学生密码
	//int m;//定位学生位置
	int flag=0;//判断帐号是否正确

	printf("请输入帐号：\n");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
	account=import_id();//输入帐号
	for(int i=0;i<cnt_stu1;i++)
	{
		if(account==stu[i].id)//帐号和学生文件的id循环比较
		{
			m=i;//定位我自己
			flag=1;//有自己的帐号则flag赋值为1
			break;
		}
	}
	
	if(flag==0)//如果flag=0，说明学生文件里没找到这个帐号
	{
		printf("帐号错误\n");//输出错误
		anykey();
		return 0;
	}
	
	if(stu[m].lock!=0)//stu[m].lock如果不为0，说明锁了
	{
		printf("帐号已锁定，请联系教师修改\n");
		anykey();
		return 0;
	}
	
	stu[m].code_cnt=0;//记录输错密码次数，这个我没有放进文件里，因为第二次打开它依旧置0
	
	if(flag==1)//如果flag为1,说明找到我自己了
	{
		if(stu[m].lock==0);//但我首先要判断我的帐号有没有上锁，为0说明没锁
		{
			do{
				if(0==stu[m].code_cnt)
				{
					printf("请输入密码:\n");
				}
				else
				{
					printf("密码错误，还剩%d次机会\n",3-stu[m].code_cnt);
				}
				stdin->_IO_read_ptr=stdin->_IO_read_end;
				strcpy(code,hide_code());
				code_encryption(code,stu[m].id);
				if(0==(code_rtl(code)))//判断是否合法
				{
					printf("密码不合法！\n");
					anykey();
					return 0;
				}	
			}while(strcmp(stu[m].code,code)!=0&&stu[m].code_cnt++<2);//如果密码输错，do while循环输入密码最多3次
		
			if(strcmp(stu[m].code,code)==0)//输入密码如果对了，那么跳出do while
			{
				printf("登录成功\n");//打印登录成功
				if(stu[m].first_logon!=0)//登录成功首先判断是否第一次登录，不等于0说明不是第一次
				{
					return 1;//直接return1，定的返回值是int
				}
				if(stu[m].first_logon==0)//等于0说明是第一次，那么就要修改密码
				{
					printf("第一次登录，请修改密码：\n");
					//char code3[32] = {};	
					scanf("%s",code2);//输入修改的密码
					if(0==(code_rtl(code2)))//判断是否合法
					{
						printf("密码不合法！\n");
						anykey();
						return 0;
					}	
					code_encryption(code2,stu[m].id);
					while(strcmp(stu[m].code,code2)==0)
					{
						printf("新密码不能和旧密码相同\n");
						printf("请重新输入密码\n");
						scanf("%s",code2);
						code_encryption(code2,stu[m].id);
					}
					stu[m].first_logon++;//同时first_logon+1,并写入文件，说明我第一次没了				
				int line_number = 0;
				char line[100];
				fseek(frp, 0, SEEK_SET);
				char id[32];
				sprintf(id,"%d",stu[m].id);
				while (fgets(line, sizeof(line), frp) != NULL)
				{
					//printf("%s",line);
					if (strstr(line,id)!=NULL)
					{
						break;
					}
					line_number++;
				}
				//printf("%d",line_number);
				fseek(frp, 0, SEEK_SET);

				FILE* temp_fp = fopen("temp.txt", "w+");
				if(NULL == temp_fp)
				{
					perror("fopen");
					return 0;
				}
			
	
				int current_line = 0;
				while (fgets(line, sizeof(line), frp) != NULL)
				{
					if (current_line != line_number)
					{
						fputs(line, temp_fp);
					}
					else
					{
						strcpy(stu[m].code,code2);
						fprintf(temp_fp, "%s %s %d %s %g %g %g %d %d %d\n", stu[m].name, stu[m].sex, stu[m].id, stu[m].code, stu[m].Chinese, stu[m].Math, stu[m].English, stu[m].first_logon,stu[m].code_cnt, stu[m].lock);
					}
					current_line++;
				}
				printf("密码修改成功\n");
				anykey();
	
				fflush(temp_fp);
				fclose(frp);
				fclose(temp_fp);
		
				remove("student.txt");
				rename("temp.txt", "student.txt");
				
				
					return 1;
				}
			}
		
			else//请注意这个else，他是和299行的if相对应，如果密码三次都输错了，那么就直接锁定帐号
			{
				printf("帐号已锁定，请联系教师解锁\n");
				anykey();
				// 锁定帐号
				stu[m].lock++;

				// 更新文件内容
				fseek(frp, 0, SEEK_SET);

				FILE* temp_fp = fopen("temp.txt", "w+");
				if (NULL == temp_fp)
				{
					perror("fopen");
				}

				char line[100];
				int line_number = 0;
				int current_line = 0;

				// 寻找要修改的行
				char id[32];
				sprintf(id,"%d",stu[m].id);
				while (fgets(line, sizeof(line), frp) != NULL)
				{
					if (strstr(line,id)!=NULL)
					{
						break;
					}
					line_number++;
				}

				// 重置文件指针到开始位置
				fseek(frp, 0, SEEK_SET);

				// 将修改后的内容写入临时文件
				while (fgets(line, sizeof(line), frp) != NULL)
				{
					if (current_line != line_number)
					{
						fputs(line, temp_fp);
					}
					else
					{
						fprintf(temp_fp, "%s %s %d %s %g %g %g %d %d %d\n", stu[m].name, stu[m].sex, stu[m].id, stu[m].code, stu[m].Chinese, stu[m].Math, stu[m].English, stu[m].first_logon, stu[m].lock,stu[m].code_cnt);
					}
					current_line++;
				}

				// 关闭文件指针并删除原始文件
				fclose(frp);
				fclose(temp_fp);
				remove("student.txt");

				// 将临时文件重命名为原始文件
				rename("temp.txt", "student.txt");
			}
		}

	
	}
	return 3;
}

void menu_stu(void)
{
	system("clear");
	puts("———————学生管理界面——————");
	puts("	1、查询成绩");
	puts("	2、修改密码");
	puts("	3、查看个人信息");
	puts("	4、退出");
	puts("	请输入指令");
	puts("————————————————————————");
}
void stu_read1(void)
{	
	FILE* frp=fopen("student.txt","r");	
	if(NULL==frp)
	{
		perror("fopen");
		return ;
	}
	char row[sizeof(Student)];
	cnt_stu1=0;
	while(NULL!=fgets(row,sizeof(Student),frp))
	{
		stu = realloc(stu,(cnt_stu1+1)*sizeof(Student));
		sscanf(row,"%s %s %d %s %f %f %f %d %d %d\n",
		stu[cnt_stu1].name,stu[cnt_stu1].sex,&stu[cnt_stu1].id,
		stu[cnt_stu1].code,&stu[cnt_stu1].Chinese,&stu[cnt_stu1].Math,
		&stu[cnt_stu1].English,&stu[cnt_stu1].first_logon,
		&stu[cnt_stu1].lock,&stu[cnt_stu1].code_cnt);
		cnt_stu1++;
	}
	fclose(frp);
	frp=NULL;

}
