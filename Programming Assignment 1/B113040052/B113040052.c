#include<stdio.h> //input & output
#include<string.h> //string
#include<fcntl.h> //file
#include<unistd.h>
#include<sys/wait.h>

int main()
{
    while(1) // shell ����
    {
    	char input, command[200][100], output_file_name[100], input_file_name[100];
    	/*
            input���ۻsgetline�������C��
            command[a][b] ���������O, a �����O�ƶq, b ����ӫ��O������, �Ҧp: char command[3][10] = {"ls", "-l", "&"}
            output_file_name, input_file_name �O���J > �� < ��, �|�ϥΨ�
    	*/
    	char *args[10][20]; // �̫�B�z��command���� > �� < �� & ��A�N�Ѿl���O���O�����m�}�Y�s�J args ���A�H�K���� execvp
    	int back_ground = 0, output_file = 0, input_file = 0, error = 0, command_num = 0, input_num = 0, now = 0, l = 0, space = 0;
    	/*
            back_ground �����O�_�b�I������
            output_file, input_file �����ϧ_���ϥ� > �� <
            now ������U���O��m
            l �����O�_�ϥ� piped
    	*/
        memset(command, '\0', sizeof(command)); //��l��
        while(scanf("%c", &input) && input != '\n') { // �ۻsgetline�\��AŪ�����O
             if(input == ' ') { // �H�Ů��@���O�������I
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
        if(!strcmp(command[now], "exit")){ // �P�_�O�_����
             return 0;
        } else if(!strcmp(command[now], "")) { // ����J���O�A���s��J
             continue;
        } else {
             int i[10] = {0};
             args[command_num][i[command_num]++] = command[now++];
             while(now <= input_num) // �P�_�O�_�B�z�����O
             {
             	  if(!strcmp(command[now], "&")){ // �P�_�O�_�I������
             	       back_ground = 1;
             	       now++;
             	  } else if(!strcmp(command[now], ">")) { // �P�_�O�_��X���ɮ�
             	       if(++now <= input_num) {
             	            strcpy(output_file_name, command[now++]); // �N�᭱���ɮצW�ƻs
             	       	    output_file = 1;
             	       } else {
             	            printf("syntax error near unexpected token `newline'\n");
             	            error = 1;
             	       	    break;
             	       }
             	  } else if(!strcmp(command[now], "<")) { // �P�_�O�_�q�ɮ�Ū���Ѽ�
             	       if(++now <= input_num) {
             	            strcpy(input_file_name, command[now++]); // �N�᭱���ɮצW�ƻs
             	            input_file = 1;
             	       } else {
             	            printf("syntax error near unexpected token `newline'\n");
             	            error = 1;
             	       	    break;
             	       }
             	  } else if(!strcmp(command[now], "|")) { // �P�_�ϧ_pipe
             	       command_num++;
             	       args[command_num][i[command_num]++] = command[++now]; // �N���򪺫��O�s�J�s����m
             	       now++;
             	  } else {
                       args[command_num][i[command_num]] = command[now++]; // �N���O�s�Jargs
                       i[command_num]++;
                  }
             }
             if(error) { // �P�_���~
                  continue;
             } else {
	          int pipes[command_num+1][2]; // �������O����J��X
	          for (int j = 0; j <= command_num; j++) {
                       if (pipe(pipes[j]) == -1) {
                            perror("pipe");
                       }
                  }
	          for(int j = 0; j <= command_num; j++) { // �N�C�ӫ��O���̫᭱�令NULL, execvp �~�i���D��ɵ���
	               args[j][i[j]] = NULL;
			  pid_t pid = fork(); // �إ� child process
			  if(pid < 0){
			      fprintf(stderr, "Fork Failed\n");
			  } else if(pid == 0){
			      if(input_file && j == command_num) {
				   int file_in = open(input_file_name, O_RDONLY); // �}���ɮ�
				   if (file_in == -1) {
		            		perror("open");
		        	   }
		        	   dup2(file_in, STDIN_FILENO); // �N�ɮפ��e��Ѽƿ�J
		        	   close(file_in); // �����ɮ�
			      } else if(j > 0) {
			           dup2(pipes[j - 1][0], STDIN_FILENO); // �N��J���ѼƱq pipe �����X
                    	           close(pipes[j - 1][0]); // �M�Ť��e
                                   close(pipes[j - 1][1]);
			      }
			      if(output_file&& j == command_num) {
				   int file_out = open(output_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666); // �}���ɮ�
				   if (file_out == -1) {
		            		perror("open");
		        	   }
		        	   dup2(file_out, STDOUT_FILENO); // �N���e��X���ɮ�
		        	   close(file_out); // �����ɮ�
			      } else if(j < command_num) {
			           dup2(pipes[j][1], STDOUT_FILENO); // �N��X���e�ɤJpipe��
                                   close(pipes[j][0]); // �M�Ť��e
                                   close(pipes[j][1]);
			      }
			      execvp(args[j][0], args[j]); // ������O
			      perror("execvp");
			  } else {
			      if(!back_ground) { // �b�I������
				   waitpid(pid, NULL, 0);
			      }
			  }
		  }
		  for(int j = 0; j <= command_num; j++) { // �M�Ť��e
		       close(pipes[j][0]);
		       close(pipes[j][1]);
		  }
	     }
        }
    }
    return 0;
}
