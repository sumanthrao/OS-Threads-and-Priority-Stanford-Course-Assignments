#include "pintos_thread.h"
void station_on_board();
struct station {
	// FILL ME IN
	int num_passengers;
	int passengers_in_train;
	int train_capacity;
	struct lock mylock;
	struct condition train_arrived;
	struct condition train_full;
};

void
station_init(struct station *station)
{
	// FILL ME IN
	station->num_passengers=0;
	station->train_capacity=0;
	station->passengers_in_train=0;
	lock_init(&station->mylock);
	cond_init(&station->train_arrived);
	cond_init(&station->train_full);
	printf("initialised");
}

void
station_load_train(struct station *station, int count)
{
	// FILL ME IN
	lock_acquire(&station->mylock);
	station->train_capacity=count;
	while(station->num_passengers>0 && station->train_capacity>0){
		cond_broadcast(&station->train_arrived,&station->mylock);
		cond_wait(&station->train_full,&station->mylock);
	}
	//printf("boarded a passenger\n");
	station->train_capacity=0;
	lock_release(&station->mylock);
}

void
station_wait_for_train(struct station *station)
{
	// FILL ME IN
	lock_acquire(&station->mylock);
	//increase the number of passengers by 1
	station->num_passengers += 1;
	while(station->train_capacity == station->passengers_in_train ){
		cond_wait(&station->train_arrived,&station->mylock);
	}
	station->passengers_in_train++;
	station->num_passengers--;
	//printf(" a passenger was seated\n");
	lock_release(&station->mylock);
}

void
station_on_board(struct station *station)
{
	// FILL ME IN
	lock_acquire(&station->mylock);
	station->train_capacity--;
	station->passengers_in_train--;
	if(station->train_capacity==0 || station->passengers_in_train==0){
		cond_signal(&station->train_full,&station->mylock);
	}
	lock_release(&station->mylock);
}
