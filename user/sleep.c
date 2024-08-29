#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[]) {
    if (argc != 2) { //判断参数数量
        printf("error! usage: sleep <time>\n");
        exit(1);
    }
    sleep(atoi(argv[1]));//系统调用sleep函数
    exit(0);
}
