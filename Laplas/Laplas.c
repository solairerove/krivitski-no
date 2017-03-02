#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <iostreem>
//#include <fstreem>

int main (int argc, char *argv[])
{
double *ptrsource,*ptrdest;
FILE *pr,*prtmp;
double	leftT,         /* left border */
	upT,	        /* up border */
	rightT,	        /* right border */
	downT,		 /* right border */
	startT,		 /* right border */
	errorT;       
	
int	taskID,	        /* task ID - also used as seed number */
	destID,
	sourceID,
	numtasks,       /* number of tasks */
	numblocks,
	rc,             /* return code */
	i,j,		 /* indexes */
	N,		/*line/columnA*/	
	rowsinblock,		 /* rows in block*/
	rowsinlastblock,/* rows in last block */
	itercount,
	maxitercount,
	priznak;
char	str[20]="99999";


N=20;
leftT=2;
upT=3;
rightT=4;
downT=5;
startT=8;
errorT=1;
maxitercount=100;
itercount=0;


/* Obtain number of tasks and task ID */
MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
MPI_Comm_rank(MPI_COMM_WORLD,&taskID);
MPI_Status status,status1,status2;
MPI_Request request1,request2;
double startTime=MPI_Wtime();

    numblocks=numtasks;
    rowsinblock=(N-1)/numtasks;
    rowsinlastblock=rowsinblock+(N-1)%numtasks;

//print initial data
if (taskID==0)
prtmp=fopen("tmp.txt","w");
else
prtmp=fopen("tmp.txt","a");
//file open
//output into file

printf("Process(taskID)=%5d startTime=%8.2f\n",taskID,startTime);
if(taskID==0)
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
    fprintf(prtmp,"iteration count=%10d\n\n",itercount);
}

fclose(prtmp);

//border and initial conditions
double T[N+1][N+1], TT[N+1][N+1];

for (j=0;j<N+1;j++)
{
    T[0][j]=upT;
    T[N][j]=downT;
    T[j][0]=leftT;
    T[j][N]=rightT;
}
for(i=1;i<N;i++)
{
    for (j=1;j<N;j++) {T[i][j]=startT;}
}
/*enOCd initial conditions*/

itercount=0;
priznak=0;
MPI_Barrier(MPI_COMM_WORLD);
//iteration_______________________________________________

while(itercount<maxitercount)
{
    itercount+=1;
    priznak=0;
    if(taskID==numtasks-1)
    {	for(i=taskID*rowsinblock+1;i<=taskID*rowsinblock+rowsinlastblock;i++)
	{    for(j=1;j<N;j++) 
	    {	TT[i][j]=0.25*(T[i-1][j]+T[i+1][j]+T[i][j-1]+T[i][j+1]);
		if(abs(TT[i][j]-T[i][j])>errorT)priznak=1;
	    }	
	}
    }
    else
    {	for(i=taskID*rowsinblock+1;i<=(taskID+1)*rowsinblock;i++)
	{   for(j=1;j<N;j++)
	    {	TT[i][j]=0.25*(T[i-1][j]+T[i+1][j]+T[i][j-1]+T[i][j+1]);
		if(abs(TT[i][j]-T[i][j])>errorT)priznak=1;
 	    }
        }
    }
    
MPI_Barrier(MPI_COMM_WORLD);

//receive_____________________________________________
    
    
    if(taskID<numtasks-2 && taskID>0)
    //inner blocks send
        {
	sourceID=taskID+1;
	ptrdest=&TT[(taskID+1)*rowsinblock+1][1];

    MPI_IRECV(ptrdest,N-1,MPI_DOUBLE,sourceID,1,MPI_COMM_WORLD,&request1);
	    MPI_Wait(&request1,&status1);    
	
	sourceID=taskID-1;
	ptrdest=&TT[taskID*rowsinblock][1];
    MPI_IRECV(ptrdest,N-1,MPI_DOUBLE,sourceID,2,MPI_COMM_WORLD,&request2);
	    MPI_Wait(&request2,&status2);    


    }
    
    else if(taskID=0)
    {
	sourceID=taskID+1;
	ptrdest=&TT[(taskID+1)*rowsinblock+1][1];

    MPI_IRECV(ptrdest,N-1,MPI_DOUBLE,sourceID,1,MPI_COMM_WORLD,&request1);
	    MPI_Wait(&request1,&status1);    

    }
    else if(taskID==numtasks-1)
    {
	sourceID=taskID-1;
	ptrdest=&TT[taskID*rowsinblock][1];

    MPI_IRECV(ptrdest,N-1,MPI_DOUBLE,sourceID,2,MPI_COMM_WORLD,&request2);
	    MPI_Wait(&request2,&status2);    
    }


    //send______________________________________________________________

    if(taskID<numtasks-1 && taskID>0)
    //inner blocks send
    {	 //pointer to array element
    
	destID=taskID+1;
	ptrsource=&TT[taskID*rowsinblock+1][1];//source
		    printf("iteration %5d task  %5d\n",itercount,taskID);
	MPI_ISEND(ptrsource,N-1,MPI_DOUBLE,destID,1,MPI_COMM_WORLD,&request1);

	    printf("iteration %5d task  %5d\n",itercount,taskID);
	    MPI_Wait(&request1,&status1);

	destID=taskID-1;
	ptrsource=&TT[(taskID+1)*rowsinblock][1];//source

	MPI_ISEND(ptrsource,N-1,MPI_DOUBLE,destID,2,MPI_COMM_WORLD,&request2);

	    printf("iteration %5d task  %5d\n",itercount,taskID);
	    MPI_Wait(&request2,&status2);
	    
    }
    else if(taskID==0)
    {
	destID=taskID+1;
	ptrsource=&TT[1][1];//source
	    printf("iteration %5d task  %5d\n",itercount,taskID);
	MPI_ISEND(ptrsource,N-1,MPI_DOUBLE,destID,2,MPI_COMM_WORLD,&request1);
	    printf("iteration %5d task  %5d\n",itercount,taskID);
	    MPI_Wait(&request1,&status1);
    }
    else if(taskID==numtasks-1)
    {
	destID=taskID-1;
	ptrsource=&TT[taskID*rowsinblock+1][1];//source
	    printf("iteration %5d task  %5d\n",itercount,taskID);
MPI_ISEND(ptrsource,N-1,MPI_DOUBLE,destID,1,MPI_COMM_WORLD,&request1);
	    printf("iteration %5d task  %5d\n",itercount,taskID);
	    MPI_Wait(&request1,&status1);
    }

   
MPI_Barrier(MPI_COMM_WORLD);
  
//redimention__________________________________________
    if(taskID==numtasks-1)
    {
	for(i=taskID*rowsinblock+1;i<=taskID*rowsinblock+rowsinlastblock;i++)
	{
	    for(j=1;j<N;j++){ T[i][j]=TT[i][j];}
	}
    }
    else
    {
	for(i=taskID*rowsinblock+1;i<=(taskID+1)*rowsinblock;i++)
	{
	    for(j=1;j<N;j++){ T[i][j]=TT[i][j];}
	}
    }
MPI_Barrier(MPI_COMM_WORLD);
}//while

double endTime=MPI_Wtime();

MPI_Finalize();
//    if(taskID==0)
//    {
prtmp=fopen("tmp.txt","a");

fprintf(prtmp,"End Process=%5d Time=%8.2f\n iteration count=%6d\n",taskID,endTime-startTime,itercount);
    if(taskID==numtasks-1)
    {
	fprintf(prtmp,"blok %3d\n",taskID);
	for(i=taskID*rowsinblock+1;i<=taskID*rowsinblock+rowsinlastblock+1;i++)
	{
	    for(j=0;j<=N;j++){fprintf(prtmp,"%8.2f ", T[i][j]);}
	    	fprintf(prtmp,"\n");
	}
	
    }
    else if(taskID==0)
    {
	fprintf(prtmp,"blok %3d\n",taskID);
	for(i=0;i<=rowsinblock;i++)
	{
	    for(j=0;j<=N;j++){fprintf(prtmp,"%8.2f ", T[i][j]);}
	    	fprintf(prtmp,"\n");
	}

    }
    
    else
    {
	fprintf(prtmp,"blok %3d\n",taskID);
	for(i=taskID*rowsinblock+1;i<=(taskID+1)*rowsinblock;i++)
	{
	    for(j=0;j<=N;j++){fprintf(prtmp,"%8.2f ", T[i][j]);}
	    	fprintf(prtmp,"\n");
	}
	
    }



fclose(prtmp);



//}

return 0;
}

