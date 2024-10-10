#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdarg.h>
#include "shared_memory.h"

#define BUFFER_SIZE 100

char input_buffer[BUFFER_SIZE]; // 緩衝區
int buffer_index = 0; // buffer 的最後位置

void Putchar(char c) { // 輸出
    putchar(c);
}

void Output(const char* format, ...) { // 自製printf
    va_list args;
    va_start(args, format); // 文字拆解

    if (format) { // 判斷是否有 %d
        vprintf(format, args);
    } else {
        int value = va_arg(args, int); // 否則正常輸出
        char c = (char)value;
        Putchar(c);
    }

    va_end(args);
}

void Backspace() { // 刪除,減少buffer的index
    if (buffer_index > 0) {
        buffer_index--;
    }
}

void Getchar(char c) { // 讀取輸入
    if (c == '\n') { // 遇到換行, 代表指令結束
        input_buffer[buffer_index] = '\0';
        buffer_index = 0;
    } else if (c == '\b') { // 遇到backspace
        Backspace();
    } else {
        if (buffer_index < BUFFER_SIZE - 1) { // 判斷是否超出buffer大小
            input_buffer[buffer_index++] = c;
        }
    }
}

int main() {
    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666); // 獲取共享記憶體的標籤，如果不存在則建一個
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    shared_memory_t *shm = (shared_memory_t *)shmat(shmid, NULL, 0); // 連接到共享記憶體
    if (shm == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    shm->lock = 0; // 初始化共享記憶體中的鎖為0（未鎖定狀態）
    while(1) {
        while(shm->lock == 1) {usleep(60);} // 等待鎖被釋放
    	Output("Input something: ");
    	while (1) { // 等待輸入訊息
            char c = getchar();
            Getchar(c);
            if(c == '\n') {
            	break;
            }
    	}
    	strncpy(shm->message, input_buffer, sizeof(shm->message) - 1); // 複製訊息至共享記憶體中
    	usleep(80);
    	shm->lock = 1; // 設置鎖為1（鎖定狀態）
    }

    if (shmdt(shm) == -1) { // 分離共享記憶體
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) { // 刪除共享記憶體
        perror("shmctl");
        exit(EXIT_FAILURE);
    }
    return 0;
}
