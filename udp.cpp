#include <iostream>
#include <stdio.h>
#include <string>

#ifdef _WIN32 
    #include <winsock2.h>
    #pragma comment(lib, "Ws2_32.lib")
#endif
#ifdef __linux__ 
    #include <unistd.h>
    #include<sys/socket.h>    //socket
    #include<arpa/inet.h> //inet_addr
#endif



char* _send(const char* IP, unsigned int PORT, const char* msg)
{
    int sock;
    struct sockaddr_in server;
    char message[2000] , server_reply[2000];

#ifdef _WIN32 
    WSADATA Data;
    WSAStartup(MAKEWORD(2, 2), &Data);
#endif
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        //std::cout << "Could not create socket!" << std::endl;
        return NULL;
    }
    //std::cout << "Socket created!" << std::endl;

    server.sin_addr.s_addr = inet_addr(IP); //IP Address of UDP Server
    server.sin_family = AF_INET;
    server.sin_port = htons( PORT ); //PORT

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) != 0)
    {
        perror("Connect failed. Error");
        return NULL;
    }

    //std::cout << "Connected!" << std::endl;

    //keep communicating with server
    //while(1)
    //{
        //std::cout << "\033[1;32mUser $: "; //green color
        //printf("Enter message : ");
        //std::string line;
        //std::getline(std::cin, line);
        //strcpy(message, line.c_str());

        strcpy(message, msg);


        std::cout << std::endl;

        //Send some data
        if( send(sock , message , strlen(message)+1 , 0) < 0)
        {
            //std::cout << "Send failed" << std::endl;
            return NULL;
        }

        memset(server_reply, 0, 2000);
        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            //std::cout << "recv failed" << std::endl;
        }

        //std::cout << "\033[1;31mServer #: "; //red color
        //std::cout << server_reply;
        //std::cout << std::endl << std::endl;


    //}

#ifdef __linux__  
    close(sock);
#endif

#ifdef _WIN32     
    closesocket(sock);
    WSACleanup();
#endif

    return server_reply;

}