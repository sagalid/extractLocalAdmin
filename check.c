#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

int ejecuta(char[], FILE *);
int cliente_socket(char *);

int main(){
    FILE * file_host;
    FILE * file_netAdm;

    char comando [] = "hostname & net localgroup administradores";
    ejecuta(comando, file_host);

    //char comando_lg []= "net localgroup administradores";
    //ejecuta(comando_lg, file_netAdm);


    //char * test = "password2, test";
    //int intCliente = cliente_socket(test);

    pclose(file_host);
    pclose(file_netAdm);
    return 0;
}


int cliente_socket(char * message){
    WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;

    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0){
		printf("Error: %d",WSAGetLastError());
		return 1;
	}

    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET){
		printf("No es posible crear el socket: %d" , WSAGetLastError());
	}

    server.sin_addr.s_addr = inet_addr("192.168.8.102"); //TODO: IP del server
	server.sin_family = AF_INET;
	server.sin_port = htons(4443); //TODO: Puerto del server

    if (connect(s, (struct sockaddr *)&server , sizeof(server)) < 0){
		puts("Error de conexion");
		return 1;
	}
    puts("Conectado");

    if( send(s, message, strlen(message), 0) < 0)
	{
		puts("Error en envio");
		return 1;
	}
	puts("Datos enviados a CSIRT. Gracias ;)");

    return 0;
}

int ejecuta(char comando [], FILE * fp){
    //FILE *fp;
    char output[1024];
    char temp[1024] = "password2,";//TODO: poner clave del server
    fp = popen(comando, "r");

    if(fp==NULL){ 
        perror("popen"); 
        exit(EXIT_FAILURE);
    }

    while(fgets(output, 1023, fp) != NULL){
        //printf("%s",output);
        strcat(temp,output);
    }
    strcat(temp, "\nEOF\n");
    char * mensaje = temp;
    cliente_socket(mensaje);
    pclose(fp);
    return 0;
}







