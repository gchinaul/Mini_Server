
mini_serv

    Un serveur de chat TCP minimaliste en C

✨ Présentation

mini_serv est un serveur de chat TCP écrit en C, pensé pour la simplicité :

    Plusieurs clients peuvent s’y connecter et échanger des messages en temps réel.
    Le serveur gère l’identification de chaque client (ID unique) et informe tous les autres lors des connexions et déconnexions.

⚙️ Build & Exécution

Compilation :
bash

gcc -Wall -Wextra -o mini_serv mini_serv.c

Lancement :
bash

./mini_serv <port>

Testez-le avec nc dans plusieurs terminaux :
bash

nc 127.0.0.1 <port>

🛠️ Fonctionnement

    Utilise select() pour un I/O multiplexé (non-bloquant)
    Les messages incomplets sont mis en tampon jusqu’à réception du caractère \n
    Les événements (connexion, déconnexion, messages) sont diffusés à tous les clients
    Pas de threads, aucune bibliothèque externe : pur C standard + sockets POSIX

ℹ️ Notes

    Le serveur n’écoute que sur 127.0.0.1
    Les clients sont identifiés par un ID auto-incrémenté (départ à 0)
