#include <stdio.h>
#include <stdlib.h>



int sum(int * tab, int n, int i){

int j = i;
int a = 1; 

for(int c = 0; c < n; c ++){
	if(j % 2 == 0){
		if(i + a < n){
			tab[i] += tab[i + a];
		}
		j /= 2;
		a *= 2;
	}
}

}


int main(int argc,char *argv[]){

int k;

if(argv[1] == NULL){

	printf("Veuillez entrer un parametre k\n");
	scanf("%d", &k);
	if(k < 0){
		printf("k doit etre superieur ou egale a 0\n");
		exit(1);
	}
}
else {

	k = atoi(argv[1]);	
	
}

if(argv[1] < 0){
	printf("le nombre k doit etre superieur ou egale a 0\n");
	exit(1);
}


int pk = 1 << k;
int tab[pk];

	for(int i=0; i<pk; i++){
		tab[i] = i;		
	}
	
	for(int i = 0; i<pk; i ++){//en parallel
		sum(tab, pk, i);
	}
	
// Complexity O(n) ou O(2^k)
	
printf("resultat : %d\n",tab[0]);

return 0;
}
