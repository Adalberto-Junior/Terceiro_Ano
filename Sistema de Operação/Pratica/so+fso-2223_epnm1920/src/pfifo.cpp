#include <dbc.h>
#include <string.h>
#include "pfifo.h"

#include "thread.h"
//#include "process.h"


	//static pthread_mutex_t pfifoAcess = PTHREAD_MUTEX_INITIALIZER;
	//static pthread_cond_t fifoNotFull = PTHREAD_COND_INITIALIZER;
	//static pthread_cond_t fifoNotEmpety = PTHREAD_COND_INITIALIZER;

/* changes may be required to this function */
void init_pfifo(PriorityFIFO* pfifo)
{
   require (pfifo != NULL, "NULL pointer to FIFO");   // a false value indicates 
   memset(pfifo->array, 0, sizeof(pfifo->array));
   pfifo->inp = pfifo->out = pfifo->cnt = 0;
   mutex_init(&pfifo->pfifoAcess,NULL);
   cond_init(&pfifo->fifoNotFull,NULL);
   cond_init(&pfifo->fifoNotEmpety,NULL);
}

/* --------------------------------------- */

int empty_pfifo(PriorityFIFO* pfifo)
{
   require (pfifo != NULL, "NULL pointer to FIFO");   // a false value indicates a program error

   return pfifo->cnt == 0;
}

/* --------------------------------------- */

int full_pfifo(PriorityFIFO* pfifo)
{
   require (pfifo != NULL, "NULL pointer to FIFO");   // a false value indicates a program error

   return pfifo->cnt == FIFO_MAXSIZE;
}

/* --------------------------------------- */

/* changes may be required to this function */
void insert_pfifo(PriorityFIFO* pfifo, uint32_t id, uint32_t priority)
{
   mutex_lock(&pfifo->pfifoAcess);
   require (pfifo != NULL, "NULL pointer to FIFO");   // a false value indicates a program error
 
   require (id <= MAX_ID, "invalid id");              // a false value indicates a program error
   require (priority > 0 && priority <= MAX_PRIORITY, "invalid priority value");  // a false value indicates a program error
   //require (!full_pfifo(pfifo), "full FIFO");         // in a shared fifo, it may not result from a program error!

   //printf("[insert_pfifo] value=%d, priority=%d, pfifo->inp=%d, pfifo->out=%d\n", id, priority, pfifo->inp, pfifo->out);
  // print_pfifo(pfifo);

   while(full_pfifo(pfifo)){
   	cond_wait(&pfifo->fifoNotFull, &pfifo->pfifoAcess);
   
   }
   uint32_t idx = pfifo->inp;
   uint32_t prev = (idx + FIFO_MAXSIZE - 1) % FIFO_MAXSIZE;
   while((idx != pfifo->out) && (pfifo->array[prev].priority > priority))
   {
     // printf("[insert_pfifo] idx=%d, prev=%d\n", idx, prev);
      pfifo->array[idx] = pfifo->array[prev];
      idx = prev;
      prev = (idx + FIFO_MAXSIZE - 1) % FIFO_MAXSIZE;
   }
  // print_pfifo(pfifo);
  //: printf("[insert_pfifo] idx=%d, prev=%d\n", idx, prev); //mudei aqui;
   pfifo->array[idx].id = id;
   pfifo->array[idx].priority = priority;
   pfifo->inp = (pfifo->inp + 1) % FIFO_MAXSIZE;
   pfifo->cnt++;
   // printf("[insert_pfifo] pfifo->inp=%d, pfifo->out=%d\n", pfifo->inp, pfifo->out);
   // printf("pfifo[%d] = %u\n", pfifo->inp, pfifo->array[pfifo->inp].id);
   // print_pfifo(pfifo);
   cond_broadcast(&pfifo->fifoNotEmpety);
   mutex_unlock(&pfifo->pfifoAcess);
}

/* --------------------------------------- */

/* changes may be required to this function */
uint32_t retrieve_pfifo(PriorityFIFO* pfifo)
{
   mutex_lock(&pfifo->pfifoAcess);//aqui---
   require (pfifo != NULL, "NULL pointer to FIFO");   // a false value indicates a program error
 // require (!empty_pfifo(pfifo), "empty FIFO");       // in a shared fifo, it may not result from a program error!

   printf("fffffffffffffffff\n");
   while(empty_pfifo(pfifo)){
   printf("fffffffffffffffff\n");
   	cond_wait(&pfifo->fifoNotEmpety, &pfifo->pfifoAcess);
   printf("fffffffffffffffff\n");
   }//aqui!!!!!!!!!!!!!!!!!!

   check_valid_id(pfifo->array[pfifo->out].id);
   check_valid_priority(pfifo->array[pfifo->out].priority);

   //printf("Antes do Retrive: Array[%u] = %u \n", pfifo->out, pfifo->array[pfifo->out].id);


   uint32_t result = pfifo->array[pfifo->out].id;
   pfifo->array[pfifo->out].id = INVALID_ID;
   pfifo->array[pfifo->out].priority = INVALID_PRIORITY;
   pfifo->out = (pfifo->out + 1) % FIFO_MAXSIZE;
   pfifo->cnt--;
   pfifo->out = (pfifo->out + 1) % FIFO_MAXSIZE;
   pfifo->cnt--;
  // print_pfifo(pfifo);

   // update priority of all remaing elements (increase priority by one)
   uint32_t idx = pfifo->out;
   for(uint32_t i = 1; i <= pfifo->cnt; i++)
   {
      if (pfifo->array[idx].priority > 1 && pfifo->array[idx].priority != INVALID_PRIORITY)
         pfifo->array[idx].priority--;
      idx = (idx + 1) % FIFO_MAXSIZE;
   }

    //print_pfifo(pfifo);
     //printf("result = %u \n ",result);
     //printf("depois do retrive: Array[%u] = %u \n", pfifo->out, pfifo->array[pfifo->out].id);
   
   cond_broadcast(&pfifo->fifoNotFull);//Aqui
   mutex_unlock(&pfifo->pfifoAcess);//Aqui

   return result;
}

/* --------------------------------------- */

/* changes may be required to this function */
void print_pfifo(PriorityFIFO* pfifo)
{
   mutex_lock(&pfifo->pfifoAcess);
   require (pfifo != NULL, "NULL pointer to FIFO");   // a false value indicates a program error

   uint32_t idx = pfifo->out;
   for(uint32_t i = 1; i <= pfifo->cnt; i++)
   {
      check_valid_id(pfifo->array[pfifo->out].id);
      check_valid_priority(pfifo->array[pfifo->out].priority);
      printf("[%02d] value = %d, priority = %d\n", i, pfifo->array[idx].id, pfifo->array[idx].priority);
      idx = (idx + 1) % FIFO_MAXSIZE;
   }
   mutex_unlock(&pfifo->pfifoAcess);
}
