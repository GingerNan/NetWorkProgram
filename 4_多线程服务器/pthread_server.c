#include <stdio.h>
#include <pthread.h>
#include "wrap.h"

void* client_fun(void* arg);

typedef struct c_info
{
    int cfd;
    struct sockaddr_in cliaddr;
}CINFO;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("argc < 2 ??? \n ./a.out 8000 \n");
        return 0;
    }

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);


    short port = atoi(argv[1]);
    int lfd = tcp4bind(port, NULL);

    Listen(lfd, 128);
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    CINFO *info;
    while (1)
    {
        int cfd = Accept(lfd, (struct sockaddr*)&cliaddr, &len);
        
        info = malloc(sizeof(CINFO));
        info->cfd = cfd;
        info->cliaddr = cliaddr;

        pthread_t pthid;
        pthread_create(&pthid, &attr, client_fun, info);
    }

    return 0;
}

void* client_fun(void* arg)
{
    CINFO *info = (CINFO*)arg;

    char clientIp[16] = "";
    printf("new client ip=%s port=%d",
            inet_ntop(AF_INET, &(info->cliaddr.sin_addr.s_addr), clientIp, 16),
            ntohs(info->cliaddr.sin_port));

    while (1)
    {
        char buf[1024] = "";
        int count = 0;
        count = read(info->cfd, buf, sizeof(buf));
        if (count < 0)
        {
            perror("");
            break;
        }
        else if (count == 0)
        {
            printf("clinet close\n");
            break;
        }
        else
        {
            printf("%s\n", buf);
            write(info->cfd, buf, count);
        }
    }
    
    close(info->cfd);
    free(info);
}