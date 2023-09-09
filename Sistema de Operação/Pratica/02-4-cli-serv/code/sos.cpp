/*
 *  \brief SoS: Statistics on Strings, a simple client-server application
 *    that computes some statistics on strings
 *
 * \author (2022) Artur Pereira <artur at ua.pt>
 * \author (2022) Miguel Oliveira e Silva <mos at ua.pt>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <stdint.h>

#include <new>

#include "sos.h"

#include "dbc.h"

/*
 * TODO point
 * Uncomment the #include that applies
 */
//#include "process.h"
#include "thread.h"

namespace sos
{
    /** \brief Number of transaction buffers */
    #define  NBUFFERS         5

    /** \brief indexes for the fifos of free buffers and pending requests */
    enum { FREE_BUFFER=0, PENDING_REQUEST };

    /** \brief interaction buffer data type */
    struct BUFFER 
    {
        char req[MAX_STRING_LEN+1];
        Response resp;
    };

    /** \brief the fifo data type to store indexes of buffers */
    struct FIFO
    {
        uint32_t ii;               ///< point of insertion
        uint32_t ri;               ///< point of retrieval
        uint32_t cnt;              ///< number of items stored
        uint32_t tokens[NBUFFERS]; ///< storage memory
    };

    /** \brief the data type representing all the shared area.
     *    Fifo 0 is used to manage tokens of free buffers.
     *    Fifo 1 is used to manage tokens of pending requests.
     */
    struct SharedArea
    {
        /* A fix number of transaction buffers */
        BUFFER pool[NBUFFERS];

        /* A fifo for tokens of free buffers and another for tokens with pending requests */
        FIFO fifo[2];

        /*
         * TODO point
         * Declare here all you need to accomplish the synchronization,
         * semaphores (for implementation using processes) or
         * mutexes, conditions and condition variables (for implementation using threads)
         */
         /*static*/ pthread_mutex_t bufferaccess;
         /*static*/ pthread_mutex_t fiforaccess[2]; // = PTHREAD_MUTEX_INITIALIZER;
	     /*static*/ pthread_cond_t fifoNotFull[2]; //= PTHREAD_COND_INITIALIZER;
	     /*static*/ pthread_cond_t fifoNotEmpty[2]; //= PTHREAD_COND_INITIALIZER;
	     /*static*/ pthread_cond_t awserd[NBUFFERS];
	     bool _awserd[NBUFFERS];
    };

    /** \brief pointer to shared area dynamically allocated */
    SharedArea *sharedArea = NULL;


    /* -------------------------------------------------------------------- */

    /* Allocate and init the internal supporting data structure,
     *   including all necessary synchronization resources
     */
    void open(void)
    {
#if __DEBUG__
        fprintf(stderr, "%s()\n", __FUNCTION__);
#endif

        require(sharedArea == NULL, "Shared area must not exist");

        /* 
         * TODO point FEITO
         * Allocate the shared memory
         */
	    sharedArea = new SharedArea();

        /* init fifo 0 (free buffers) */
        FIFO *fifo = &sharedArea->fifo[FREE_BUFFER];
        for (uint32_t i = 0; i < NBUFFERS; i++)
        {
            fifo->tokens[i] = i;
        }
        fifo->ii = fifo->ri = 0;
        fifo->cnt = NBUFFERS;

        /* init fifo 1 (pending requests) */
        fifo = &sharedArea->fifo[PENDING_REQUEST];
        for (uint32_t i = 0; i < NBUFFERS; i++)
        {
            fifo->tokens[i] = NBUFFERS; // used to check for errors
        }
        fifo->ii = fifo->ri = 0;
        fifo->cnt = 0;

        /* 
         * TODO point
         * Init synchronization elements
         */
         sharedArea->bufferaccess = PTHREAD_MUTEX_INITIALIZER;
         for(uint32_t i = 0; i < NBUFFERS; i++){
			  sharedArea->awserd[i] = PTHREAD_COND_INITIALIZER;
			  sharedArea->_awserd[i] = false; 
		 }
		 for(uint32_t i = 0; i < 2; i++){
				sharedArea->fiforaccess[i] =  PTHREAD_MUTEX_INITIALIZER;
				sharedArea->fifoNotEmpty[i] = PTHREAD_COND_INITIALIZER;
				sharedArea->fifoNotFull[i]  = PTHREAD_COND_INITIALIZER;
		 }
			 
    }

    /* -------------------------------------------------------------------- */

    /* Free all allocated synchronization resources and data structures */
    void close()
    {
        require(sharedArea != NULL, "sharea area must be allocated");

        /* 
         * TODO point
         * Destroy synchronization elements
         */

        /* 
         * TODO point
        *  Destroy the shared memory
        */
        delete sharedArea;

        /* nullify */
        sharedArea = NULL;
    }

    /* -------------------------------------------------------------------- */
    /* -------------------------------------------------------------------- */

    /* Insertion a token into a fifo */
    static void fifoIn(uint32_t idx, uint32_t token)
    {
#if __DEBUG__
        fprintf(stderr, "%s(idx: %u, token: %u)\n", __FUNCTION__, idx, token);
#endif

        require(idx == FREE_BUFFER or idx == PENDING_REQUEST, "idx is not valid");
        require(token < NBUFFERS, "token is not valid");

        /* 
         * TODO point
         * Replace with your code, 
         * avoiding race conditions and busy waiting
         */
         mutex_lock(&sharedArea->fiforaccess[idx]);
         while(sharedArea->fifo[idx].cnt < NBUFFERS){
				cond_wait(&sharedArea->fifoNotFull[idx], &sharedArea->fiforaccess[idx]);
		 }
		 sharedArea->fifo[idx].tokens[sharedArea->fifo[idx].ii] = token;
		 sharedArea->fifo[idx].ii = (sharedArea->fifo[idx].ii + 1) % NBUFFERS;
		 sharedArea->fifo[idx].cnt++;
		 cond_broadcast(&sharedArea->fifoNotEmpty[idx]);
		 mutex_unlock(&sharedArea->fiforaccess[idx]);
    }

    /* -------------------------------------------------------------------- */

    /* Retrieve a token from a fifo  */

    static uint32_t fifoOut(uint32_t idx)
    {
#if __DEBUG__
        fprintf(stderr, "%s(idx: %u)\n", __FUNCTION__, idx);
#endif

        require(idx == FREE_BUFFER or idx == PENDING_REQUEST, "idx is not valid");

        /* 
         * TODO point
         * Replace with your code, 
         * avoiding race conditions and busy waiting
         */
         mutex_lock(&sharedArea->fiforaccess[idx]);
         while(sharedArea->fifo[idx].cnt == 0){
				cond_wait(&sharedArea->fifoNotEmpty[idx], &sharedArea->fiforaccess[idx]);
		 }
		 uint32_t val_res = sharedArea->fifo[idx].tokens[sharedArea->fifo[idx].ri];
		 sharedArea->fifo[idx].ri = (sharedArea->fifo[idx].ri + 1) % NBUFFERS;
		 sharedArea->fifo[idx].cnt--;
		 cond_broadcast(&sharedArea->fifoNotFull[idx]);
		 mutex_unlock(&sharedArea->fiforaccess[idx]);
		 return val_res;
    }

    /* -------------------------------------------------------------------- */
    /* -------------------------------------------------------------------- */

    uint32_t getFreeBuffer()
    {
#if __DEBUG__
        fprintf(stderr, "%s()\n", __FUNCTION__);
#endif

        /* 
         * TODO point
         * Replace with your code, 
         */
         return fifoOut(FREE_BUFFER);
    }

    /* -------------------------------------------------------------------- */

    void putRequestData(uint32_t token, const char *data)
    {
#if __DEBUG__
        fprintf(stderr, "%s(token: %u, ...)\n", __FUNCTION__, token);
#endif

        require(token < NBUFFERS, "token is not valid");
        require(data != NULL, "data pointer can not be NULL");

        /* 
         * TODO point
         * Replace with your code, 
         */
         mutex_lock(&sharedArea->bufferaccess);
         memcpy(&sharedArea->pool[token].req,data,(size_t)(MAX_STRING_LEN + 1));
         mutex_unlock(&sharedArea->bufferaccess);
    }

    /* -------------------------------------------------------------------- */

    void submitRequest(uint32_t token)
    {
#if __DEBUG__
        fprintf(stderr, "%s(token: %u)\n", __FUNCTION__, token);
#endif

        require(token < NBUFFERS, "token is not valid");

        /* 
         * TODO point
         * Replace with your code, 
         */
         fifoIn(PENDING_REQUEST,token);
    }

    /* -------------------------------------------------------------------- */

    void waitForResponse(uint32_t token)
    {
#if __DEBUG__
        fprintf(stderr, "%s(token: %u)\n", __FUNCTION__, token);
#endif

        require(token < NBUFFERS, "token is not valid");

        /* 
         * TODO point
         * Replace with your code, 
         * avoiding race conditions and busy waiting
         */
         mutex_lock(&sharedArea->bufferaccess);
         while(sharedArea->_awserd[token] == false){
		     cond_signal(&sharedArea->awserd[token]);	 
		}
		mutex_unlock(&sharedArea->bufferaccess);
    }

    /* -------------------------------------------------------------------- */

    void getResponseData(uint32_t token, Response *resp)
    {
#if __DEBUG__
        fprintf(stderr, "%s(token: %u, ...)\n", __FUNCTION__, token);
#endif

        require(token < NBUFFERS, "token is not valid");
        require(resp != NULL, "resp pointer can not be NULL");

        /* 
         * TODO point
         * Replace with your code, 
         */
         mutex_lock(&sharedArea->bufferaccess);
         resp->noChars = sharedArea->pool[token].resp.noChars;
         resp->noDigits = sharedArea->pool[token].resp.noDigits;
         resp->noLetters = sharedArea->pool[token].resp.noLetters;
         mutex_unlock(&sharedArea->bufferaccess);
         
    }

    /* -------------------------------------------------------------------- */

    void releaseBuffer(uint32_t token)
    {
#if __DEBUG__
        fprintf(stderr, "%s(token: %u)\n", __FUNCTION__, token);
#endif

        require(token < NBUFFERS, "token is not valid");

        /* 
         * TODO point
         * Replace with your code, 
         */
         mutex_lock(&sharedArea->bufferaccess);
         memset(sharedArea->pool[token].req, '\0', (size_t)(MAX_STRING_LEN+1));
         sharedArea->_awserd[token] = false;
         mutex_unlock(&sharedArea->bufferaccess);
         fifoIn(FREE_BUFFER,token);
    }

    /* -------------------------------------------------------------------- */
    /* -------------------------------------------------------------------- */

    uint32_t getPendingRequest()
    {
#if __DEBUG__
        fprintf(stderr, "%s()\n", __FUNCTION__);
#endif

        /* 
         * TODO point
         * Replace with your code, 
         */
         return fifoOut(PENDING_REQUEST);
    }

    /* -------------------------------------------------------------------- */

    void getRequestData(uint32_t token, char *data)
    {
#if __DEBUG__
        fprintf(stderr, "%s(token: %u, ...)\n", __FUNCTION__, token);
#endif

        require(token < NBUFFERS, "token is not valid");
        require(data != NULL, "data pointer can not be NULL");

        /* 
         * TODO point
         * Replace with your code, 
         */
         mutex_lock(&sharedArea->bufferaccess);
         memcpy(data,&sharedArea->pool[token].req, (size_t)(MAX_STRING_LEN + 1));
         mutex_unlock(&sharedArea->bufferaccess);
    }

    /* -------------------------------------------------------------------- */

    void putResponseData(uint32_t token, Response *resp)
    {
#if __DEBUG__
        fprintf(stderr, "%s(token: %u, ...)\n", __FUNCTION__, token);
#endif

        require(token < NBUFFERS, "token is not valid");
        require(resp != NULL, "resp pointer can not be NULL");

        /* 
         * TODO point
         * Replace with your code, 
         */
         mutex_lock(&sharedArea->bufferaccess);
         sharedArea->pool[token].resp.noChars   = resp->noChars;
         sharedArea->pool[token].resp.noDigits  = resp->noDigits;
         sharedArea->pool[token].resp.noLetters = resp->noLetters;
         sharedArea->_awserd[token] = false;
         mutex_unlock(&sharedArea->bufferaccess);
    }

    /* -------------------------------------------------------------------- */

    void notifyClient(uint32_t token)
    {
#if __DEBUG__
        fprintf(stderr, "%s(token: %u)\n", __FUNCTION__, token);
#endif

        require(token < NBUFFERS, "token is not valid");

        /* 
         * TODO point
         * Replace with your code, 
         * avoiding race conditions and busy waiting
         */
         mutex_lock(&sharedArea->bufferaccess);
			cond_broadcast(&sharedArea->awserd[token]); 
         mutex_unlock(&sharedArea->bufferaccess);
    }

    /* -------------------------------------------------------------------- */

}
