#ifndef THREADS_THREAD_H
#define THREADS_THREAD_H

#include <debug.h>
#include <list.h>
#include <stdint.h>
#include "filesys/file.h"
#include "threads/synch.h"

/** States in a thread's life cycle. */
enum thread_status
  {
    THREAD_RUNNING,     /**< Running thread. */
    THREAD_READY,       /**< Not running but ready to run. */
    THREAD_BLOCKED,     /**< Waiting for an event to trigger. */
    THREAD_DYING        /**< About to be destroyed. */
  };
struct exec_info
{
   struct list_elem elem; // children_listelem
   int exit_status; // 退出的状态
   struct semaphore sema; // 用于和父进程通信
   bool killed_by_exit; // 标志是否是通过exit被Killed
   bool parent_proc_alive; // 父进程是否还alive
   struct semaphore load_sema; // 用于load的通信
   int tid;
   bool load_success;
};
/** Thread identifier type.
   You can redefine this to whatever type you like. */
typedef int tid_t;
#define TID_ERROR ((tid_t) -1)          /**< Error value for tid_t. */

/** Thread priorities. */
#define PRI_MIN 0                       /**< Lowest priority. */
#define PRI_DEFAULT 31                  /**< Default priority. */
#define PRI_MAX 63                      /**< Highest priority. */

/** File Descriptor.*/
#define FDT_SIZE 64

/** A kernel thread or user process.

   Each thread structure is stored in its own 4 kB page.  The
   thread structure itself sits at the very bottom of the page
   (at offset 0).  The rest of the page is reserved for the
   thread's kernel stack, which grows downward from the top of
   the page (at offset 4 kB).  Here's an illustration:

        4 kB +---------------------------------+
             |          kernel stack           |
             |                |                |
             |                |                |
             |                V                |
             |         grows downward          |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             +---------------------------------+
             |              magic              |
             |                :                |
             |                :                |
             |               name              |
             |              status             |
        0 kB +---------------------------------+

   The upshot of this is twofold:

      1. First, `struct thread' must not be allowed to grow too
         big.  If it does, then there will not be enough room for
         the kernel stack.  Our base `struct thread' is only a
         few bytes in size.  It probably should stay well under 1
         kB.

      2. Second, kernel stacks must not be allowed to grow too
         large.  If a stack overflows, it will corrupt the thread
         state.  Thus, kernel functions should not allocate large
         structures or arrays as non-static local variables.  Use
         dynamic allocation with malloc() or palloc_get_page()
         instead.

   The first symptom of either of these problems will probably be
   an assertion failure in thread_current(), which checks that
   the `magic' member of the running thread's `struct thread' is
   set to THREAD_MAGIC.  Stack overflow will normally change this
   value, triggering the assertion. */
/** The `elem' member has a dual purpose.  It can be an element in
   the run queue (thread.c), or it can be an element in a
   semaphore wait list (synch.c).  It can be used these two ways
   only because they are mutually exclusive: only a thread in the
   ready state is on the run queue, whereas only a thread in the
   blocked state is on a semaphore wait list. */
struct thread
  {
    /* Owned by thread.c. */
    tid_t tid;                          /**< Thread identifier. */
    enum thread_status status;          /**< Thread state. */
    char name[16];                      /**< Name (for debugging purposes). */
    uint8_t *stack;                     /**< Saved stack pointer. */
    int priority;                       /**< Priority. */
    struct list_elem allelem;           /**< List element for all threads list. */
    struct lock *wait_for;              /**< 记录这个线程正在等待的锁.*/
    struct list donations;              /**< 记录给这个线程的捐助者 .*/
    struct list_elem donor_elem;        /**< 放在donations中的元素 .*/
    int32_t original_priority;              /**< 记录没有被捐助之前的priority .*/
    int32_t nice;                           /**< 用于mlfqs 调度器策略 .*/
    int32_t recent_cpu;                     /**< 用于mlfqs 调度器策略 .*/
    int32_t exit_status;                /**< 用于exit 状态记录      .*/
    struct file *fdt[FDT_SIZE];                /**< 进程拥有的文件描述符 .*/
    int next_fd;                        /**< 下一个被分配的文件描述符 .*/
    /* ===================Execution Information=============================. */
    struct list children;
    struct exec_info *exec_info_;
    bool info_released;

    /* Shared between thread.c and synch.c. */
    struct list_elem elem;              /**< List element. */
    

#ifdef USERPROG
    /* Owned by userprog/process.c. */
    uint32_t *pagedir;                  /**< Page directory. */
#endif

    /* Owned by thread.c. */
    unsigned magic;                     /**< Detects stack overflow. */
  };

/** If false (default), use round-robin scheduler.
   If true, use multi-level feedback queue scheduler.
   Controlled by kernel command-line option "-o mlfqs". */
extern bool thread_mlfqs;

void thread_init (void);
void thread_start (void);

void thread_tick (void);
void thread_print_stats (void);

typedef void thread_func (void *aux);
tid_t thread_create (const char *name, int priority, thread_func *, void *);

void thread_block (void);
void thread_unblock (struct thread *);

struct thread *thread_current (void);
tid_t thread_tid (void);
const char *thread_name (void);

void thread_exit (void) NO_RETURN;
void thread_yield (void);

/** Performs some operation on thread t, given auxiliary data AUX. */
typedef void thread_action_func (struct thread *t, void *aux);
void thread_foreach (thread_action_func *, void *);

int thread_get_priority (void);
void thread_set_priority (int);

int thread_get_nice (void);
void thread_set_nice (int);
int thread_get_recent_cpu (void);
int thread_get_load_avg (void);

bool
priority_larger (const struct list_elem *a_, const struct list_elem *b_,
            void *aux UNUSED);

bool
priority_less (const struct list_elem *a_, const struct list_elem *b_,
            void *aux UNUSED);

void donate_priority(struct thread *donee , struct thread *donor);
void calc_load_avg(void);
void calc_recent_cpu(struct thread*t , void *aux UNUSED);
void calc_priority(struct thread*t , void *aux UNUSED);
bool should_yield(void);
void incr_cpu_recent(void);
void calc_priority_recent_cpu(struct thread*t , void *aux UNUSED);
#endif /**< threads/thread.h */
