#include <stdio.h>
#include "wrap.h"
#include <sys/epoll.h>
#include <fcntl.h>

int main()
{
    // 创建套接字 绑定
    int lfd = tcp4bind(8888, NULL);
    // 监听
    Listen(lfd, 128);
    // 创建树
    int epfd = epoll_create(1);
    // 将lfd上树
    struct epoll_event ev, evs[1024];
    ev.data.fd = lfd;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
    // while监听
    while (1)
    {
        int nready = epoll_wait(epfd, evs, 1024, -1);
        if (nready < 0)
        {
            perror("");
            break;
        }
        else if (nready == 0)
        {
            continue;
        }
        else // 有文件描述符变化
        {
            for (int i = 0; i < nready; ++i)
            {
                // 判断lfd变化，斌且是读事件变化
                if (evs[i].data.fd == lfd && evs[i].events & EPOLLIN)
                {
                    struct sockaddr_in cliaddr;
                    char ip[16] = "";
                    socklen_t len = sizeof(cliaddr);
                    int cfd = Accept(lfd, (struct sockaddr *)&cliaddr, &len);

                    printf("received from %s at PORT %d\n",
                           inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)),
                           ntohs(cliaddr.sin_port));

                    // 设置cfd为非阻塞
                    int flags = fcntl(cfd, F_GETFL); //获取cfd的标志位
                    flags |= O_NONBLOCK;
                    fcntl(cfd, F_SETFL, flags);

                    // 将cfd上树
                    ev.data.fd = cfd;
                    ev.events = EPOLLIN | EPOLLET; // 设置 监听读事件 和 边沿触发方式
                    epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
                }
                else if (evs[i].events & EPOLLIN) // cfd变化，而且是读事件变化
                {
                    while (1)
                    {
                        char buf[4] = "";
                        // 如果读一个缓冲区，缓冲区没有数据，如果是带阻塞，就阻塞等待
                        // 如果是非阻塞，返回值等于-1，并且将errno值设置为EAGAIN
                        int n = read(evs[i].data.fd, buf, sizeof(buf));
                        if (n < 0)
                        {
                            // 如果缓冲区读干净了，这个时候应该跳出while(1)循环，继续监听
                            if (errno == EAGAIN)
                            {
                                break;
                            }

                            // 普通错误
                            perror("");
                            close(evs[i].data.fd);
                            epoll_ctl(epfd, EPOLL_CTL_DEL, evs[i].data.fd, &evs[i]);
                            break;
                        }
                        else if (n == 0)
                        {
                            printf("client close\n");
                            close(evs[i].data.fd);
                            epoll_ctl(epfd, EPOLL_CTL_DEL, evs[i].data.fd, &evs[i]);
                            break;
                        }
                        else
                        {
                            write(STDOUT_FILENO, buf, 4);
                            write(evs[i].data.fd, buf, n);
                        }
                    }
                }
            }
        }
    }

    close(lfd);
    return 0;
}