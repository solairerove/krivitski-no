#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main (int argc, char *argv[])
{
double *ptrsource, *ptrdest;
FILE *pr,*prtmp;
double leftT,upT,rightT,downT,startT,errorT;
int taskID,destId,sourceId,numtasks,numblocks,rc,i,j,N,rowsinblock,
rowsinlastblock,itercount,maxitercount,
priznak;
char str[20] = "99999";
N=20;
leftT=2;
upT=3;
rightT=4;
downT=5;
startT=8;
errorT=1;
maxitercount=100;
itercount=0;

MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
MPI_Comm_rank(MPI_COMM_WORLD,&taskID);
MPI_Status status;
double startTime=MPI_Wtime();
numblocks = numtasks;
rowsinblock = (N-1)/numtasks;
rowsinlastblock= rowsinblock+(N-1)%numtasks;
if (taskID==0)
prtmp=fopen("tmp.txt","w");
else
prtmp=fopen("tmp.txt","a");

printf("Process(taskID)=%5d startTime=%8.2f\n",taskID,startTime);
if (taskID==0)
{
fprintf(prtmp,"number of tasks=%6d\n",numtasks);
fprintf(prtmp,"N=%6d\n",N);
fprintf(prtmp,"leftT=%6.1f\n",leftT);
fprintf(prtmp,"upT=%6.1f\n",upT);
fprintf(prtmp,"rightT=%6.1f\n",rightT);
fprintf(prtmp,"downT=%6.1f\n",downT);
fprintf(prtmp,"startT=%6.1f\n",startT);
fprintf(prtmp,"errorT=%6.1f\n\n",errorT);

fprintf(prtmp,"blocks number=%3d\n",numblocks);
fprintf(prtmp,"rows in block=%6d\n",rowsinblock);
fprintf(prtmp,"rows in last block=%6d\n",rowsinlastblock);
fprintf(prtmp,"maximum iterations=%8d\n",maxitercount);
fprintf(prtmp,"iteration count %10d\n\n",itercount);
}
fclose(prtmp);
double T[N+1][N+1], TT[N+1][N+1];
for (j=0;j<N+1;j++)
{
    T[0][j]=upT;
    T[N][j]=downT;
    T[j][0]=leftT;
    T[j][N]=rightT;
}
for (i=1;i<N;i++)
{
    for (j=1;j<N;j++)
    {
	T[i][j]=startT;
    }
}
itercount=0;
priznak=0;
while(itercount<maxitercount)
{
itercount+=1;
priznak=0;
if (taskID==numtasks-1)
{
for(i=taskID*rowsinblock+1;i<=taskID*rowsinblock+rowsinlastblock;i++)
{ for (j=1;j<N;j++)
{
TT[i][j]=0.25*(T[i-1][j]+T[i+1][j]+T[i][j-1]+T[i][j+1]);
if (abs(TT[i][j]-T[i][j])>errorT) priznak=1;
}
}
}
else
{
for (i=taskID*rowsinblock+1;i<=(taskID+1)*rowsinblock;i++)
{
for (j=1;j<N;j++)
{
TT[i][j]=0.25*(T[i-1][j]+T[i+1][j]+T[i][j-1]+T[i][j+1]);
if (abs(TT[i][j]-T[i][j])>errorT) priznak=1;
}
}
}
///////
}



//redimention
    if (taskID == numtasks-1)
    {
	for (i=taskID*rowsinblock+1;i<=taskID*rowsinblock+rowsinlastblock;i++)
	{
	    for (j=1;j<N;j++) { T[i][j]=TT[i][j];}

	}
    }
    else
    {
	for (i=taskID*rowsinblock+1;i<=(taskID+1)*rowsinblock;i++)
	{
	    for(j=1;j<N;j++) { T[i][j]=TT[i][j];}
	}
    }
double endTime = MPI_Wtime();
MPI_Finalize();
prtmp=fopen("tmp.txt","a");
fprintf(prtmp,"End Process =%d Time=%8.2f\n iteration count =%6d\
n",taskID,endTime-startTime,itercount);
if (taskID=numtasks-1)
{
    fprintf(prtmp,"blok %3d\n",taskID);
    for (i=taskID*rowsinblock+1;i<=taskID*rowsinblock+rowsinlastblock+1;i++)
    {
	for (j=0;j<=N;j++) { fprintf(prtmp,"%8.2f ",T[i][j]);}
	fprintf(prtmp,"\n");
    }
}
else if (taskID==0)
{
    fprintf(prtmp,"blok %3d\n",taskID);
    for (i=0;i<=rowsinblock;i++)
    {
	for (j=0;j<=N;j++) { fprintf(prtmp,"%8.2f ",T[i][j]);}
	fprintf(prtmp,"\n");
    }
}
else
{
    fprintf(prtmp,"blok %3d\n",taskID);
    for (i=taskID*rowsinblock+1;i<=(taskID+1)*rowsinblock;i++)
    {
	for (j=0;j<=N;j++) { fprintf(prtmp,"%8.2f ",T[i][j]);}
	fprintf(prtmp,"\n");
    }
}
fclose(prtmp);
return 0;
}
