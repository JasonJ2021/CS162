#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
  // char *exec_argv[] = { "strace", "-T" , argv[1], NULL, };
  // char *exec_envp[] = { "PATH=/bin", NULL, };
  // execve("strace",          exec_argv, exec_envp);
  // execve("/bin/strace",     exec_argv, exec_envp);
  // execve("/usr/bin/strace", exec_argv, exec_envp);
  // perror(argv[0]);
  // exit(EXIT_FAILURE);
  
  // 解析出command 和 arg
  // argv[1] 为 要跟踪的程序
// ./sperf-64 tree /

struct syscall_type{
  char name[64];
  double time;
};
static struct syscall_type syscall_buckets[256];
static int sys_num = 0;
static double total_time = 0;
void readin_syscall(char *line , int size);
void handle_sys(char *name , double time);
void sort_sys();
int main(int argc, char *argv[]) {

  if(argc < 2){
    perror("e.g. $./sperf-64 tree /");
    exit(EXIT_FAILURE);
  }
  char *exec_argv[argc + 2];
  exec_argv[0] = "strace";
  exec_argv[1] = "-T";
  for(int i = 2 ; i < argc + 1 ; i++){
    exec_argv[i] = (char *)malloc(sizeof(argv[i- 1]));
    strcpy(exec_argv[i],argv[i - 1]);
  }
  exec_argv[argc + 1] = NULL;
  char *exec_envp[] = { "PATH=/bin", NULL, };
  // execve("/bin/strace",     exec_argv, exec_envp);
  int fildes[2];
  if(pipe(fildes) != 0 ){
    perror("pipe error\n");
    exit(1);
  }
  int pid = fork();
  if(pid == 0){
    // 子进程
    close(fildes[0]);
    dup2(fildes[1] , 2);
    // write(fildes[1] , "Hello\n" , 6);
    // perror("wtf\n");
    execve("/bin/strace",     exec_argv, exec_envp);
  }else{
    close(fildes[1]);
    char *line;
    FILE *fp = fdopen(fildes[0],"r");
    size_t num = 0;
    while(getline(&line,&num,fp) > 0){
      readin_syscall(line,num);
    }
  }
  printf("total time = %lf\n" , total_time);
  // printf("sum = %d\n" , sys_num);
  sort_sys();

  // for(int i = 0 ; i < sys_num ;i++){
  //   printf("%lf\n",syscall_buckets[i].time);
  // }
  printf("The top five syscall is listed below\n");
  if(sys_num < 5){
    for(int i = 0 ; i < sys_num ; i++){
      printf("syscall name = %s , cover time = %lf.2",syscall_buckets[i].name,syscall_buckets[i].time/total_time*100);
    }
  }else{
    for(int i = 0 ; i < 5 ;i++){
      printf("syscall name = %s , cover time = %.2lf\n",syscall_buckets[i].name,syscall_buckets[i].time/total_time*100);
    }
  }
}

void readin_syscall(char *line , int size){
  char temp[64];
  double time = -1;
  memset(temp , 0 , 64);
  bool flag = false;
  for(int i = 0 ; i < size ; i++){
    if(line[i] == '(' && !flag){
      line[i] = '\0';
      strncpy(temp,line,i);
      flag = true;
    }
    if(line[i] == '<'){
      for(int j = i ; j < size ;j++){
        if(line[j] == '>'){
          line[j] = '\0';
          sscanf(line + i + 1 ,"%lf" , &time);
          break;
        }
      }
    }
  }
  if(time < 0){
    return;
  }
  if(flag){
    handle_sys(temp , time);
  }

}

void handle_sys(char *name , double time){
  total_time += time;
  for(int i = 0 ; i < sys_num ; i++){
    if(strcmp(syscall_buckets[i].name , name)==0){
      syscall_buckets[i].time += time;
      return;
    }
  }
  sys_num++;
  strcpy(syscall_buckets[sys_num - 1].name,name);
  syscall_buckets[sys_num -1].time = time;
}

void sort_sys(){
  for(int i = 0 ; i < sys_num ; i++){
    for(int j = 0 ; j < sys_num - i - 1 ; j++){
      if(syscall_buckets[j].time < syscall_buckets[j+1].time){
        struct syscall_type temp = syscall_buckets[j];
        syscall_buckets[j] = syscall_buckets[j+1];
        syscall_buckets[j+1] = temp;
      }
    }
  }
}