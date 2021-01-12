#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*PAR OVERFLOW
void fct(char *chaine){
    char tmp[100];
    strcpy(tmp, chaine);
    printf("Affichage %s\n", tmp);
}*/

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage: %s <shellcode>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Size: %d bytes.\n", strlen(argv[1]));
    void (*shellcode)() = (void((*)())) (argv[1]);

    shellcode();

    return EXIT_SUCCESS;
}

