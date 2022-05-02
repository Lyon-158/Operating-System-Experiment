#include<stdio.h>
#include<cstdlib>
#include<sys/types.h>
#include<sys/stat.h>
#include<limits.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#define MAX_FILE_LEN 1000
#define MAX_FILE_SIZE 1024

int main(int argc, char *argv[]){
    char *src_path = argv[1];    //源文件路径
    char *dst_path = argv[2];   //目标文件路径
    struct stat s_buf;

    if(argc != 3){
        printf("请输入两个代表源文件路径和目标文件路径的参数\n");
        exit(1);
    }

    /*将获取的文件信息放到 s_buf 中*/
    stat(src_path, &s_buf);

    /* 判断源路径是否是目录，若是，则退出*/
    if(S_ISDIR(s_buf.st_mode)){
        printf("不支持目录\n");
        exit(1);
    }      


    /*打开文件*/
    char src[MAX_FILE_LEN], dst[MAX_FILE_LEN];
    realpath(src_path, src);
    realpath(dst_path, dst);

    stat(dst_path, &s_buf);
    if(S_ISDIR(s_buf.st_mode)){
        strcat(dst, "/");
        strcat(dst, src_path);
    }

    /*打开源文件*/
    int f_src = open(src, O_RDONLY);
    if(f_src == -1){
        printf("文件打开失败\n");
        exit(1);
    }

    /*打开目标文件*/
    int f_dst = open(dst, O_CREAT | O_WRONLY, s_buf.st_mode);
    if(f_dst == -1){
        printf("创建文件失败\n");
        exit(1);
    }

    char buf[MAX_FILE_SIZE];
    while(1){
        int i = read(f_src, buf, MAX_FILE_SIZE);
        if(i == 0){
            break;
        }
        write(f_dst, buf, i);
    }

    close(f_src);
    close(f_dst);

    return 0;
}
