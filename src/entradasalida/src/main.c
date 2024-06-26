#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>

#include "main.h"

int main(int argc, char* argv[]) {
    decir_hola("una Interfaz de Entrada/Salida\n");

    int conexion_file_descriptor = 1;
    char* ip;
	char* puerto;
	char* valor;

    t_config* config;

    decir_hola("POR INICIAR CONFIGURACIÓN\n"); // para debug
	
    config = iniciar_config_io();

    decir_hola("CONFIGURACIÓN INICIADA\n");  // para debug
	
    decir_hola("POR OBTENER DATOS DE CONFIG\n");  // para debug
	
	ip = config_get_string_value(config, "IP_KERNEL");
	puerto = config_get_string_value(config, "PUERTO_KERNEL");

    decir_hola("DATOS DE CONFIG OBTENIDOS\n");  // para debug
    decir_hola("ip:"); // para debug
    decir_hola(ip); // para debug
    decir_hola("puerto:"); // para debug
    decir_hola(puerto); // para debug

    decir_hola("POR CREAR CONEXION\n");  // para debug

    conexion_file_descriptor = crear_conexion(ip, puerto);

    decir_hola("CONEXION ESTABLECIDA\n");  // para debug

    decir_hola("POR ENVIAR MENSAJE\n");  // para debug

    enviar_mensaje("Hola Kernel, como va. Soy IO.", conexion_file_descriptor);

    decir_hola("MENSAJE ENVIADO\n");  // para debug

    decir_hola("TERMINANDO PROGRAMA\n");  // para debug

    terminar_programa(conexion_file_descriptor, config);

    return 0;
}

t_config* iniciar_config_io(void)
{
	t_config* nuevo_config = config_create("/home/utnso/so1C2024/tp-2024-1c-Aprobamos-O-Aprobamos/entradasalida/config/default.config");

	if (nuevo_config == NULL)
	{
		printf("archivo  \"cliente.config\" no encontrado");
		printf("no se pudo instanciar la config del cliente");
		exit(3);

	} else
	{
		printf("config del cliente instanciada");
	}

	return nuevo_config;
}

void terminar_programa(int socket_conexion_file_descriptor, t_config* config)
{
	// Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	 // con las funciones de las commons y del TP mencionadas en el enunciado /
	int err = close(socket_conexion_file_descriptor);
	if(err != 0)
	{
		printf("error en funcion close()\n");
		exit(3);
	}
	config_destroy(config);
}

/*
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // Para close()
#include <utils/hello.h>
#include "main.h"

#define ARCHIVO_CONFIG "config/default.config"
#define CODIGO_SALIDA_ERROR 3

int main(int argc, char* argv[]) {
    decir_hola("una Interfaz de Entrada/Salida");

    int descriptor_archivo_conexion = 1;
    char* ip = NULL;
    char* puerto = NULL;

    t_config* config = iniciar_config_io();

    if (config != NULL) {
        ip = config_get_string_value(config, "IP_KERNEL");
        puerto = config_get_string_value(config, "PUERTO_KERNEL");

        descriptor_archivo_conexion = crear_conexion(ip, puerto);

        if (descriptor_archivo_conexion != -1) {
            enviar_mensaje("Hola Kernel, como va.", descriptor_archivo_conexion);
        } else {
            fprintf(stderr, "Error: No se pudo crear la conexión.\n");
        }

        terminar_programa(descriptor_archivo_conexion, config);
    } else {
        fprintf(stderr, "Error: No se pudo inicializar la configuración.\n");
    }

    return 0;
}

t_config* iniciar_config_io(void) {
    t_config* nueva_configuracion = config_create(ARCHIVO_CONFIG);

    if (nueva_configuracion == NULL) {
        perror("Error: No se pudo abrir el archivo de configuración");
    } else {
        printf("Archivo de configuración inicializado.\n");
    }

    return nueva_configuracion;
}

void terminar_programa(int descriptor_archivo_conexion, t_config* config) {
    // Cerrar la conexión del socket
    if (close(descriptor_archivo_conexion) == -1) {
        perror("Error: No se pudo cerrar la conexión del socket");
    }

    // Destruir la estructura de configuración
    config_destroy(config);
}
*/
