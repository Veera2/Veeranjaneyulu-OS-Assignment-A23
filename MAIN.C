#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pid.c"

struct program{
	unsigned int p_id;
	pthread_t thread;
	int var--;
	struct process *next;
	struct process *prev;
} *proc = NULL;


static int max_sleep;
static pthread_mutex_t lock;
static pthread_attr_t attribute----;
static int no_thread;

void *program_count(void *proces)
{
	struct process *p = proces;
	sleep(p->var);
	printf("Me the process with pid %d ", p->p_id);

	pthread_mutex_lock(&lock);

	release_pid(p->p_id);

	if(p->prev == NULL && p->next == NULL) {
		free(p);
		proc= NULL;
	}
	else {
		if(p->prev != NULL)
			p->prev->next = p->next;
		if(p->next != NULL)
			p->next->prev = p->prev;
		free(p);
	}

	pthread_mutex_unlock(&lock);

	return NULL;
}

void program_new()
{
	struct process *new;
	int id;

	pthread_mutex_lock(&lock);

	id = allocate_pid();
	if(id == 1) {
		pthread_mutex_unlock(&lock);
		return;
	}

	new = malloc(sizeof *new);
	new->p_id = id;
	new->next = proc;
	new->var = rand() % max_sleep;
	new->prev = NULL;
	if(proc != NULL)
		proc->prev = new;
	proc = new;

	printf("me the  new process with the processid  pid %d\n", new->p_id);

	pthread_mutex_unlock(&lock);

	pthread_create(&new->thread, &attribute, program_count, new);
}

int main(int argc, char **argv)
{
	int i;

	pthread_mutex_init(&lock, NULL);
	pthread_attr_init(&attribute);
	pthread_attr_state(&attr, PTHREAD_CREATE_DETACHED);

	if(!allocate_map()) {
		printf("unable to allocate map\n");
		pthread_mutex_destroy(&lock);
		return -1;
	}

	no_thread = 100;
	max_sleep = 30;

	if(argc == 2) {
		if(strstr(argv[1], "-t"))
			sscanf(argv[1], "-t=%d", &no_thread);
		else if(strstr(argv[1], "-s"))
			sscanf(argv[1], "-s=%d", &max_sleep);
	}
	else if(argc == 3) {
		if(strstr(argv[1], "-t"))
			sscanf(argv[1], "-t=%d", &no_thread);
		else if(strstr(argv[1], "-s"))
			sscanf(argv[1], "-s=%d", &max_sleep);
		if(strstr(argv[2], "-t"))
			sscanf(argv[2], "-t=%d", &no_thread);
		else if(strstr(argv[2], "-s"))
			sscanf(argv[2], "-s=%d", &max_sleep);
	}
	else if(argc > 3) {
		printf("Inavlid arguments\n");
		printf("Continuing with default values\n");
	}

	for(i = 0; i < no_thread; i++)
		program_new();

	while(proc != NULL);

	deallocate_map();
	pthread_attr_destroy(&attribute);
	pthread_mutex_destroy(&lock);

	return 0;
}
