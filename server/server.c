#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/wait.h>
#include<errno.h>

#define PORT "8008"
#define BACKLOG 10

void *get_in_addr(const struct sockaddr *addr);
void sigchld_handler(int s);

int main(void) {
    struct addrinfo hints, *result, *rv;
    struct sockaddr_storage client_addr;
    struct sigaction sa;
    int sockfd, connfd, send_bytes;
    char recv_msg[255], send_msg[255], s[INET6_ADDRSTRLEN];
    socklen_t sin_size;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(NULL, PORT, &hints, &result);

    if(status != 0){
        fprintf(stderr, "getaddressinfo(): %s\n", gai_strerror(status));
    }

    for(rv = result; rv != NULL; rv = rv->ai_next){
        if((sockfd = socket(rv->ai_family, rv->ai_socktype, rv->ai_protocol)) == -1){
            perror("socket()");
            continue;
        }
        if(bind(sockfd, rv->ai_addr, rv->ai_addrlen) == -1){
            perror("bind()");
            continue;
        }
        break;
    }

    if(sockfd == -1 || rv == NULL){
        exit(1);
    }

    if(listen(sockfd, BACKLOG) == -1){
        perror("listen():");
        exit(1);
    } 

    inet_ntop(rv->ai_family, get_in_addr(rv->ai_addr), s, sizeof s);
    printf("SERVER INFO\n%-8s  %-10s\n", "PORT", "ADDRESS");
    printf("%-8s  %-10s\n", PORT, s);

    freeaddrinfo(result);

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL) == -1){
        perror("sigaction");
        exit(1);
    }

    printf("Aguardando conexoes...\n");

    while(1){
        sin_size = sizeof client_addr;
        connfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
        if(connfd == -1){
            perror("accept()");
            continue;
        }

        printf("Conexao recebida!\n");

        inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *)&client_addr), s, sizeof s);
        printf("Cliente conectado: %s\n", s);

        if(!fork()){
            close(sockfd);

            if(send(connfd, "Ola! Por favor, informe seu nome\n", 33, 0) == -1){
                perror("send()");
                exit(1);
            }

            if(recv(connfd, recv_msg, sizeof recv_msg, 0) == -1){
                perror("recv()");
                exit(1);
            }

            char new_msg[255] = "Seja bem-vindo(a) ";
            strcat(new_msg, recv_msg);

            if(send(connfd, new_msg, sizeof new_msg, 0) == -1){
                perror("send()");
                exit(1);
            }

            exit(0);
        }

        close(connfd);
    }

    return 0;
}

void *get_in_addr(const struct sockaddr *addr){
    if(addr->sa_family == AF_INET){
        return &((struct sockaddr_in *)addr)->sin_addr;
    }

    return &((struct sockaddr_in6 *)addr)->sin6_addr;
}

void sigchld_handler(int s){
    (void)s;

    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}
