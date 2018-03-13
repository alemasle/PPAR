#include <stdlib.h>
#include <stdio.h>

////////// QUESTION 1 //////////

/**
* increase and decrease
*/
void log2_series(int n){
  float res = 0.0;
  float res2 = 0.0;

  for (int i = 1, k = n; i <= n && k > 0; i++, k--) {
    float compute = (1. / (float) i);
    float comp = (1. / (float) k);

    if (i % 2 == 0) {
      res -= compute;
    }
    else{
      res += compute;
    }

    if(k % 2 == 0){
      res2 -= comp;
    }
    else{
      res2 += comp;
    }

  }

  printf("%f\n%f\n", res, res2);
}

//////////// QUESTION 2 /////////////
/*
* Float soustraction n'est pas assiocatif
*
*/

int main(int argc, char ** argv){

  if(argc < 2){
    printf("%s\n","Entrez un parametre N");
    exit(0);
  }

  log2_series(atoi(argv[1]));
}
