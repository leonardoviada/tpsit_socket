#include"./SocketUDP.hpp"
#include<iostream>
using namespace std;

/*lavoro in classe sui socket*/
int main(int argc, char** argv){
    if(argc != 4){
        printf("USAGE: IP PORT MESSAGGIO %s\n", argv[0]);
        return -1;
    }

    
    Address server(argv[1], atoi(argv[2]));
    char* msg = argv[3];
    char* risposta;
    
    SocketUDP socket(server);
    

    socket.invia(&server, msg);

    risposta = socket.ricevi(&server);

    cout<<risposta<<endl;
    
    socket.~SocketUDP();
    
    return 0;
}