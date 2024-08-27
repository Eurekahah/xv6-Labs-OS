#include "kernel/types.h"
#include "user/user.h"

void transmitData(int lpipe[2],int rpipe[2], int first)
{
    int data;
    while(read(lpipe[0],&data,sizeof(int))==sizeof(int))
    {
        if(data%first)
        {
            write(rpipe[1],&data,sizeof(int));
        }
    }
    close(lpipe[0]);
    close(rpipe[1]);
}

void primes(int lpipe[2])
{
    close(lpipe[1]);//左管道不需要写入，关闭，以免浪费导致崩溃
    int first;
    if(read(lpipe[0],&first,sizeof(int))!= sizeof (int))
    {
        exit(0);//递归退出点，在管道中没有数据后退出
    }
    printf("prime %d\n", first);

    int p[2];//新的管道来存放筛选过后的数据，即右管道
    pipe(p);

    transmitData(lpipe,p,first);//将左管道的数据筛选后写入右管道

    if(fork() == 0)
    {
        primes(p);//递归出子进程
    } else{
        close(p[0]);//关闭无用的管道端口
        wait(0);//同样需要等待子进程
    }
    exit(0);
}

int main(int argc,char const *argv[])
{
    int p[2];
    pipe(p);

    for(int i=2;i<=35;i++){
        write(p[1],&i,sizeof(int)); //向管道写入数据，实际上写入的是缓冲区，保证了程序正常运行
    }

    if(fork()==0){//子进程：fork之后生成副本，从fork处父子进程并行，子进程fork返回0，父进程返回pid
        primes(p);//进入递归
    } else{
        close(p[1]);
        close(p[0]);
        wait(0);//父进程等待，以免出现僵尸进程
    }
    exit(0);
}