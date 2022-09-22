#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    
    WSADATA WSAData;
    SOCKET sock;
    SOCKET csock1;
    SOCKET csock2;
    SOCKADDR_IN sin;
    SOCKADDR_IN csin;
    u_long iMode = 1;
    WSAStartup(MAKEWORD(2,0), &WSAData);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(23);
    bind(sock, (SOCKADDR *)&sin, sizeof(sin));
    listen(sock, 0);
    char buffer[256] = "salut";
    char buffer2[256] = "salut";
    int sinsize = sizeof(csin);
    csock1 = accept(sock, (SOCKADDR *)&csin, &sinsize);   
    printf("player 1 a rejoint\n");
    csock2 = accept(sock, (SOCKADDR *)&csin, &sinsize); 
    printf("player 2 a rejoint\n");
    ioctlsocket(csock1, FIONBIO, &iMode);
    ioctlsocket(csock2, FIONBIO, &iMode);
    struct sockaddr SenderAddr;
    int SenderAddrSize = sizeof (SenderAddr);
    

    while(1) /* Boucle infinie. Exercice : améliorez ce code. */
    {   
        
        
         
        if(recvfrom(csock1, buffer, sizeof(buffer), 0, &SenderAddr, &SenderAddrSize)> 0){
            sendto(csock2,buffer,strlen(buffer)+1, 0, (struct sockaddr*)&sin, sizeof(sin));
            printf("buffer player 1 : %s\n",buffer);
        }
        
        if(recvfrom(csock2, buffer, sizeof(buffer), 0, &SenderAddr, &SenderAddrSize) > 0){
            sendto(csock1,buffer,strlen(buffer)+1, 0, (struct sockaddr*)&sin, sizeof(sin));
            printf("buffer player 2 : %s\n",buffer);
        }
        
    }
    closesocket(csock1);
    closesocket(csock2);
    /* On devrait faire closesocket(sock); puis WSACleanup(); mais puisqu'on a entré une boucle infinie ... */
    return 0;
}