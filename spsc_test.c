#include <stdio.h>
#include "spsc.h"
#include <pthread.h>
#include <unistd.h>
#include <time.h>

struct timespec start, end;
size_t ops = 0;
bool done = false;

void* worker(void* p){
    spscq* q = (spscq*) p;
    size_t c = 0;
    size_t n = 0;
    while(1){
        int* i = spscq_pop(q);
        if(i){
           c++;
           n += *i; 
        }
        if(c == ops) break;
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    

    size_t t = (end.tv_sec - start.tv_sec)*1000;
    t += (end.tv_nsec - start.tv_nsec)/1e6;

    printf("ops: %lu  elapsed: %lums  op/num: %dns\n", c,t, (int)(t*1e6/ops)); 
    done = true;  
}

int roundup2(int v);

int main(int argc, char** argv){
    {
        int a[10] = {1,1,1,1,1,1,1,1,1,1};

        spscq* q = spscq_new(100);
        ops = 1e7;
        done = false;

        printf("\n*********** q.size: 100 ************\n");
        pthread_t th;
        pthread_create(&th, NULL, worker, q);
        pthread_detach(th);
        
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
       
        for(int i = 0; i < ops; i++){
            if(!spscq_push(q,&a[0])){
                i--;
            }
        }

        while(!done);
        printf("\n***********************\n");
    
    }

    {
        int a[10] = {1,1,1,1,1,1,1,1,1,1};

        spscq* q = spscq_new(1024*1024);
        ops = 1e8;
        done = false;

        printf("\n********** q.size: 64*1024 ************\n");
        pthread_t th;
        pthread_create(&th, NULL, worker, q);
        pthread_detach(th);
        
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
       
        for(int i = 0; i < ops; i++){
            if(!spscq_push(q,&a[0])){
                usleep(10);
                i--;
            }
        }

        while(!done);
        printf("\n***************************************\n");
    
    }

    {
        int a[10] = {1,1,1,1,1,1,1,1,1,1};

        spscq* q = spscq_new(1024);
        ops = 1e8;
        done = false;

        printf("\n********** q.size: 1024 ************\n");
        pthread_t th;
        // pthread_create(&th, NULL, worker, q);
        // pthread_detach(th);
        
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

        int c = 0;
        for(int i = 0; i < ops; i++){
            spscq_push(q,&a[0]);
            spscq_pop(q);
        }

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    

        size_t t = (end.tv_sec - start.tv_sec)*1000;
        t += (end.tv_nsec - start.tv_nsec)/1e6;

        printf("ops: %u  elapsed: %lums  op/num: %dns\n", c,t, (int)(t*1e6/ops)); 
        done = true;  

        printf("\n***************************************\n");
    
    }


}