#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "wrap.h"

int main()
{
    // 1.创建套接字
    int sock_fd = Socket(AF_INET, SOCK_STREAM, 0);

    // 2.连接服务器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7701);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
    Connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr));

    // 3.读写数据
    char buf[1024];
    while (1)
    {
        int n = Read(STDIN_FILENO, buf, sizeof(buf));   // 从标准输入读取数据
        Write(sock_fd, buf, n);

        n = Read(sock_fd, buf, sizeof(buf));
        printf("Server: %s", buf);
    }
    
    // 关闭
    Close(sock_fd);
}