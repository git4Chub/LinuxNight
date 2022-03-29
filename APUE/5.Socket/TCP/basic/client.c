#include  <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "proto.h"

int main(void)
{
    int sfd;
    struct sockaddr_in raddr; //remote addr

    /* create socket and init param */
    sfd = socket(AF_INET, SOCKET_STREAM, 0/* IPPROTO_TCP */);

    raddr.sin_family = AF_INET;
    raddr.sin_port = hton(atoi(SERVERPORT));
    inet_pton(AF_INET, "127.0.0.1", &(raddr.sin_addr));

    /* start connect */
    if(connect(sfd, (void *)&raddr, sizeof(raddr)) < 0) {
        perror("connect");
        exit(1);
    }

    FILE *fp;
    //convert fd --> file ptr, to make sure work with FILE-style api
    fp = fdopen(sfd, "r+");
    if(fp == NULL) {
        perror("fopen()");
        exit(1);
    }

    long long stamp;
    if(fscanf(fp, FMT_STAMP, &stamp) < 1) {
        fprintf(stderr, "bad format\n");
    } else {
        fprintf(stdout, FMT_STAMP, stamp);
    }

    close(sfd);

    exit(0);
}