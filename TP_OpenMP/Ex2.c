#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREAD 4


int main(int argc, char ** argv){
	
	if(argv[1] == NULL){
		printf("Veuillez entrer une taille de matrice\n");
		exit(1);
	}

	int n = atoi(argv[1]);
	int matrice[n][n];
	int mat[n][n];
	int res[n][n];
	int nb = NUM_THREAD;
	omp_set_num_threads(NUM_THREAD);
	
	srand(time(NULL));


//#pragma omp parallel for schedule(dynamic)
	for(int i = 0; i < n; i ++){
		for(int j = 0; j < n; j++){
			matrice[i][j] =  rand() % 25;
			mat[i][j] = rand() % 25;
		}
	}	
	
/*	
	printf("Matrice A:\n");
	for(int i = 0; i < n; i ++){
		for(int j = 0; j < n; j++){
			printf(" %d ", matrice[i][j]);
		}
		printf("\n");
	}
		
	printf("\nMatrice B:\n");
	for(int i = 0; i < n; i ++){
		for(int j = 0; j < n; j++){
			printf(" %d ", mat[i][j]);
		}
		printf("\n");
	}
*/	
	
//#pragma omp parallel for schedule(dynamic)
	for(int i = 0; i < n; i ++){
		for(int j = 0; j < n; j++){
			for(int k = 0; k < n; k ++){
				res[i][j] += matrice[i][k] * mat[k][j];
			}
		}
	}
	
/*	
	printf("\nMatrice C = A*B:\n");
	for(int i = 0; i < n; i ++){
		for(int j = 0; j < n; j++){
			printf(" %d ", res[i][j]);
		}
		printf("\n");
	}
	*/
	return 0;
}
