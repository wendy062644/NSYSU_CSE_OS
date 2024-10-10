#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdarg.h>
#include "shared_memory.h"

#define BUFFER_SIZE 100

char input_buffer[BUFFER_SIZE]; // �w�İ�
int buffer_index = 0; // buffer ���̫��m

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

void Backspace() { // �R��,���buffer��index
    if (buffer_index > 0) {
        buffer_index--;
    }
}

void Getchar(char c) { // Ū����J
    if (c == '\n') { // �J�촫��, �N����O����
        input_buffer[buffer_index] = '\0';
        buffer_index = 0;
    } else if (c == '\b') { // �J��backspace
        Backspace();
    } else {
        if (buffer_index < BUFFER_SIZE - 1) { // �P�_�O�_�W�Xbuffer�j�p
            input_buffer[buffer_index++] = c;
        }
    }
}

int main() {
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
    shm->lock = 0; // ��l�Ʀ@�ɰO���餤���ꬰ0�]����w���A�^
    while(1) {
        while(shm->lock == 1) {usleep(60);} // ������Q����
    	Output("Input something: ");
    	while (1) { // ���ݿ�J�T��
            char c = getchar();
            Getchar(c);
            if(c == '\n') {
            	break;
            }
    	}
    	strncpy(shm->message, input_buffer, sizeof(shm->message) - 1); // �ƻs�T���ܦ@�ɰO���餤
    	usleep(80);
    	shm->lock = 1; // �]�m�ꬰ1�]��w���A�^
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
