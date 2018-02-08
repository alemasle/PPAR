#include <stdio.h>
#include <omp.h>

#define NUM_THREAD 64

int main(){
	int nb = NUM_THREAD;
	omp_set_num_threads(NUM_THREAD);

#pragma omp parallel
 {
 	nb = omp_get_num_threads();
 	int me = omp_get_thread_num();
	printf("Hello World! from thread %d!\n", me);
	
	sleep(1); // Sert a se rendre compte de l'interruption du thread par les autres et 
		  // Voir la parallelisation
	if(me == 0){
		printf("I am %d and we are %d threads.\n",me, nb);
	}
	
 }
	return 0;
}
