#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include<sys/wait.h>

#include<time.h> 

#define MSGSIZE 10

int main(int argc, char const *argv[])
{
    char inbuf[MSGSIZE]; // buffer
    int pid;
    int fd[2]; // pipe
    int jumlahProses;

    int jumlahProducer;
    int jumlahKonsumer;


    // create pipe
    if(pipe(fd) < 0){
        exit(1);
    }
    int pidWait, pidWait2, status, status2;
    int i = 0;

    printf("Masukkan jumlah producer: ");
    scanf("%d", &jumlahProducer);

    printf("Masukkan jumlah konsumer: ");
    scanf("%d", &jumlahKonsumer);

    printf("mulai\n");

    switch (pid = fork())
    {
    case 0: // fork return 0 ke proses anak
        printf("proses anak\n");
        // tutup bagian input dari pipe
        close(fd[0]);

        // tulis ke pipe
        srand(time(0));
        int randomNumberBuffer[MSGSIZE];
        for (i = 0; i < jumlahProducer; i++)
        {
            fork();
            randomNumberBuffer[i] = rand() % (50 + 1 - 0) + 0;
            printf("%d. (prod) pid= %d, parent= %d, tulis= %d\n",i, getpid(), getppid(), randomNumberBuffer[i]);
        }

       
        write(fd[1], randomNumberBuffer, sizeof(randomNumberBuffer));


        break;
    default: // fork return pid ke proses ortu

        // tutup bagian output dari pipe
        close(fd[1]);

        // baca yang ditulis child dari pipe
        int addChildNumber[MSGSIZE];

        read(fd[0], addChildNumber, sizeof(addChildNumber));
        int jumlahNumber = 0;
        int status;
        for (int i = 0; i < jumlahKonsumer; i++)
        {
            fork();
            // wait(&status);
            jumlahNumber += addChildNumber[i];
            printf("%d. (kons) pid= %d, parent= %d, baca= %d, jumlah= %d\n",i, getpid(), getppid(), addChildNumber[i], jumlahNumber);
        }
        break;
    
    
    }
    return 0;
}
