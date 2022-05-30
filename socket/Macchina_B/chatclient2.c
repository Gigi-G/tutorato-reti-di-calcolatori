#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    int sockfd, n;
    struct sockaddr_in remote_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char sendline[1000];
    char recveline[1000];

    if(argc < 3) {
        printf("Use: sender IP_dest PORT_dest");
        return 0;
    }

    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Error in socket opening ... exit!");
        return -1;
    }

    memset(&remote_addr, 0, sizeof(remote_addr));

    remote_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(remote_addr.sin_addr));
    remote_addr.sin_port = htons(atoi(argv[2]));
    printf("Insert a message or nothing to quit: ");
    while(fgets(sendline, sizeof(sendline), stdin) != NULL) {
        if(sendline[0] == '\n') return 1;
        sendto(sockfd, sendline, strlen(sendline) + 1, 0, (struct sockaddr *) &remote_addr, sizeof(remote_addr));
        printf("Insert a message or nothing to quit: ");
    }
}