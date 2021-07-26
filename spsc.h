// implement an efficient bounded queue, single-producer, single-consumer lock-free queue.


#ifndef __SPSC_H__
#define __SPSC_H__

#include <stdbool.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct spscq_t {
	int w;
	int r;
    int size;
    void* *buf;
}spscq;



spscq* spscq_new(int size);

void spscq_delete(spscq* q);

bool spscq_push(spscq* q, void* v);

void* spscq_pop(spscq* q);

void* spscq_peek(spscq* q); 

int spscq_size(spscq* q);

bool spscq_empty(spscq* q);


#ifdef __cplusplus
}
#endif

#endif