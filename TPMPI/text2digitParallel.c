#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int i, n, my_rank, NUMBER_PROC;;
  int count, ascii_code;
  char *text;
  char filename[20];
  short notblank, notpoint, notnewline;
  short number, minletter, majletter;
  FILE *input;

  // getting started (we suppose that the argv[1] contains the filename related
  // to the text)
  input = fopen(argv[1], "r");
  if (!input) {
    fprintf(stderr, "%s: impossible to open file '%s', stopping\n", argv[0],
            argv[1]);
    return 1;
  };

  // checking file size
  fseek(input, 0, SEEK_END);
  n = ftell(input);
  rewind(input);

  // reading the text
  text = (char *)calloc(n + 1, sizeof(char));
  for (i = 0; i < n; i++)
    text[i] = fgetc(input);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  count = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &NUMBER_PROC);
  for (i = my_rank*(n/NUMBER_PROC); i < my_rank*(n/NUMBER_PROC) + (n/NUMBER_PROC); i++) {
    ascii_code = (int)text[i];
    notblank = (ascii_code != 32);
    notpoint = (ascii_code != 46);
    notnewline = (ascii_code != 10);
    number = (ascii_code >= 48 && ascii_code <= 57);     // 0-9
    majletter = (ascii_code >= 65 && ascii_code <= 90);  // A-Z
    minletter = (ascii_code >= 97 && ascii_code <= 122); // a-z

    if(notpoint && notnewline){
    	if(number){printf("%c", text[i]);}
    	else if(majletter || minletter){count ++;}
    	else{
	    	if(!notblank && count != 0){
    			printf("%d", count); count = 0;
    		}
    		else if(notblank){
    			printf("%d0", count);
    			count = 0;
    		}
    	}
    }

  }

  printf("\n");

  // closing
  free(text);
  fclose(input);

  MPI_Finalize();
  return(0);
}
