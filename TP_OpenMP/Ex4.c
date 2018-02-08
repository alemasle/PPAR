#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define NUM_THREAD 4

void recurrence(char * str, char * ascii, int * occ){
  for(int i = 0; str[i] != '\0'; i++){
    int tmp = (int) str[i];
    ascii[tmp] = str[i];
    occ[tmp] += 1;
  }
}

int main(int argc, char ** argv){

  omp_set_num_threads(NUM_THREAD);

  char ascii[128];
  int occ[128];
  int sum = 0, v = 0, c = 0;
  char maps[1024];
  char stmp[1024];

  memset(ascii,'a', sizeof(ascii));
  memset(occ, 0, sizeof(occ));
  

  // parallelisation pour chaque mot
  #pragma omp parallel for schedule(dynamic)
  for(int i = 1; i < argc; i++){
  
    recurrence(argv[i],ascii, occ);
    
  }

  for(int i = 0; i < sizeof(ascii); i++){
  
    if(ascii[i] == 'a' || ascii[i] == 'e' || ascii[i] == 'i' || ascii[i] == 'o' || ascii[i] == 'u' || ascii[i] == 'y'){
      v += occ[i];
    }
    else{c += occ[i];}
    
    if(occ[i] != 0){
    	snprintf(stmp, sizeof(ascii[i]) + sizeof(":") + sizeof(occ[i]) + sizeof(" "), "%c:%d ", ascii[i], occ[i]);
    	strncat(maps, stmp, sizeof(stmp));
    }
    sum += occ[i];
    
  }

  printf("\n1.Compute the recurrence of each letter in the text:\n%s\n",maps);
  printf("\n2.Compute the number of vowels and consonants from the result of step1:\nvowels = %d\nconsonants = %d\n", v, c);
  printf("\n3.Compute the total number of letters: %d\n\n", sum);
	return 0;
}
