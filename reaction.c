#include "pintos_thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction {
	int h;
	int o;
	struct lock myLock;
	struct condition hWait;
	struct condition oWait;
};

void
reaction_init(struct reaction *reaction) {
	reaction -> h = 0;
	reaction -> o = 0;
	lock_init(&reaction -> myLock);
	cond_init(&reaction -> hWait);
	cond_init(&reaction -> oWait);
}

void
reaction_h(struct reaction *reaction) {
	lock_acquire(&reaction -> myLock);
	(reaction -> h)++;
	if(reaction -> h >= 2 && reaction -> o >= 1) {
		(reaction -> h) -= 2;
		(reaction -> o)--;
		make_water();
		cond_signal(&reaction -> hWait, &reaction -> myLock);
		cond_signal(&reaction -> oWait, &reaction -> myLock);
	}
	else
		cond_wait(&reaction -> hWait, &reaction -> myLock);
	lock_release(&reaction -> myLock);
}

void
reaction_o(struct reaction *reaction) {
	lock_acquire(&reaction -> myLock);
	(reaction -> o)++;
	if(reaction -> h >= 2 && reaction -> o >= 1) {
		(reaction -> h) -= 2;
		(reaction -> o)--;
		make_water();
		cond_signal(&reaction -> hWait, &reaction -> myLock);
		cond_signal(&reaction -> hWait, &reaction -> myLock);
		//cond_signal(&reaction -> oWait, &reaction -> myLock);
	}
	else
		cond_wait(&reaction -> oWait, &reaction -> myLock);
	lock_release(&reaction -> myLock);	
}
