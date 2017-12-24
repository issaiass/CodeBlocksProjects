/*Programming Problem
A prime number is an integer greater than 1 that has no integer divisors
other than itself and 1.

A left-truncatable prime is a prime number which contains no 0s and
remains prime when any contiguous sequence of left-most digits is removed.

For example: 5167 is a left-truncatable prime, since 5167, 167, 67 and 7
are all prime;
2179 is not a left-truncatable prime, even though 2179, 179 and 79 are prime,
since 9 is not prime.

There are 2166 left-truncatable primes under 1,000,000,000.

Write a program in C for an embedded system that reads in a single integer,
n (1 ≤ n ≤ 2166), and outputs the nth left-truncatable prime.

For example: The input 10 should give
            the output 47;

            the input 100 should give
            the output 5167;

            the input 1000 should give
            the output 8391283.
*/

#include <stdio.h>
#include <stdlib.h>

#define PRIME_TOP_NUM    100000000
#define TRUE                     1
#define FALSE                    0
#define NOT_PRIME                0
#define IS_PRIME                 1
#define NOT_LEFT_TRUNCABLE       0
#define LEFT_TRUNCABLE           1


typedef unsigned char    BOOL;
typedef          char   INT8S;
typedef unsigned char   INT8U;
typedef          short INT16S;
typedef unsigned short INT16U;
typedef          int   INT32S;
typedef unsigned int   INT32U;
typedef          long  INT64S;
typedef unsigned long  INT64U;


void GeneratePrimes(INT64U n);

void memset0(INT64U *ptr, INT64U c, INT64U size);
void fillconsecutive(INT64U *arr, INT64U n);
void incrementalsieve(INT64U *arr, INT64U n);
void printarr(INT64U *arr, INT64U n);
INT64U countprimes(INT64U *arr, INT64U n);
void eliminatezeros(INT64U *arr, INT64U n);
BOOL isPrime(INT64U num);
BOOL islefttruncable(INT64U num);
INT64U GetLeftTruncable(INT64U n);
INT64U countitems(INT64U *arr);

INT64U *numarr;
INT64U *primearr;


int main(void)
{
    INT32U n;
    INT32U answer;


    printf("Left-Truncable Number Nth Finder Program\n\n");
    GeneratePrimes(PRIME_TOP_NUM);
    printf("Which left truncable you want?: ");
    scanf("%d", &n);
    if(n > 2166 || n < 1) {
        printf("not inside the limits, bye.\n");
    } else {
        answer = GetLeftTruncable(n);
        printf("\nTruncable no. %d is %d\n", n, answer);
    }
    return 0;
}

void memset0(INT64U *ptr, INT64U c, INT64U size) {
    INT64U i;

    for(i = 0; i < size; i++) {
        *(ptr + i) = c;
    }
}

void GeneratePrimes(INT64U n) {
    INT64U primes;

    numarr = malloc(sizeof(INT32S) * (n - 1));  // preserve storage
    memset0(numarr, -1, n - 1);                 // fill the array with -1
    fillconsecutive(numarr, n);                 // fill the array starting from '2' up to 'n - 1'
    incrementalsieve(numarr, n);                // conserve primes in table, others equal to 0
    primes = countprimes(numarr, n);
//    printf("There are %d prime numbers\n", primes - 1);
    eliminatezeros(numarr, n);                  // delete zeros and reallocate array
//    printarr(primearr, primes + 1);
}

INT64U GetLeftTruncable(INT64U n) {
    INT64U count;
    INT64U i;


    for(i = 0, count = 0; count < (n - 1); i++) {
        if(islefttruncable(*(primearr + i))) {
     //       printf("truncable[%d] = %d\n", count, *(primearr + i));
            count++;
        }
    }
    while(!islefttruncable(*(primearr + i))) {
        i++;
     //   printf("%d\n", i);
    }
    return *(primearr + i);
}


void eliminatezeros(INT64U *arr, INT64U n) {
    INT32U primes;
    INT64U nprime;
    INT64U i;


    primes = countprimes(arr, n);
    primearr = malloc(sizeof(INT32S) * primes); // preserve storage for primes, no need to free
    memset0(primearr, -1, primes);                // fill the array with -1
    for(i = 0, nprime = 0; i < n; i++) {
        if(*(arr + i) != 0) {
            *(primearr + nprime++) = *(arr + i);
        }
    }
    free(numarr);
    *(primearr + primes - 1) = 0;
}

void incrementalsieve(INT64U *arr, INT64U n) {
    INT64U   i;
    INT64U    j;
    INT64U    m;


    for(j = 2, m = 0; j*j < n; j++) {
        for(i = m + j; i < (n - 1); i +=j) {
            if(*(arr + i)) {
                *(arr + i) = 0;
            }
        }
        m++;
    }
}

INT64U countitems(INT64U *arr) {
    INT32U i;


    i = 0;
    while(*(arr + i++)) {

    }
    return --i;
}


INT64U countprimes(INT64U *arr, INT64U n) {
    INT64U primes;
    INT64U i;


    for(i = 0, primes = 0; i < n; i++) {
        if(*(arr + i)) {
            primes++;
        }
    }
    return (primes - 1);
}

void printarr(INT64U *arr, INT64U n) {
    INT64U i;


    for(i = 0; i < n - 1; i++) {
        printf("%d\t", arr[i]);
    }
}

void fillconsecutive(INT64U *arr, INT64U n) {
    INT64U i;

    for(i = 0; i <= n - 1; i++) {
        *(arr + i) = i + 2;
    }
}

BOOL islefttruncable(INT64U num) {
    INT64U div;


    div = 1;
  //  if(isPrime(num)) {
       while(num > div) {
        div *= 10;
       }
       while(num) {
           if(isPrime(num)) {
               div /= 10;
               if(num < div) {
                   return NOT_LEFT_TRUNCABLE;
               }
               num %= div;
               continue;
           } else {
               return NOT_LEFT_TRUNCABLE;
           }
       }
       return LEFT_TRUNCABLE;
  //  }
  //  return NOT_LEFT_TRUNCABLE;
}

// do not consider if the number is above the table.
BOOL isPrime(INT64U num) {
    INT64U i;


    for(i = 0; num >= *(primearr + i) && *(primearr + i); i++) {
        if(num == *(primearr + i)) {
            return IS_PRIME;
        }
    }
    return NOT_PRIME;
}
