#include <stdio.h>
typedef enum{true, false} boolean;
boolean part4(int number)
{
	boolean res;
	if(number/10==0) res = number%10 * number%10;
	else res = number%10 * number%10 + part4(number/10);
	if(res == number) return true;
	else return res;
}
int main()
{
	int num = 11;
	if(part4(num)==true) printf("equal");
	else printf("notequalt");
}
