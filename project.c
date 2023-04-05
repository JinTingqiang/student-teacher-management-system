#include "tool.h"
#include <stdio.h>
#include <stdlib.h>
#include "student.h"
#include "teacher.h"
#include "headmaster.h"
#include "project.h"
int num_tch_in;//在职教师人数
int num_tch_quit;//离职教师人数

void init(void)
{
	Mst=realloc(Mst,sizeof(Master));
	FILE* frp = fopen("headmaster.txt","r");
	if(NULL==frp)
	{
		perror("fopen");
		return;
	}
	fscanf(frp,"%s %s %s %d %d",Mst->name,Mst->sex,Mst->code,&Mst->num_tch_in,&Mst->num_tch_quit);
	num_tch_in=Mst->num_tch_in;
	num_tch_quit=Mst->num_tch_quit;
	fclose(frp);
	Tch=realloc(Tch,sizeof(Teacher)*(num_tch_in+1));
	Tch_quit=realloc(Tch_quit,sizeof(Teacher)*(num_tch_quit+1));

}

void start(void)
{	
	int i;
	for(;;)
	{
		show();
		stdin->_IO_read_ptr=stdin->_IO_read_end;
		scanf("%d",&i);
		switch(i)
		{
			case 1: student();break;
			case 2: teacher();break;
			case 3: master();break;
			case 4: return;
		}
		
	}
}

void end(void)
{
	free(stu);
	free(quit_stu);
	free(Tch);
	free(Mst);
	free(Tch_quit);
	stu=NULL;//在校学生指针
	quit_stu=NULL;//离校学生指针
	Tch=NULL;//定义在职教师指针
	Mst=NULL;//定义校长指针
	Tch_quit=NULL;//离职教师指针
}
