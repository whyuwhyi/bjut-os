#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void thread1_function();
void thread2_function();

int main(int argc, char **argv) {
    int ret1, ret2;
    pthread_t myThread1, myThread2;

    ret1 = pthread_create(&myThread1, NULL, (void*)thread1_function, NULL);
    ret2 = pthread_create(&myThread2, NULL, (void*)thread2_function, NULL);
    
    if (ret1) {
        fprintf(stderr, "Create Thread1 Failed!\n");
        return -1;
    }
    if (ret2) {
        fprintf(stderr, "Create Thread2 Failed!\n");
        return -1;
    }

    pthread_join(myThread1, NULL);
    pthread_join(myThread2, NULL);

    return 0;
}

void thread1_function() {
    printf("Name:余意\n");
}

void thread2_function() {
    printf("StudentNumber:22074117\n");
}
