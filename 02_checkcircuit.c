#include <stdio.h>
#include <mpi.h>

// Da se reshi koloto od slajd 10 vo Aud_PPPVP_3.ppt

int main( int argc, char *argv[]){
	int i, count, global_count;
	int id;
	int p;
	double elapsed_time;

	int check_circuit(int, int);

	MPI_Init(&argc, &argv);
	MPI_Barrier(MPI_COMM_WORLD);

	elapsed_time = - MPI_Wtime(); // start_time = MPI_Wtime();

	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	count = 0; //kolku reshenija imame najdeno

	for(i = id; i < 65536; i += p)
		count += check_circuit(id, i);

	printf("Process %d is done\n", id);
	fflush(stdout);

	MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	elapsed_time += MPI_Wtime(); //end_time = MPI_Wtime();
	// elapsed_time = end_time - start_time;
	
	if (!id){
		printf("There are %d solutions\n", global_count);
		printf("Time: %.3f\n", elapsed_time);
	}

	MPI_Finalize();
	return 0;
}

#define EXTRACT_BIT(n, i) ((n&(1<<i))?1:0)

int check_circuit(int id, int z){
	int v[16];
	int i, res = 0;

	for (i = 0; i < 16; i++)
		v[i] = EXTRACT_BIT(z, i);
	
	if ((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3])
   	   	&& (!v[3] || !v[4]) && (v[4] || !v[5])
   	   	&& (v[5] || !v[6]) && (v[5] || v[6])
   	   	&& (v[6] || !v[15]) && (v[7] || !v[8])
   	   	&& (!v[7] || !v[13]) && (v[8] || v[9])
   	   	&& (v[8] || !v[9]) && (!v[9] || !v[10])
   	   	&& (v[9] || v[11]) && (v[10] || v[11])
   	   	&& (v[12] || v[13]) && (v[13] || !v[14])
   	   	&& (v[14] || v[15])) 
	{
		res++;
		printf ("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n", id,
			v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9],
			v[10],v[11],v[12],v[13],v[14],v[15]);
		fflush(stdout);

	}

	return res;
}
