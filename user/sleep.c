#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

// argc表示命令行参数数量 argv[] 表示命令行参数
int main(int argc,char* argv[]){
    // 如果命令行参数不为2 则打印错误信息
  if(argc != 2){
     fprintf(2,"sleep <arg>\n");
      exit(1);
    }
    // 将命令行参数转换为整数 赋值给time变量
    int time = atoi(argv[1]);
    if(sleep(time) < 0){
    // 睡眠值小于0也将打印错误
         fprintf(2,"sleep err\n");
       exit(1);
    }
    // 程序结束后 以状态码0退出
    exit(0);
 }