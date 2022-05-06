Philosophers
	
This project allows us to understand the difference between processes and threads, how to work with the same memory space, how to manipulate threads and shared memory, understand what mutex and semaphores are. 

 

### Which is the difference between process and thread?

| Process  | Thread |
| --- | --- |
| Process means a whole program is in execution   | Thread means a segment(part of the program) is in execution |
| A program can contain multiple threads  | multiples threads are part of a singe process sharing the same memory space |
| Programs consume more resources | Threads consume less resources |
| Different process are treaded separately by the OS | Threads are treated as a single task by the OS with the same process ID |
| Process are isolated | Threads share memory |
| It does not share data | Threads share data with each other |

### Example thread

```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int x = 2;

void* routine() {
    x += 5;
    sleep(2);
    printf("Value of x: %d\n", x);
}

void* routine2() {
    sleep(2);
    printf("Value of x: %d\n", x);
}

int main(int argc, char* argv[]) {
    pthread_t t1, t2;
    if (pthread_create(&t1, NULL, &routine, NULL)) {
        return 1;
    }
    if (pthread_create(&t2, NULL, &routine2, NULL)) {
        return 2;
    }
    if (pthread_join(t1, NULL)) {
        return 3;
    }
    if (pthread_join(t2, NULL)) {
        return 3;
    }
    return 0;
}
#result
Value of x: 7 Proces Id: 8068
Value of x: 7 Process id: 8068
```

### Example process

```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
    int x = 2;
    int pid = fork();
    if (pid == -1) {
        return 1;
    }
    
    if (pid == 0) {
        x++;
    }
    sleep(2);
    printf("Value of x: %d\n", x);
    if (pid != 0) {
        wait(NULL);
    }
    return 0;
}
#result
Value of x: 3 Process id: 7668
Value of x: 2 Process id: 7666
```

## Functions to be use in the project

### pthread_create(...)

This function allows us to create a thread

Synopsis

```c

#include<pthread.h>
													/*pthread_t data type where we will store the
													thread ID*/
															|             /* we set it to null creat a thread with default parameters */
															|                  |
		                          v                  v
	int pthread_create(pthread_t *thread, const pthread_attr_t *attr,\
										void *(*start_routine)(void *), void *arg);
																^                       ^
																|                   /*args to be passed to our functions */     
														/* the function to be executed with the thread */

/* we need declare a object called pthread_t t(whatever) where we
store information needed by the system as the thread id */

void  routine(void)
{
	printf("this is a thread\n");
}
int main(void)
{
	pthread_t t;
	pthread_create(&t, NULL, &routine, NULL);
	return (0);
}
```

## pthread_join

synopsis

```c
#include <pthread.h>

/* Funtion waits for the thread specified by thread to terminate. if
that thread has already terminated, then thread_join() returns inmediately*/
/* if we want to return something from our function used in pthread_create
we can use the pthread_join function to the return this poiner, is there were the second
argument of pthread_join will healp us */
/* Return value - On success, pthread_join() returns 0; on error, it return an error number */

int pthread_join(pthread_t thread, void **retval)

//example
pthread_join(t, NULL);
```

### Example:

In this example, we are going to populate an int array with 10 prime numbers, the idea is to sum the first half with one thread and the other half with another thread. And return a pointer with the result, then free it. 

```c
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>

/*global array containing 10 prime numbers */
int l_prime[10] = {0};

/* function that will determine prime numbers*/
int prime(int num)
{
    int i = 2;

    if (num < 2)
        return 0;
    while (i < num)
    {
        if ((num % i) == 0)
            return (0);
            i++;
    }   
    return (1);
}

/* function call by pthread_creat, that will canculate fist or second half
depending on the argment passed */
void *routine(void  *arg)
{
    int *result = malloc(sizeof(int));
    *result = 0;
    for (int i = 0; i < 5; i++)
        *result += l_prime[*(int *)arg + i];
    free(arg);
    return (result);
}

int main(void)
{
    int counter = 0;
    int iter = 1;
    pthread_t t[2];
/*loop to populate our array of int*/
    while (counter < 10)
    {
        if (prime(iter))
        {
            l_prime[counter] = iter;
            counter++;
            printf("%d", iter);
            printf("\n");

        }
        iter++;
    }
/*thread creation */
    for (int i = 0; i < 2; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i * 5;
        if (pthread_create(&t[i], NULL, &routine, a) != 0)
        {
            perror("Error creating thread\n");
            exit(-1);
        }
    }
/*thread joining and returning value from routine function that will contain
fist result or second result*/
    for (int i = 0; i < 2; i++)
    {
        int *result;
        if (pthread_join(t[i], (void **)&result) != 0)
            perror ("Error joining thread\n");
        printf("result thread%d: %d\n", i, *result);
    }
    return (0);
}
```

### Personal conclusion over pthread_join

in conclusion, pthread_join help us to wait until the specified thread has finished and to return a pointer from our routine if we needed it.

[How to pass arguments to and get results from threads. (pthread_create, pthread_join)](https://www.youtube.com/watch?v=It0OFCbbTJE)

## Mutex (mutual exclusion)

El algoritmo de exclusión mutua se usa en la programación concurrente para evitar que entre mas de un proceso o thread a la vez en la sección critica. La sección critica es un fragmento de código donde puede modificarse un recurso compartido(una variable o estructura de datos).

[Exclusión mutua (informática) - Wikipedia, la enciclopedia libre](https://es.wikipedia.org/wiki/Exclusi%C3%B3n_mutua_(inform%C3%A1tica))

### Notes:

We need to take into account that pthread_join is not enough to help us wait up until  one thread is finish in certain conditions to execute the next one.  As an example let’s consider the next code:

```c
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int mails = 0;

void* routine() {
    for (int i = 0; i < 1000000; i++) {
        mails++;
        // read mails
        // increment
        // write mails
    }
}

int main(int argc, char* argv[]) {
    pthread_t p1, p2, p3, p4;
    if (pthread_create(&p1, NULL, &routine, NULL) != 0) {
        return 1;
    }
    if (pthread_create(&p2, NULL, &routine, NULL) != 0) {
        return 2;
    }
    if (pthread_join(p1, NULL) != 0) {
        return 3;
    }
    if (pthread_join(p2, NULL) != 0) {
        return 4;
    }
    printf("Number of mails: %d\n", mails);
    return 0;
}
//result
Number of mails: 1337879
```

Logically our result should be 2.000.000 as we are executing 2 threads with the same routine adding 1.000.000 to mails variable each time. However, we’ve got an undefined result due to the fact that the for loop is still looping when the second thread starts.  

In order to avoid this demeanor, we will use a set of functions that will allow us to implement a mutex algorithm.

 

### pthreat_mutex_t mutex;

Data type that will store information regarding our mutex algorithm;

### pthread_mutex_init(&mutex, NULL);

This function will initialize our mutex variable.

### pthread_mutex_destroi(&mutex);

This function will destroy our mutex variable.

### pthread_mutex_lock(&mutex)

Will lock our critical section(variable or data type to be modified) so that any other thread can have access to it up until the current one has finished.

### pthread_mutex_unlock(&mutex)

unlock our critical section so other threads can have access to the critical section.

# Example

```c
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int mails = 0;
pthread_mutex_t mutex;

void* routine() {
    for (int i = 0; i < 10000000; i++) {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
        // read mails
        // increment
        // write mails
    }
}

int main(int argc, char* argv[]) {
    pthread_t p1, p2, p3, p4;
    pthread_mutex_init(&mutex, NULL);
    if (pthread_create(&p1, NULL, &routine, NULL) != 0) {
        return 1;
    }
    if (pthread_create(&p2, NULL, &routine, NULL) != 0) {
        return 2;
    }
    if (pthread_join(p1, NULL) != 0) {
        return 5;
    }
    if (pthread_join(p2, NULL) != 0) {
        return 6;
    }
    pthread_mutex_destroy(&mutex);
    printf("Number of mails: %d\n", mails);
    return 0;
}
result = "Number of mails: 2.000.000
```

## gettimeofday()

function gets the system’s clock time. The current time is expressed in elapsed seconds and micro seconds since 00:00:00, January 1, 1970 (unix epock).

Syntax:

```c
int gettimeofday ( struct timeval *tp ,  struct timezone *tz )
```

- First argument points to timeval structure
- The second argument should normally be specified as NULL.

The timeval structure is declared as below in the <sys/time.h> header file.

```c
struct    timeval  {
  time_t        tv_sec ;   //used for seconds
  suseconds_t       tv_usec ;   //used for microseconds
}
```

- tv_sec it is the number of seconds since the epoch
- it is additional microseconds after number of seconds calculation since the epoch

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/0a0294dd-fa66-4ebd-bb0d-013dbdf4749d/Untitled.png)

```c
u_int64_t    get_time(void)
{
    struct timeval  tp;
    u_int64_t            milliseconds;

    gettimeofday(&tp, NULL);
    milliseconds = tp.tv_sec * 1000;
    milliseconds += tp.tv_usec / 1000;
    return (milliseconds);j
}

```
