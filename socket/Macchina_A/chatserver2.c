#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    int sockfd, num, on;
    struct sockaddr_in local_addr, remote_addr;
    struct sockaddr_in online[1000];
    socklen_t len = sizeof(struct sockaddr_in);
    char buffer[1000];

    if(argc < 2) {
        printf("Use: <listening_PORT>\n");
        return 0;
    }

    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Error in socket opening ... exit!");
        return -1;
    }

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.1.2", &(local_addr.sin_addr));
    local_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr*) &local_addr, sizeof(local_addr)) < 0) {
        printf("Binding error! Error code n. %d\n", errno);
        return -1;
    }

    on = 0;
    while(1) {
        if((num = recvfrom(sockfd, buffer, 99, 0, (struct sockaddr*) &remote_addr, &len)) < 0) {
            printf("Error in receiving message...\n");
        }
        else {
            printf("Packet from IP: %s PORT: %d Msg: %s\n", inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port), buffer);
            int find = -1;
            for(int i = 0; i < on; i++) {
                char buffer1[100];
                char buffer2[100];
                sprintf(buffer1, "%s", inet_ntoa(online[i].sin_addr));
                sprintf(buffer2, "%s", inet_ntoa(remote_addr.sin_addr));
                if(strcmp(buffer1, buffer2) == 0) {
                    find = i;
                    break;
                }
            }
            if(find == -1 && on < 1000) {
                online[on].sin_family = remote_addr.sin_family;
                online[on].sin_addr = remote_addr.sin_addr;
                online[on].sin_port = htons(atoi("8889"));
                sprintf(buffer, "Sei un prescelto!\n");
                sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr*) &online[on], len);
                on++;
            }
            else if(find > -1) {
                for(int i = 0; i < on; i++) {
                    if(i != find) {
                        sendto(sockfd, buffer, num, 0, (struct sockaddr*) &online[i], len);
                    }
                }
            }
            else {
                sprintf(buffer, "Server pieno!\n");
                sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr*) &remote_addr, len);
            }
        }
    }
}
