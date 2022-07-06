#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int is_digits_composed(char *s, int len);
int is_digit(char c);


/**
 * @brief
 * -p, --show-pids: 打印每个进程的进程号。
 * -n --numeric-sort: 按照pid的数值从小到大顺序输出一个进程的直接孩子。
 * -V --version: 打印版本信息。
 *
 * @param argc
 * @param argv
 * @return int
 */

int main(int argc, char *argv[])
{
  char p = 0; // 打印每个进程的进程号
  char n = 0; // 按照pid的大小输出
  char v = 0; // 打印版本信息
  char temp[261];
  memset(temp , 0 , 261);
  strcpy(temp , "/proc/");

  // ===================================命令行参数处理==========================================
  for (int i = 0; i < argc; i++)
  {
    assert(argv[i]);
    if (strcmp(argv[i], "-V") == 0 || strcmp(argv[i], "--version") == 0)
    {
      v = 1;
    }
    if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--numeric-sort") == 0)
    {
      n = 1;
    }
    if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--show-pids") == 0)
    {
      p = 1;
    }
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  printf("p = %d , n = %d , v = %d\n", p, n, v);
  assert(!argv[argc]);
  // ==================================读取/proc=====================================================
  DIR *proc = opendir("/proc");
  struct dirent *dr = readdir(proc);

  while (dr != NULL)
  {
    memset(temp , 0 , 261);
    strcpy(temp , "/proc/");
    printf("%s\n" ,strcat(temp , dr->d_name ) );
    if (is_digits_composed(dr->d_name, 256))
    {
      strcat(temp , "/stat");
      FILE *fp = fopen(temp, "r");
      pid_t pid;
      char comm[256];
      char state;
      pid_t ppid;
      memset(comm,0,256);
      // 获取进程号和父进程号
      if(fp){
        char buffer[1000];
        memset(buffer , 0 , 1000);
        fgets(buffer ,1000 , fp);
        char *token = strtok(buffer , " ");
        pid = atoi(token);
        token = strtok(NULL , " ");
        strcpy(comm,token);
        token = strtok(NULL , " ");
        token = strtok(NULL , " ");
        ppid = atoi(token);
      }else{
        fclose(fp);
        perror("File reading error \n");
        exit(1);
      }
      fclose(fp);
      printf("pid = %d , name = %s ,ppid = %d\n" , pid ,comm , ppid );
    }
    dr = readdir(proc);
  }
  closedir(proc);

  return 0;
}

int is_digits_composed(char *s, int len)
{
  int i;
  for (i = 0; i < len; i++)
  {
    if (!is_digit(s[i]))
    {
      if (s[i] == '\0')
      {
        return 1;
      }
      else
      {
        return 0;
      }
    }
  }
  return 1;
}
int is_digit(char c)
{
  if (c <= '9' && c >= '0')
  {
    return 1;
  }
  return 0;
}
