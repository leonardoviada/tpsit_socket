#ifndef __ADDRESS_HPP
#define __ADDRESS_HPP

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "utilities.h"


#define MAX_STR 16384
#define PORT 8080


#define IP_LO "127.0.0.1"
#define IP_DHCP "0.0.0.0"


//creazione classe
class Address {
private: struct sockaddr_in* address;
        int port;
        char* ip;
        static pthread_mutex_t* mutex_id;

        /*costructr*/    
public: Address(char* ip, int port);
        Address(struct sockaddr_in*);
        // quando c'e un const all' interno di un parametro, questo non puo' essere modificato/cambiato
        Address(Address&); 
        Address();
        //disposer
        ~Address();

        /*prototipi*/

        struct sockaddr_in* setIp(char*);
        struct sockaddr_in* setPort(int);
        struct sockaddr_in* setAddress(struct sockaddr_in);
        char* getIp();
        int getPort();
        sockaddr_in* getAddress();
        char* toString();

};
        //inizializzazione per tutta la classe
        pthread_mutex_t* Address::mutex_id;

        //per evitare segmentation fault
        Address :: Address(){
                this->address = (sockaddr_in*) malloc(sizeof(sockaddr_in));
                address->sin_family = AF_INET;
                address->sin_port = PORT;
                inet_aton(IP_LO, &this->address->sin_addr);
                for(int i = 0; i<8; i++){
		address->sin_zero[i]=0;
                }
                pthread_mutex_t mutex_id;
                int rc = pthread_mutex_init(&mutex_id, NULL);
                if(rc!=0)errore("mutex_init()", -12);
        }

        Address :: Address(char* ip, int port){
                this->address = (sockaddr_in*) malloc(sizeof(sockaddr_in));
                address->sin_family = AF_INET;
                address->sin_port = port;
                inet_aton(ip, &address->sin_addr);
                for(int i=0; i<8;i++){
                address->sin_zero[i] = 0;
                }
                pthread_mutex_t mutex_id;
                int rc = pthread_mutex_init(&mutex_id, NULL);
                if(rc!=0)errore("mutex_init()", -12);
        }

        Address :: Address(Address& mitt){
                this->address->sin_family = AF_INET;
                this->address->sin_port = mitt.getPort();
                inet_aton(mitt.getIp(), &this->address->sin_addr);
                for(int i = 0; i<8; i++){
		this->address->sin_zero[i]=0;
	}       
                pthread_mutex_t mutex_id;
                int rc = pthread_mutex_init(&mutex_id, NULL);
                if(rc!=0)errore("mutex_init()", -12);
        
        }

        Address :: Address(struct sockaddr_in* sockaddr){
                this->address = (sockaddr_in*) malloc(sizeof(sockaddr_in));
                address->sin_family = AF_INET;
                address->sin_port = sockaddr->sin_port;
                address->sin_addr = sockaddr->sin_addr;
                for(int i = 0; i<8; i++){
		address->sin_zero[i]=0;
	}
                pthread_mutex_t mutex_id;
                pthread_mutex_init(&mutex_id, NULL);
        }

        Address :: ~Address(){

        }

        struct sockaddr_in* Address::setIp(char* ip){
                pthread_mutex_lock(mutex_id);
                inet_aton(ip, &address->sin_addr);
                pthread_mutex_unlock(mutex_id);
                return address;
        }

        struct sockaddr_in* Address::setPort(int port){
                address->sin_port = htons(port);
                
                return address;
        }

        struct sockaddr_in* Address::setAddress(struct sockaddr_in sockaddr){
                address->sin_family = AF_INET;
                address->sin_port = sockaddr.sin_port;
                address->sin_addr = sockaddr.sin_addr;
                for(int i = 0; i<8; i++){
		address->sin_zero[i]=0;
	}
                return address;

        }
        char* Address::getIp(){
                return inet_ntoa(address->sin_addr);
        }

        int Address::getPort(){
                return address->sin_port;
        }

        sockaddr_in* Address::getAddress(){
                return address;
        }
        
        char* Address :: toString(){
                char buffer[MAX_STR + 1];
               // sprintf(" %s [%s:%d]", buffer, this->getIp(), this->port);
                return strdup(buffer);
        }





#endif