/*
 * hLoco.h
 *
 *  Created on: 1/6/2015
 *      Author: utnso
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdint.h>
#ifndef HLOCO_H_
#define HLOCO_H_

#define BACKLOG 5			// Define cuantas conexiones vamos a mantener pendientes al mismo tiempo
#define MAX_PACKAGE_SIZE 1024	//El servidor no admitira paquetes de mas de 1024 bytes CAMBIAR
#define MAXUSERNAME 30
#define MAX_MESSAGE_SIZE 300
#define MAXDATASIZE 100


void deciHola();

void crearConexion (char* ip_destino, char* puerto_destino);

void escucharConexiones(char* puerto_escucha);
#endif /* HLOCO_H_ */
