
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

sem_t agentSem,
    tobacco,
    paper,
    match;

bool isTobacco = false,
    isPaper = false, 
    isMatch = false;

sem_t tobaccoSem,
    paperSem,
    matchSem,
    mutex;
    

// sleeping function

void fiftySleep(void)
{
    time_t t;
    srand((unsigned) time(&t));
    int time = rand() % 50000;
    usleep(time);
}

void twoHunnidSleep(void)
{
    int time = rand() % 200000;
    usleep(time);
}




   // 3 agent Threads
// 
// TODO : Make each agent run 6 times AKA ADD COUNTER
// AND add Couts so i know what's going on.

    void * agentTobPap(void * arg) {
        int count = 0;
        while (count < 6)
        {
            twoHunnidSleep(); // Zzzzz
            sem_wait(&agentSem); // sem Wait - > P()
            printf("agent is handing out tobacco and paper \n");
            sem_post(&tobacco); // sem post - > V()
            sem_post(&paper); // maybe add V and P function, we need to declare by reference
        printf("agent exiting  providing tobacco and paper. \n");
        count++;
       }
	   pthread_exit(0);
    }

    void* agentTobMat(void * arg) {

        int count = 0;
        while (count < 6)
        {
            twoHunnidSleep(); // Zzzzz
            sem_wait(&agentSem);
            printf("agent is handing out tobacco and matches \n");
            sem_post(&tobacco);
            sem_post(&match);
            printf("agent exiting  providing tobacco and matches. \n");
            count++;
        }
		pthread_exit(0);
    }

    void* agentMatPap(void * arg) {

        int count = 0;
        while (count < 6)
        {
            twoHunnidSleep(); // Zzzzz
            sem_wait(&agentSem);
            printf("agent is handing out matches and paper \n");
            sem_post(&match);
            sem_post(&paper);
            printf("agent exiting  providing matches and paper. \n");
            count++;
        }
		pthread_exit(0);
   }

// pusher threads !!!!!
// and cout's so I know what's going on
// and make sure counter is working right

void * tPusher(void * arg) // tobacco pusher
{
    int count = 0;
    while (count < 12) // the pushers should each loop 12 times
    {
        sem_wait(&tobacco); // wait for tobacco and critical section to be released.
        sem_wait(&mutex);
		printf("placing out the tobacco on table \n");
        if (isPaper) // if paper is found, paper pusher has already ran
        {
            isPaper = false;
            sem_post(&matchSem); // signal smoker w/ matches
        }
        else if (isMatch) // if matches are found, match pusher already has ran
        {
            isMatch = false;
            sem_post(&paperSem); // signal smokeer w/ papers
            
        }
        else // if this is the case, this is the first pusher to run. 
        { 
            isTobacco = true; 
        }
        sem_post(&mutex);
        count++;
       
    }
     printf("tobacco puisher exiting. \n");
	 pthread_exit(0);
}

void * pPusher(void * arg) // paper pusher
{
    int count = 0;
    while (count < 12) // the pushers should each loop 12 times
    {
        sem_wait(&paper); // wait for paper and critical section to be released.
        sem_wait(&mutex);
		printf("placing out the paper on table \n"); // SOMETHING IS WRONG HERE
        if (isTobacco)
        {
            isTobacco = false;
            sem_post(&matchSem);
        }
        else if (isMatch)
        {
            isMatch = false;
            sem_post(&tobaccoSem);
        }
        else
        {
            isPaper = true;
        }
        sem_post(&mutex);
        count++;
    }
    printf("paper pusher exiting. \n");
	pthread_exit(0);
}

void * mPusher(void * arg) // match pusher
{
    int count = 0;
    while (count < 12) // the pushers should each loop 12 times
    {
        sem_wait(&match); // wait for paper and critical section to be released.
        sem_wait(&mutex); // - > P
		printf("placing out the match on table \n");
        if (isTobacco)
        {
            isTobacco = false;
            sem_post(&paperSem); // V
        }
        else if (isPaper)
        {
            isPaper = false;
            sem_post(&tobaccoSem); // V
        }
        else
        {
            isMatch = true; // found the error here <-
        }
        sem_post(&mutex);
        count++;
    }
    printf("Match Pusher exiting. \n");
	pthread_exit(0);
}



// smoker threads
// TODO: 
// ADD counters, and waiting !


void * tobaccoSmoker(void * arg) // each smoker will finish 3 cigarettes before leaving, loop 3 times
{
    
    int count = 0;
    while (count < 3)
    {
        sem_wait(&tobaccoSem); // sem_wait - > P
        //Make a cigarette
        printf("Smoker w/ tobacco: making cigarette \n");
        fiftySleep(); // ZzzZ
        sem_post(&agentSem); // sem_post - V
        printf("Smoker w/ tobacco: smoking cigarette. \n");
        //Smoke the cigarette
        fiftySleep(); // ZzZz
        count++;
    }
    printf("### Smoker w/ tobacco smoked 3 cigarettes and is leaving \n");
	pthread_exit(0);
}

void * paperSmoker(void * arg)
{
    int count = 0;
    while (count < 3)
    {
        sem_wait(&paperSem);
        //Make a cigarette
        printf("Smoker w/ paper: making cigarette. \n");
        fiftySleep(); // ZzZz
        
        sem_post(&agentSem);
        //Smoke the cigarette
        printf("Smoker w/ paper: smoking cigarette. \n");
        fiftySleep(); // ZzZz
        count++;
    }
    printf("### Smoker w/ paper smoked 3 cigarettes and is leaving. \n");
	pthread_exit(0);
}

void * matchSmoker(void * arg)
{
    int count = 0;
    while (count < 3)
    {
        sem_wait(&matchSem); // p
        //Make a cigarette
        printf("Smoker w/ matches: making cigarette \n");
        fiftySleep(); // ZzzZ
        sem_post(&agentSem); // V
        printf("Smoker w/ matches: smoking cigarette. \n");
        //Smoke the cigarette
        fiftySleep(); // ZzZz
        count++;
    }
    printf("### Smoker w/ tobacco smoked 3 cigarettes and is leaving \n");
	pthread_exit(0);
}



int main() {

    sem_init(&mutex, 0, 1);
    sem_init(&agentSem, 0, 1);
    sem_init(&tobaccoSem, 0, 0);
    sem_init(&paperSem, 0, 0);
    sem_init(&matchSem, 0, 0);
    sem_init(&paper, 0, 0);
    sem_init(&match, 0, 0);
    sem_init(&tobacco, 0, 0);


    pthread_t agentTobPap_t, agentTobMat_t, agentMatPap_t; // I am adding _T so I don't get confused

    pthread_t tobaccoPusher_t, paperPusher_t, matchPusher_t;

    pthread_t smokerWTobacco_t, smokerWPaper_t, smokerWMatch_t;
    pthread_t smokerWTobacco2_t, smokerWPaper2_t, smokerWMatch2_t;

    // creating 3 agent threads

    pthread_create(&agentTobPap_t, NULL, agentTobPap, NULL);
    pthread_create(&agentTobMat_t, NULL, agentTobMat, NULL);
    pthread_create(&agentMatPap_t, NULL, agentMatPap, NULL);
    

    // creating 3 pusher threads
    pthread_create(&tobaccoPusher_t, NULL, tPusher, NULL);
    pthread_create(&paperPusher_t, NULL, pPusher, NULL);
    pthread_create(&matchPusher_t, NULL, mPusher, NULL);

    // creating 6 smoker threads
    pthread_create(&smokerWTobacco_t, NULL, tobaccoSmoker, NULL);
    pthread_create(&smokerWTobacco2_t, NULL, tobaccoSmoker, NULL);
    pthread_create(&smokerWPaper_t, NULL, paperSmoker, NULL);
    pthread_create(&smokerWPaper2_t, NULL, paperSmoker, NULL);
    pthread_create(&smokerWMatch_t, NULL, matchSmoker, NULL);
    pthread_create(&smokerWMatch2_t, NULL, matchSmoker, NULL);


// joining all the threads before the program terminates ! :)
    pthread_join(agentTobPap_t, NULL);
    pthread_join(agentTobMat_t, NULL);
    pthread_join(agentMatPap_t, NULL);
    pthread_join(tobaccoPusher_t, NULL);
    pthread_join(paperPusher_t, NULL);
    pthread_join(matchPusher_t, NULL);
    pthread_join(smokerWTobacco_t, NULL);
    pthread_join(smokerWPaper_t, NULL);
    pthread_join(smokerWMatch_t, NULL);
    pthread_join(smokerWTobacco2_t, NULL);
    pthread_join(smokerWPaper2_t, NULL);
    pthread_join(smokerWMatch2_t, NULL);
    
}
