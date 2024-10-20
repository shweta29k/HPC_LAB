#include<stdio.h>
#include<omp.h>

int main()
{
	int thread_no;
	
	printf("Enter the number of threads: ");
	scanf("%d", &thread_no);
	//sequentionally
	printf("Sequential o/p: ");
	for(int i=0;i<thread_no;i++)
	{
		printf("Hello World\n");
	}
	
	printf("Parallel o/p: ");
	omp_set_num_threads(thread_no);
	#pragma omp parallel
	{
		printf("hello world from thread no %d\n",omp_get_thread_num());
	}
	return 0;
}
