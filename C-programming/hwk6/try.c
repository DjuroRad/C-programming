#include <stdio.h>
void combine(int arr1[], int arr2[], int n1, int n2, int arr[]);
void sort_recr(int arr[], int n);
void main()
{
	int arr1[4]={1,2, 7,9};

	int arr2[4]={0,2, 3,6};
	int res[8];
	combine(arr1, arr2, 4, 4, res);
	for(int i=0; i<8; i++) printf("%d ", res[i]);
	return;
}
void sort_recr(int arr[], int n)
{
		printf("problem with length of an array");
	if(n>=2)
	{
		int n1 = n/2;
		int n2 = n-n1;
		int arr1[n1];
		int arr2[n2];
		int i,j;
		printf("problem with length of an array");
		for(i=0; i<n1; i++)arr1[i] = arr[i];
		printf("problem with length of an array");
		for(i=0; i<n2;i++) arr2[i] = arr[n2+i];
		printf("not problem with combine");
		sort_recr(arr1, n1);
		printf("not problem with combine");
		sort_recr(arr2, n2);
		printf("problem with combine");
		combine(arr1, arr2, n1, n2, arr);

	}

	return;
}
void combine(int arr1[], int arr2[], int n1, int n2, int arr[])
{
	int n = n1+n2;
	int i=0, j=0, k=0;
	while(j<n1 && k<n2)
	{
		if(arr1[j]<arr2[k])
		{
			arr[i]=arr1[j];
			i++;
			j++;
		}
		else
		{
			arr[i]=arr2[k];
			i++;
			k++;
		}
	}
	while(j<n1) 
	{
		arr[i] = arr1[j];
		i++; j++;
	}
	while(k<n1) 
	{
		arr[i] = arr2[k];
		i++; k++;
	}
}
