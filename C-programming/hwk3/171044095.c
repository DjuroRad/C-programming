#include <stdio.h>
#define HOMEWORKGRADES 10
#define LABGRADES 10
#define EXAMGRADES 2
#define CALCULATOR 1 
#define GRADE_CALC 2
#define DRAW_DIAMOND 3
#define EXIT 4
#define midtermIndex 0
#define finalIndex 1
int take_grades(int [] );
int take_exam_grades(int[]);
double calculate_homework(int[]);
double calculate_lab(int[]);
double calculate_all(int ,int[],int);/*ask prof bcs I changed the function prototype and put doubles instead of ints, but it is impossible to do it with ints, i mean it isn't but it makes no sense to do it with ints*/
void menu();
void grade_calculator();
void calculator();
int doit (int operation(int number1, int number2), int first_number, int second_number);
int add( int num1, int num2);
int sub( int num1, int num2);
int div( int num1, int num2);
int mult( int num1, int num2);
int pwr( int num1, int num2);
int mod( int num1, int num2);
void draw_shape();
void calculator_menu();
int main()
{
	menu();
	return 0;
}
void menu()
{
	int choice;
	do{
		printf("1 - Part I\n");
		printf("2 - Part II\n");
		printf("3 - Part III\n");
		printf("4 - exit \n");
		scanf("%d",&choice);
		switch(choice)
		{
			case CALCULATOR:
				calculator();
			break;
			case GRADE_CALC:
				grade_calculator();
			break;
			case DRAW_DIAMOND:
				draw_shape();
			break;
			case EXIT:
			printf("Exiting....\n");
			break;
			default:
			printf("Invalid input, please try again\n");
		}
	}while(choice != 4);

	return;
}
void grade_calculator()
{
	int lab_grades[LABGRADES];
	int homework_grades[HOMEWORKGRADES];
	int exam_grades[EXAMGRADES];
	double homework_average, lab_average, weighted_average;
	printf("Enter %d lab grades: \n", LABGRADES);
	take_grades(lab_grades);
	printf("Enter %d homework grades: \n", HOMEWORKGRADES);
	take_grades(homework_grades);
	homework_average = calculate_homework(homework_grades);
	lab_average = calculate_lab(lab_grades);
	take_exam_grades(exam_grades);
	weighted_average = calculate_all((int)lab_average, exam_grades, (int)homework_average);
	/*printf("Lab average grade for this student is: %.2f\n", lab_average);
	printf("Homework average grade for this student is: %.2f\n", homework_average);
	*/
	/*you can uncomment part above to see lab and homework averages*/
	/*Note that there will a certain error in calculation because of type casting requiried by prototype functions*/
	printf("Weighted average grade for this student is: %.2f\n\n", weighted_average);	
}
void calculator()
{
	char choice;/*checking the operation*/
	char choiceCheck;/*checking if it is ** operation*/
	char numberCountControl;/*checking number of integer inputs from the user*/
	int number1;
	int number2=0;/*zero taken as previous value if only one number is entered by the user*/
	int temp;/*when only one input occurs*/
	do{
		calculator_menu();/*just some console output for user*/
		scanf("\n%c",&choice);/*giving leftovers to buffer so that it could pass it to them when scanf is called the next time*/
		/*printf("%c\n",choice);*/
		if(choice == 'q') break;
		else
		{
			scanf("%c%d%c", &choiceCheck, &number1, &numberCountControl);
			if( choice == '+' || choice == '-' || choice == '%' || choice == '/' || choice == '*' ) 
			{ 
				if(numberCountControl==' ') scanf("%d",&number2);
				else{
					temp = number2;
					number2 = number1;
					number1 = temp;
					/*If user enters only one number, we want to use result of the previous operation as the first number,
					hence we will need to transfer value of number1 (new input) to number 2
					and number 2's value (result of previous operation) to number1
					*/
				}
			}
			else{
				if(numberCountControl==' ') scanf("%d",&temp);/*removing the trash left in the stdin stream's buffer, when temp is used this value is overwritten anyways*/
			}
		}
		if(choiceCheck == '*')
			{
				number2 = doit(pwr, number1, number2);
			}
		else{/*when there is a space in choice check*/
			switch(choice)
			{
				case '+':
					number2 = doit(add, number1, number2);
					break;
				case '-':
					number2 = doit(sub, number1, number2);
					break;
				case '*':
					number2 = doit(mult, number1, number2);
					break;
				case '/':
					number2 = doit(div, number1, number2);
					break;
				case '%':
					number2 = doit(mod, number1, number2);
					break;
				default:
					printf("Invalid choice, try again\n");	
			}
		}
	}while(choice!='q');
	printf("Going back!\n");
}
void draw_shape()
{
	int height;
	int i,j;
	printf("Insert height of your shape: ");
	scanf("%d", &height);
	/*first loop makes a normal traingle*/
	for(i=0;i<height;i++)
	{
	/*upper part*/
	for(j=1;j<height-i;j++) printf(" ");
	printf("/");
	for(j=0;j<i*2;j++) printf("*");
	printf("\\");
	printf("\n");
	}
	/*this second loop makes an upside down triangle*/
	for(i=0;i<height;i++)
	{
	/*down part*/
	for(j=0;j<i;j++) printf(" ");
	printf("\\");
	for(j=0;j<2*(height-1-i);j++) printf("*");
	printf("/");
	printf("\n");
	}
}
int take_grades(int lab_or_homework_grades[] )
{
	int i;
	for(i=0;i<HOMEWORKGRADES;i++)
	{
		printf("%d: ", i+1); 
		scanf("%d", lab_or_homework_grades+i);
	}
	return 0;
}
int take_exam_grades(int exam_grades[])
{
		printf("Please enter the midterm grade: ");
		scanf("%d", &(exam_grades[midtermIndex]));
		printf("Please enter the final grade: ");
		scanf("%d", &(exam_grades[finalIndex]));
		return 0;
}
double calculate_homework(int homework_grades[])
{
	double hw_average;
	int sum = 0;
	int i;
	for(i = 0; i<HOMEWORKGRADES; i++)
	{
		sum = sum + homework_grades[i];
	}
	hw_average = (double)sum/(double)HOMEWORKGRADES;
	return hw_average;
}
double calculate_lab(int lab_grades[])
{
	double lab_average;
	int sum = 0;
	int i;
	for(i = 0; i<LABGRADES; i++)
	{
		sum = sum + lab_grades[i];
	}
	lab_average = (double)sum/(double)LABGRADES;
	return lab_average;
}
/*calculating weighted average*/
double calculate_all(int lab_average,int exam_grades[],int homework_average)
{
	double weighted_average;
	weighted_average =(double)homework_average/10.0 + (double)lab_average/5.0 + ((double)exam_grades[midtermIndex]/10.0)*3.0 + ((double)exam_grades[finalIndex]/10.0)*4.0;
	return weighted_average;
}
int doit (int operation(int , int ), int first_number, int second_number)
{
	int value = operation(first_number,second_number);
	printf("%d \n", value);
	return value;/*updating so that we could do something if user enters only one of the numbers*/
}
/*addition function*/
int add(int num1, int num2)
{
	int res;
	res = num1 + num2;
	num1 = res;
	return res;
}
/*substitution function*/
int sub(int num1, int num2)
{
	int res;
	res = num1 - num2;
	num1 = res;	
	return res;
}
/*division function*/
int div(int num1, int num2)
{
	int res=0;
	while(num1>=num2)
	{
		num1 = num1 - num2;
		res++;
	}
	return res;
}
/*multiplication function*/
int mult( int num1, int num2)
{
	int res=0;
	int i;
	for(i=0; i<num2; i++)
	{
		res = res+num1;
	}
	return res;
}
/*power function*/
int pwr( int num1, int num2)
{
	int i;
	int res = 1;
	for(i = 0; i<num2; i++)
	{
		res = res*num1;
	}
	return res;
}
/*modulus function*/
int mod( int num1, int num2)
{
	int res = num1;
	while(res>=num2)
	{
		res = res-num2;
	}
	return res;
}

void calculator_menu()
{
	printf("'+' -> Addition\n");
	printf("'-' -> Substraction\n");
	printf("'%%' -> Modulus\n");
	printf("'*' -> Multiplication\n");
	printf("'/' -> Division\n");
	printf("'**' -> Power\n");
	printf("'q' -> Return back to main program\n");
	
}
