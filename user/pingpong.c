#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ 0
#define WRITE 1


int main(int argc, char *argv[])
{
    if(argc != 1) printf("dont input arguments\n");

    // 两个管道用于父进程和子进程通信
    int pipeParToChr[2];
    int pipeChrToPar[2];
    char buf[8];

    // 创建
    pipe(pipeParToChr);
    pipe(pipeChrToPar);

    // 创建
    int pid = fork();
    if( pid == 0 ) // 子进程
    {
        // 关闭父到子的写入端
        close(pipeParToChr[WRITE]);
        read(pipeParToChr[READ] , buf , sizeof(buf));
        close(pipeParToChr[READ]); // 关闭读取端

        close(pipeChrToPar[READ]);
        write(pipeChrToPar[WRITE],"pong\n",5);
        close(pipeChrToPar[WRITE]);

        // 打印接收数据 退出
        printf("%d: received %s",getpid(),buf);
        exit(0);

    }else  // 父进程
    {
        close(pipeParToChr[READ]);
        write(pipeParToChr[WRITE] , "ping\n" , 5);
        close(pipeParToChr[WRITE]);

        close(pipeChrToPar[WRITE]);
        read(pipeChrToPar[READ],buf,sizeof(buf));
        close(pipeChrToPar[READ]);

        wait((int*)0); // 等待子进程退出
        printf("%d: received %s",getpid(),buf);  // 打印从子进程接收的数据
        exit(0); // 父进程退出
    }
}

