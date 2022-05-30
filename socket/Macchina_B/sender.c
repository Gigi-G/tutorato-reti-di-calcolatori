#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

/*struct in_addr {
    u_int32_t s_addr;
};

struct sockaddr_in {
    short int sin_family;
    unsigned short int sin_port;
    struct in_addr sin_addr;
    unsigned char sin_zero[8];
};*/

int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in dest_addr;
    //extern int errno;
    int i, n;
    char buffer[100];

    if(argc < 3) {
        printf("Use: sender IP_dest PORT_dest");
        return 0;
    }

    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Error in socket opening ... exit!");
        return -1;
    }

    //Ci assicuriamo che la stuttura sia vuota!
    memset(&dest_addr, 0, sizeof(dest_addr));

    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(dest_addr.sin_addr));
    dest_addr.sin_port = htons(atoi(argv[2]));

    printf("Insert an integer value: ");

    fgets(buffer, sizeof(buffer), stdin);
    
    if(!(n = atoi(buffer))) {
        printf("Error in cast char* to int ... exit!\n");
        return -1;
    }

    for(i = 0; i < 10; i++) {
        sprintf(buffer, "%d", i+n);
        printf("Sending: %s Size: %d...\n", buffer, strlen(buffer) + 1);
        sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *) &dest_addr, sizeof(dest_addr));
    }
}