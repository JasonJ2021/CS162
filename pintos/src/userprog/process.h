#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H
#include "vm/page.h"
#include "threads/thread.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);
bool handle_vm_fault(struct vm_entry *entry);

#endif /**< userprog/process.h */
