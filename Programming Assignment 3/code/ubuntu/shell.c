#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define MAX_ARGS 10
#define MEMORY_SIZE 1024
#define MAX_COMMAND_LENGTH 100
#define BUFFER_SIZE 100

char input_buffer[BUFFER_SIZE], num[2];
int buffer_index = 0, Process_num = 0;

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

int ProcessFork() {
    pid_t pid = fork(); // 調用 fork 函數創建一個新的子進程
    if (pid == -1) {
        return -2; // 返回錯誤碼表示 fork 失敗
    } else if (pid == 0) {
        // 子Process
        //printf("Child process created with PID: %d\n", getpid());
    } else {
        return pid; // 返回子Process的 PID
    }
    return pid;
}

int Waitpid(pid_t pid) {
    int status;
    pid_t ret_pid = waitpid(pid, &status, 0); // 等待子Process
    if (ret_pid == -1) {
        //perror("waitpid");
        return -1; // 錯誤
    }
    if (WIFEXITED(status)) {
        Output("Child process %d exited with status %d\n", ret_pid, WEXITSTATUS(status));
    } else {
        Output("Child process %d did not exit normally\n", ret_pid);
    }
    return ret_pid; // 返回子Process的ID
}

int Spawn(char *prog, char *arg[], int background) {
    int result = ProcessFork(); // 調用 ProcessFork 函數創建一個新的Process
    if (result < 0) {
        return result; // 如果創建失敗，直接返回錯誤碼
    } else if(result == 0) {
    	execvp(prog, arg);
    	//perror("execvp");
    }
    if(!background)
    	return Waitpid(result);
    usleep(50);
    return result;
}

void parse_command(char *command, char *args[], int *background) {
    char *token;
    int i = 0;
    *background = 0;

    // 使用空格作為分隔符解析指令
    token = strtok(command, " ");
    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            *background = 1; // 標記後台執行
        } else {
            args[i++] = token; // 將每個參數添加到 args 數組中
        }
        token = strtok(NULL, " ");
    }

    if(args[0][0] == '.' && args[0][1] == '/') {
    	sprintf(num, "%d", Process_num++);
    	args[i++] = num;
    }

    args[i] = NULL; // 最後一個參數為 NULL，用於 execvp 函數
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
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];
    int background;

    while (1) {
        Output("Shell > ");
        while (1) {
            char c = getchar();
            Getchar(c);
            if(c == '\n') {
            	break;
            }
    	}

        Output("run: %s\n", input_buffer);

        // 如果用戶輸入 exit 則退出 shell
        if (strcmp(input_buffer, "exit") == 0) {
            break;
        }

        // 解析用戶輸入的指令
        parse_command(input_buffer, args, &background);
        // 創建新進程執行指定程序
        int pid = Spawn(args[0], args, background);
        if (pid < 0) {
            Output("Error spawning process for command: %s\n", command);
        }
    }

    return 0;
}
