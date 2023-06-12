#pragma once

#include "common.h"

#include <WinSock2.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

#define KM_VERSION "0.1"
#define KM_PORT 2023
#define KM_IP "127.0.0.1"
#define KM_MAXUSERS 32


namespace user
{
    static int userCounter = 0;
    struct s_userInfo
    {
        std::string username, password, code;
        SOCKET user;

    } userInfo[KM_MAXUSERS];

};


namespace kilimanjaro
{
    WSAData wsadata;
    SOCKADDR_IN addr;
    SOCKET socketServer;
    SOCKET sUser[KM_MAXUSERS];
    unsigned sUserIdCounter = 0;

    std::string getKMVersion()
    {
        return KM_VERSION;
    }

    void ClientHandler(int id)
    {
        char msg[128];
        while(true)
        {
            recv(sUser[id], msg, sizeof(msg), NULL);
            for(int i = 0; i < KM_MAXUSERS; i++)
            {
                if(i == id) continue;

                send(sUser[i], msg, sizeof(msg), NULL);
            }
        }
    }

    void Run()
    {
        

        WSAStartup(MAKEWORD(2,1), &wsadata);


        
        addr.sin_addr.s_addr = inet_addr(KM_IP);
        addr.sin_port = htons(KM_PORT);
        addr.sin_family = AF_INET;


        socketServer = socket(AF_INET, SOCK_STREAM, NULL);
        bind(socketServer, (SOCKADDR*)&addr, sizeof(addr));
        listen(socketServer, SOMAXCONN);
        
        int sizeofaddr = sizeof(addr);


        for(int i = 0; i < KM_MAXUSERS; i++)
        {
            

            SOCKET connector;
            connector = accept(socketServer,(SOCKADDR*)&addr, &sizeofaddr);
            
            if(connector == 0)
            {
                std::cout << "#KMUSER_1 Error!\n"; 
            }
            else
            {
                // NEW REALISE

                // -- ClientSide should be - > 
                std::cout << "Username: ";

                user::s_userInfo user;
                std::cin >> user::userInfo[i].username;
                
                std::cout << "Password: ";
                std::cin >> user::userInfo[i].password;

                std::cout << "Special code (if exists): ";
                std::cin >> user::userInfo[i].code;
                // --
                user::userInfo[user::userCounter++].user = connector;
                // -----
                std::cout << "Connected client!\n";
                
                //sUser[sUserIdCounter] = connector;
                //sUserIdCounter++;
                

                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
            }
        }   
    }
};


