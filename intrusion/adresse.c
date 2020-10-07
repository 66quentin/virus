#include <unistd.h>
#include <stdio.h>

int main (void){
	printf("Adresse de SHELLCODE: 0x%lx\n", getenv("SHELLCODE"));
	return 0;
}
