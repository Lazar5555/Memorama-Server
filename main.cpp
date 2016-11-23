#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

/*Librer�as para red*/
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <poll.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 25500
#define LEN_LOAD 12

#define WAITING_CODE "9999"

using namespace std;

/*Funci�n para verificar que el numero solo est� dos veces en el tablero*/
bool repeat(int arreglo[], int num, int cont){
    int flag = 0;
    for(int i = 0; i <= cont; i++){
        if(arreglo[i] == num)
            flag++;
    }
    if(flag > 1)
        return true;
    else
        return false;
}

int main(){
    int pares = 5;//variable para saber cuando el juego termine
    int scorePlayers[2];
    scorePlayers[0] = 0;
    scorePlayers[1] = 0;
    /*Variable que se manda al cliente con las posiciones y turno*/
    char load[LEN_LOAD];

    /*Iniciar la semilla de n�meros aleatorios*/
    srand(time(NULL));

    /*Cargar el tablero con n�meros aleatorios*/
    int tablero[10], num, i = 0;
    memset(tablero, 0, sizeof(tablero));
    while(i < 10){
        num = 1 + rand()%(5 + 1 - 1);
        if(!repeat(tablero, num, i)){
            tablero[i] = num;
            i++;
        }
        else
            continue;
    }


    /*Mostrar valores del tablero*/
    bzero(load, LEN_LOAD);
    for(int i = 0; i < 10; i++){
        cout<<tablero[i]<<endl;
    }

    int s, ns;
    int on = 1, res, res2, npolls = 1, tamano_actual = 0;
    int terminar_servidor = 0, recorrer_arreglo = 0;
    struct sockaddr_in server, client, clients[50];
    socklen_t clielen;
    char buffer[1500], ipClient[16];
    struct pollfd polls[50];

    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        perror("Error en socket()");
        close(s);
        return EXIT_FAILURE;
    }

    //inicializar la estructura sockaddr_in
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    res = bind(s, (struct sockaddr*)&server, sizeof(server));
    if(res < 0){
        perror("Error en bind()");
        close(s);
        return EXIT_SUCCESS;
    }

    res = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    if(res < 0){
        perror("Error en setsockopt()");
        close(s);
        return EXIT_FAILURE;
    }

    res = ioctl(s, FIONBIO, (char*)&on);
    if(res < 0){
        perror("Error en iocntl()");
        close(s);
        return EXIT_FAILURE;
    }

    res = listen(s, 49);
    if(res < 0){
        perror("Error en listen()");
        close(s);
        return EXIT_FAILURE;
    }

    memset(polls, 0, sizeof(polls));
    polls[0].fd = s;
    polls[0].events = POLLIN;
    cout<<"Servidor en espera..."<<endl<<endl;
    do{
        res = poll(polls, npolls, 30);

        if(res < 0){
            perror("Error en poll()");
            break;
        }

        tamano_actual = npolls;
        for(int i = 0; i < tamano_actual; i++){

            if(polls[i].revents == 0)
                continue;

            if(polls[i].fd == s){
                do{
                    memset(&client, 0, sizeof(client));
                    clielen = sizeof(client);
                    ns = accept(s, (struct sockaddr*)&client, &clielen);
                    if(ns < 0){
                        if(errno != EWOULDBLOCK){
                            perror("Error en accept()");
                            terminar_servidor = 1;
                        }
                        break;
                    }
                    inet_ntop(AF_INET, &(client.sin_addr), ipClient, 16);
                    cout<<"Se conecto "<<ipClient<<" por el puerto "<<client.sin_port<<endl;

                    inet_pton(AF_INET, ipClient, &(clients[npolls].sin_addr));
                    clients[npolls].sin_port = client.sin_port;

                    polls[npolls].fd = ns;
                    polls[npolls].events = POLLIN;
                    //Enviar el n�mero de jugador
                    sprintf(load, "%i%i%i%i%i%i%i%i%i%i%i", tablero[0], tablero[1], tablero[2], tablero[3], tablero[4], tablero[5], tablero[6], tablero[7], tablero[8], tablero[9], npolls);
                    cout<<"Enviando al socket "<<npolls<<" :"<<load<<endl;
                    res = send(polls[npolls].fd, load, strlen(load), 0);
                    if(res < 0){
                        perror("Error en send():");
                        terminar_servidor = 1;
                    }
                    cout<<"Se enviaron los datos."<<endl;

                    /*if(npolls == 1){
                        res = send(polls[npolls].fd, WAITING_CODE, sizeof(WAITING_CODE), 0);
                        if(res < 0){
                            perror("Error en send()");
                            terminar_servidor = 1;
                        }
                    }
                    else if(npolls == 2){
                        res = send(polls[1].fd, "8888", 4, 0);
                        if(res < 0){
                            perror("Error en send()");
                            terminar_servidor = 1;
                        }
                        res = send(polls[2].fd, "8888", 4, 0);
                        if(res < 0){
                            perror("Error en send()");
                            terminar_servidor = 1;
                        }
                    }*/

                    npolls++;
                }while(ns != -1);
            }
            else{

                if((polls[i].revents & POLLERR) || (polls[i].revents & POLLHUP) || (polls[i].revents & POLLPRI) || (polls[i].revents & POLLIN)){
                    res2 = recv(polls[i].fd, buffer, 1500, 0);
                    if(res2 < 0){
                        perror("Error en recv()");
                        close(s);
                        close(ns);
                        return EXIT_FAILURE;
                    }else if(res2 == 0){
                        inet_ntop(AF_INET, &(clients[i].sin_addr), ipClient, 16);
                        cout<<ipClient<<" puerto "<<clients[i].sin_port<<" cerro la conexion."<<endl;
                        close(polls[i].fd);
                        polls[i].fd = -1;
                        recorrer_arreglo = 1;
                    }else{
                        buffer[res2] = '\0';
                        inet_ntop(AF_INET, &(clients[i].sin_addr), ipClient, 16);
                        cout<<ipClient<<" puerto "<<clients[i].sin_port<<" envio: "<<buffer<<endl;

                        if(buffer[0] == '5'){
                            int peer, c1, c2;
                            char respuesta[10];
                            peer = (int)(buffer[1] - 48);//jugador al que vamos a responder
                            if(peer == 1)
                                peer = 2;
                            else
                                peer = 1;

                            c1 = (int)(buffer[2] - 48);
                            c2 = (int)(buffer[3] - 48);

                            if(tablero[c1] == tablero[c2]){
                                scorePlayers[i - 1]++;
                                pares--;
                            }

                            cout<<"Pares restantes: "<<pares<<endl;
                            cout<<"Jugador 1: "<<scorePlayers[0]<<" puntos."<<endl;
                            cout<<"Jugador 2: "<<scorePlayers[1]<<" puntos."<<endl<<endl;

                            sprintf(respuesta, "%i%i", c1, c2);
                            cout<<"ENVIANDO: "<<respuesta<<endl;
                            res = send(polls[peer].fd, respuesta, sizeof(respuesta), 0);
                            if(res < 0){
                                perror("Error en send():");
                                terminar_servidor == 1;
                            }

                            if(pares == 0){
                                cout<<"GAMEOVER"<<endl;
                            }
                        }
                    }
                }
            }
        }

        if(recorrer_arreglo == 1){

            recorrer_arreglo = 0;
            for (int i = 0; i < npolls; i++){
                if (polls[i].fd == -1){

                    for(int j = i; j < npolls; j++){
                        polls[j].fd = polls[j+1].fd;
                        clients[j] = clients[j+1];
                    }
                    npolls--;
                }
            }
        }

    }while(terminar_servidor == 0);

    for(int i = 0; i < npolls; i++){
        if(polls[i].fd >= 0)
            close(polls[i].fd);
    }
    close(ns);
    close(s);

    return EXIT_SUCCESS;
}
