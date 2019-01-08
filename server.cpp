#include <sys/types.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#define _BSD_SOURCE
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
/* portul folosit */
#define PORT 4444

/* codul de eroare returnat de anumite apeluri */
extern int errno;
char msg[100], msg2[100];
struct infoPlayer {
    char numePlayer[256], semnJucator;
    int numar;
    int scor = 0;
};
infoPlayer jucator1, jucator2;
char tabla[9][10];
int latime = 7, lungime = 6, coloanaAleasa, win, full, again;


extern int errno; /* eroarea returnata de unele apeluri */

char* conv_addr(struct sockaddr_in address)
{
    static char str[25];
    char port[7];

    strcpy(str, inet_ntoa(address.sin_addr));
    bzero(port, 7);
    sprintf(port, ":%d", ntohs(address.sin_port));
    strcat(str, port);
    return (str);
}

int randJucator(char tabla[][10], infoPlayer activ)
{
    char buffer[100];
    char msgeroare[200];
    int coloanaAleasa;
    write(activ.numar, "r", 100);
    do {
        cout << "Randul lui " << activ.numar << "\n";
        char primit[100];
        if (read(activ.numar, primit, 100) <= 0) {
            perror("[server]Eroare la read() de la client.\n");
            close(activ.numar); /* inchidem conexiunea cu clientul */
            continue; /* continuam sa ascultam */
        }
        coloanaAleasa = atoi(primit);

        while (tabla[1][coloanaAleasa] == 'X' || tabla[1][coloanaAleasa] == 'O') {
            write(activ.numar, "e", sizeof(msgeroare));
            if (read(activ.numar, primit, 100) <= 0) {
                perror("[server]Eroare la read() de la client.\n");
                close(activ.numar); /* inchidem conexiunea cu clientul */
                continue; /* continuam sa ascultam */
            }
            coloanaAleasa = atoi(primit);
        }

    } while (coloanaAleasa < 1 || coloanaAleasa > 7);

    return coloanaAleasa;
}

void muta(char tabla[][10], infoPlayer activ, int coloanaAleasa)
{
    int length, turn;
    length = 6;
    turn = 0;

    do {
        if (tabla[length][coloanaAleasa] != 'X' && tabla[length][coloanaAleasa] != 'O') {
            tabla[length][coloanaAleasa] = activ.semnJucator;
            turn = 1;
        }
        else
            --length;
    } while (turn != 1);
}

void afisareTabla(char tabla[][10], infoPlayer activ)
{
    int randuri = 6, coloane = 7, i, j;

    char ceva[2024];
    for (i = 1; i <= randuri; i++) {
        cout << "| ";
        strcat(ceva, "1");
        for (j = 1; j <= coloane; j++) {
            if (tabla[i][j] != 'X' && tabla[i][j] != 'O') {
                tabla[i][j] = '*';
                strcat(ceva, "2");
            }
            cout << tabla[i][j] << " ";
            if (tabla[i][j] == 'X') {
                strcat(ceva, "3");
            }
            if (tabla[i][j] == 'O') {
                strcat(ceva, "4");
            }
        }

        cout << "| " << endl;
        strcat(ceva, "1");
    }
    printf("Matricea de trimis %s\n", ceva);
    write(activ.numar, "t", 100);
    if (write(activ.numar, ceva, sizeof(ceva)) <= 0) {
        perror("[server]Eroare la write() catre client.\n");
    }
    else
        printf("[server]Mesajul a fost trasmis cu succes.\n"), bzero(ceva, 2000);
}

int verificareSolutie(char tabla[][10], infoPlayer activ)
{
    char semn;
    int win;

    semn = activ.semnJucator;
    win = 0;

    for (int i = 8; i >= 1; --i) {

        for (int j = 9; j >= 1; --j) {

            if (tabla[i][j] == semn && tabla[i - 1][j - 1] == semn && tabla[i - 2][j - 2] == semn && tabla[i - 3][j - 3] == semn) {
                win = 1;
            }

            if (tabla[i][j] == semn && tabla[i][j - 1] == semn && tabla[i][j - 2] == semn && tabla[i][j - 3] == semn) {
                win = 1;
            }

            if (tabla[i][j] == semn && tabla[i - 1][j] == semn && tabla[i - 2][j] == semn && tabla[i - 3][j] == semn) {
                win = 1;
            }

            if (tabla[i][j] == semn && tabla[i - 1][j + 1] == semn && tabla[i - 2][j + 2] == semn && tabla[i - 3][j + 3] == semn) {
                win = 1;
            }

            if (tabla[i][j] == semn && tabla[i][j + 1] == semn && tabla[i][j + 2] == semn && tabla[i][j + 3] == semn) {
                win = 1;
            }
        }
    }

    return win;
}

int tablaPlina(char tabla[][10])
{
    int full;
    full = 0;
    for (int i = 1; i <= 7; ++i) {
        if (tabla[1][i] != '*')
            ++full;
    }
    return full;
}

void castigator(infoPlayer activ, infoPlayer activ1)
{
    cout << endl;
    cout << "Jucatorul " << activ.numar << " a conectat 4 piese. Ai castigat!" << endl;
    char* x;
    write(activ.numar, "w", 100);
    read(activ.numar, x, 100);
    write(activ1.numar, "l", 100);
    read(activ.numar, x, 100);
}
void egalitate(infoPlayer activ, infoPlayer activ1)
{
    cout << endl;
    cout<<"Egalitate boys !\n";
    char* x;
    write(activ.numar, "i", 100);
    read(activ.numar, x, 100);
    write(activ1.numar, "j", 100);
    read(activ.numar, x, 100);
}

int restart(char tabla[][10], infoPlayer activ)
{
    int restart;
    char* u;
    cout << "Vrei sa restartezi jocul? 1 - Da ,2 - Nu ";
    read(activ.numar, u, 10);
    restart = atoi(u);
    if (restart == 1) {
        for (int i = 1; i <= 6; i++) {
            for (int j = 1; j <= 7; j++) {
                tabla[i][j] = '*';
            }
        }
    }
    else
        cout << "Pe curand!" << endl;
    return restart;
}

int main()
{
    struct sockaddr_in server; // structura folosita de server
    struct sockaddr_in from;
    char msg[100]; //mesajul primit de la client
    int sd; //descriptorul de socket

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("[server]Eroare la socket().\n");
        return errno;
    }

    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    if (bind(sd, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1) {
        perror("[server]Eroare la bind().\n");
        return errno;
    }

    if (listen(sd, 1) == -1) {
        perror("[server]Eroare la listen().\n");
        return errno;
    }

    while (1) {
        int player1, player2;
        unsigned int length = sizeof(from);

        printf("[server]Asteptam la portul %d...\n", PORT);
        fflush(stdout);

        player1 = accept(sd, (struct sockaddr*)&from, &length);
        player2 = accept(sd, (struct sockaddr*)&from, &length);
        if (player1 < 0 ) {
            perror("[server]Eroare la acceptul clientului 1.\n");
            continue;
        }
        if ( player2 < 0)
        {
            perror("[server]Eroare la acceptul clientului 2.\n");
            continue;
        }

        int pid;
        if ((pid = fork()) == -1) {
            close(player1);
            close(player2);
            continue;
        }
        else if (pid > 0) {
            close(player1);
            close(player2);
            while (waitpid(-1, NULL, WNOHANG));
            continue;
        }
        else if (pid == 0) {
            close(sd);
            if (write(player1, "P", 100) <= 0) {
                perror("[server]Eroare la write() catre client.\n");
                continue; /* continuam sa ascultam */
            }
            if (write(player2, "D", 100) <= 0) {
                perror("[server]Eroare la write() catre client.\n");
                continue; /* continuam sa ascultam */
            }
                printf("[server]Asteptam mesajul...\n");
                fflush(stdout);
            while (1) {
                bzero(msg, 100);


                infoPlayer jucator1, jucator2;
                char tabla[9][10];
                int latime = 7;
                int lungime = 6;
                int coloanaAleasa, win, full, again;

                cout << "Sa incepem jocul" << endl
                     << endl;
                jucator1.semnJucator = 'X';
                jucator1.numar = player1;
                jucator2.semnJucator = 'O';
                jucator2.numar = player2;

                full = 0;
                win = 0;
                again = 0;
                int egal = 0;
                do {
                    coloanaAleasa = randJucator(tabla, jucator1);
                    muta(tabla, jucator1, coloanaAleasa);
                    afisareTabla(tabla, jucator2);
                    win = verificareSolutie(tabla, jucator1);
                     egal=0;
                    egal=tablaPlina(tabla);
                    if (egal==7)
                    {
                        egalitate(jucator1,jucator2);
                        again = 2;
                        if (again == 2) {
                            break;
                        }
                    }
                    if (win == 1) {
                        jucator1.scor++;
                        castigator(jucator1, jucator2);

                        again = 2;
                        if (again == 2) {
                            break;
                        }
                    }

                    coloanaAleasa = randJucator(tabla, jucator2);
                    muta(tabla, jucator2, coloanaAleasa);
                    afisareTabla(tabla, jucator1);
                    win = verificareSolutie(tabla, jucator2);
                     egal=0;
                    egal=tablaPlina(tabla);
                    if (egal==7)
                    {
                        egalitate(jucator2,jucator1);
                        again = 2;
                        if (again == 2) {
                            break;
                        }
                    }
                    if (win == 1) {
                        jucator2.scor++;
                        castigator(jucator2, jucator1);

                        again = 2;
                        if (again == 2) {
                            break;
                        }
                    }


                } while (again != 2);
                printf("Scor final : Jucatorul 1 %d - %d Jucatorul 2\n", jucator1.scor, jucator2.scor);
                break;
            }
            close(player1);
            close(player2);
            exit(0);
        }

    } 
}
