#include "hLoco.h"

void deciHola()
{
	printf("Hola loco");

}

void crearConexion(char* ip_destino, char* puerto_destino)
{
	//char *username = malloc(MAXUSERNAME);
		//get_Username(&username);


		 //Obtiene los datos de la direccion de red y lo guarda en serverInfo.

		struct addrinfo hints;
		struct addrinfo *serverInfo;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;		// Permite que la maquina se encargue de verificar si usamos IPv4 o IPv6
		hints.ai_socktype = SOCK_STREAM;	// Indica que usaremos el protocolo TCP

		getaddrinfo(ip_destino, puerto_destino, &hints, &serverInfo);

		int serverSocket;
		serverSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
		connect(serverSocket, serverInfo->ai_addr, serverInfo->ai_addrlen);
		freeaddrinfo(serverInfo);
		printf("Felicidades, te conectaste.");

}

void escucharConexiones (char* puerto_escucha)
{

	struct addrinfo hints;
		struct addrinfo *serverInfo;
		struct sockaddr_in client_addr;

		fd_set master;
		fd_set read_fds;
		FD_ZERO(&master);    // borra los conjuntos maestro y temporal
		FD_ZERO(&read_fds);

		int fdmax; //cantidad maxima de descriptores de fichero
		int numBytes; //cantidad de bytes recibidos
		int new_fd; //descriptor de fichero del cliente
		int i;

		char bufer[MAXDATASIZE];


		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;		// No importa si uso IPv4 o IPv6
		hints.ai_flags = AI_PASSIVE;		// Asigna el address del localhost: 127.0.0.1
		hints.ai_socktype = SOCK_STREAM;	// Indica que usaremos el protocolo TCP

		getaddrinfo(NULL, puerto_escucha, &hints, &serverInfo);

		int listenerSockfd;
		if((listenerSockfd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol)) == -1){
			perror("socket");
			exit(1);
		}

		int yes;
		if((setsockopt(listenerSockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) == -1){
			perror("setsockopt");
			exit(1);
		}

		if((bind(listenerSockfd,serverInfo->ai_addr, serverInfo->ai_addrlen)) == -1){
			perror("bind");
			exit(1);
		}
		freeaddrinfo(serverInfo);

		int sin_size;

			if((listen(listenerSockfd, BACKLOG)) == -1 )
			{
				perror("listen");
				exit(1);
			}

			FD_SET(listenerSockfd, &master); // agrega listenerSockfd al conjunto maestro
			fdmax = listenerSockfd; // descriptor maximo al principio

			while(1){
				read_fds = master;
				if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1){
					perror("select");
					exit(1);
				}

				for(i = 0; i <= fdmax; i++) {// explorar conexiones existentes en busca de datos que leer

					if (FD_ISSET(i, &read_fds)) {// comprueba si hay datos

						if (i == listenerSockfd) {// gestionar nuevas conexiones
							sin_size = sizeof(client_addr);
							if((new_fd = accept(listenerSockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1 ){
								perror("accept");
							}
							else{
								FD_SET(new_fd, &master); // añadir al conjunto maestro
								if (new_fd > fdmax) {    // actualizar el máximo
									fdmax = new_fd;
									}

								printf("server: nueva coneccion de %s on "
								                                "socket %d\n", inet_ntoa(client_addr.sin_addr), new_fd);
							}
						}
						else{// ver si hay datos de un cliente
							if((numBytes=recv(i, bufer, MAXDATASIZE-1, 0)) <= 0){
								if(numBytes == 0) {// conexión cerrada
									printf("server: socket %d se desconecto\n", i);
								    }
								else {
									perror("recv");
									}
								close(i); //cerrar el descriptor
								FD_CLR(i, &master); // eliminar del conjunto maestro
								perror("recv");
							}
							/*else{//hay datos del cliente
								for(j = 0; j <= fdmax; j++) { // enviar msj a todos
								    if (FD_ISSET(j, &master)) {
								        if (j != listenerSockfd && j != i) {// excepto al listener y a nosotros mismos
								        	do
								        	{
								        	if((send(j, bufer, numBytes, 0)) == -1 ){
								        		perror("send");
								        	}
								        	else{
								        		mjsEnviado = 0;
								        	}
								        	}while(mjsEnviado);
								        }
								    }
								}

								bufer[numBytes] = '\0';
								printf("Cliente: %s\n", bufer);// muestra msj del cliente

							}*/

						}

						}
					}

			}

			close(listenerSockfd);
}
