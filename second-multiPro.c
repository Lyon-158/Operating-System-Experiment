#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

#define MAX_LINE 80
#define MAX_COM_NUMBER 100

char *history[MAX_COM_NUMBER];
int index_his;

int main()
{
    char *args[MAX_LINE/2 + 1];
    int should_run = 1;
    pid_t pid;
    char command[MAX_LINE];
    char flag;
    char *close_command = "close command";

    // 历史指令赋初值
    index_his = 1; // 指向下一条指令
    for(int i = 0;i < MAX_COM_NUMBER;++i){
        history[i] = NULL;
    }

    // shell 程序主体
    while(should_run){
        printf("osh>");
        fflush(stdout);
        gets(command);

        // 关闭 shell 判断 —— "close command"
        if(strcmp(command, close_command) == 0){
            printf("successfully close the osh.\n");
            return 0;
        }

        // 历史 !! 判断
        if(strcmp(command, "!!") == 0){
            if(index_his == 1){
                printf("No commands in the history.\n");
                continue;
            }
            // printf("The command = history: %s\n", command);
            // printf("The command = history: %s\n", history[index_his - 1]);
            strcpy(command, history[index_his - 1]);
            // printf("The command = history: %s\n", command);
        }

        // 历史 !N 判断
        if(command[0] == '!' && (command[1] > '0' && command[1] < '9')){
            char number_command[MAX_LINE] = {'\0'};
            int j = 0;
            while(command[j+1] != '\0'){
                number_command[j] = command[j+1];
                ++j;
            }
            number_command[j] = '\0';

            int number = atoi(number_command);

            if(number >= index_his){
                printf("No such command in history.\n");
                continue;
            }
            strcpy(command, history[number]);
        }
        
        // 指令加入历史记录
        history[index_his] = (char*)malloc(sizeof(char)*(strlen(command)+1));
        strcpy(history[index_his], command);
        // history[index_his] = command;
        // printf("The %dst add %s \n", index_his, command);
        ++index_his;
        
        // flag 代表指令的最后一位，&或者其他
        flag = command[strlen(command)-1];

        // 指令切片存放到数组里面
        char *token;
        token = strtok(command, " ");
        int i = 0;
        while(token != NULL && strcmp(token, "&") != 0){
            args[i] = token;
            token = strtok(NULL, " ");
            ++i;
        }
        args[i] = NULL;

        // fork() a child process
        pid = fork();
        
        // deal the process
        if(pid < 0){ // 错误
            printf("error fork().\n");
        }
        else if(pid == 0){ // 子进程
            // 使用命令
            // system(command);
            execvp(args[0], args);
            exit(0);
        }
        else{ // 父进程
            // invoke wait() if the command contains &
            if(flag != '&'){
                waitpid(pid, NULL, 0);
            }
        }
    }

    return 0;
}