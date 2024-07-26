#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char* argv[]) {
    char buf[12];  // 缓冲区用于存储从标准输入读取的行
    int i = 2;     
    if (argc < 3) {
        fprintf(2, "xargs <function> <args>\n");
        exit(1);
    }
    
    // 创建一个新的参数数组，用于存储要传递给 exec 的参数
    char* new_argv[MAXARG];
    new_argv[0] = argv[1];  
    
    // 将命令行参数（除了前两个参数）复制到 new_argv 中
    while (argv[i] != 0) {
        new_argv[i - 1] = argv[i];
        if (i > MAXARG - 1) {
            fprintf(2, "too many args\n");
            exit(1);
        }
        i++;
    }
    new_argv[i] = 0;  // 设置参数数组的结束标志

    char c, *p = buf;
    // 从标准输入读取字符直到 EOF
    while (read(0, &c, 1) != 0) {
        if (c == '\n') {
            // 如果读取到换行符，将 buf 末尾添加终止符 '\0'
            *p = '\0';
            new_argv[i - 1] = buf;  // 将 buf 作为最后一个参数传递给 exec
            if (fork() == 0) {
                // 子进程执行命令
                exec(new_argv[0], new_argv);
                exit(0);  
            }
            wait(0);  // 等待子进程完成
            p = buf;  // 重置缓冲区指针
        } else {
            *p++ = c;  // 将读取到的字符存储到 buf 中
        }
    }
    
    exit(0);  // 程序正常退出
}
