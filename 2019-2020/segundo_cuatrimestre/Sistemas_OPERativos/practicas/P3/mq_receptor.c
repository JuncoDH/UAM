/**
 * @file mq_receptor.c
 * @author Junco de las Heras junco.heras@estudiante.uam.es Marta Vaquerizo marta.vaquerizo@estudiante.uam.es
 * Grupo: 2201
 * Fecha: 28-03-2020
 * Descripcion: Test para probar el mq_receive y las colas de mensajes
 * */

/* RECECPTOR */
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define MQ_NAME "/mq_example"
#define N 33

typedef struct {
    int valor;
    char aviso[80];
} Mensaje;

int main(void) {
    struct mq_attr attributes = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_curmsgs = 0,
        .mq_msgsize = sizeof(Mensaje)
    };

    /* This process is only going to send messages */
    mqd_t queue = mq_open(MQ_NAME,
        O_CREAT | O_RDONLY | O_NONBLOCK, /*NONBLOCKING QUEUE*/
        S_IRUSR | S_IWUSR, /* The user can read and write */
        &attributes);

    if(queue == (mqd_t)-1) {
        fprintf(stderr, "Error opening the queue\n");
        return EXIT_FAILURE;
    }

    Mensaje msg;

    if (mq_receive(queue, (char *)&msg, sizeof(msg), NULL) == -1) {
        fprintf(stderr, "Error receiving message\n");
        return EXIT_FAILURE;
    }

    printf("%d: %s\n", msg.valor, msg.aviso);

    /* Wait for input to end the program */
    fprintf(stdout, "Press any key to finish\n");
    getchar();

    mq_close(queue);

    return EXIT_SUCCESS;
}
