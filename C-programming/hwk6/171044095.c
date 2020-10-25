#include <stdio.h>
#define max 1000
#define TRUE 1
#define FALSE 0
int part_1(int num1, int num2, int mult1, int mult2);
void combine(int arr1[], int arr2[], int n1, int n2, int arr[]);
void sort_recr(int arr[], int n);
char part_5(char* string);
void part_3(int num);
int part_4(int num, int n);
void part_2();
void menu();
int main() 
{
	menu();
}
void menu()
{
	/*for part 1*/
	int mult1, mult2;
	mult1=mult2=2;/*smallest divisor here, otherwise program returns 1*/
	int num1;/*used for 3 also*/
	int num2;
	/*for part 4*/
	int num_part4;
	int temp_num_part4;
	int length;
	/*for part 5*/
	char str[max];
	char uppercase_letter;
	int choice;

	do{
		printf("1 - Part1\n");
		printf("2 - Part2\n");
		printf("3 - Part3\n");
		printf("4 - Part4\n");
		printf("5 - Part5\n");
		printf("0 - Exit\n");
		printf("Your choice: ");
		scanf("%d", &choice);
		switch(choice)
		{
			case 1:
				do{
				printf("Please enter num1 and num2 respectively: ");
				scanf("%d %d", &num1, &num2);
				if(num1 == 0 || num2 == 0)printf("Please enter two numbers both of which have multipliers\n");
				}while(num1 == 0 || num2 == 0);
				printf("%d\n", part_1(num1,num2, mult1, mult2));

				break;
			case 2:
				part_2();
				printf("\n");
				break;
			case 3:
				printf("Enter a number: ");
				scanf("%d", &num1);
				part_3(num1);	
				printf("\n");
				break;
			case 4:
				printf("Please enter a number: ");
				scanf("%d", &num_part4);
				temp_num_part4=num_part4;
				for(length=1; temp_num_part4/10!=0; temp_num_part4/=10, length++);
				if(part_4(num_part4,length)) printf("Equal\n");
				else printf("Not Equal\n");

				break;
			case 5:
				printf("Please enter a string: ");
				scanf("%s", str);
				uppercase_letter = part_5(str);
				if(uppercase_letter==FALSE) printf("No uppercase letters in inputted string\n");
				else printf("First uppercase letter from your input is %c\n", uppercase_letter);

				break;
			case 0:
				printf("Exiting...\n");
				break;
			default:
				printf("Please enter a valid option\n");		
		}
	}while(choice!=0);
}
void sort_recr(int arr[], int n)
{
	if(n>=2)
	{
		int n1 = n/2;
		int n2 = n-n1;
		int arr1[n1];
		int arr2[n2];
		int i;
		for(i=0; i<n1; i++)arr1[i] = arr[i];/*filling in the new array with elements of first half of the divided array*/
		for(i=n1; i<n; i++) arr2[i-n1] = arr[i];/*filling in the new array with elements of first second half of the divided array*/
		sort_recr(arr1, n1);/*now dividing newly created array until it is sorted or has 1 element inside it*/
		sort_recr(arr2, n2);/*the same just for the other half*/
		combine(arr1, arr2, n1, n2, arr);/*now returning combining these sorted arrays with this function*/
	}

	return;
}
void part_2()
{
	int n;
	int i;
	printf("Enter the length of the list: ");
	scanf("%d", &n);
	int arr[n];
	printf("Enter the elements of the list:\n");
	for(i=0; i<n; i++) scanf("%d", &arr[i]);
	sort_recr(arr,n);
	printf("Sorted list is: ");
	for(i=0; i<n; i++) printf("%d ", arr[i]);
}
void combine(int arr1[], int arr2[], int n1, int n2, int arr[])
{
	int i=0, j=0, k=0;
	while(j<n1 && k<n2)/*putting together two sorted arrays till they one of them is exhausted*/
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
	while(j<n1)/*in case there is something left here, putting it in the resulting array since all those elements are bigger than in resulting array called arr*/
	{
		arr[i] = arr1[j];
		i++; j++;
	}
	while(k<n2)/*the same as for arr1, just in case arr1 is exhaused insted of arr2*/
	{
		arr[i] = arr2[k];
		k++; i++;
	}
}
int part_1(int num1, int num2, int mult1, int mult2)
{
	int res;
	if(num1==1 || num2==1) res = 1;/*after finding all the multipliers*/
	else if(num1%mult1==0 && num2%mult2==0)
	{
		if(mult1==mult2)
		{
			res=mult1*part_1(num1/mult1,num2/mult2, mult1, mult2);/*found a common multiplier*/
		}
		else if(mult1==num1 || mult2==num2)/*found all multipliers of one of the numbers and inside the statement executing updates so that program would terminate*/
		{
			if(mult1==num1 && mult2==num2) res = part_1(num1/mult1, num2/mult2, mult1, mult2);/*checked all the multipliers and now the program can terminate by setting numbers to 1*/
			else if(mult1!=num1 && mult2==num2) res = part_1(num1, num2, mult1+1, mult2);
			else if(mult1==num1 && mult2!=num2) res = part_1(num1, num2, mult1, mult2+1);		
		}
		else if(mult1>mult2)
		{
			res = part_1(num1, num2/mult2, mult1, mult2);/*updating*/
		}
		else 
		{
			res = part_1(num1/mult1, num2, mult1, mult2);/*updating*/
		}
	}
	else if(num1%mult1==0 && num2%mult2!=0) res = part_1(num1, num2, mult1, mult2+1);
	else if(num1%mult1!=0 && num2%mult2==0) res = part_1(num1, num2, mult1+1, mult2);
	else res = part_1(num1, num2, mult1 + 1, mult2 + 1);
	return res;
}
char part_5(char* string)
{
	char res;
	if( string[0] == 0 ) res=FALSE;/*came to the last element of the string without finding the big letter*/
	else if(string[0]>='A' && string[0]<='Z') res = string[0];/*setting big letter*/
	else res = part_5(&string[1]);
	return res;
}
void part_3(int num)
{
	if(num==1) printf("%d ", num);/*terminating condition, by the function provided because number is now smaller than 2 and it doesnt make sense to do this operation anymore*/
	else if(num%2==0)
	{
		printf("%d ", num);
		part_3(num/2);/*just following the definition*/
	}
	else 
	{
		printf("%d ", num);
		part_3(3*num+1);/*here also*/
	}
	return;
}
int part_4(int num, int n)
{
	int res=1;
	int length;
	int state;
	int temp;
	int i;
	if(num/10==0)/*terminating condition*/
	{
		temp = num;
		length = 1;/*just one digit*/
		for(i=0; i<n;i++) res *= (num%10);
	}
	else
	{
		temp = num;
		for(length=1; temp/10!=0; temp /= 10,length++);/*length used to check last exit from the recursion*/
		for(i=0; i<n;i++) res *= (num%10);
		res = res + part_4(num/10,n);
	}
	if(n==length)/*to check here if the statement is true*/
	{
		if(num==res) state = TRUE;
		else state = FALSE;
		return state;
	}
	else return res;/*updating the result otherwise*/
}




