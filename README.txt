Virus réalisés par Quentin Guardia (qguardia66@gmail.com)

Faire très attention à l'éxecution des fichiers, certains peuvent corrompre des fichiers. Je ne suis pas responsable de vos actes

----ECRASEMENT----
Le virus scanne en profondeur l'arborescence à partir du dossier dans lequel il se trouve.
Il détecte chaque fichier éxecutable.
Il les remplace par lui-même.
On pourrait ajouter une fonction qui permettrait de garder la taille originale de l'éxecutable infecté.

----RESIDENT----
Le virus crée un nouveau processus fils chaque seconde et tue le père.
Il se renomme avec un nom similaire à celui d'un programme de l'ordinateur.
Toute les 3 secondes, il scanne le dossier $HOME.
Il cherche alors un fichier .c édité il y a moins d'une heure.
S'il y en a un, il vérifie qu'il ne soit pas marqué.
S'il est marqué, il ne fait rien.
S'il n'est pas marqué, il ajoute sous la ligne du main une commande infectée et marque le fichier.

----INTRUSION----
Exécute du shellcode par débordement de tampon. Non terminé.
Compilation avec -m32 et -z execstack

----VBASHSUB----


