#include <arpa/inet.h>
#include <stdlib.h>

int main()
{
    // 1.将点分制十进制串改为大端字节序
    char buf[] = "192.1.1.4";
    unsigned int num = 0;
    inet_pton(AF_INET, buf, &num);
    unsigned char *p = (unsigned char*)&num;
    printf("%d %d %d %d\n",*p, *(p+1), *(p+2), *(p+3));

    // 2.将大端字节序的ip地址转为点分十进制
    char ip[16] = "";
    printf("%s\n", inet_ntop(AF_INET, &num, ip, 16));
    return 0;
}