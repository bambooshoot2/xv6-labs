#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char* argv[]){
	int pid,pl[2];
    // 创建管道
	if(pipe(pl) < 0){
		fprintf(2,"pipe error\n");
		exit(1);
	}
    // 创建子进程
	if((pid = fork()) < 0){
		fprintf(2,"fork error\n");
		exit(1);
	}
    
	else if(pid == 0){
		close(pl[1]);// 关闭写端
		int prime = 0,n,pr[2];
		int first = 1;
		while(read(pl[0],&n,1) != 0){
			if(prime == 0){
				prime = n;//第一个数
				printf("prime %d\n",prime);
			}
			else if(first){ // 第二个数
				first = 0;
                // 创建新管道
				pipe(pr);
                // 创建新的子进程
				if(fork() == 0){
                    // 重新设置素数和非素数
					first = 1;
					prime = 0;
					pl[0] = pr[0]; // 重设管道
					close(pr[1]);
				}
				else
					close(pr[0]);
			}
			if(prime != 0 && n % prime != 0){ // 写入新管道
				write(pr[1],&n,1);
			}
		}
		close(pr[1]);// 关闭管道写端
		close(pl[0]); // 关闭管道读取端
		wait(0); // 等到子进程结束
		exit(0);
	}
    // 父进程 写入初始数据
	close(pl[0]);
	for(int i = 2 ;i <= 35 ;i++){
		write(pl[1],&i,1);
	}
	close(pl[1]);
	wait(0);
	exit(0);
}
