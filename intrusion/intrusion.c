#include <stdio.h>
#include <string.h>

void fct(char *chaine){
	char tmp[100];
	strcpy(tmp, chaine);
	printf("Affichage %s\n", tmp);
}

int main(int argc, char *argv[]){
	fct(argv[1]);
	return 0;
}
