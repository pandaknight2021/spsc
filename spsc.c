#include <stdbool.h>
#include "spsc.h"

static int roundup2(int v);

spscq* spscq_new(int size)
{
    spscq* q = malloc(sizeof(spscq));
	if(!q) return NULL;

    size = roundup2(size);
    q->size = size - 1;
    q->w = q->r = 0;
    q->buf = malloc(size * sizeof(void*));

    if(!q->buf){
        free(q);
        return NULL;
    }

    return q;
}

void spscq_delete(spscq* q)
{
 	if(!q) return;
   
    if(q->buf){
        free(q->buf);
        q->buf = NULL;
    }
    free(q);
}



//only be safe to call from single thread, if q is full , return false
bool spscq_push(spscq* q, void* v)
{
    int n = q->w - q->r;

	if (n <= q->size) {
		q->buf[q->w & q->size] = v;
        q->w++;
		return true;
	} 
	
    return false;
}


int spscq_size(spscq* q)  
{
	return q->w - q->r;
}

bool spscq_empty(spscq* q) 
{
	return spscq_size(q) == 0;
}

//must be call from single consumer
void* spscq_pop(spscq* q)
{
	if (q->w - q->r > 0) {
        void* p = q->buf[q->r & q->size];
        q->r++;
        return p;
	}
	return NULL;
}

//must be call from single consumer
void* spscq_peek(spscq* q) 
{
	if (q->w - q->r > 0) {
        return q->buf[q->r & q->size];
	}
	return NULL;
}

//roundup to power of 2 for bitwise modulus: x % n == x & (n - 1).
static int roundup2(int v){
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	//v |= v >> 32
	v++;
	return v;
}