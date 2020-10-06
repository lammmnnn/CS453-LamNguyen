/**
 * Name: Lam Nguyen
 * Email: lamnguyen@u.boisestate.edu
 * Github Username: lammmnnn
 */
#include "lab.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>



/*
 * struct passing_para:
 *
 * description: Passing the value from paralel_mergesort parameter.
 */
struct passing_para {
  void *A;
  int p;
  int r;
  int nthread;
};

/*
 * insertion_sort(int A[], int p, int r):
 *
 * description: Sort the section of the array A[p..r].
 */
void insertion_sort(int A[], int p, int r)
{
	int j;

	for (j=p+1; j<=r; j++) {
		int key = A[j];
		int i = j-1;
		while ((i > p-1) && (A[i] > key)) {
			A[i+1] = A[i];
			i--;
		}
		A[i+1] = key;
	}
}


/*
 * serial_mergesort(int A[], int p, int r):
 *
 * description: Sort the section of the array A[p..r].
 */
void serial_mergesort(int A[], int p, int r)
{
	if (r-p+1 <= INSERTION_SORT_THRESHOLD)  {
			insertion_sort(A,p,r);
	} else {
		int q =  r + (p - r)/2;
		serial_mergesort(A, p, q);
		serial_mergesort(A, q+1, r);
		merge(A, p, q, r);
	}
}

/*
 * *parallel(void* arg):
 *
 * description: for pthread_create's usage.
 */
void *parallel(void* arg) {
  struct passing_para *para = arg;
  paralel_mergesort(para->A,para->p,para->r,para->nthread-1);
  
  return arg;
}


/*
 * paralel_mergesort(int A[], int p, int r, int numthread):
 *
 * description: Sort array A[p..r] using multiple threads.
 */
void paralel_mergesort(int A[], int p, int r, int numthread) {
  if (numthread <= 1) {
    serial_mergesort(A,p,r);
  }
  else {
    pthread_t thread;
    int q = (p+r)/2;
    struct passing_para *par = (struct passing_para *) malloc(sizeof(struct passing_para));
    par->A = A;
    par->p = p;
    par->r = q;
    par->nthread = numthread;
    int rc = pthread_create(&thread,NULL,parallel,par);
    if (rc) {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(1);
    }
    paralel_mergesort(A,q+1,r,numthread-1);
    pthread_join(thread,NULL);
    merge(A,p,q,r);
    free(par);
  }
}


/*
 * merge(int A[], int p, int q, int r):
 *
 * description: Merge two sorted sequences A[p..q] and A[q+1..r]
 *              and place merged output back in array A. Uses extra
 *              space proportional to A[p..r].
 */
void merge(int A[], int p, int q, int r)
{
	int *B = (int *) malloc(sizeof(int) * (r-p+1));

	int i = p;
	int j = q+1;
	int k = 0;
	int l;

	// as long as both lists have unexamined elements
	// this loop keeps executing.
	while ((i <= q) && (j <= r)) {
		if (A[i] < A[j]) {
			B[k] = A[i];
			i++;
		} else {
			B[k] = A[j];
			j++;
		}
		k++;
	}

	// now only at most one list has unprocessed elements.

	if (i <= q) {
		// copy remaining elements from the first list
		for (l=i; l<=q; l++) {
			B[k] = A[l];
			k++;
		}
	} else {
		// copy remaining elements from the second list
		for (l=j; l<=r; l++) {
			B[k] = A[l];
			k++;
		}
	}

	// copy merged output from array B back to array A
	k=0;
	for (l=p; l<=r; l++) {
		A[l] = B[k];
		k++;
	}

	free(B);
}