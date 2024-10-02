#ifndef THREADS_H_
#define THREADS_H_

// TODO: necessary includes, if any

#include <setjmp.h>
// TODO: necessary defines, if any
#define MAX_STACK_SIZE 2048

// TODO: definition of struct thread
struct thread {
	struct thread* next;
	struct thread* prev;
	void* stack;
	void* rsp;
	void* rbp;
	jmp_buf ctxt;
	void (*f)(void*);
	void *arg;
	int has_run;
};

struct thread *thread_create(void (*f)(void *), void *arg);
void thread_add_runqueue(struct thread *t);
void thread_yield(void);
void dispatch(void);
void schedule(void);
void thread_exit(void);
void thread_start_threading(void);

#endif // THREADS_H_
