/** This file is derived from source code for the Nachos
   instructional operating system.  The Nachos copyright notice
   is reproduced in full below. */

/** Copyright (c) 1992-1996 The Regents of the University of California.
   All rights reserved.

   Permission to use, copy, modify, and distribute this software
   and its documentation for any purpose, without fee, and
   without written agreement is hereby granted, provided that the
   above copyright notice and the following two paragraphs appear
   in all copies of this software.

   IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO
   ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
   CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE
   AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA
   HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
   PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
   BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
   PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
   MODIFICATIONS.
*/

#include "threads/synch.h"
#include <stdio.h>
#include <string.h>
#include "threads/interrupt.h"
#include "threads/thread.h"


/** Initializes semaphore SEMA to VALUE.  A semaphore is a
   nonnegative integer along with two atomic operators for
   manipulating it:

   - down or "P": wait for the value to become positive, then
     decrement it.

   - up or "V": increment the value (and wake up one waiting
     thread, if any). */
void
sema_init (struct semaphore *sema, unsigned value) 
{
  ASSERT (sema != NULL);

  sema->value = value;
  list_init (&sema->waiters);
}

/** Down or "P" operation on a semaphore.  Waits for SEMA's value
   to become positive and then atomically decrements it.

   This function may sleep, so it must not be called within an
   interrupt handler.  This function may be called with
   interrupts disabled, but if it sleeps then the next scheduled
   thread will probably turn interrupts back on. */
void
sema_down (struct semaphore *sema) 
{
  enum intr_level old_level;

  ASSERT (sema != NULL);
  ASSERT (!intr_context ());

  old_level = intr_disable ();
  while (sema->value == 0) 
    {
      list_push_back (&sema->waiters, &thread_current ()->elem);
      thread_block ();
    }
  sema->value--;
  intr_set_level (old_level);
}

/** Down or "P" operation on a semaphore, but only if the
   semaphore is not already 0.  Returns true if the semaphore is
   decremented, false otherwise.

   This function may be called from an interrupt handler. */
bool
sema_try_down (struct semaphore *sema) 
{
  enum intr_level old_level;
  bool success;

  ASSERT (sema != NULL);

  old_level = intr_disable ();
  if (sema->value > 0) 
    {
      sema->value--;
      success = true; 
    }
  else
    success = false;
  intr_set_level (old_level);

  return success;
}

/** Up or "V" operation on a semaphore.  Increments SEMA's value
   and wakes up one thread of those waiting for SEMA, if any.

   This function may be called from an interrupt handler. */
void
sema_up (struct semaphore *sema) 
{
  enum intr_level old_level;

  ASSERT (sema != NULL);

  old_level = intr_disable ();
  sema->value++; // 调换了位置，原来是在if语句下面的
  if (!list_empty (&sema->waiters)){ 
    // thread_unblock (list_entry (list_pop_front (&sema->waiters),
    //                             struct thread, elem));
    struct list_elem *t;
    t = list_max(&sema->waiters , priority_less ,NULL);
    list_remove(t);
    thread_unblock (list_entry (t , struct thread, elem));
  }
  intr_set_level (old_level);
}

static void sema_test_helper (void *sema_);

/** Self-test for semaphores that makes control "ping-pong"
   between a pair of threads.  Insert calls to printf() to see
   what's going on. */
void
sema_self_test (void) 
{
  struct semaphore sema[2];
  int i;

  printf ("Testing semaphores...");
  sema_init (&sema[0], 0);
  sema_init (&sema[1], 0);
  thread_create ("sema-test", PRI_DEFAULT, sema_test_helper, &sema);
  for (i = 0; i < 10; i++) 
    {
      sema_up (&sema[0]);
      sema_down (&sema[1]);
    }
  printf ("done.\n");
}

/** Thread function used by sema_self_test(). */
static void
sema_test_helper (void *sema_) 
{
  struct semaphore *sema = sema_;
  int i;

  for (i = 0; i < 10; i++) 
    {
      sema_down (&sema[0]);
      sema_up (&sema[1]);
    }
}

/** Initializes LOCK.  A lock can be held by at most a single
   thread at any given time.  Our locks are not "recursive", that
   is, it is an error for the thread currently holding a lock to
   try to acquire that lock.

   A lock is a specialization of a semaphore with an initial
   value of 1.  The difference between a lock and such a
   semaphore is twofold.  First, a semaphore can have a value
   greater than 1, but a lock can only be owned by a single
   thread at a time.  Second, a semaphore does not have an owner,
   meaning that one thread can "down" the semaphore and then
   another one "up" it, but with a lock the same thread must both
   acquire and release it.  When these restrictions prove
   onerous, it's a good sign that a semaphore should be used,
   instead of a lock. */
void
lock_init (struct lock *lock)
{
  ASSERT (lock != NULL);

  lock->holder = NULL;
  sema_init (&lock->semaphore, 1);
}

/** 用于donate_elem , list比较         */
static bool
priority_less_by_donorelem (const struct list_elem *a_, const struct list_elem *b_,
            void *aux UNUSED) 
{
  const struct thread *a = list_entry (a_, struct thread, donor_elem);
  const struct thread *b = list_entry (b_, struct thread, donor_elem);
  
  return a->priority < b->priority;
}
/** Acquires LOCK, sleeping until it becomes available if
   necessary.  The lock must not already be held by the current
   thread.

   This function may sleep, so it must not be called within an
   interrupt handler.  This function may be called with
   interrupts disabled, but interrupts will be turned back on if
   we need to sleep. */
void
lock_acquire (struct lock *lock)
{
  ASSERT (lock != NULL);
  ASSERT (!intr_context ());
  ASSERT (!lock_held_by_current_thread (lock));

  if(!thread_mlfqs){
    // 对链表进行操作需要关闭中断~
    enum intr_level old_value;
    old_value = intr_disable();
    // 添加当前线程正在等待lock锁
    struct thread * cur_thread = thread_current();
    struct thread * waitfor_thread = lock->holder;
    cur_thread->wait_for = lock;
    bool flag = false;
    // 当前的锁已经被持有，并且请求acquire线程的优先级比持有锁的线程优先级高，donate...
    while(waitfor_thread != NULL && waitfor_thread->priority < cur_thread->priority){
      if(flag  == false){
        struct list *donate_list = &waitfor_thread->donations;
        // 如果当前donate_list为空，那么需要保存当前的priority , 以便之后恢复
        if(list_empty(donate_list) == true){
          waitfor_thread->original_priority = waitfor_thread->priority;
        }
        list_push_back(donate_list , &cur_thread->donor_elem);
        donate_priority(waitfor_thread , cur_thread);
        
        flag = true;
      }else{
        waitfor_thread->priority = cur_thread->priority;
      }
      if(waitfor_thread->wait_for != NULL){
        waitfor_thread = waitfor_thread->wait_for->holder;
      }else{
        break;
      }
    }
    intr_set_level(old_value);
  }


  sema_down (&lock->semaphore);
  thread_current()->wait_for = NULL;
  lock->holder = thread_current ();
}

/** Tries to acquires LOCK and returns true if successful or false
   on failure.  The lock must not already be held by the current
   thread.

   This function will not sleep, so it may be called within an
   interrupt handler. */
bool
lock_try_acquire (struct lock *lock)
{
  bool success;

  ASSERT (lock != NULL);
  ASSERT (!lock_held_by_current_thread (lock));

  success = sema_try_down (&lock->semaphore);
  if (success)
    lock->holder = thread_current ();
  return success;
}

/** Releases LOCK, which must be owned by the current thread.

   An interrupt handler cannot acquire a lock, so it does not
   make sense to try to release a lock within an interrupt
   handler. */
void
lock_release (struct lock *lock) 
{
  ASSERT (lock != NULL);
  ASSERT (lock_held_by_current_thread (lock));
  if(!thread_mlfqs){
    enum intr_level old_value = intr_disable();
    struct list *donate_list = &lock->holder->donations;
    struct list_elem *e;
    for(e = list_begin(donate_list) ; e != list_end(donate_list) ;){
      // 遍历donate_list 寻找正在wait for lock 的线程，找到了从中删除，最后更新当前线程的priority
      struct thread *t = list_entry(e,struct thread , donor_elem );
      if(t->wait_for == lock){
        e = list_next(e);
        t->wait_for = NULL;
        list_remove(e->prev);
        continue;
      }
      if(list_end(donate_list) == e){
        break;
      }
      e = list_next(e);
    }
    if(list_empty(donate_list) && thread_current()->original_priority >= 0){
      thread_current()->priority = thread_current()->original_priority;
    }else if(!list_empty(donate_list)){
      thread_current()->priority = list_entry(list_max(donate_list,priority_less_by_donorelem,NULL),struct thread,donor_elem)->priority;
    }
    intr_set_level(old_value);
  }


  lock->holder = NULL;
  sema_up (&lock->semaphore);
}

/** Returns true if the current thread holds LOCK, false
   otherwise.  (Note that testing whether some other thread holds
   a lock would be racy.) */
bool
lock_held_by_current_thread (const struct lock *lock) 
{
  ASSERT (lock != NULL);

  return lock->holder == thread_current ();
}

/** One semaphore in a list. */
struct semaphore_elem 
  {
    struct thread* thread_ptr; 
    struct list_elem elem;              /**< List element. */
    struct semaphore semaphore;         /**< This semaphore. */
  };

/** Initializes condition variable COND.  A condition variable
   allows one piece of code to signal a condition and cooperating
   code to receive the signal and act upon it. */
void
cond_init (struct condition *cond)
{
  ASSERT (cond != NULL);

  list_init (&cond->waiters);
}

/** Atomically releases LOCK and waits for COND to be signaled by
   some other piece of code.  After COND is signaled, LOCK is
   reacquired before returning.  LOCK must be held before calling
   this function.

   The monitor implemented by this function is "Mesa" style, not
   "Hoare" style, that is, sending and receiving a signal are not
   an atomic operation.  Thus, typically the caller must recheck
   the condition after the wait completes and, if necessary, wait
   again.

   A given condition variable is associated with only a single
   lock, but one lock may be associated with any number of
   condition variables.  That is, there is a one-to-many mapping
   from locks to condition variables.

   This function may sleep, so it must not be called within an
   interrupt handler.  This function may be called with
   interrupts disabled, but interrupts will be turned back on if
   we need to sleep. */
void
cond_wait (struct condition *cond, struct lock *lock) 
{
  struct semaphore_elem waiter;
  waiter.thread_ptr = thread_current();
  ASSERT (cond != NULL);
  ASSERT (lock != NULL);
  ASSERT (!intr_context ());
  ASSERT (lock_held_by_current_thread (lock));
  
  sema_init (&waiter.semaphore, 0);
  list_push_back (&cond->waiters, &waiter.elem);
  lock_release (lock);
  sema_down (&waiter.semaphore);
  lock_acquire (lock);
}


/** 我们要求按照priority从大到小排序         */
static bool
priority_less_by_condelem (const struct list_elem *a_, const struct list_elem *b_,
            void *aux UNUSED) 
{
  const struct thread *a = list_entry (a_, struct semaphore_elem, elem)->thread_ptr;
  const struct thread *b = list_entry (b_, struct semaphore_elem, elem)->thread_ptr;
  
  return a->priority < b->priority;
}

/** If any threads are waiting on COND (protected by LOCK), then
   this function signals one of them to wake up from its wait.
   LOCK must be held before calling this function.

   An interrupt handler cannot acquire a lock, so it does not
   make sense to try to signal a condition variable within an
   interrupt handler. */
void
cond_signal (struct condition *cond, struct lock *lock UNUSED) 
{
  ASSERT (cond != NULL);
  ASSERT (lock != NULL);
  ASSERT (!intr_context ());
  ASSERT (lock_held_by_current_thread (lock));

  if (!list_empty (&cond->waiters)){
    // sema_up (&list_entry (list_pop_front (&cond->waiters),
    //                       struct semaphore_elem, elem)->semaphore);
    struct list_elem *t;
    t = list_max(&cond->waiters , priority_less_by_condelem , NULL);
    struct semaphore_elem *s= list_entry (t, struct semaphore_elem, elem);
    list_remove(t);
    sema_up (&s->semaphore);
  }
}

/** Wakes up all threads, if any, waiting on COND (protected by
   LOCK).  LOCK must be held before calling this function.

   An interrupt handler cannot acquire a lock, so it does not
   make sense to try to signal a condition variable within an
   interrupt handler. */
void
cond_broadcast (struct condition *cond, struct lock *lock) 
{
  ASSERT (cond != NULL);
  ASSERT (lock != NULL);

  while (!list_empty (&cond->waiters))
    cond_signal (cond, lock);
}
