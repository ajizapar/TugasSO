#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MSGSIZE 10

pthread_mutex_t lock;

int buffer[MSGSIZE];

void *producer(void *vargp)
{
    srand(time(0));

    for (int i = 0; i < MSGSIZE; i++)
    {
        
        pthread_mutex_lock(&lock);
        int randomNumber = rand() % (50 + 1 - 0) + 0;
        buffer[i] = randomNumber;
        printf("(producer) %d. pid= %d, menulis= %d\n", i, getpid(), randomNumber);
        pthread_mutex_unlock(&lock);

    }
    return NULL;

    
}

void *consumer(void *vargp)
{
    int jumlah = 0;
    for (int i = MSGSIZE - 1; i >= 0; i--)
    {
        pthread_mutex_lock(&lock);
        int getNumber = buffer[i];
        jumlah += getNumber;
        printf("(consumer) %d. pid= %d, membaca= %d\n", i, getpid(), getNumber);
        pthread_mutex_unlock(&lock);
    }
    
    printf("Jumlah akhir semua data pada batch ini= %d\n", jumlah);

    return NULL;
    
}

int main(int argc, char const *argv[])
{
    
    int jumlahProduser, jumlahKonsumer;

    printf("Masukkan jumlah produser: ");
    scanf("%d", &jumlahProduser);
    printf("Masukkan jumlah konsumer: ");
    scanf("%d", &jumlahKonsumer);

    pthread_t t1[jumlahProduser], t2[jumlahKonsumer];

    // init mutex
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init gagal\n");
        return 1;
    }
    
    for (int i = 0; i < jumlahProduser; i++)
    {
        // create thread for producer
        pthread_create(&t1[i], NULL, producer, NULL);
    }

    for (int i = 0; i < jumlahKonsumer; i++)
    {
        // create thread for consumer
        pthread_create(&t2[i], NULL, consumer, NULL);
    }

    for (int i = 0; i < jumlahProduser; i++)
    {
        pthread_join(t1[i], NULL);
    }

    for (int i = 0; i < jumlahKonsumer; i++)
    {
        pthread_join(t2[i], NULL);
    }

    // destroy mutex
    pthread_mutex_destroy(&lock);
    exit(0);
    
    return 0;
}
