#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>


int main()
{
    int fd = socket(AF_INET,SOCK_DGRAM,0);
    
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    bzero(&addr,len);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(50002);

    bind(fd,(struct sockaddr *)&addr,len);

    struct sockaddr_in peeraddr;
    int peerlen = sizeof(peeraddr);
    bzero(&peeraddr,peerlen);

    char buf[100];

    while(1)
    {
        bzero(buf,100);
        recv(fd,buf,100,0);
        printf("收到【%s:%hu】的信息：\"%s\"\n",
                inet_ntoa(peeraddr.sin_addr),
                ntohs(peeraddr.sin_port),
                buf);
    
    
        sleep(1);
    
    
    }






return 0;
}
