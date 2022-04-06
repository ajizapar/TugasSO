#include <sys/mman.h>   /* mmap()  */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h>   /*wait*/
#include <errno.h>        /* errno */

#include<time.h> 

#define UPPER 10
#define LOWER 1
#define MAX_NMAP 100

int main(int argc, char **argv) {
        int pid, N;

        printf("mulai \n");
        int size = MAX_NMAP * sizeof(int);
        //buat memory map
        void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        printf("Mapped at %p\n", addr);
        int *shared = addr;

        switch (pid = fork()) {
        case 0: /*  proses anak */
                //isi data di memory map
                srand(time(0));
                printf("Masukan jumlah proses produksi: ");
                scanf("%d", &N);
                for (int  i = 0; i < N; i++)
                {
                    //generate nilai random antara 1 - 10
                    shared[i] = (rand() % (UPPER - LOWER + 1)) + LOWER;
                    printf("Produksi nilai %d\n", shared[i]);
                }
                break;
        default:        /*  ortu */
                printf("Proses ortu\n");
                int pidWait,status,M,jumlah=0;
                /* wait sampai child selesai */
                while (pidWait = wait(&status)) {   
                    if (pidWait == pid)  /* child sukses selesai*/
                        printf("Masukan jumlah proses konsumsi: ");
                        scanf("%d",&M);
                        if (M > N)
                            M = N;

                        for (int i = 0; i<M; i++) {
                            //cetak isi dari memori map
                            printf("Konsumsi nilai %d\n", shared[i]);
                            jumlah = jumlah + shared[i];
                        }
                        printf("Jumlah nilai dalam buffer %d\n", jumlah);
                        break; //keluar dari loop wait
                    if ((pidWait == -1) && (errno != EINTR)) {
                        /* ada error*/
                        perror("waitpid");
                        exit(1);
                    }
                }
                break;
        case -1:        /* error */
                perror("fork");
                exit(1);
        }
        exit(0);
}
