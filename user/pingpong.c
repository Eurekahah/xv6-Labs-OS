#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[]) {
    char buf = 'X'; //待传送的字节

    //0读，1写
    int ch2Fa[2];//子进程到父进程
    int fa2Ch[2];//父进程到子进程

    pipe(ch2Fa);
    pipe(fa2Ch);

    //创建子进程
    int pid = fork();
    int exit_status = 0;

    if (pid < 0) //错误情况
    {
        printf("pid小于0!\n");
        close(ch2Fa[0]);
        close(ch2Fa[1]);
        close(fa2Ch[0]);
        close(fa2Ch[1]);
        exit(1);
    }
    else if(pid == 0) //子进程
    {
        close(fa2Ch[1]);
        close(ch2Fa[0]);
        if (read(fa2Ch[0], &buf, sizeof(char)) != sizeof(char)) {
            printf("child read() error!\n");
            exit_status = 1; //标记出错
        } else {
            printf("%d: received ping\n", getpid());
        }
        if (write(ch2Fa[1], &buf, sizeof(char)) != sizeof(char)) {
            printf("child write() error!\n");
            exit_status = 1; //标记出错
        }
        close(fa2Ch[0]);
        close(ch2Fa[1]);
        exit(exit_status);
    }
    else //父进程
    {
        close(fa2Ch[0]);
        close(ch2Fa[1]);
        if (write(fa2Ch[1], &buf, sizeof(char)) != sizeof(char)) {
            printf("parent write() error!\n");
            exit_status = 1; //标记写出错
        }
        if (read(ch2Fa[0], &buf, sizeof(char)) != sizeof(char)) {
            printf("parent read() error!\n");
            exit_status = 1; //标记出错
        } else {
            printf("%d: received pong\n", getpid());
        }
        close(fa2Ch[1]);
        close(ch2Fa[0]);
        exit(exit_status);
    }

}
