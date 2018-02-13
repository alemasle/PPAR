#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define NUM_THREAD 4


void sieve(bool * era, long long n){

	long long i,j;
	long long a = sqrt(n);
	era[0] = false;
	era[1] = false;
#pragma omp parallel for schedule(dynamic) private(i,j)
	for(i = 2; i < a; i ++){
		for(j = 2; j* i < n; j ++){
			era[j * i] = false;
		}
	}
}


int main(int argc, char ** argv){

	omp_set_num_threads(NUM_THREAD);
	long long n = atol(argv[1]);
	long long sum = 0;
	bool era[n];
	
	for(long long i = 0; i < n; i++){
		era[i] = true;
	}
	
	sieve(era, n);
	printf("Sum of Eratosthene: \n");
	
	// affichage
	for(long long i = 2; i < n; i ++){
		if(era[i] == true){
			printf("%d ",i);
			sum += i;
		}
	}
	printf("\n%ld\n", sum);
		
	return 0;
}
