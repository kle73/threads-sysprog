#include "threads.h"
#include <stdlib.h>
#include <stdint.h>


static struct thread* current_thread;

struct thread* thread_create(void (*f)(void*), void *arg)
{
	if (f == NULL) return NULL; // cant run a null pointer...
	struct thread* new_thread = (struct thread*)malloc(sizeof(struct thread));
	new_thread->stack = malloc(MAX_STACK_SIZE);  // allocate default stack size
	new_thread->rsp = new_thread->stack + MAX_STACK_SIZE;
	new_thread->rbp = new_thread->stack + MAX_STACK_SIZE;
	new_thread->f = f;
	new_thread->arg = arg;
	new_thread->has_run = 0;
	return new_thread;
}


void thread_add_runqueue(struct thread *t)
{
	if (current_thread == NULL) 
	{
		current_thread = t;
		current_thread->next = t;
		current_thread->prev = t;
		return;
	}
	t->next = current_thread;
	t->prev = current_thread->prev;
	current_thread->prev->next = t;
	current_thread->prev = t;
}

void thread_yield(void)
{
	if (current_thread == NULL) return;
	if (!setjmp(current_thread->ctxt))
	{
		schedule();
		dispatch();
	}
}


void schedule(void)
{
	if (current_thread == NULL) return;
	current_thread = current_thread->next;
}

void dispatch(void)
{
	if (current_thread == NULL) return;
	if (current_thread->has_run != 0)
	{
		// has already been run -> ctxt has been set by setjmp
		// save previous threads' rsp and rbp:
		//current_thread->prev->rbp = current_thread->prev->ctxt+8;
		// current_thread->prev->rsp =
		longjmp(current_thread->ctxt, 1);
	}
	else
	{
		current_thread->has_run = 1;
		// set rbp and rsp
		uint64_t rsp = (uint64_t)current_thread->rsp;
		uint64_t rbp = (uint64_t)current_thread->rbp;
		__asm__ __volatile__("mov %%rsp, %0\n\t"
							 : "=a"(rsp)
							 :
							 :);
		__asm__ __volatile__("mov %%rbp, %0\n\t"
							 : "=a"(rbp)
							 :
							 :);
		//call corresponding function
		current_thread->f(current_thread->arg);
		thread_exit();
	}	

}


void thread_exit(void)
{
	if (current_thread == NULL) return;
	struct thread* tmp = current_thread;
	free(current_thread->stack);
  	current_thread->prev->next = current_thread->next;
	current_thread->next->prev = current_thread->prev;
	current_thread = current_thread->next;
	if (tmp != NULL) free(tmp);
	dispatch();
}


void thread_start_threading(void)
{
	schedule();
	dispatch();
}








