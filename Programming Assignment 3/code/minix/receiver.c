#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include "shared_memory.h"

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

int main(int argc, char *argv[]) {
    int *lock;
    char *mapped_memory, input[100];

    if(argv[1] == NULL) // 如果沒輸入參數, 則預設為0
    	argv[1] = 0;

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

    while(1) {
        while(shm->lock == 0){usleep(10);} // 等待接收訊息
    	Output("Process %s: %s\n", argv[1], shm->message); // 輸出共享記憶體中的訊息
    	usleep(80);
    	shm->lock = 0; // 設置鎖為0（鎖定狀態）
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
