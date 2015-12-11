/***************************************************************************
 *
 * Sequential version of Matrix-Matrix multiplication
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 1024

static double a[SIZE][SIZE];
static double b[SIZE][SIZE];
static double c[SIZE][SIZE];

static pthread_mutex_t lock;

typedef struct
{
	int aStartY;
	int bStartX;
	int cStartX;
	int cStartY;
	int length;
} Data;

static void init_matrix(void)
{
	int i, j;

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++) 
		{
			/* Simple initialization, which enables us to easy check
			* the correct answer. Each element in c will have the same 
			* value as SIZE after the matmul operation. */
			
			a[i][j] = 1.0;
			b[i][j] = 1.0;
			c[i][j] = 0.0;
		}
	}
}

static void* matmul(void* arg)
{
	int i, j, k;
	
	Data* d = (Data*)arg;
	
	int aStartY = d->aStartY;
	int bStartX = d->bStartX;
	int cStartX = d->cStartX;
	int cStartY = d->cStartY;
	int length 	= d->length;
	
	int cEndY = cStartY + length;
	int cEndX = cStartX + length;
	
	for (i = cStartY; i < cEndY; i++)
	{
		for (j = cStartX; j < cEndX; j++)
		{
			for (k = 0; k < length; k++)
			{
				c[i][j] += a[i][aStartY+k] * b[bStartX+k][j];
			}
		}
	}	
}

static void print_matrix(void)
{
	int i, j;

	for (i = 0; i < SIZE; i++) 
	{
		for (j = 0; j < SIZE; j++)
			printf(" %7.2f", c[i][j]);
			
		printf("\n");
	}
	
}

int main(int argc, char **argv)
{
	srand(0);
	pthread_t threads[8];
	init_matrix();
	
	//a1*a2 -> c00
	Data d;
	d.aStartY = 0;
	d.bStartX = 0;
	
	d.cStartX = 0;
	d.cStartY = 0;
	
	d.length = SIZE / 2;
	
	pthread_create(&(threads[0]), NULL, matmul, &d);
	
	//b1*c2 -> c00
	Data d1;
	d1.aStartY = SIZE / 2;
	d1.bStartX = SIZE / 2;
	
	d1.cStartX = 0;
	d1.cStartY = 0;
	
	d1.length = SIZE / 2;

	pthread_create(&(threads[1]), NULL, matmul, &d1);
	
	//a1*b2 ->c10
	Data d2;
	d2.aStartY = 0;
	d2.bStartX = 0;
	
	d2.cStartX = SIZE / 2;
	d2.cStartY = 0;
	
	d2.length = SIZE / 2;

	pthread_create(&(threads[2]), NULL, matmul, &d2);
	
	//b1*d2 -> c10
	Data d3;
	d3.aStartY = SIZE / 2;
	d3.bStartX = SIZE / 2;
	
	d3.cStartX = SIZE / 2;
	d3.cStartY = 0;
	
	d3.length = SIZE / 2;

	pthread_create(&(threads[3]), NULL, matmul, &d3);
	
	//c1*a2 -> c01
	Data d4;
	d4.aStartY = 0;
	d4.bStartX = 0;
	
	d4.cStartX = 0;
	d4.cStartY = SIZE / 2;
	
	d4.length = SIZE / 2;

	pthread_create(&(threads[4]), NULL, matmul, &d4);
	
	//d1*c2 -> c01
	Data d5;
	d5.aStartY = SIZE / 2;
	d5.bStartX = SIZE / 2;
	
	d5.cStartX = 0;
	d5.cStartY = SIZE / 2;
	
	d5.length = SIZE / 2;

	pthread_create(&(threads[5]), NULL, matmul, &d5);
	
	//c1*b2 -> c11
	Data d6;
	d6.aStartY = 0;
	d6.bStartX = 0;
	
	d6.cStartX = SIZE / 2;
	d6.cStartY = SIZE / 2;
	
	d6.length = SIZE / 2;

	pthread_create(&(threads[6]), NULL, matmul, &d6);
	
	//d1*d2 -> c11
	Data d7;
	d7.aStartY = SIZE / 2;
	d7.bStartX = SIZE / 2;
	
	d7.cStartX = SIZE / 2;
	d7.cStartY = SIZE / 2;
	
	d7.length = SIZE / 2;

	pthread_create(&(threads[7]), NULL, matmul, &d7);
	
	int i;
	for (i = 0; i < 8; i++) 
	{
		pthread_join(threads[i], NULL);
	}
	
	print_matrix();
}

