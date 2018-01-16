#include <stdio.h>
#include <stdlib.h>



int main(int argc,char *argv[]){

int k;
int res = 0;

if(argv[1] == NULL){

	printf("Veuillez entrer un parametre k");
	scanf("%d", &k);
	if(k < 0){
		printf("k doit etre superieur ou egale a 0");
		exit(1);
	}
}
else {

	k = *argv[1];	
	
}

if(argv[1] < 0){
	printf("le nombre k doit etre superieur ou egale a 0");
	exit(1);
}


int pk = 1 << k;
int tab[pk];

	for(int i=0; i<pk; i++){
		tab[i] = i;
		res += tab[i];
	}
	
	
printf(res);


	
return 0;
}
