#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include "shared_memory.h"

void Putchar(char c) { // ��X
    putchar(c);
}

void Output(const char* format, ...) { // �ۻsprintf
    va_list args;
    va_start(args, format); // ��r���

    if (format) { // �P�_�O�_�� %d
        vprintf(format, args);
    } else {
        int value = va_arg(args, int); // �_�h���`��X
        char c = (char)value;
        Putchar(c);
    }

    va_end(args);
}

int main(int argc, char *argv[]) {
    int *lock;
    char *mapped_memory, input[100];

    if(argv[1] == NULL) // �p�G�S��J�Ѽ�, �h�w�]��0
    	argv[1] = 0;

    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666); // ����@�ɰO���骺���ҡA�p�G���s�b�h�ؤ@��
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    shared_memory_t *shm = (shared_memory_t *)shmat(shmid, NULL, 0); // �s����@�ɰO����
    if (shm == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    while(1) {
        while(shm->lock == 0){usleep(10);} // ���ݱ����T��
    	Output("Process %s: %s\n", argv[1], shm->message); // ��X�@�ɰO���餤���T��
    	usleep(80);
    	shm->lock = 0; // �]�m�ꬰ0�]��w���A�^
    }

    if (shmdt(shm) == -1) { // �����@�ɰO����
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) { // �R���@�ɰO����
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}
