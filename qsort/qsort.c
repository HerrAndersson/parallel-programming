/***************************************************************************
 *
 * Sequential version of Quick sort
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define KILO (256)
//#define KILO (1024)

#define MEGA (KILO*KILO)

//#define MAX_ITEMS (64*MEGA)
#define MAX_ITEMS (64*MEGA)

#define NUM_THREADS 16

#define swap(v, a, b) {unsigned tmp; tmp=v[a]; v[a]=v[b]; v[b]=tmp;}

static int *v;

typedef struct
{
	int *v;
	unsigned low;
	unsigned high;
} Data;


static void print_array(void)
{
	int i;

	for (i = 0; i < MAX_ITEMS; i++)
	{
		printf("%d ", v[i]);
	}
	printf("\n");
}

static void init_array(void)
{
	int i;

	v = (int *) malloc(MAX_ITEMS*sizeof(int));
	for (i = 0; i < MAX_ITEMS; i++)
	{
	   // v[i] = rand();
	   v[i] = rand() % 500;
	}
}

static unsigned partition(int *v, unsigned low, unsigned high, unsigned pivot_index)
{
	/* move pivot to the bottom of the vector */
	if (pivot_index != low)
	{
		swap(v, low, pivot_index);
	}

	pivot_index = low;
	low++;

	/* invariant:
	 * v[i] for i less than low are less than or equal to pivot
	 * v[i] for i greater than high are greater than pivot
	 */

	/* move elements into place */
	while (low <= high) 
	{
		if (v[low] <= v[pivot_index])
		{
			low++;
		}
		else if (v[high] > v[pivot_index])
		{
			high--;
		}
		else
		{
			swap(v, low, high);
		}
	}

	/* put pivot back between two groups */
	if (high != pivot_index)
	{
		swap(v, pivot_index, high);
	}
	
	return high;
}

static void seq_quick_sort(int *v, unsigned low, unsigned high)
{
	unsigned pivot_index;
	
	/* no need to sort a vector of zero or one element */
	if (low >= high)
	{
		return;
	}

	/* select the pivot value */
	pivot_index = (low+high)/2;

	/* partition the vector */
	pivot_index = partition(v, low, high, pivot_index);

	/* sort the two sub arrays */
	if (low < pivot_index)
	{
		seq_quick_sort(v, low, pivot_index-1);
	}
	if (pivot_index < high)
	{
		seq_quick_sort(v, pivot_index+1, high);
	}
}

/*int *v, unsigned low, unsigned high*/
static void* quick_sort(void* arg)
{
	Data* d = (Data*)arg;
	int* v = d->v;
	unsigned low = d->low;
	unsigned high = d->high;
	
	unsigned pivot_index;
	/* no need to sort a vector of zero or one element */
	if (low >= high)
	{
		return 0;
	}

	/* select the pivot value */
	pivot_index = (low+high)/2;
	/* partition the vector */
	pivot_index = partition(v, low, high, pivot_index);

	pthread_t threads[2];
	int r1 = 0;
	int r2 = 0;
	int isSeq = ((high-low) < MEGA);
	//printf("%d %d\n",high-low,  isSeq);
	/* sort the two sub arrays */
	if (low < pivot_index)
	{
		if(isSeq)
		{
			seq_quick_sort(v, low, pivot_index-1);
		}
		else
		{	
			r1 = 1;
			Data d;
			d.v = v;
			d.low = low;
			d.high = pivot_index-1;
		
			pthread_create(&(threads[0]), NULL, quick_sort, &d);
			//printf("Create thread\n");
		}
	}
	
	if (pivot_index < high)
	{	
		if(isSeq)
		{
			seq_quick_sort(v, pivot_index+1, high);
		}
		else
		{
			r2 = 1;
			Data d;
			d.v = v;
			d.low = pivot_index+1;
			d.high = high;
		
			pthread_create(&(threads[1]), NULL, quick_sort, &d);
			//printf("Create thread\n");
		}
	}	
	
	if(r1 == 1)
	{
		pthread_join(threads[0], NULL);
	}
	
	if(r2 == 1)
	{
		pthread_join(threads[1], NULL);
	}
}

int main(int argc, char **argv)
{
	srand(time(0));
	init_array();
	//print_array();
	
	
	/*Data d;
	d.v = v;
	d.low = 0;
	d.high = MAX_ITEMS - 1;
	*/
	
	//quick_sort(&d);

	
	//print_array();
	
	seq_quick_sort(v, 0, MAX_ITEMS-1);
	
	//print_array();
}

