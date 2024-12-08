#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // chdir, getcwd, access, fork, execvp
#include <sys/wait.h> // wait

#include "ls_command.h"

#define MAX_LINE 80
#define MAX_ARGS 10

void execute_cat(char **argv) {
    FILE *file;
    char buffer[256];

    if (argv[1] == NULL) {
        printf("cat: Missing file operand\n");
        return;
    }

    for (int i = 1; argv[i] != NULL; i++) {
        file = fopen(argv[i], "r");
        if (file == NULL) {
            printf("cat: %s: No such file or directory\n", argv[i]);
            continue;
        }

        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("%s", buffer);
        }
        fclose(file);
    }
}

int main() {

    char input[MAX_LINE];
    char *token;
    char *argv[MAX_ARGS];

    while(1) {

        //prompt 출력
        printf("myshell> ");

        //read command
        if(fgets(input,MAX_LINE,stdin)==NULL) {
            break;
        };
        // printf("%s\n",input);

        //tokenize input
        token = strtok(input," \n\t");
        int i = 0;

        while(token != NULL) {
            argv[i++] = token;
            token = strtok(NULL," \n\t");
        }
        argv[i] = NULL;

        if(argv[0] == NULL) {   //empty command
            continue;
        }

        if (strcmp(argv[0], "exit") == 0) {
            printf("Goodbye!\n");
            exit(0);
        } else if(strcmp(argv[0],"cd") == 0) {
            if(argv[1] == NULL){
                printf("cd: Missing argument\n");
            }
            else if(chdir(argv[1]) !=0){
                perror("cd");
            } //change directory

        } else if(strcmp(argv[0], "pwd") == 0) {
            if(getcwd(input,MAX_LINE) == NULL) {
                perror("pwd");
            }
            else {
                printf("%s\n",input);
            }
        } else if(strcmp(argv[0], "ls") == 0) {
            my_ls();
        } else if(strcmp(argv[0], "cat") == 0) {
            execute_cat(argv);
        } else {
             
            //실행파일이거나 명령어 잘못 입력했거나 했을 경우에도 처리 해야 함
            if(access(argv[0], X_OK)==0) {
                printf("execute %s\n",argv[0]); //실행파일일 경우
                if(fork() == 0) {
                    execvp(argv[0],argv);
                    perror("execvp");
                    exit(1);
                } else {
                    wait(NULL);
                }
            } 
            else {
                printf("command not found: %s\n", argv[0]);  //명령어가 없을 경우
            }
        }
    }

    return 0;
}