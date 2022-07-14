#include <stdio.h>
#include <setjmp.h>
#include <stdint.h>
#define STACK_SIZE 8192

void init() __attribute__((constructor));

enum co_status {
  CO_NEW = 1, // 新创建，还未执行过
  CO_RUNNING, // 已经执行过
  CO_WAITING, // 在 co_wait 上等待 , 表示main函数
  CO_DEAD,    // 已经结束，但还未释放资源
};

struct co {
  char *name;
  void (*func)(void *); // co_start 指定的入口地址和参数
  void *arg;

  enum co_status status;  // 协程的状态
  struct co *    waiter;  // 是否有其他协程在等待当前协程
  jmp_buf        context; // 寄存器现场 (setjmp.h)
  uint8_t *      heap_top;// 栈帧的指针
  uint8_t        stack[STACK_SIZE]; // 协程的堆栈
};


struct co *cur;

int main() {
    int n = 0;
    jmp_buf buf;
    setjmp(buf);
    printf("Hello %d\n", n);
    longjmp(buf, n++);
}
void init(){
    cur = (struct co *)malloc(sizeof(struct co));
    cur->name = "main";
    cur->status = CO_WAITING;
    
}