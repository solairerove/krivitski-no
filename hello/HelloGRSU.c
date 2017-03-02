#include <stdio.h>
#include <mpi.h>
int main (int argc,char *argv[])
{
  int rank,size;
  MPI_Init(&argc,&argv); /* starts MPI */
  MPI_Comm_rank(MPI_COMM_WORLD,&rank); /* get current process id */
  if (rank==0){
  MPI_Comm_size(MPI_COMM_WORLD,&size); /* get number of processes */
  printf("Hello GRSU from process %d of %d\n",rank,size);}
  MPI_Finalize();
  return 0;
}