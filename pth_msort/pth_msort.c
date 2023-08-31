// Include your C header files here

#include "pth_msort.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>

//pthread_mutex_t mutex[4];
int n;
int bin;
int* v;
int* t;
 
void quickSort (int low, int high){
	int pivot = v[high];
	int i = low - 1;
	int temp;
	int index;
	int j;	
	if(low < high){
		for(j = low; j <= high; j++){
			if(v[j] < pivot){
				i++;
				temp = v[i];
				v[i] = v[j];
				v[j] = temp;
			}
		}
		temp = v[high];
		v[high] = v[i+1];
		v[i+1] = temp;
		index = i + 1;
	
		quickSort(low, index-1);
		quickSort(index+1, high);
	}
	return;
}
void serialMerge (int low, int middle, int high){
	int i, j, k, temp;
	int NL, NH;
	NL = middle - low + 1;
	NH = high - middle;
	//printf("BP\n");
	//int L[NL], H[NH];	
	for(i = 0; i < NL; i++)
		t[low + i] = v[low + i];
	for(j = 0; j < NH; j++)
		t[middle + 1 + j] = v[middle + 1 + j];
	i = 0; j = 0; k = low;
	while(i < NL && j < NH){
		if(t[low+i] < t[middle+1+j]){
			v[k] = t[low+i];
			i++;
		}
		else{
			v[k] = t[middle+1+j];
			j++;
		}
		k++;
		/*printf("i = %d, j = %d, k = %d\n",i,j,k);
		for(n = low; n <= high; n++) printf("%d ",v[n]);
		printf("\n");*/ 
	}
	while(i < NL){
		v[k] = t[low+i];
		i++; k++;
	}
	while(j < NH){
		v[k] = t[middle+1+j];
		j++; k++;
	}
	/*for(i = low; i < high; i++)
		if(v[i] > v[i+1]){
			printf("Job Not Done!\n");
			return;
		}
	printf("Job Done!\n");*/
	return;
}
void lastSerialMerge (int low, int middle, int high){
	int i, j, k, temp;
	int NL, NH;
	NL = middle - low + 1;
	NH = high - middle;
	//printf("BP\n");
	//int L[NL], H[NH];	
	/*for(i = 0; i < NL; i++)
		t[low + i] = v[low + i];
	for(j = 0; j < NH; j++)
		t[middle + 1 + j] = v[middle + 1 + j];*/
	i = 0; j = 0; k = low;
	while(i < NL && j < NH){
		if(v[low+i] < v[middle+1+j]){
			t[k] = v[low+i];
			i++;
		}
		else{
			t[k] = v[middle+1+j];
			j++;
		}
		k++;
		/*printf("i = %d, j = %d, k = %d\n",i,j,k);
		for(n = low; n <= high; n++) printf("%d ",v[n]);
		printf("\n");*/ 
	}
	while(i < NL){
		t[k] = v[low+i];
		i++; k++;
	}
	while(j < NH){
		t[k] = v[middle+1+j];
		j++; k++;
	}
	/*for(i = low; i < high; i++)
		if(t[i] > t[i+1]){
			printf("Job Not Done!\n");
			return;
		}*/
	//printf("Job Done!\n");
	return;
}
void parallelMerge(int low1, int high1, int low2, int high2, int des_index){
	int i, j, k, temp;
	int NL, NH;
	NL = high1 - low1 + 1;
	NH = high2 - low2 + 1;
	i = 0; j = 0; k = des_index;
	while(i < NL && j < NH){
		if(v[low1+i] < v[low2+j]){
			t[k] = v[low1+i];
			i++;
		}
		else{
			t[k] = v[low2+j];
			j++;
		}
		k++;
		/*printf("i = %d, j = %d, k = %d\n",i,j,k);
		for(n = low; n <= high; n++) printf("%d ",v[n]);
		printf("\n");*/ 
	}
	while(i < NL){
		t[k] = v[low1+i];
		i++; k++;
	}
	while(j < NH){
		t[k] = v[low2+j];
		j++; k++;
	}
	/*for(i = low; i < high; i++)
		if(t[i] > t[i+1]){
			printf("Job Not Done!\n");
			return;
		}*/
	//printf("Job Done!\n");
	return;
}
void* quickSortThFunc(void* id){
	int r = (int) id;
	//printf("Thread %d\n",r);
	int low = r*(n/4);
	int high = (r+1)*(n/4) - 1;
	quickSort(low,high);
}
void* serialMergeThFunc(void* id){
	int r = (int) id;
	//printf("Thread %d\n",r);
	int low = r*(n/2);
	int middle = (2*r+1)*(n/4) - 1;
	int high = (r+1)*(n/2) - 1;
	serialMerge(low,middle,high);
}
void* parallelMergeThFunc(void* id){
	int r = (int) id;
	int low1, low2, high1, high2, des_index;
	if(r == 0){
		low1 = 0;
		high1 = (n/4)-1;
		low2 = n/2;
		high2 = bin-1;
		des_index = 0;
	}
	else if(r == 1){
		low1 = n/4;
		high1 = (n/2)-1;
		low2 = bin;
		high2 = n - 1;
		des_index = bin - n/4;
	}
	/*else if(r == 2){	
		low1 = n/4;
		high1 = ((3*n)/8) - 1;
		low2 = bin2;
		high2 = bin3 - 1;
		des_index = bin2 - (n/4);
	}
	else if(r == 3){
		low1 = ((3*n)/8);
		high1 = (n/2) - 1;
		low2 = bin3;
		high2 = n-1;
		des_index = bin3 - (n/8);
	}*/
	//printf("Thread %d: %d %d %d %d %d\n",r,low1,high1,low2,high2,des_index);
	parallelMerge(low1,high1,low2,high2,des_index);	
}

void mergeSortParallel (const int* values, unsigned int N, int* sorted) {
	n = (int) N;
	int m = n/4;
	//printf("n = %d\n",n);
	/*int i;
	int v[] = {9,16,21,23,26,32,-32,9,16,23,26,32};
	t = malloc(12*sizeof(int));
	serialMerge(v,0,5,11);
	for(i = 0; i < 12; i++) printf("%d ",v[i]);
	printf("\n");*/
	v = (int*) values;
	t = sorted;
	//memmove(v,values,sizeof(values));
	pthread_t threads[4];
	int i;
	
	for(i = 0; i < 4; i++){
		pthread_create(&threads[i], (pthread_attr_t*) NULL, quickSortThFunc, (void*) i);
		//printf("Thread %d created\n",i);
	}
	for(i = 0; i < 4; i++){
		pthread_join(threads[i], NULL);
	}
	for(i = 0; i < 2; i++){
		pthread_create(&threads[i], (pthread_attr_t*) NULL, serialMergeThFunc, (void*) i);
		//printf("Thread %d created\n",i);
	}
	for(i = 0; i < 2; i++){
		pthread_join(threads[i], NULL);
	}
		
	//printf("Threads done!\n");

	
	//lastSerialMerge(0, 2*m-1, n-1);
	//
	for(bin = n/2; bin < n; bin++)
		if(v[bin] >= v[n/4]) break;
	
	/*for(bin2 = bin1; bin2 < n; bin2++)
		if(v[bin2] >= v[n/4]) break;
	
	for(bin3 = bin2; bin3 < n; bin3++)
		if(v[bin3] >= v[((3*n)/8)]) break;*/
	
	for(i = 0; i < 2; i++){
		pthread_create(&threads[i], (pthread_attr_t*) NULL, parallelMergeThFunc, (void*) i);
	}
	for(i = 0; i < 2; i++){
		pthread_join(threads[i], NULL);
	}
	return;

}
