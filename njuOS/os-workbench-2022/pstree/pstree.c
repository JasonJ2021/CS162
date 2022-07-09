#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>





// 变量
static int p = 0; // 打印每个进程的进程号
static int n = 0; // 按照pid的大小输出
static int v = 0; // 打印版本信息
static struct _proc *list = NULL;

typedef struct _proc
{
  // 进程的结构定义
  pid_t pid;               //进程的Pid
  pid_t ppid;              //父进程的pid
  char comm[64];           // 进程的名字
  struct _proc *parent;    // 父进程
  struct _proc *next;      // 用一个链表表示所有进程
  struct _child *children; // 用一个链表表示进程的所有子进程
} PROC;

typedef struct _child
{
  /* data */
  struct _proc *child;
  struct _child *next;
} CHILD;


// 函数
int is_digits_composed(char *s, int len);
int is_digit(char c);
void truncate_right_bracket(char *s);
PROC *find_proc(pid_t pid);
PROC *new_proc(char *comm, pid_t pid, pid_t ppid);
void add_child(PROC *p, PROC *c);


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
  char temp[261];
  memset(temp, 0, 261);
  strcpy(temp, "/proc/");

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

  // 遍历一个文件夹
  while (dr != NULL)
  {
    memset(temp, 0, 261);
    strcpy(temp, "/proc/");
    // printf("%s\n", strcat(temp, dr->d_name));
    if (is_digits_composed(dr->d_name, 256))
    {
      strcat(temp, "/stat");
      FILE *fp = fopen(temp, "r");
      pid_t pid;
      char comm[66];
      char state;
      pid_t ppid;
      memset(comm, 0, 66);
      // 获取进程号和父进程号
      if (fp)
      {
        char buffer[1000];
        memset(buffer, 0, 1000);
        fgets(buffer, 1000, fp);
        char *token = strtok(buffer, " ");
        pid = atoi(token);
        token = strtok(NULL, " ");
        // 获取进程名称
        strcpy(comm, token);
        token = strtok(NULL, " ");
        token = strtok(NULL, " ");
        // 获取父进程号
        ppid = atoi(token);
      }
      else
      {
        fclose(fp);
        perror("File reading error \n");
        exit(1);
      }
      fclose(fp);
      truncate_right_bracket(comm); // 除去最后一个)
      printf("comn = %s\n" , comm + 1);
      PROC* temp = new_proc(comm + 1 , pid, ppid);
    }
    dr = readdir(proc);
  }
  closedir(proc);
  PROC *walk = list;
  while(walk){
    printf("pid = %d , ppid = %d , comm = %s\n" , walk->pid , walk->ppid , walk->comm );
    walk = walk->next;
  }

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

// 用于删除一个字符串中的)
void truncate_right_bracket(char *s)
{
  int i = 0;
  while (s[i] != '\0' && s[i] != ')')
  {
    i++;
  }
  s[i] = '\0';
}

// 寻找一个进程号为pid的proc
PROC *find_proc(pid_t pid)
{
  PROC *temp = list;
  while (temp)
  {
    if (temp->pid == pid)
    {
      return temp;
    }
    temp = temp->next;
  }
  return NULL;
}

// 创建一个新的proc 添加到list中,然后返回相应的指针
PROC *new_proc(char *comm, pid_t pid, pid_t ppid)
{
  PROC *new = (PROC *)malloc(sizeof(PROC));
  memset(new, 0, sizeof(PROC));
  strncpy(new->comm, comm, 64);
  new->pid = pid;
  new->ppid = ppid;
  new->next = list;
  list = new;
  return new;
}

/**
 * @brief 把子进程加入到父进程的children中
 *
 * @param p 父进程
 * @param c 子进程
 */
void add_child(PROC *p, PROC *c)
{
  CHILD *new_child = (CHILD *)malloc(sizeof(CHILD));
  memset(new_child, 0, sizeof(CHILD));
  new_child->child = c;
  CHILD *cur = p->children;
  CHILD *prev = NULL;
  if (p->children == NULL)
  {
    // 当前还没有children
    p->children = new_child;
  }
  else
  {
    if (n)
    {
      // 需要按照pid从小到大排列
      while (cur != NULL && cur->child->pid < c->pid)
      {
        prev = cur;
        cur = cur->next;
      }
      if(prev == NULL){
        // 插入到最左端
        new_child->next = cur;
        p->children = new_child;
      }else if(cur == NULL){
        // 插入到最右端
        prev->next = new_child;
      }else{
        // 插入到prev , cur之间
        new_child->next = cur;
        prev->next = new_child;
      }

    }
    else
    {
      new_child->next = cur;
      p->children = new_child;
    }
  }
}