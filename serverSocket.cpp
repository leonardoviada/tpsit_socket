#include"./SocketUDP.hpp"
#include <pthread.h>

#define PROMPT "$>"
#define EXIT_CMD "quit"

typedef struct{
    char* msg;
    SocketUDP socket;
}client;

void* listener(void* demon);

/*lavoro in classe sui socket*/
int main(int argc, char** argv){
    if(argc != 3){
        printf("USAGE: PORT MESSAGE %s", argv[0]);
        return -1;
    }

    Address mySelf(IP_LO, atoi(argv[1]));
    char* msg = argv[2];
    SocketUDP mySocket(mySelf);

    client param = {msg, mySocket};
    pthread_t thread_id;
    int rc = pthread_create(&thread_id, NULL, listener, (void*) &param);
    if(rc!=0)errore("pthread_create()", -2);

    printf("%s", PROMPT);
	char* command = inputStr();
	printf("\n");
	while(strcmp(command, EXIT_CMD)){
		free(command);
		printf("%s", PROMPT);
		command = inputStr();
		printf("\n");
	}

	free(command);
    
    mySocket.~SocketUDP();
    return 0;
}

void* listener(void* demon){
    client *param = (client*)demon;

    char* buffer = (char*) malloc(sizeof(char) * (MAX_STR + 1));
    Address* mitt = new Address();

    buffer = param->socket.ricevi(mitt);
    
    cout<< "ricevuto da" <<mitt->toString()<<endl;
    
    cout<<buffer<<endl;

    param->socket.invia(mitt, param->msg);

    

    pthread_exit(NULL);
}