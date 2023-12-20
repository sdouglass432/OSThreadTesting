/*
This program takes 4 numbers from each of 3 input text files, sorts them in ascending order, and writes them to corresponding output files.
One thread is used for each file. Threads run the sort() function and pass it their input file, then a case statement is used to determine the name of the output file.

The biggest challenge was figuring out how to pull and sort the numbers from the input files.
Initially I used a fixed length array with a size of 1000 and fgets. I also  made my own sort function because I was unaware of qsort.
I had issues with my function because it was iterating through all the elements instead of just using what fgets put in the array.
I switched to malloc to avoid using extra space and fscanf instead of fgets to store the numbers as ints instead of a string. Also switched to qsort so my code is easier to follow.

I also initially had sort() take 2 arguments, one for input file and one for output.
 Threads cannot pass multiple args & I didn't have time to mess around with structs so I decided to use a switch statement instead.
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int count = 0;
pthread_mutex_t mutex;

int cmpfunc (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

void sort(void *inf) {
    int a, b, c, d, n;
    int *buf;
    FILE *out;
    pthread_mutex_lock(&mutex);
    switch(count) {
        case 0:
            out = fopen("output1.txt", "w");
            count++;
            break;
        case 1:
            out = fopen("output2.txt", "w");
            count++;
            break;
        case 2:
            out = fopen("output3.txt", "w");
            count++;
            break;

    }
    pthread_mutex_unlock(&mutex);

    buf = (int*)malloc(4 * sizeof(int));
    if (buf == NULL) {
        printf("Memory not allocated.\n");
    }
    if (inf != NULL) {
        fscanf(inf, "%d %d %d %d", &a, &b, &c, &d );
        buf[0] = a;
        buf[1] = b;
        buf[2] = c;
        buf[3] = d;
        qsort(buf, 4, sizeof(int), cmpfunc);
//        check to see if sorted properly
//        printf("After sorting: ");
//        for( n = 0 ; n < 4; n++ ) {
//            printf("%d ", buf[n]);
//        }
    } else {
        printf("file reads as NULL\n");
    }
    char sortd[100];
    for (n = 0; n < 4; n++){
        //tried a few different ways to insert spaces after numbers but couldn't get it to work
        sprintf(&sortd[n], "%d", buf[n]);
    }
    fputs(sortd, out);
    fclose(inf);
    fclose(out);
}

int main(){
    FILE *fp1, *fp2, *fp3;
    fp1 = fopen("input1.txt", "r");
    fp2 = fopen("input2.txt", "r");
    fp3 = fopen("input3.txt", "r");

    pthread_t t1, t2, t3;
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&t1, NULL, (void *(*)(void *)) sort, fp1);
    pthread_create(&t2, NULL, (void *(*)(void *)) sort, fp2);
    pthread_create(&t3, NULL, (void *(*)(void *)) sort, fp3);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&mutex);
    return 0;
}