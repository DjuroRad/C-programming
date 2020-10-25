#define PI 3
#define SQUARE 1
#define RECTANGULAR 2
#define CIRCULAR 3
#define red 0
#define yellow 1
#define blue 2
#define black 3
#define white 4
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
double CreateBody(int shape);
int SetColor(int color);
double LoadMoves(int shape, double body_size);
int SetAttackPower(int lower_bound, int upper_bound);
void ShowPokemon(int shape, double body_size, int color, double
move_length, int attack_power);
void PrintColor(int color);
void DrawSquare(int side);
void DrawCircle(int radius);
void DrawRectangle(int side);
void DrawEvenCircle(int radius);
void DrawOddCircle(int radius);
double GetParameterFromArea(int shape, double size_of_body);
int main()
{
	int shape, color, attack_power;
	double size_of_body, move_length;
	shape = RECTANGULAR; //RECTANGULAR/ CIRCULAR / SQUARE // Pick one of them	
	size_of_body = CreateBody (shape);
	color = 798;
	color = SetColor(color);
	move_length = LoadMoves(shape, size_of_body);
	attack_power = SetAttackPower (0,150);
	ShowPokemon(shape, size_of_body, color, move_length, attack_power);
	return 0;
}
//CREATE BODY FUNCTION
double CreateBody(int shape)
{
	double parameter,parameter2,area;
	switch(shape){
	case 1:
		printf("Please insert the length of one side of your squared pokemon\n");
		scanf("%lf",&parameter);
		area = parameter*parameter;
		break;
	case 2:
		printf("Please insert length and width of your rectangular pokemon respectively\n");
		scanf("%lf %lf",&parameter,&parameter2);
		area = parameter*parameter2;
		break;
	case 3:
		printf("Please insert radius of your circular pokemon\n");
		scanf("%lf",&parameter);
		area = parameter*parameter*PI;
		break;
	}
	return area;
}
//SET COLOR FUNCTION
int SetColor(int color)
{
	if(color>=0 && color<=1000) return color%5;//0 1 2 3 4 are defined in preprocessor, when this number is returning 0, it is actually red, 1 yellow etc. until 4, i thought about doing it with switches and returning its value like that, but the code would be much longer. I even sent an email regarding this topic and since I couldn't get a response i just left it like this
	else return yellow;//yellow is one 
}
double LoadMoves(int shape, double body_size)
{
	double move;
	double shorter_side = 5;
	double a;//side, radius -> parameter in general...
	switch(shape){
		case SQUARE:
			a = GetParameterFromArea(SQUARE,body_size);
			move = 4*a;
			break;
		case RECTANGULAR:
			a = GetParameterFromArea(RECTANGULAR,body_size);
			move = 2*shorter_side + 2*a;
			break;
		case CIRCULAR:
			a = GetParameterFromArea(CIRCULAR,body_size);
			move = 2*PI*a;
			break;
	}
	return move;
}
//SET ATTACK FUNCTION
int SetAttackPower(int lower_bound, int upper_bound)
{
	srand(time(NULL));//setting the rand num according to current time
	int random = rand()%(upper_bound-lower_bound);//getting random num from within the range
	if( rand()%2==0) random = random + lower_bound;
	else random = upper_bound - random;//setting the num to be between two entered ones
	return random;
}
//SHOW POKEMON FUNCTION
void ShowPokemon(int shape, double body_size, int color, double
move_length, int attack_power)
{
	double a;//side, radius...
	switch(shape){
		case SQUARE:
			a = GetParameterFromArea(SQUARE,body_size);;//getting the side for reference in order to draw
			DrawSquare((int)a);
			printf("Name: Square Pokémon\n");
			break;
		case RECTANGULAR:
			a = GetParameterFromArea(RECTANGULAR,body_size);//getting the side for reference in order to draw
			DrawRectangle((int)a);
			printf("Name: Rectangular Pokémon\n");
			break;
		case CIRCULAR:
			a = GetParameterFromArea(CIRCULAR,body_size);//getting the side for reference in order to draw
			DrawCircle((int)a);
			printf("Name: Circular Pokémon\n");
			break;
	}
	printf("Size: %.2lf\n", body_size);//for rectuangular for example it would print with .2 precision also here even though in homework it printed an integer. You told me to move this all down hocam because the code was repeating 
	printf("Color: "); PrintColor(color);
	printf("Move: %.2lf\n",move_length);//here the same case as it is for size
	printf("Attack Power: %d\n", attack_power);
	return;
}
// red is 0, yellow is 1, blue is 2, black is 3 and white is 4//This function is used just for printing
void PrintColor(int color)
{
	switch(color){
		case red:
			printf("red\n");
			break;
		case yellow:
			printf("yellow\n");
			break;
		case blue:
			printf("blue\n");
			break;
		case black:
			printf("black\n");
			break;
		case white:
			printf("white\n");
			break;
	}
	return;
}
//SQUARE
void DrawSquare(int side)
{
	for(int i=0;i<side;i++)
	{
		for(int j=0;j<side;j++)
		{
			printf("X");
		}
		printf("\n");
	}
	return;
}
//RECTANGLE, number of rows and columns won't appear in a way that user inputed it bcs it is not possible to find the real value of both side just from area
//won't work for numbers smaller than 5 since it is a guessed value and area becomes less than 5 and since int is used another side would be 0
void DrawRectangle(int side)
{
	//since i am casting parameter of side to double nums bigger from example than 7.5 will be considered as 8 and smaller as 7
	int shorter_side=5;
	for(int i=0;i<shorter_side;i++)
	{
		for(int j=0;j<side;j++)
		{
			printf("X");
		}
		printf("\n");
	}
	return;
}
//CIRCLE, separated into two functions for readability, this is used just for calling those two
void DrawCircle(int side)
{
	if(side%2==0) DrawEvenCircle(side);
	else DrawOddCircle(side);
	return;
}
void DrawOddCircle(int radius)
{
	for(int i= 0;i<radius;i++)
	{
		if(i <= radius/2)
		{
			for(int j=0;j<radius;j++)
			{
				if(j==radius/2-i || j==radius/2+i) printf("X");
				printf("\t");		
			}
			printf("\n");
		}
		else
		{
			for(int j=0;j<radius;j++)
			{
				if(j==i-radius/2 || j==radius+radius/2-1-i) printf("X");
				printf("\t");		
			}
			printf("\n");
		}
	}
	return;
}
void DrawEvenCircle(int radius)
{
	//almost the same as odd circle just made radius smaller by one and printed first and last x additionally
	for(int k=0;k<(radius-1)/2;k++)
	{
		printf("\t");
	}
	printf("    X\n");
	for(int i= 0;i<radius-1;i++)
	{
		if(i <= (radius-1)/2)
		{
			for(int j=0;j<radius;j++)
			{
				if(j==(radius-1)/2-i || j==radius/2+i) printf("X");
				printf("\t");		
			}
			printf("\n");
		}
		else
		{
			for(int j=0;j<radius;j++)
			{
				if(j==i-(radius-1)/2 || j==radius+(radius-1)/2-1-i) printf("X");
				printf("\t");		
			}
			printf("\n");
		}
	}
	//done like this because of problem with radius 2 where it didn't want to print the last X
	for(int k=0;k<(radius-1)/2;k++)
	{
		printf("\t");					
	}
	printf("    X\n");
	return;
}
double GetParameterFromArea(int shape, double size_of_body)
{
	double parameter;//side, radius...
	double shorter_side = 5;
	switch(shape)
	{
		case SQUARE:
			parameter = sqrt(size_of_body);
			break;
		case RECTANGULAR:
			parameter = size_of_body/shorter_side;
			break;
		case CIRCULAR:
			parameter = sqrt(size_of_body/PI);
			break;
	}
	return parameter;
}




