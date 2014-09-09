/*
c-pthread-queue - c implementation of a bounded buffer queue using posix threads
Copyright (C) 2008  Matthew Dickinson

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

#ifdef PQUE_TEST

// Size of the shared queue buffer
// Must be greater than zero
const int buffer_size = 10;
// Number of producer threads to start
// Must be greater than zero
const int producers = 10;
// Number of integers to be produced by each producer thread
// Must be greater than zero
const int producer_amount = 100;
// Number of consumer threads to start
// Must be greater than zero
const int consumers = 5;
// producers * producer_amount / consumers must be a whole integer

void *producer(void *arg)
{
	int i;
	for (i = 0; i < producer_amount; ++ i)
	{
		int *value = malloc(sizeof(*value));
		*value = i;
		queue_enqueue(arg, value);
	}
}

void *consumer(void *arg)
{
	int i;
	for (i = 0; i < producer_amount * producers / consumers; ++ i)
	{
		int *value = queue_dequeue(arg);
		free(value);			
	}
}

int main()
{
	void *buffer[buffer_size];
	queue_t queue = QUEUE_INITIALIZER(buffer);
	
	int i;
	pthread_t producers[producers];
	for (i = 0; i < sizeof(producers) / sizeof(producers[0]); ++ i)
	{
		pthread_create(&producers[i], NULL, producer, &queue);
	}
	pthread_t consumers[consumers];
	for (i = 0; i < sizeof(consumers) / sizeof(consumers[0]); ++ i)
	{
		pthread_create(&consumers[i], NULL, consumer, &queue);
	}
	for (i = 0; i < sizeof(producers) / sizeof(producers[0]); ++ i)
	{
		pthread_join(producers[i], NULL);
	}
	for (i = 0; i < sizeof(consumers) / sizeof(consumers[0]); ++ i)
	{
		pthread_join(consumers[i], NULL);
	}
}

#endif

void queue_enqueue(queue_t *queue, void *value)
{
	pthread_mutex_lock(&(queue->mutex));
	while (queue->size == queue->capacity)
		pthread_cond_wait(&(queue->cond_full), &(queue->mutex));
	printf("enqueue %d\n", *(int *)value);
	queue->buffer[queue->in] = value;
	++ queue->size;
	++ queue->in;
	queue->in %= queue->capacity;
	pthread_mutex_unlock(&(queue->mutex));
	pthread_cond_broadcast(&(queue->cond_empty));
}

void *queue_dequeue(queue_t *queue)
{
	pthread_mutex_lock(&(queue->mutex));
	while (queue->size == 0)
		pthread_cond_wait(&(queue->cond_empty), &(queue->mutex));
	void *value = queue->buffer[queue->out];
	printf("dequeue %d\n", *(int *)value);
	-- queue->size;
	++ queue->out;
	queue->out %= queue->capacity;
	pthread_mutex_unlock(&(queue->mutex));
	pthread_cond_broadcast(&(queue->cond_full));
	return value;
}

int queue_size(queue_t *queue)
{
	pthread_mutex_lock(&(queue->mutex));
	int size = queue->size;
	pthread_mutex_unlock(&(queue->mutex));
	return size;
}
