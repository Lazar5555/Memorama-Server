#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <poll.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>*/

using namespace std;

int main(){

    /*int s, ns;
    int on = 1, port, res, res2, npolls = 1, tamano_actual = 0;
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

    cout<<"Puerto a la escucha: ";
    cin>>port;

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
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

            if(polls[i].revents != POLLIN){
                cout<<"Error en revents, no es POLLIN"<<endl;
                terminar_servidor = 1;
                break;
            }

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
                    }
                }
            }
        }

        if (recorrer_arreglo == 1){

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
    close(s);*/

    return EXIT_SUCCESS;
}
