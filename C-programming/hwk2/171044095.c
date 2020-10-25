#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define MAXIMUM 1024 /*can be changed to change limits in lucky  number game*/
#define MINIMUM 1
#define MAXTRIAL 10
void menu();
void PlayLuckyNumber(int* score_human, int* score_program);
void draw_hourglass (int height);
void HourGlass();
void UpperHourGlass(int base);
void LowerHourGlass(int base);
int GenerateRandomNumber(int upper_limit);/*This function will generate a random number between 0 and 1024, including both as stated*/
 int make_a_guess (int trial, int min, int max);
void calculate_distance(int lucky_number, int guessed_number, int* min, int* max);
void show_scores(int score_human, int score_program);
void draw_mountain_road (int length, int max_radius);
void draw_road_from_right(int radius, int max);/*max respresents max radius in both draw road from right and from left functions*/
void draw_road_from_left(int radius, int max);
int main()
{
	/*still printing lucky number before everything, CHANGE IT*/
	menu();

	return 0;
}
void menu()
{
	int length, max_radius;/*putting input into these addresses when drawing the road*/
	int choice;/*user's choice from 1 to 4 with respect to menu*/
	int score_human=0,score_program=0;/*used for lucky number game*/
	do{
	printf("***** MENU *****\n");
	printf("1. Play Lucky Number\n");
	printf("2. Draw Hourglass\n");
	printf("3. Draw Mountain Road\n");
	printf("4. Exit\n");
	printf("Choice:\n");
	scanf("%d",&choice);
	switch(choice)
		{
		case 1:	
			PlayLuckyNumber( &score_human, &score_program);
			break;
		case 2:
			HourGlass();
			break;
		case 3:
			printf("Enter the length of your mountain road: ");
			scanf("%d",&length);
			printf("Enter the maximum radius of your mountain road: ");
			scanf("%d",&max_radius);
			draw_mountain_road( length, max_radius);
			break;
		case 4:
			printf("Exiting....\n");
			break;
		default:
		printf("This is an invalid choice. Try again!\n");
		}
	}while(choice != 4);

	 return;
}
void PlayLuckyNumber(int* score_human, int* score_program)
{
	int max_limit=MAXIMUM;/*this limit can be changed in preprocessor to make program with smaller values*/
	int min_limit=MINIMUM;
	int T = 0; /*the number of trials according to the value in preprocessor's MAXTRIAL*/
	int LN =GenerateRandomNumber(max_limit);
	/*printf("%d \n",LN);*//*you can print it to see the lucky number if you remove comment indicators*/
	int GN; /*number player is guessing*/
	do{
		T++;
		GN = make_a_guess(T,min_limit,max_limit);
		while(!( GN>=min_limit && GN<=max_limit ))
		{
			printf("Try again, your number is not between %d and %d \n", min_limit, max_limit);
			GN = make_a_guess(T,min_limit,max_limit);
		}
	calculate_distance(LN, GN, &min_limit, &max_limit);	
	}while(GN != LN && T!=MAXTRIAL);
	if(GN == LN) *score_human = *score_human + 1;/*this will update the scores of values that are in score human and score program addresses*/
	else *score_program = *score_program + 1;
	show_scores(*score_human, *score_program);

	return;
}
 int make_a_guess (int trial, int min_limit, int max_limit)
{
	int guess;
	printf("(Trial: %d) Make a guess between %d and %d: ", trial, min_limit, max_limit);
	scanf("%d",&guess);

	return guess;
}
int GenerateRandomNumber(int upper_limit)/*function for generating a lucky number*/
{
	srand(time(NULL));
	int random_number =rand()%(upper_limit+1);
	do{
		random_number =rand()%(upper_limit+1);
	}while(random_number == 0);/*zero is not included and i could generate a number between any min and max value, but there is no need as it is not stated in the homework*/
	return random_number;
}

void HourGlass()
{
	/*function is writted to aviod making menu function less readable*/
	int height;
	do{
	printf("Enter height of your hourglass: ");
	scanf("%d",&height);
	if(height % 2 == 0) printf("Please enter an odd number\n");
	}while(height%2 == 0) ;/*Until user enter odd number*/
	if(height<0) height = height*(-1);/*if user enters a negative number*/
	draw_hourglass (height);

	return;
}
void draw_hourglass (int height)
{
	printf("\nHeight:%d \n",height);
	UpperHourGlass(height);
	LowerHourGlass(height);
	printf("\n");

	return;
}

void UpperHourGlass(int base)
{
	/*drawing an upside down traingle*/
	printf("\n");
	int j;
	int i;
	for(i=0;i<=base/2;i++)
	{
		for(j=0;j<i;j++) printf(" ");
		for(j=0;j<base-2*i;j++) printf("*");
		printf("\n");
	}

	return;
}
void LowerHourGlass(int base)
{
	/*drawing a normal triangle without the first layer since it is already drawn in upperhourglass which comes before it when calling draw_hourglass function*/
	int i;
	int j;
	for(i=1;i<=base/2;i++)
	{
		for(j=0; j<base/2-i; j++) printf(" ");
		for(j=0;j<2*i+1;j++) printf("*");
		printf("\n");
	}

	return;
}
 void draw_mountain_road (int length, int max_radius)
{
	srand(time(NULL));
	int random;
	int i;
	for(i=0; i<length; i++)
	{
		random = rand()%(max_radius+1);/*added +1 beacuse i want to have that max number included, note theat 0 is also included */
		if(i%2==0){/*i%2 because i want to make it change side for every newly generated random number*/
			draw_road_from_right(random, max_radius);
			}
		else {
			draw_road_from_left(random, max_radius);
		}
	}

	return;
}
void draw_road_from_right(int radius, int max)
{
	/*in both draw from right and from left function indentations are made according to maximum radius value which is constant so that they could begin from the same place every time */
	int i,j;
	for(i=0;i<radius;i++) 
	{
		for(j=0;j<max-i;j++) printf(" ");
		printf("/\n");
	}
	for(i=0; i<max-radius; i++) printf(" ");
	printf("|\n");
	for( i=0; i<radius; i++)
	{
		for(j=0; j<=max-radius+i;j++) printf(" ");
		printf("\\\n");
	}

	return;
}
void draw_road_from_left(int radius, int max)
{
	/*in both draw from right and from left function indentations are made according to maximum radius value which is constant so that they could begin from the same place every time */
	int i,j;
	
	for( i=0; i<radius; i++)
	{
		for( j=0; j<= max; j++) printf(" ");
		for( j=0; j<i; j++) printf(" ");
		printf("\\\n");
	}
	for( i=0; i<= max+radius; i++) printf(" ");
	printf("|\n");
	for( i=0; i< radius; i++)
	{
		for( j=1; j<= max + radius - i; j++) printf(" ");
		printf("/\n");
	}

	return;
}
void calculate_distance(int lucky_number, int guessed_number,int* min_limit, int* max_limit)
{
	int distance,i;
	if( lucky_number>guessed_number)
	{
		distance = lucky_number - guessed_number;
		*min_limit = guessed_number;
	}
	else 
	{
		distance = guessed_number - lucky_number;
		*max_limit = guessed_number;
	}
	if(distance != 0) /*or if lucky number is not the guessed number*/
	{
		for( i=0; !(distance>=pow(2,i) && distance<pow(2,i+1)) ; i++);
 		distance = i+1;/*1024 is 2^10 and 512 2^9 so  i will come to 9 and exit the loop, to come to the real distance i will have to increment it by 1*/
		printf("Distance: %d\n", distance);
	}
	else printf("Distance: %d. You win!\n", distance);/*when dist is 0 or lucky num is equal to guessed num*/
 	return;
}
void show_scores(int score_human, int score_program)
{
	printf("Your score: %d	Program score: %d \n\n", score_human, score_program);

	return;
}








