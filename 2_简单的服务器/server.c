#include <stdio.h>
#include "wrap.h"

int main()
{
    // 1.创建socket
    // 2.绑定端口
    int lfd = tcp4bind(7701, NULL);

    // 3.监听端口
    Listen(lfd, 128);

    // 4.提取连接
    struct sockaddr_in cli_addr;
    socklen_t len = sizeof(cli_addr);
    int cfd = Accept(lfd, (struct sockaddr*)&cli_addr, &len);
    
    char cip[16];
    printf("new client ip=%s port=%d\n", 
            inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, cip, 16),
            ntohs(cli_addr.sin_port));

    // 读写
    char buf[1024] = "";
    while (1)
    {
        bzero(buf, sizeof(buf));
        // 从终端读取数据
        //int n = Read(STDIN_FILENO, buf, sizeof(buf));
        //Write(cfd, buf, n);

        int n = Read(cfd, buf, sizeof(buf));
        printf("%s: %s\n", cip, buf);
    }
    
    // 5.关闭连接
    Close(cfd);
    Close(lfd);
}