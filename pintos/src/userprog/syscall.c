#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "devices/shutdown.h"
#include "lib/kernel/stdio.h"
#include "filesys/filesys.h"
#include "threads/synch.h"
#include "threads/malloc.h"
#include "devices/input.h"
#include "userprog/process.h"
#include "lib/string.h"

// 系统调用函数
static void syscall_handler (struct intr_frame *);
static bool validate_user_ptr(const void *user_ptr);
void exit(int status);
static int write(int fd, const void *buffer, unsigned size);
static bool create (const char *file, unsigned initial_size);
static int open (const char *file);
static void close (int fd);
static int read (int fd, void *buffer, unsigned size);
static int filesize (int fd);
static bool remove (const char *file);
static void seek (int fd, unsigned position);
static unsigned tell (int fd);
static int wait (int pid);
static int exec (const char *cmd_line);

// 控制关联文件的系统调用同时只能有一个线程访问
static struct lock file_lock; 

void
syscall_init (void) 
{
  lock_init(&file_lock);
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

/**--------------------------
 *      arv[2] or int (Optimal)
 *      arv[1] or int (Optimal)
 *      arv[0] or int (Optimal)
 * esp->SYS_CODE （通过这个变量来识别有多少参数）
 * @param  
 */
static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  // 首先查看esp指针是否有效
  if(!validate_user_ptr(f->esp)){
    // 暂时这样处理
    exit(-1);
  }
  // 添加对系统调用的支持
  switch(*(int *)f->esp){
    case SYS_WRITE:
    {
      if(!validate_user_ptr((int *)f->esp + 1)){
        exit(-1);
      }
      if(!validate_user_ptr((int *)f->esp + 2)){
        exit(-1);
      }
      if(!validate_user_ptr((unsigned*)f->esp + 3)){
        exit(-1);
      }
      int fd = *((int *)f->esp + 1);
      const void *buffer = (void*)(*((int*)f->esp + 2));
      unsigned size = *((unsigned*)f->esp + 3);
      if(!validate_user_ptr(buffer)){
        exit(-1);
      }
      f->eax = write(fd , buffer , size);
      break;
    }
    case SYS_EXIT:
      // 打印信息并且退出
      {
        // ERROR : 这里的esp指向的status也要进行检查！
        if(!validate_user_ptr((int *)f->esp + 1)){
          exit(-1);
        }
        int status = *((int *)f->esp + 1);
        exit(status);
        break;
      }
    case SYS_HALT:
      // 关闭pintos 
      shutdown_power_off();
      break;
    case SYS_CREATE:
    {
      // bool create (const char *file, unsigned initial_size)
      if(!validate_user_ptr((int *)f->esp + 1)){
        exit(-1);
      }
      if(!validate_user_ptr((unsigned *)f->esp + 2)){
        exit(-1);
      }
      const char *file = (char *)(*((int *)f->esp + 1));
      if(!validate_user_ptr(file)){
        exit(-1);
      }
      unsigned initial_size =  *((unsigned*)f->esp + 2);
      f->eax = create(file , initial_size);
      break;
    }
    case SYS_OPEN:
    {
      // int open (const char *file)
      if(!validate_user_ptr((unsigned *)f->esp + 1)){
        exit(-1);
      }
      const char* file = (char *)(*((int *)f->esp + 1));
      if(!validate_user_ptr(file)){
        exit(-1);
      }
      f->eax = open(file);
      break;
    }
    case SYS_CLOSE:
    {
      // void close (int fd)
      if(!validate_user_ptr((int *)f->esp + 1)){
          exit(-1);
      }
      int fd = *((int *)f->esp + 1);
      close(fd);
      break;
    }
    case SYS_READ:
    {
      // int read (int fd, void *buffer, unsigned size)
      if(!validate_user_ptr((int *)f->esp + 1)){
        exit(-1);
      }
      if(!validate_user_ptr((int *)f->esp + 2)){
        exit(-1);
      }
      if(!validate_user_ptr((unsigned*)f->esp + 3)){
        exit(-1);
      }
      int fd = *((int *)f->esp + 1);
      const void *buffer = (void*)(*((int*)f->esp + 2));
      unsigned size = *((unsigned*)f->esp + 3);
      if(!validate_user_ptr(buffer)){
        exit(-1);
      }
      f->eax = read(fd , buffer , size);
      break;
    }
    case SYS_FILESIZE:
    {
      // int filesize (int fd)
      if(!validate_user_ptr((int *)f->esp + 1)){
          exit(-1);
      }
      int fd = *((int *)f->esp + 1);
      f->eax = filesize(fd);
      break;
    }
    case SYS_REMOVE:
    {
      //  bool remove (const char *file)
      if(!validate_user_ptr((unsigned *)f->esp + 1)){
        exit(-1);
      }
      const char* file = (char *)(*((int *)f->esp + 1));
      if(!validate_user_ptr(file)){
        exit(-1);
      }
      bool ans = remove(file);
      f->eax = ans;
      break;
    }
    case SYS_SEEK:
    {
      // static void seek (int fd, unsigned position)
      if(!validate_user_ptr((int *)f->esp + 1)){
          exit(-1);
      }
      if(!validate_user_ptr((unsigned *)f->esp + 2)){
          exit(-1);
      }
      int fd = *((int *)f->esp + 1);
      unsigned position = *((unsigned *)f->esp + 2);
      seek(fd,position);
      break;
    }
    case SYS_TELL:
    {
      // unsigned tell (int fd)
      if(!validate_user_ptr((int *)f->esp + 1)){
          exit(-1);
      }
      int fd = *((int *)f->esp + 1);
      f->eax = tell(fd);
      break;
    }
    case SYS_WAIT:
    {
      // int wait (pid_t pid)
      if(!validate_user_ptr((int *)f->esp + 1)){
          exit(-1);
      }
      int pid = *((int *)f->esp + 1);
      f->eax = wait(pid);
      break;
    }
    case SYS_EXEC:
    {
      // pid_t exec (const char *cmd_line)
      if(!validate_user_ptr((unsigned *)f->esp + 1)){
        exit(-1);
      }
      const char* cmd_line = (char *)(*((int *)f->esp + 1));
      if(!validate_user_ptr(cmd_line)){
        exit(-1);
      }
      f->eax = exec(cmd_line);
      break;
    }
    default:
      break;
  }
}

// 检查一个用户指针是否有效，有效返回true
static bool validate_user_ptr(const void *user_ptr){
  return user_ptr != NULL && is_user_vaddr(user_ptr) && user_ptr >= 0x08048000 \
  && pagedir_get_page(thread_current()->pagedir,user_ptr) != NULL;
}

void exit(int status){
  struct thread *t = thread_current();
  t->exec_info_->exit_status = status;
  t->exec_info_->killed_by_exit = true;
  struct list_elem *e;
  enum intr_level old_level = intr_disable();
  for(e = list_begin(&t->children) ; e != list_end(&t->children) ; e = list_next(e)){
    struct exec_info *exec_info_ = list_entry(e , struct exec_info , elem);
    exec_info_->parent_proc_alive = false;
  }
  intr_set_level(old_level);
  // 通知父进程自己退出
  sema_up(&t->exec_info_->sema);
  // 如果父进程已经被killed , 需要子进程自己回收资源
  if(!t->exec_info_->parent_proc_alive){
    free(t->exec_info_);
    t->info_released = true;
  }
  // 之后还需要添加唤醒等待父进程的阶段
  printf("%s: exit(%d)\n" , t->name , t->exec_info_->exit_status);
  thread_exit();
}

static int write(int fd, const void *buffer, unsigned size){
  // // 这里需要上锁！
  // lock_acquire(&file_lock);
  // if(fd == 1){
  //   // 如果fd == 1 输出到终端
  //   putbuf(buffer , size);
  // }else{
  //   lock_release(&file_lock);
  //   return file_write(thread_current()->fdt[fd] , buffer , size);
  // }
  // lock_release(&file_lock);
  // return size;
  // 这里需要上锁！
  if(fd <= 0 || fd >= 64){
    return 0;
  }
  lock_acquire(&file_lock);
  if(fd == 1){
    // 如果fd == 1 输出到终端
    putbuf(buffer , size);
    lock_release(&file_lock);
    return size;
  }else{
    if(thread_current()->fdt[fd] == NULL){
      lock_release(&file_lock);
      exit(-1);// 向一个不存在的file写
      return 0;
    }
    int ans = file_write(thread_current()->fdt[fd] , buffer , size);
    lock_release(&file_lock);
    return ans;
  }
}

static bool create (const char *file, unsigned initial_size){
  bool ans = false;
  lock_acquire(&file_lock);
  ans = filesys_create(file , initial_size);
  lock_release(&file_lock);
  return ans;
}

static int open (const char *file){
  lock_acquire(&file_lock);
  struct file* fp;
  int i = 2;
  for(; i < FDT_SIZE ; i++){
    if(thread_current()->fdt[i] == NULL){
      break;
    }
  }
  if(i == FDT_SIZE){
    lock_release(&file_lock);
    exit(-1);
  }
  fp = filesys_open(file);
  lock_release(&file_lock);
  if(fp == NULL){
    return -1;
  }else{
    thread_current()->fdt[i] = fp;
  }
  return i;
}

static void close (int fd){
  if(fd >=64 || fd <=2){
    return;
  }
  lock_acquire(&file_lock);
  struct thread *t = thread_current();
  // 实际上不用判断NULL
  if(fd >= 2 && t->fdt[fd] != NULL){
    file_close(t->fdt[fd]);
    t->fdt[fd] = NULL;
  }
  lock_release(&file_lock);
}

void close_all(void){
  lock_acquire(&file_lock);
  enum intr_level old_level = intr_enable();
  struct thread *t = thread_current();
  for(int i = 0 ; i < FDT_SIZE ; i++){
    file_close(t->fdt[i]);
  }
  intr_set_level(old_level);
  lock_release(&file_lock);
}

static int read (int fd, void *buffer, unsigned size){
  // 这里需要上锁！
  if(fd < 0 || fd >= 64 || fd == 1){
    return 0;
  }
  lock_acquire(&file_lock);
  int readin_size = 0;
  char *ptr = (char *)buffer;
  if(fd == 0){
    // 如果fd == 0 从stdin输入
    while(size != 0 ){
      ptr[readin_size++] = input_getc();
      size--;
      if(ptr[readin_size -1] == -1){
        break;
      }
    }
    lock_release(&file_lock);
    return readin_size;
  }else{
    if(thread_current()->fdt[fd] == NULL){
      lock_release(&file_lock);
      exit(-1);
      return 0;
    }
    readin_size = file_read(thread_current()->fdt[fd] , buffer , size);
    lock_release(&file_lock);
    return readin_size;
  }
}

static int filesize (int fd){
  int size = 0 ;
  if(fd <2 || fd >=64){
    return 0;
  }
  if(thread_current()->fdt[fd] == NULL){
    return 0;
  }
  lock_acquire(&file_lock);
  size = file_length(thread_current()->fdt[fd]);
  lock_release(&file_lock);
  return size;
}

static bool remove (const char *file){
  lock_acquire(&file_lock);
  bool ans = filesys_remove(file);
  lock_release(&file_lock);
  return ans;
}

static void seek (int fd, unsigned position){
  if(fd < 2 || fd >=64){
    return;
  }
  if(thread_current()->fdt[fd] == NULL){
    return;
  }
  lock_acquire(&file_lock);
  file_seek(thread_current()->fdt[fd] , position);
  lock_release(&file_lock);
}

static unsigned tell (int fd){
  if(fd < 2 || fd >= 64){
    return 0;
  }
  if(thread_current()->fdt[fd] == NULL){
    return 0;
  }
  lock_acquire(&file_lock);
  unsigned ans = file_tell(thread_current()->fdt[fd]);
  lock_release(&file_lock);
  return ans;
}

static int wait (int pid){
  struct thread *cur_t = thread_current();
  enum intr_level old_level = intr_disable();
  struct exec_info * exec_info_ = NULL;
  struct list_elem *e;
  for(e = list_begin(&cur_t->children); e != list_end(&cur_t->children) ; e=list_next(e)){
    struct exec_info *temp_exec = list_entry(e,struct exec_info , elem);
    if(temp_exec->tid == pid){
      exec_info_ = temp_exec;
      break;
    }
  }
  intr_set_level(old_level);
  if(exec_info_ != NULL){
    sema_down(&exec_info_->sema);
    list_remove(e); // 这边中断关闭有风险
    int exit_status_return = -1;
    if(exec_info_->killed_by_exit){
      exit_status_return = exec_info_->exit_status;
    }else{
      exit_status_return = -1;
    }
    // 父进程需要释放资源
    free(exec_info_);
    return exit_status_return;
  }else{
    return -1;
  }
}

static int exec (const char *cmd_line){
  char *command_line = (char *)malloc(strlen(cmd_line) + 1);
  strlcpy(command_line , cmd_line,strlen(cmd_line) + 1);
  int pid = process_execute(command_line);
  free(command_line);
  struct list_elem *e;
  struct thread*t = thread_current();
  struct exec_info *exec_info_;
  for(e = list_begin(&t->children) ; e != list_end(&t->children) ; e = list_next(e)){
    exec_info_ = list_entry(e,struct exec_info , elem);
    if(exec_info_->tid == pid){
      break;
    }
  }
  sema_down(&exec_info_->load_sema);
  if(!exec_info_->load_success){
    pid = -1;
  }
  return pid;
}