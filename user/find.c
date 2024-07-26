#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path , char *target)
{
    char buf[512], *p;
    int fd;
    // 定义目录结构体和文件描述符结构体
    struct dirent de;
    struct stat st;

    // 首先打开目录文件
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // 根据文件类型查找
    switch(st.type)
    {
        
        case T_FILE:
            // 定位到path的最后 和target进行比较
            if(strcmp( path + strlen(path) - strlen(target) , target ) ==0)
            {
                 printf("%s\n", path);
            }
            break;

        case T_DIR:
            // 设置是否能放进缓冲区
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            // 构建路径
            p = buf+strlen(buf);
            *p++ = '/';

            while(read(fd, &de, sizeof(de)) == sizeof(de))
            {
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0)
                {
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }

                // 忽略'.'和'..'的文件夹
                if(strcmp(".", de.name) != 0 && strcmp("..", de.name) != 0)
                {
                    find(buf, target);
                }
            }
            break;
  }
  close(fd);

}


int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("input arguments : find <path> <file name>\n");
        exit(1);
    }
   
    find(argv[1], argv[2]);
    exit(0);
}
