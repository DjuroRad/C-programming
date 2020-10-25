#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define PI 3.1415926
#define SQUARE 1
#define RECTANGULAR 2
#define CIRCULAR 3
double CreateBody(int shape);
// red is 0, yellow is 1, blue is 2, black is 3 and white is 4
int SetColor(int color);
//Square is 1, rectangular is 2 and circular is 3.
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
int main()
{
	//DrawEvenCircle(12);
	//DrawOddCircle(9);
	/*
	printf("1-square pokemon\n2-rectangular pokemon\n3-circular pokemon\n");
	int option;
	scanf("%d",&option);
	double area = CreateBody(option);
	printf("%.2lf",area);
	*/

	/*
	printf("Enter color between 0-1000 to assign to your pokemon\n");
	int option2;
	scanf("%d",&option2);
	int color = SetColor(option2);
	if(option2>=0 && option2<=1000)
	{
		switch(color){
			case 0:
				printf("red\n");
				break;
			case 1:
				printf("yellow\n");
				break;
			case 2:
				printf("blue\n");
				break;
			case 3:
				printf("black\n");
				break;
			case 4:
				printf("white\n");
				break;
		}
	}
	else printf("yellow");
	*/
	int shape, color, attack_power;
	double size_of_body, move_length;
	shape = CIRCULAR; // CIRCULAR / SQUARE // Pick one of them //who choses, should be there a menu for this thing!?
	size_of_body = CreateBody (shape);
	color = 798;
	color = SetColor(color);
	move_length = LoadMoves(shape, size_of_body);
	attack_power = SetAttackPower (0, 150);
	ShowPokemon(shape, size_of_body, color, move_length, attack_power);
	return 0;
}
//CREATE BODY FUNCTION
double CreateBody(int shape)
{
	double parameter;
	double parameter2;
	switch(shape){
	case 1:
		printf("Please insert the length of one side of your squared pokemon\n");
		scanf("%lf",&parameter);
		return parameter*parameter;
	case 2:
		printf("Please insert length and width of your rectangular pokemon respectively\n");
		scanf("%lf %lf",&parameter,&parameter2);
		return parameter*parameter2;
	case 3:
		printf("Please insert radius of your circular pokemon\n");
		scanf("%lf",&parameter);
		return parameter*parameter*PI;
	}
}
//SET COLOR FUNCTION
int SetColor(int color)
{
	if(color>=0 && color<=1000) return color%5;
	else return 1;//yellow is one 
}
double LoadMoves(int shape, double body_size)
{
	double a;//side, radius...
	double shorter_side = 5;
	switch(shape){
		case 1:
			a = sqrt(body_size);
			return 4*a;
		case 2:
			a = body_size/shorter_side;
			return 2*shorter_side + 2*body_size;
		case 3:
			a = sqrt(body_size/PI);
			return 2*PI*a;
	}
}
//SET ATTACK FUNCTION
int SetAttackPower(int lower_bound, int upper_bound)
{
	int random = rand()%(upper_bound-lower_bound);
	random += lower_bound;
	return random;
}
//SHOW POKEMON FUNCTION
void ShowPokemon(int shape, double body_size, int color, double
move_length, int attack_power)
{
	double a;//side, radius...
	double shorter_side = 5;
	switch(shape){
		case 1:
			a = sqrt(body_size);
			DrawSquare((int)a);
			printf("Name: Square Pokémon\n");
			printf("Size: %.2lf\n", body_size);
			printf("Color: "); PrintColor(color);
			printf("Move: %.2lf\n",move_length);
			printf("Attack Power: %d\n", attack_power);
			break;
		case 2:
			a = body_size/shorter_side;
			DrawRectangle((int)a);
			printf("Name: Rectangular Pokémon\n");
			printf("Size: %.2lf\n", body_size);
			printf("Color: "); PrintColor(color);
			printf("Move: %.2lf\n",move_length);
			printf("Attack Power: %d\n", attack_power);
			break;
		case 3:
			a = sqrt(body_size/PI);
			DrawCircle((int)a);
			printf("Name: Circular Pokémon\n");
			printf("Size: %.2lf\n", body_size);
			printf("Color: "); PrintColor(color);
			printf("Move: %.2lf\n",move_length);
			printf("Attack Power: %d\n", attack_power);
			break;
	}
	return;
}
// red is 0, yellow is 1, blue is 2, black is 3 and white is 4
void PrintColor(int color)
{
	switch(color){
		case 0:
			printf("red\n");
			break;
		case 1:
			printf("yellow\n");
			break;
		case 2:
			printf("blue\n");
			break;
		case 3:
			printf("black\n");
			break;
		case 4:
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
//RECTANGLE
void DrawRectangle(int side)
{
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
//CIRCLE
void DrawCircle(int side)
{
	if(side%2==0) DrawEvenCircle(side);
	else DrawOddCircle(side);
	return;
}
//PROBLME WITH RANDOM, IT ALWAYS MAKES 133, OR THE SAME NUMBER LET'S SAY ASK THE HOCA!!!
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
			for(int j=0;j<radius;j++)//IT WORK WITH AND WITHOUT RADIUS-1
			{
				if(j==i-radius/2 || j==radius+radius/2-1-i) printf("X");// radius/2-1 but why??
				printf("\t");		
			}
			printf("\n");
		}
	}
	return;
}
void DrawEvenCircle(int radius)
{
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
				if(j==(radius-1)/2-i || j==radius/2+i) 
					{
					/*		
					if(i==0 && j==(radius-1)/2-i)
						{
							 printf("    X\n");
							for(int k=0;k<(radius-1)/2-i;k++) printf("\t");
							//if(radius==2) {printf("X\tX\n    X"); return;}//very very very very bad code
						}
					*/
					printf("X");
					}
				printf("\t");		
			}
			printf("\n");
		}
		else
		{
			for(int j=0;j<radius;j++)//IT WORK WITH AND WITHOUT RADIUS-1
			{
				if(j==i-(radius-1)/2 || j==radius+(radius-1)/2-1-i) printf("X");
				/*
				if(i==radius-2 && j==radius+(radius-1)/2-1-i)
				{
					printf("\n");
					for(int k=0;k<i-(radius-1)/2;k++)
					{
						printf("\t");					
					}
					printf("    X");
				}
				*/
				printf("\t");		
			}
			printf("\n");
		}
	}
		//done like this because of problem with radius 2 where it didn't want to print th elast
	for(int k=0;k<(radius-1)/2;k++)
	{
		printf("\t");					
	}
	printf("    X\n");
	return;
}




