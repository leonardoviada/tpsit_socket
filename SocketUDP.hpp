/*spiegazione 29/10/2020*/
#ifndef __SOCKET_UDP_
#define __SOCKET_UDP_

#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include "./Address.hpp"
#include<iostream>
using namespace std;

class SocketUDP{
    private: int sock_id;
    public: SocketUDP();    //socket()
            SocketUDP(Address); //bind()
            ~SocketUDP();   //close()

            int invia(Address*, char*);
            int invia(Address*, void*, int); //sendto()
            char* ricevi(Address*);
            void* ricevi(Address*, int*); //recvfrom()
};

    SocketUDP :: SocketUDP(){
        this->sock_id = socket(AF_INET, SOCK_DGRAM, 0);
    }
    SocketUDP :: SocketUDP(Address address) : SocketUDP(){
        bind(this->sock_id, (struct sockaddr*) address.getAddress(), (socklen_t)sizeof(struct sockaddr));
    }
    SocketUDP :: ~SocketUDP(){
        close(this->sock_id);
    }

    int SocketUDP :: invia(Address* dest, char* msg){
        
        int msgLen = strlen(msg) + 1;
        return invia(dest, (void*) msg, msgLen);
    }

    int SocketUDP :: invia(Address* dest, void* buffer, int len){  
        int rc = sendto(this->sock_id, buffer, len,  0, (struct sockaddr*) dest->getAddress(), (socklen_t) sizeof(struct sockaddr));
        
        return (rc <= 0);
    }

    char* SocketUDP :: ricevi(Address* mitt){
        
        int len;
        char* buffer = (char*) ricevi(mitt, &len);
        if(len >0) buffer[len] = '\0';
        return buffer;
    }

        void* SocketUDP :: ricevi(Address* mitt, int* len){

        char* buffer = (char*) malloc(sizeof((char) MAX_STR + 1));
        int lenAddr = sizeof(struct sockaddr);

        int rc = recvfrom(this->sock_id, (void*)buffer, MAX_STR, 0, (struct sockaddr*) mitt->getAddress(),(socklen_t*) &lenAddr);

        *len = rc;

        if(rc >=0){
            
            return buffer;
        }
        else{
            
            free(buffer);
            return NULL;
        } 
        
    } 


#endif