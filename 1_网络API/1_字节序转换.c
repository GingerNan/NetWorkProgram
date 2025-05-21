#include <arpa/inet.h>
#include <stdio.h>

int main()
{
    // 1.将ip地址改为大端字节序
    char buf[4] = {192, 168, 1, 2};
    int num = *(int *)buf;
    int sum = htonl(num);
    unsigned char *p = &sum;
    printf("%d %d %d %d\n",*p, *(p+1), *(p+2), *(p+3));

    // 2.将2字节的数据改为大端字节序
    unsigned short a = 0x0102;
    unsigned short b = htons(a);
    printf("%x\n", b);

    // 3.将ip地址从大端字节序改为小端字节序
    unsigned char buf1[4] = {1, 1, 168, 192};
    int num1 = *(int *)buf;
    int sum1 = ntohl(num);
    unsigned char *p1 = &sum;
    printf("%d %d %d %d\n",*p1, *(p1+1), *(p1+2), *(p1+3));

    return;
}