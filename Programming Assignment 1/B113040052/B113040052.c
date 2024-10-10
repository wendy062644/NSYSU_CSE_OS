#include<stdio.h> //input & output
#include<string.h> //string
#include<fcntl.h> //file
#include<unistd.h>
#include<sys/wait.h>

int main()
{
    while(1) // shell 執行
    {
    	char input, command[200][100], output_file_name[100], input_file_name[100];
    	/*
            input為自製getline的中間媒介
            command[a][b] 為紀錄指令, a 為指令數量, b 為單個指令的長度, 例如: char command[3][10] = {"ls", "-l", "&"}
            output_file_name, input_file_name 是當輸入 > 或 < 時, 會使用到
    	*/
    	char *args[10][20]; // 最後處理完command中的 > 或 < 或 & 後，將剩餘指令的記憶體位置開頭存入 args 中，以便執行 execvp
    	int back_ground = 0, output_file = 0, input_file = 0, error = 0, command_num = 0, input_num = 0, now = 0, l = 0, space = 0;
    	/*
            back_ground 紀錄是否在背景執行
            output_file, input_file 紀錄使否有使用 > 或 <
            now 紀錄當下指令位置
            l 紀錄是否使用 piped
    	*/
        memset(command, '\0', sizeof(command)); //初始化
        while(scanf("%c", &input) && input != '\n') { // 自製getline功能，讀取指令
             if(input == ' ') { // 以空格當作指令的切割點
                  space = 1;
                  l = 0;
             }
             else {
                  if(space) {
                       space = 0;
                       input_num++;
                  }
                  command[input_num][l++] = input;
             }
        }
        if(!strcmp(command[now], "exit")){ // 判斷是否結束
             return 0;
        } else if(!strcmp(command[now], "")) { // 未輸入指令，重新輸入
             continue;
        } else {
             int i[10] = {0};
             args[command_num][i[command_num]++] = command[now++];
             while(now <= input_num) // 判斷是否處理完指令
             {
             	  if(!strcmp(command[now], "&")){ // 判斷是否背景執行
             	       back_ground = 1;
             	       now++;
             	  } else if(!strcmp(command[now], ">")) { // 判斷是否輸出至檔案
             	       if(++now <= input_num) {
             	            strcpy(output_file_name, command[now++]); // 將後面的檔案名複製
             	       	    output_file = 1;
             	       } else {
             	            printf("syntax error near unexpected token `newline'\n");
             	            error = 1;
             	       	    break;
             	       }
             	  } else if(!strcmp(command[now], "<")) { // 判斷是否從檔案讀取參數
             	       if(++now <= input_num) {
             	            strcpy(input_file_name, command[now++]); // 將後面的檔案名複製
             	            input_file = 1;
             	       } else {
             	            printf("syntax error near unexpected token `newline'\n");
             	            error = 1;
             	       	    break;
             	       }
             	  } else if(!strcmp(command[now], "|")) { // 判斷使否pipe
             	       command_num++;
             	       args[command_num][i[command_num]++] = command[++now]; // 將後續的指令存入新的位置
             	       now++;
             	  } else {
                       args[command_num][i[command_num]] = command[now++]; // 將指令存入args
                       i[command_num]++;
                  }
             }
             if(error) { // 判斷錯誤
                  continue;
             } else {
	          int pipes[command_num+1][2]; // 紀錄指令的輸入輸出
	          for (int j = 0; j <= command_num; j++) {
                       if (pipe(pipes[j]) == -1) {
                            perror("pipe");
                       }
                  }
	          for(int j = 0; j <= command_num; j++) { // 將每個指令的最後面改成NULL, execvp 才可知道何時結束
	               args[j][i[j]] = NULL;
			  pid_t pid = fork(); // 建立 child process
			  if(pid < 0){
			      fprintf(stderr, "Fork Failed\n");
			  } else if(pid == 0){
			      if(input_file && j == command_num) {
				   int file_in = open(input_file_name, O_RDONLY); // 開啟檔案
				   if (file_in == -1) {
		            		perror("open");
		        	   }
		        	   dup2(file_in, STDIN_FILENO); // 將檔案內容當參數輸入
		        	   close(file_in); // 關閉檔案
			      } else if(j > 0) {
			           dup2(pipes[j - 1][0], STDIN_FILENO); // 將輸入的參數從 pipe 中拿出
                    	           close(pipes[j - 1][0]); // 清空內容
                                   close(pipes[j - 1][1]);
			      }
			      if(output_file&& j == command_num) {
				   int file_out = open(output_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666); // 開啟檔案
				   if (file_out == -1) {
		            		perror("open");
		        	   }
		        	   dup2(file_out, STDOUT_FILENO); // 將內容輸出至檔案
		        	   close(file_out); // 關閉檔案
			      } else if(j < command_num) {
			           dup2(pipes[j][1], STDOUT_FILENO); // 將輸出內容導入pipe內
                                   close(pipes[j][0]); // 清空內容
                                   close(pipes[j][1]);
			      }
			      execvp(args[j][0], args[j]); // 執行指令
			      perror("execvp");
			  } else {
			      if(!back_ground) { // 在背景執行
				   waitpid(pid, NULL, 0);
			      }
			  }
		  }
		  for(int j = 0; j <= command_num; j++) { // 清空內容
		       close(pipes[j][0]);
		       close(pipes[j][1]);
		  }
	     }
        }
    }
    return 0;
}
