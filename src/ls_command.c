#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>


#include "ls_command.h" // ls_command.h에 정의된 함수를 사용하기 위해 include



void my_ls(void) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if(dir == NULL) {
        perror("opendir");
        return;
    } else {
        while((entry = readdir(dir)) != NULL) {
            if(entry->d_type == DT_DIR) {
                printf("%s/\n",entry->d_name);   //struct구조체안에 있는 de_name을 출력(디렉토리인지 알아보기 위해)
            }
            printf("%s\n",entry->d_name);   //struct구조체안에 있는 de_name을 출력
        }
    }
    closedir(dir);
}