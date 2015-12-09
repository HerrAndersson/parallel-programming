/***************************************************************************
 *
 * Sequential version of Matrix-Matrix multiplication
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//#define SIZE 1024
#define SIZE 4

static double a[SIZE][SIZE];
static double b[SIZE][SIZE];
static double c[SIZE][SIZE];

static pthread_mutex_t lock;

typedef struct
{
	int aStartX;
	int aStartY;
	int bStartX;
	int bStartY;
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
			
			a[i][j] = rand() % 10;
			b[i][j] = rand() % 10;
			c[i][j] = 0.0;
		}
	}
}

static void* matmul(void* arg)
{
	int i, j, k;
	
	Data* data = (Data*)arg;
	
	printf("%d %d %d %d %d %d %d\n", data->aStartX, data->aStartY, data->bStartX, data->bStartY, data->cStartX, data->cStartY, data->length);
	
	int l = data->cStartY;
	int m = data->cStartX;
	
	for (i = data->aStartX; i < data->aStartX + data->length; i++) 
	{
		for (j = data->bStartY; j < data->bStartY + data->length; j++) 
		{
			for (k = 0; k < data->length; k++)
			{
				pthread_mutex_lock(&lock);
				
				c[m][l] += a[i][data->aStartY + k] * b[data->bStartX + k][j];
	
				pthread_mutex_unlock(&lock);
			}
			
			l += 1;
		}
		m += 1;
	}
}

static void print_matrix(void)
{
	int i, j;

	for (i = 0; i < SIZE; i++) 
	{
		for (j = 0; j < SIZE; j++)
			printf(" %7.2f", a[i][j]);
			
		printf("\n");
	}
	
	printf("\n");
	
	for (i = 0; i < SIZE; i++) 
	{
		for (j = 0; j < SIZE; j++)
			printf(" %7.2f", b[i][j]);
			
		printf("\n");
	}
	
	printf("\n");
	
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
	
	//a1*b1
	Data d;
	d.aStartX = 0;
	d.aStartY = 0;
	d.bStartX = 0;
	d.bStartY = 0;
	
	d.cStartX = 0;
	d.cStartY = 0;
	
	d.length = SIZE / 2;
	
	pthread_create(&(threads[0]), NULL, matmul, &d);
	
	//a2*b2
	Data d1;
	d1.aStartX = SIZE / 2;
	d1.aStartY = 0;
	d1.bStartX = SIZE / 2;
	d1.bStartY = 0;
	
	d1.cStartX = SIZE / 2;
	d1.cStartY = 0;
	
	d1.length = SIZE / 2;

	pthread_create(&(threads[1]), NULL, matmul, &d1);
	
	//a3*b3
	Data d2;
	d2.aStartX = 0;
	d2.aStartY = SIZE / 2;
	d2.bStartX = 0;
	d2.bStartY = SIZE / 2;
	
	d2.cStartX = 0;
	d2.cStartY = SIZE / 2;
	
	d2.length = SIZE / 2;

	pthread_create(&(threads[2]), NULL, matmul, &d2);
	
	//a4*b4
	Data d3;
	d3.aStartX = SIZE / 2;
	d3.aStartY = SIZE / 2;
	d3.bStartX = SIZE / 2;
	d3.bStartY = SIZE / 2;
	
	d3.cStartX = SIZE / 2;
	d3.cStartY = SIZE / 2;
	
	d3.length = SIZE / 2;

	pthread_create(&(threads[3]), NULL, matmul, &d3);
	
	//a2*b3 -> c00
	Data d4;
	d4.aStartX = SIZE / 2;
	d4.aStartY = 0;
	d4.bStartX = 0;
	d4.bStartY = SIZE / 2;
	
	d4.cStartX = 0;
	d4.cStartY = 0;
	
	d4.length = SIZE / 2;

	pthread_create(&(threads[4]), NULL, matmul, &d4);
	
	//a1*b4 -> c01
	Data d5;
	d5.aStartX = 0;
	d5.aStartY = 0;
	d5.bStartX = SIZE / 2;
	d5.bStartY = SIZE / 2;
	
	d5.cStartX = SIZE / 2;
	d5.cStartY = 0;
	
	d5.length = SIZE / 2;

	pthread_create(&(threads[5]), NULL, matmul, &d5);
	
	//a4*b1 -> c10
	Data d6;
	d6.aStartX = SIZE / 2;
	d6.aStartY = SIZE / 2;
	d6.bStartX = 0;
	d6.bStartY = 0;
	
	d6.cStartX = 0;
	d6.cStartY = SIZE / 2;
	
	d6.length = SIZE / 2;

	pthread_create(&(threads[6]), NULL, matmul, &d6);
	
	//a3*b2 -> c11
	Data d7;
	d7.aStartX = 0;
	d7.aStartY = SIZE / 2;
	d7.bStartX = SIZE / 2;
	d7.bStartY = 0;
	
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

