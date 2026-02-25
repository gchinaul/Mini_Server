/*
 * =====================================================================================
 *
 *       Filename:  mini_serv.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  25/02/2026 20:52:53
 *       Dossier:  none
 *       Compiler:  gcc
 *
 *         Author:  gchinaul (gchinaul@student.42nice.fr)
 *   Organization:  42 School Student
 *
 * =====================================================================================
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

int ids[65536], nb = 0, nid = 0;
char *bufs[65536];
int fds[65536];
char tmp[1 << 20], msg[1 << 20];

void fatal() {
    write(2, "fatal error\n", 12);
    exit(1);
}

void send_all(int exc, char *m) {
    for (int i = 0; i < nb; i++) {
        if (fds[i] != exc)
            send(fds[i], m, strlen(m), 0);
    }
}

int main(int ac, char **av) {

    if (ac != 2) {
        write(2, "Wrong number of arguments\n", 26);
        exit(1);
    }

    int srv = socket(AF_INET, SOCK_STREAM, 0);
    if (srv < 0) fatal();

    struct sockaddr_in a;
    bzero(&a, sizeof(a));
    a.sin_family = AF_INET;

    unsigned char *ip = (unsigned char *)&a.sin_addr.s_addr;
    ip[0] = 127; ip[1] = 0; ip[2] = 0; ip[3] = 1;

    int port = atoi(av[1]);
    unsigned char *pt = (unsigned char *)&a.sin_port;
    pt[0] = (port >> 8) & 0xff; pt[1] = port & 0xff;

    if (bind(srv, (struct sockaddr *)&a, sizeof(a)) < 0 || listen(srv, 128) < 0) fatal();

    fd_set r;
    while (1) {
        int mx = srv; FD_ZERO(&r); FD_SET(srv, &r);

        for (int i = 0; i < nb; i++) {
            FD_SET(fds[i], &r);
            if (fds[i] > mx)
                mx = fds[i];
        }

        if (select(mx + 1, &r, NULL, NULL, NULL) < 0) continue;
        if (FD_ISSET(srv, &r)) {
            int fd = accept(srv, NULL, NULL);
            if (fd >= 0) {
                fds[nb] = fd; ids[nb] = nid++; bufs[nb] = NULL;
                sprintf(msg, "server: client %d just arrived\n", ids[nb]);
                send_all(fd, msg);
                nb++;
            }
        }

        for (int i = 0; i < nb; i++) {
            if (!FD_ISSET(fds[i], &r)) continue;
            int n = recv(fds[i], tmp, sizeof(tmp) - 1, 0);

            if (n <= 0) {
                sprintf(msg, "server: client %d just left\n", ids[i]);
                close(fds[i]);
                free(bufs[i]);

                for (int j = i; j < nb - 1; j++) {
                    fds[j] = fds[j + 1];
                    ids[j] = ids[j + 1];
                    bufs[j] = bufs[j + 1];
                    }

                nb--;
                send_all(-1, msg);
                i--;
                continue;
            }

            tmp[n] = 0;

            int ol = bufs[i] ? strlen(bufs[i]) : 0;
            bufs[i] = realloc(bufs[i], ol + n + 1);
            if (!bufs[i]) fatal();
            if (!ol) bufs[i][0] = 0;
            strcat(bufs[i], tmp);
            char *s = bufs[i], *p;

            while ((p = strstr(s, "\n"))) {
                *p = 0;
                sprintf(msg, "client %d: %s\n", ids[i], s);
                send_all(fds[i], msg);
                s = p + 1;
            }

            if (*s) {
                char *b = malloc(strlen(s) + 1);
                if (!b) fatal();
                strcpy(b, s);
                free(bufs[i]);
                bufs[i] = b;
            }

            else {
                free(bufs[i]);
                bufs[i] = NULL;
            }
        }
    }
}
