#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "Address.hpp"
#include <iostream>
using namespace std;

#define PROMPT "$>"
#define EXIT_CMD "quit"
#define IP_DHCP "0.0.0.0"

//inizializzata per tutti i parametri che ci servono nel thread
typedef struct 
{
	char* msg;
	int sock_id;
}client;

//prototipo della funzione in cui lavorerà il thread
void* listener(void* demon);

int main(int argc,char** argv){

	//solito controllo errori
	if(argc != 3){
		printf("USAGE: %s PORT MESSAGE\n", argv[0]);
		return -1;
	}
	//prendiamo i parametri da argv
	int port = atoi(argv[1]);
	char* msg = argv[2];
	Address* mySelf = new Address(IP_LO, port);

	//apertura socket
	int socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	//controllo errore apertura
	if(socket_id == -1)errore("socket()", -2);

	/*
	Uguale alla bind del server scritto in C
	*/
    int rc = bind(socket_id,(struct sockaddr*) mySelf->getAddress(), (socklen_t)sizeof(struct sockaddr));
    //controllo errori
	if(rc != 0)errore("bind()", -3);

	//dichiarzione nuova struttura
	client parametri = {msg, socket_id};

	//creazione thread
	pthread_t demon_id;
	rc = pthread_create(&demon_id, NULL, listener, (void*) &parametri);
	if(rc!=0)errore("pthread_create()", -4);


	//fino a quando non scrivi quit, puoi scrivere
	printf("%s", PROMPT);
	char* command = inputStr();
	printf("\n");
	while(strcmp(command, EXIT_CMD)){
		free(command);
		printf("%s", PROMPT);
		command = inputStr();
		printf("\n");
	}

	//chiusura/rimozione di tutto
	free(command);
	close(socket_id);

	return 0;

}


void* listener(void* demon){
	//il reinterpret_cast forza un cast, bloccato dal demon, dal void alla struttura dichiarata precedentemente
	client *param = reinterpret_cast<client*>(&demon);
    
    //contenitore dove mettiamo il messaggio del mittente
	char buffer[MAX_STR + 1];
	//struttura che serve alla recvfrom da modificare, per vedere chi ci ha scritto
	Address* mitt = new Address();
	//definisce la grandezza della struttura sockaddr
	int addr_len = sizeof(struct sockaddr);

	/*
	Rimane uguale a quella del client in C perchè ancora non abbiamo il socketUDP. 
	L'unico parametro che cambia è il quinto.
	*/
	int rc = recvfrom(param->sock_id, buffer, MAX_STR, 0, (struct sockaddr*) mitt->getAddress(), (socklen_t*) &addr_len);
	//solito controllo errori
	if(rc<=0) errore("recvfrom()", -5);
	//mettiamo alla fine del buffer un carattere di fine stringa per avere un po' di sicurezza in più
	buffer[rc] = '\0';

	printf("%s ricevuto da [%s:%d] '%s'\n", PROMPT, mitt->getIp(), mitt->getPort(), buffer);

	/*
	Rimane uguale a quella del server in C perchè ancora non abbiamo il socketUDP. 
	L'unico parametro che cambia è il quinto, 
	che viene passato per referenza tramite un metodo del server che ritorna alla struttura sockaddr_in 
	*/
	rc = sendto(param->sock_id, param->msg, strlen(param->msg)+1, 0, (struct sockaddr*) mitt->getAddress(), (socklen_t) sizeof(struct sockaddr));
	//solito controllo errori
	if(rc != strlen(param->msg)+1) errore("sendto()", -6);
	printf("%s invia a [%s:%d] '%s'\n", PROMPT, mitt->getIp(), mitt->getPort(), param->msg);

	return NULL;
}