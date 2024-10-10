#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define MAIL_FILE_NAME "mailbox.bin"
#define LOCK_FILE_NAME "lock.bin"
#define BUFFER_SIZE 100

char input_buffer[BUFFER_SIZE];
int buffer_index = 0;

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

void Backspace() {
    if (buffer_index > 0) {
        buffer_index--;
    }
}

void Getchar(char c) {
    if (c == '\n') {
        input_buffer[buffer_index] = '\0';
        buffer_index = 0;
    } else if (c == '\b') {
        Backspace();
    } else {
        if (buffer_index < BUFFER_SIZE - 1) {
            input_buffer[buffer_index++] = c;
        }
    }
}

int main() {
    int lock_fd, mail_fd, *lock;
    char *mapped_memory;

    // 打開二進制檔案
    mail_fd = open(MAIL_FILE_NAME, O_CREAT | O_RDWR, 0666);
    if (mail_fd == -1) {
        perror("open");
        exit(1);
    }

    ftruncate(mail_fd, sizeof(char) * 256);

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

    ftruncate(lock_fd, sizeof(int));

    // 映射二進制檔案到進程的地址空間
    lock = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, lock_fd, 0);
    if (lock == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    *lock = 0;

    while(1) {
        while(*lock == 1) {usleep(60);}
    	Output("Input something: ");
    	while (1) {
            char c = getchar();
            Getchar(c);
            if(c == '\n') {
            	break;
            }
    	}
    	sprintf(mapped_memory, "%s", input_buffer);
    	usleep(80);
    	*lock = 1;
    }

    // 解除映射並關閉檔案
    close(mail_fd);
    close(lock_fd);
    munmap(mapped_memory, sizeof(char) * 256);
    munmap(lock, sizeof(int));
    return 0;
}
