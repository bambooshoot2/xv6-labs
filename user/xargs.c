#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char* argv[]) {
   if(argc<2){
        fprintf(2,"Error:too less paremeters");
        exit(1);
   }

    int i;
    int pNumber=0; // 参数数量
    char* p[MAXARG];    // 参数数组
    // 从第二个参数开始 拷贝到p数组中
    for (i=1;i<argc;++i){
        p[pNumber++]=argv[i];
    }

    int place = pNumber;
    char ch;
    char *line;
    char s[512];
    line=s;// line为指向缓冲区的指针
    int index=0;    // 当前字符索引
    // 读取标准输入中的参数
    while(read(0,&ch,1)>0){
        if(ch=='\n'){
            // 读到换行符则将参数加入命令中
            line[index]='\0';
            index=0;
            p[pNumber++]=line;
            p[pNumber]=0;
            // 创建子进程
            if(fork()){
                wait(0);
                pNumber=place;
            }
            else{
                // 子进程执行命令
                exec(argv[1],p);
            }
        }
        else if (ch==' ')
        { 
            // 空格表示当前参数结束
            line[index]='\0'; 
            index=0; 
            p[pNumber++]=line; 
            char s[512]; 
            line=s;
        }
        else{
            // 加入到缓冲区中
            line[index++]=ch;
        }
        
    }
    exit(0);
}
