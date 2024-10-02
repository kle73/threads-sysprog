#include "threads.h"
#include <stdlib.h>


static struct thread* current_thread;
static struct thread* first_thread;
static struct thread* last_thread;

struct thread* thread_create(void (*f)(void*), void *arg)
{
	struct thread* new_thread = (struct thread*)malloc(sizeof(struct thread));
	current_thread = new_thread;
	if (first_thread == NULL)
	{
		first_thread = current_thread;
		last_thread = current_thread;
	} // invariant: last_thread->next = first_thread
	new_thread->next = first_thread;
	new_thread->prev = first_thread;
	new_thread->stack = malloc(2048);  // allocate default stack size
	new_thread->rsp = new_thread->stack + 2048;
	new_thread->rbp = new_thread->stack + 2048;
	return new_thread;
}


void thread_add_runqueue(struct thread *t)
{
	t->next = first_thread;
	t->prev = last_thread;
	first_thread->prev = t;
	last_thread->next = t;
	last_thread = t;
}

void thread_yield(void)
{
	if (setjmp(current_thread->ctxt))
		return;
	else 
	{
		schedule();
		dispatch();
	}
}


void schedule(void)
{
	current_thread = current_thread->next;
}


void thread_exit(void)
{
	struct thread* tmp = current_thread;
	free(current_thread->stack);
  current_thread->prev->next = current_thread->next;
	current_thread->next->prev = current_thread->prev;
	current_thread = current_thread->next;
	// TODO check if the deleted thread is either first or last!
	free(tmp);
	dispatch();
}











