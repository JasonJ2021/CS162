#include "co.h"
#include <stdlib.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>

#define STACK_SIZE 65536
#define CO_SIZE 128

void co_init() __attribute__((constructor));
int find_free_spot();
int find_next_co();

enum co_status
{
  CO_NEW = 1, // 新创建，还未执行过
  CO_RUNNING, // 已经执行过
  CO_WAITING, // 在 co_wait 上等待
  CO_DEAD,    // 已经结束，但还未释放资源
};

struct co
{
  char name[64];
  void (*func)(void *); // co_start 指定的入口地址和参数
  void *arg;

  enum co_status status; // 协程的状态
  // struct co *    waiter;  // 是否有其他协程在等待当前协程
  jmp_buf context; // 寄存器现场 (setjmp.h)
  int loc;         // 位于数组的位置
  void *stack_ptr;
  uint8_t stack[STACK_SIZE] __attribute__((aligned(8))); // 协程的堆栈
};

int cur_co = 0;
struct co *co_list[CO_SIZE];

struct co *co_start(const char *name, void (*func)(void *), void *arg)
{
  struct co *new_co = (struct co *)malloc(sizeof(struct co));
  strcpy(new_co->name, name);
  new_co->func = func;
  new_co->arg = arg;
  new_co->status = CO_NEW;
  new_co->stack_ptr = new_co->stack + STACK_SIZE;
  int free_spot = find_free_spot();
  co_list[free_spot] = new_co;
  new_co->loc = free_spot;
  return new_co;
}

void co_wait(struct co *co)
{
  while (co->status != CO_DEAD)
  {
    co_yield ();
  }

  free(co);
  co_list[co->loc] = NULL;
}

void co_yield ()
{
  int val = setjmp(co_list[cur_co]->context);
  if (val == 0)
  {
    // 保存完寄存器现场之后，我们需要选择下一个协程进行切换

    struct co *cur_co_ptr = co_list[cur_co];
    cur_co = find_next_co();
    struct co *next_co_ptr = co_list[cur_co];
    if (next_co_ptr->status == CO_NEW)
    {
      // 如果新创建还没有运行的，需要进行堆栈的切换
      ((struct co *volatile)next_co_ptr)->status = CO_RUNNING;

            asm volatile(
      #if __x86_64__
                "movq %0, %%rsp; movq %2, %%rdi; call *%1"
                :
                : "b"((uintptr_t)(next_co_ptr->stack + STACK_SIZE - 8)), "d"(next_co_ptr->func), "a"(next_co_ptr->arg)
                : "memory"
      #else
                "movl %0, %%esp; movl %2, 4(%0); call *%1"
                :
                : "b"((uintptr_t)(next_co_ptr->stack + STACK_SIZE - 8)), "d"(next_co_ptr->func), "a"(next_co_ptr->arg)
                : "memory"
      #endif
            );
      // 此时函数已经运行完毕
      next_co_ptr->status = CO_DEAD;
      co_yield ();
    }
    else
    {
      // 如果已经运行了，直接进行longjmp
      longjmp(next_co_ptr->context, 8);
    }
  }
  else
  {
    return;
  }
}

void co_init()
{
  // 初始化main 的协程
  for (int i = 0; i < CO_SIZE; i++)
  {
    co_list[i] = NULL;
  }
  struct co *main_co = (struct co *)malloc(sizeof(struct co));
  strcpy(main_co->name, "Main_co");
  main_co->func = NULL;
  main_co->arg = NULL;
  main_co->status = CO_RUNNING;
  main_co->loc = 0;
  main_co->stack_ptr = NULL;
  // 把main协程添加到协程list中
  co_list[cur_co] = main_co;
}

// 在co_list中寻找一个空的插槽
int find_free_spot()
{
  for (int i = 0; i < CO_SIZE; i++)
  {
    if (co_list[i] == NULL)
    {
      return i;
    }
  }
  // 根据实验手册不可能出现这种情况
  return -1;
}

// 在co_list中寻找下一个运行的协程
int find_next_co()
{
  for (int i = cur_co + 1; i < CO_SIZE; i++)
  {
    if (co_list[i] != NULL && co_list[i]->status != CO_DEAD)
    {
      return i;
    }
  }
  for (int i = 0; i < cur_co; i++)
  {
    if (co_list[i] != NULL && co_list[i]->status != CO_DEAD)
    {
      return i;
    }
  }
  // 一般不会出现这种情况
  return -1;
}