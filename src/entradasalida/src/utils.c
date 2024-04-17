#include "utils.h"

void imprimir_entero(int num)
{
	printf("%d\n", num);
}

void imprimir_a_enviar(void* a_enviar)
{
	int desplazamiento = 0;

	int* aux_cod_op = a_enviar;
	printf("cod op: %d\n", *aux_cod_op);
	desplazamiento += sizeof(int);
	int* aux_size_stream = a_enviar + desplazamiento;
	printf("size stream: %d\n", *aux_size_stream);
	desplazamiento += sizeof(int);
	char* aux_stream = a_enviar + desplazamiento;
	printf(aux_stream);
}

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *destino_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int err = getaddrinfo(ip, puerto, &hints, &destino_info);
	if(err != 0)
	{
		printf("error en funcion getaddrinfo()\n");
		exit(3);
	}

	// Ahora vamos a crear el socket.
	int socket_conexion_destino_file_descriptor =
                        socket(destino_info->ai_family,
						destino_info->ai_socktype,
						destino_info->ai_protocol);
	if(socket_conexion_destino_file_descriptor == -1)
	{
		printf("error en funcion socket()\n");
		exit(3);
	}

	// Ahora que tenemos el socket, vamos a conectarlo
	err = connect(socket_conexion_destino_file_descriptor, destino_info->ai_addr, destino_info->ai_addrlen);
	if(err != 0)
	{
		printf("error en funcion connect()\n");
		exit(3);
	}

	freeaddrinfo(destino_info);

	return socket_conexion_destino_file_descriptor;
}

    void enviar_mensaje(char* mensaje, int socket_emisor)
{
	decir_hola("por asignar memoria a paquete"); // para debug
	
	t_paquete* paquete = malloc(sizeof(t_paquete));

	decir_hola("memoria a paquete asignada"); // para debug

	decir_hola("por cargar cod. op y por asignar memoria a buffer"); // para debug
	imprimir_entero(MENSAJE); // para debug
	imprimir_entero(PAQUETE); // para debug

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));

	decir_hola("cod cargado y memoria asignada"); // para debug

	decir_hola("por cargar size del stream y por asignar memoria a stream"); // para debug
	
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);

        decir_hola("size cargado y memoria asignada"); // para debug

	decir_hola("por copiar mensaje a stream"); // para debug
	decir_hola(mensaje);
	
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	decir_hola("mensaje copiado"); // para debug

	decir_hola("por calcular bytes de paquete"); // para debug

	int bytes = paquete->buffer->size + 2*sizeof(int);

	decir_hola("bytes calculados"); // para debug

	decir_hola("por serializar"); // para debug
	decir_hola(paquete->buffer->stream); // para debug

	void* a_enviar = serializar_paquete(paquete, bytes);

	decir_hola("serializado"); // para debug

	decir_hola("por enviar"); // para debug
	imprimir_a_enviar(a_enviar);

	int bytes_send = send(socket_emisor, a_enviar, bytes, 0);

	decir_hola("enviado"); // para debug
	decir_hola("error al enviar:"); // para debug
	decir_hola(strerror(errno)); // para debug
	decir_hola("bytes enviados:"); // para debug
	imprimir_entero(bytes_send); // para debug

	decir_hola("por liberar memoria"); // para debug

	free(a_enviar);
	eliminar_paquete(paquete);

	decir_hola("memoria liberada"); // para debug
}

void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_paquete(void)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
