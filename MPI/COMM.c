#include<mpi.h>
#include<stdio.h>

			
int main(int argc, char** argv)
{
	int offices,n;
	offices= 5;
	n=15;
	//Assuming 5 offices and 15 items with each office
	int arr[offices];
	int procs, myid;
	int data = 0;
	int *buff = NULL;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	
		
	
	data =n;
	//Collecting items count from offices to the head office
	MPI_Gather(&data,1,MPI_INT,arr,1,MPI_INT,0,MPI_COMM_WORLD);
	
	if(myid==0){
		//Calculating the cost to pay for advance taxes			
		for(int i = 0;i<offices;i++)
		{
			arr[i]*=i;
		}	
		buff = arr;		
	}	
	
	printf("\nData before paying advance tax in process %d is %d",myid,data);
	
	//Sending taxes for each office	
	MPI_Scatter(buff,1,MPI_INT,&data,1,MPI_INT,0,MPI_COMM_WORLD);
	if(myid!=0){
	printf("\nCost for paying advance tax in process %d is %d",myid,data);
	}
MPI_Finalize();
printf("\n");
return 0;		

}
