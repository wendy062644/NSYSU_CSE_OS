#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define MAIL_FILE_NAME "mailbox.bin"
#define LOCK_FILE_NAME "lock.bin"

void Putchar(char c) {
    putchar(c);
}

void Output(const char* format, ...) {
    va_list args;
    va_start(args, format);

    if (format) {
        vprintf(format, args);
    } else {
        int value = va_arg(args, int);
        char c = (char)value;
        Putchar(c);
    }

    va_end(args);
}

int main(int argc, char *argv[]) {
    int lock_fd, mail_fd, *lock;
    char *mapped_memory, input[100];

    if(argv[1] == NULL)
    	argv[1] = 0;

    // 打開二進制檔案
    mail_fd = open(MAIL_FILE_NAME, O_CREAT | O_RDWR, 0666);
    if (mail_fd == -1) {
        perror("open");
        exit(1);
    }

    // 映射二進制檔案到進程的地址空間
    mapped_memory = mmap(NULL, sizeof(char) * 256, PROT_READ | PROT_WRITE, MAP_SHARED, mail_fd, 0);
    if (mapped_memory == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    lock_fd = open(LOCK_FILE_NAME, O_CREAT | O_RDWR, 0666);
    if (lock_fd == -1) {
        perror("open");
        exit(1);
    }

    // 映射二進制檔案到進程的地址空間
    lock = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, lock_fd, 0);
    if (lock == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }



    while(1) {
        while(*lock == 0){usleep(10);}
    	Output("Process %s: %s\n", argv[1], mapped_memory);
    	usleep(80);
    	*lock = 0;
    }

    close(mail_fd);
    close(lock_fd);

    return 0;
}
