#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define NUM_TESTS 10

int copy_file(const char *src, const char *dest);

void print(){
    int pid = fork();
    if(pid == 0) {
    	 freopen("top.txt", "w", stdout);
         execlp("top", "top", NULL);
    }
}

int main() {    
    for (int i = 1; i <= NUM_TESTS; ++i) {
        char src_file[20], dest_file[20];
        sprintf(src_file, "test");
        sprintf(dest_file, "test%d", i);
        int ret = copy_file(src_file, dest_file);
        if (ret != 0) {
            fprintf(stderr, "Failed to copy %s\n", src_file);
            exit(EXIT_FAILURE);
        }
        if (chmod(dest_file, S_IRUSR | S_IWUSR | S_IXUSR) == -1) {
            perror("chmod");
            exit(EXIT_FAILURE);
        }
    }

    char program[20];
    print();
    
    for (int i = 1; i <= NUM_TESTS; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            sprintf(program, "./test%d", i);
            execlp(program, program, NULL);
            perror(program);
            exit(EXIT_FAILURE);
        } else {
            printf("test%d running.\n", i);
        }
    }

    for (int i = 1; i <= NUM_TESTS; ++i) {
        wait(NULL);
    }

    for (int i = 1; i <= NUM_TESTS; ++i) {
        char filename[20];
        sprintf(filename, "test%d", i);
        if (unlink(filename) == -1) {
            perror("unlink");
            exit(EXIT_FAILURE);
        }
    }

    printf("All tests completed and test files removed.\n");

    return 0;
}

int copy_file(const char *src, const char *dest) {
    int fd_src, fd_dest;
    ssize_t bytes_read;
    char buffer[4096];

    fd_src = open(src, O_RDONLY);
    if (fd_src == -1) {
        perror("open src");
        return -1;
    }

    fd_dest = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dest == -1) {
        perror("open dest");
        close(fd_src);
        return -1;
    }

    while ((bytes_read = read(fd_src, buffer, sizeof(buffer))) > 0) {
        if (write(fd_dest, buffer, bytes_read) != bytes_read) {
            perror("write");
            close(fd_src);
            close(fd_dest);
            return -1;
        }
    }

    if (bytes_read == -1) {
        perror("read");
        close(fd_src);
        close(fd_dest);
        return -1;
    }

    close(fd_src);
    close(fd_dest);
    return 0;
}