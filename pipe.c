#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include<time.h> 

#define MSGSIZE 10

int main(int argc, char const *argv[])
{
    char inbuf[MSGSIZE]; // buffer
    int pid;
    int fd[2]; // pipe
    int jumlahProses;


    // create pipe
    if(pipe(fd) < 0){
        exit(1);
    }



    printf("mulai\n");

    switch (pid = fork())
    {
    case 0: // fork return 0 ke proses anak
        printf("proses anak\n");
        // tutup bagian input dari pipe
        close(fd[0]);
            scanf("%d", &jumlahProses);

        // tulis ke pipe
        srand(time(0));
        int randomNumberBuffer[sizeof(jumlahProses) + 1];
        for (int i = 0; i < jumlahProses; i++)
        {
            randomNumberBuffer[i] = rand() % (50 + 1 - 0) + 0;
            printf("proses anak menulis %d\n", randomNumberBuffer[i]);
        }
        write(fd[1], randomNumberBuffer, sizeof(randomNumberBuffer));

        

        break;
    default: // fork return pid ke proses ortu

        printf("proses ortu\n");
        // tutup bagian output dari pipe
        close(fd[1]);
        scanf("%d", &jumlahProses);

        // baca yang ditulis child dari pipe
        int addChildNumber[sizeof(jumlahProses) + 1];

        read(fd[0], addChildNumber, sizeof(addChildNumber));
        int jumlahNumber = 0;
        for (int i = 0; i < jumlahProses; i++)
        {
            jumlahNumber += addChildNumber[i];
            printf("jumlah number %d\n", jumlahNumber);
        }

        break;
    
    }
    return 0;
}
