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

		fd_set master;
		fd_set read_fds;
		FD_ZERO(&master);    // borra los conjuntos maestro y temporal
		FD_ZERO(&read_fds);

		int fdmax; //cantidad maxima de descriptores de fichero
		int i, j;


		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;		// No importa si uso IPv4 o IPv6
		hints.ai_flags = AI_PASSIVE;		// Asigna el address del localhost: 127.0.0.1
		hints.ai_socktype = SOCK_STREAM;	// Indica que usaremos el protocolo TCP

		getaddrinfo(NULL, puerto_escucha, &hints, &serverInfo);

		int listenningSocket;
		listenningSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
		bind(listenningSocket,serverInfo->ai_addr, serverInfo->ai_addrlen);
		freeaddrinfo(serverInfo);


		int new_fd, numBytes;
		struct sockaddr_in my_addr;
		struct sockaddr_in client_addr;
		char bufer[MAXDATASIZE];
		int listenerSockfd;

		int sin_size;

		if((listen(listenningSocket, BACKLOG)) == -1 )
			{
				perror("listen");
				exit(1);
			}

			FD_SET(listenningSocket, &master); // agrega listenerSockfd al conjunto maestro
			fdmax = listenningSocket; // descriptor maximo al principio
			read_fds = master;
			while(1){
				if(select(fdmax+1, &master, NULL, NULL, NULL) == -1){
					perror("select");
					exit(1);
				}

				printf("Cree el select");

				for(i = 0; i <= fdmax; i++) {// explorar conexiones existentes en busca de datos que leer

					if (FD_ISSET(i, &read_fds)) {// comprueba si hay datos

						if (i == listenningSocket) {// gestionar nuevas conexiones
							sin_size = sizeof(client_addr);
							if((new_fd = accept(listenningSocket, (struct sockaddr *)&client_addr, &sin_size)) == -1 ){
								perror("accept");
							}
							else{
								FD_SET(new_fd, &master); // añadir al conjunto maestro
								if (new_fd > fdmax) {    // actualizar el máximo
									fdmax = new_fd;
									}

								//strcpy(chat_dialogo, "Conexion establecida"); // avisar al cliente que se establecio conexion
								//long_chat_dialogo = strlen(chat_dialogo);
								//do
								//{
									//if((send(new_fd, chat_dialogo, long_chat_dialogo, 0)) == -1 ){
									//	perror("send");
									//}
									//else{
									//	mjsEnviado = 0;
								//	}
								//}while(mjsEnviado);

								printf("server: nueva coneccion de %s on "
								                                "socket %d\n", inet_ntoa(client_addr.sin_addr), new_fd);
									}
						}
						// ver si hay datos de un cliente
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
							else{//hay datos del cliente
								for(j = 0; j <= fdmax; j++) { // enviar msj a todos
								    if (FD_ISSET(j, &master)) {
								        if (j != listenningSocket && j != i) {// excepto al listener y a nosotros mismos
								        	/*do
								        	{
								        	if((send(j, bufer, numBytes, 0)) == -1 ){
								        		perror("send");
								        	}
								        	else{
								        		mjsEnviado = 0;
								        	}
								        	}while(mjsEnviado);*/
								        }
								    }
								}

								bufer[numBytes] = '\0';
								printf("Cliente: %s\n", bufer);// muestra msj del cliente
							}



						}
					//si el socket no esta FD_ISSET
					//else{
						//printf("se cerro el socket wacho");
					//}
					}

			}

			close(listenningSocket);




}
