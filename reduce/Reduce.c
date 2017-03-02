#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
//#include <iostreem>
//#include <fstreem>

int main (int argc, char *argv[])
{
int **T,**TT;
int *countElement;
FILE *pr,*prtmp;

int	taskID,	        /* task ID - also used as seed number */
	destID,
	sourceID,
	numtasks,       /* number of tasks */
	i,j,		 /* indexes */
	N;		/*line/columnA*/	
	
N=5;

/* ����� ��������� � �����-���� �������� */
MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
MPI_Comm_rank(MPI_COMM_WORLD,&taskID);

//������ � ���� �������� ������
if (taskID==0)
{
    prtmp=fopen("tmp1.txt","w");
    fprintf(prtmp,"rank of tasks=%6d\n",taskID);
    fprintf(prtmp,"N=%6d\n",N);
    fclose(prtmp);
}

if (taskID==1)
{
    prtmp=fopen("tmp2.txt","w");
    fprintf(prtmp,"rank of tasks=%6d\n",taskID);
    fprintf(prtmp,"N=%6d\n",N);
    fclose(prtmp);
}

//������� 2 2-� ������ �������
//countElement=(int*)malloc(sizeof(int)*numtasks);
T=(int **)malloc(sizeof(int*)*(N+1));
TT=(int **)malloc(sizeof(int*)*(N+1));
for(i=0;i<=N;i++)
{
T[i]=(int*)malloc(sizeof(int)*(N+1));
TT[i]=(int*)malloc(sizeof(int)*(N+1));
}

//�������������� ������� �������� 0//
if (taskID==0)
for(i=0;i<=N;i++)
{
for(j=0;j<=N;j++) { T[i][j]=j;TT[i][j]=0;}
}

//�������������� ������ � �������� 1
if (taskID==1)
for(i=0;i<=N;i++)
{
for(j=0;j<=N;j++) { T[i][j]=j*taskID;}
}

//����� ���������, ����������� � �������� ��������
int Element1;

Element1=4;

MPI_Barrier(MPI_COMM_WORLD);

//������ �������� � ���� �������� ������
if(taskID==0)
{
    prtmp=fopen("tmp1.txt","a");   
    fprintf(prtmp,"\n taskID=%2d before MPI_Reduce \n",taskID);
    for(i=0;i<=N;i++)
	{
	    for(j=0;j<=N;j++){fprintf(prtmp,"%4d ", T[i][j]);}
    	    fprintf(prtmp,"\n");
    	}
    fclose(prtmp);
    
}

if(taskID==1)
{
    prtmp=fopen("tmp2.txt","a");   
    fprintf(prtmp,"\n taskID=%2d before MPI_Reduce \n",taskID);
    for(i=0;i<=N;i++)
	{
	    for(j=0;j<=N;j++){fprintf(prtmp,"%4d ", T[i][j]);}
    	    fprintf(prtmp,"\n");
    	}
    fclose(prtmp);
}
    
MPI_Reduce((int*)&T[4][0],(int*)&TT[0][0],6,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

MPI_Barrier(MPI_COMM_WORLD);

//����� ���������� ��������

if(taskID==0)
    {

    prtmp=fopen("tmp1.txt","a");
    fprintf(prtmp,"\n taskID=%2d after MPI_Reduce \n",taskID);
    for(i=0;i<=N;i++)
	{
	    for(j=0;j<=N;j++){fprintf(prtmp,"%4d ", TT[i][j]);}
	    fprintf(prtmp,"\n");
	}
//	fprintf(prtmp,"\n\n ����� ������� - ������ =%2d ��������� =%10d  \n",startTime,endTime);
    fclose(prtmp);
}

//������������ �����
for(i=0;i<=N;i++)
{ free(T[i]);free(TT[i]);}
free(T);free(TT);
MPI_Finalize();
return 0;
}

