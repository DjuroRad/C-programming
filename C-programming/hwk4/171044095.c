#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define MAXNAME 11
#define ROWS 8
#define COLUMNS 8
/*Changed it to 11 bcs it doesn't work with charmander since it is 10
Compiler doesn't complain when assigning the name 
But using length of the
*/
#define POKEMONTYPES 10
#define MAXNUMOFPOKEMONS 4
#define PLAYERMAX 4
#define TRUE 1
#define FALSE 0
#define EMPTY -1
/*are we supposed to use structures
how do we compare strings if we are not allowed to use string.h
how to call pokedex function if the only user-inputed argument entered is pokemon's name but
function needs range, attc. type powr stam and similar
*/
/*pokemon[10][20] = {"pikachu"*/
typedef enum{quadratic, linear}attack_type;/*this is used only as a preprocessor directive almost nowhere else is it seen*/
typedef enum{Charmander, Pikachu, Squirtle, Bulbasaur, Pidgeotto, Ratata, Mug, Caterpie, Zubat, Krabby}pokemon;
void pokedex(char Pokemon_name[POKEMONTYPES][MAXNAME], int range[], attack_type type[], int attack_power[], int stamina[]);
void oaks_laboratory(char Pokemon_name[POKEMONTYPES][MAXNAME], pokemon Pokemons[], pokemon *my_Pokemons);
void show_Pokemons(char Pokemon_name[POKEMONTYPES][MAXNAME], pokemon Pokemons[], int pokemon_count);
void catch_a_pokemon(char Pokemon_name[POKEMONTYPES][MAXNAME], pokemon Pokemons[], pokemon *my_pocket);
void release_a_pokemon(char Pokemon_name[POKEMONTYPES][MAXNAME], pokemon Pokemons[], pokemon *my_pocket);
int is_in_pocket( pokemon *my_pocket, pokemon chosen_pokemon);
void battle(char Pokemon_name[POKEMONTYPES][MAXNAME], int range[], attack_type type[], int attack_power[], int stamina[], pokemon user_Pokemons[]);
void generate_bot(pokemon enemy_Pokemon[PLAYERMAX], int area[COLUMNS][ROWS], int pokemon_staminas[COLUMNS][ROWS], int stamina[], int bot_poks_on_area[COLUMNS][ROWS]);
void player_setup(char Pokemon_name[POKEMONTYPES][MAXNAME], int area[COLUMNS][ROWS], pokemon* my_Pokemons, int
pokemon_staminas[COLUMNS][ROWS], int stamina[], int user_poks_on_area[COLUMNS][ROWS]);
int check_if_area_space_full(int area[ROWS][COLUMNS],int row_index, int coln_index);
void show_area (char Pokemon_name[POKEMONTYPES][MAXNAME],int area[COLUMNS][ROWS],int
pokemon_staminas[COLUMNS][ROWS]);
void player_move(char Pokemon_name[POKEMONTYPES][MAXNAME], pokemon my_Pokemons[], pokemon enemy_pokemons[], int area[COLUMNS][ROWS],int pokemon_staminas[COLUMNS][ROWS], int user_poks_on_area[COLUMNS][ROWS], int bot_poks_on_area[COLUMNS][ROWS],int range[], attack_type type[], int attack_power[]);/*bot poks not needed*/
void bot_move(pokemon bot_Pokemons[], pokemon user_Pokemons[], int area[COLUMNS][ROWS], int pokemon_staminas[COLUMNS][ROWS], int bot_poks_on_area[COLUMNS][ROWS], int user_poks_on_area[COLUMNS][ROWS], int range[],attack_type type[],int attack_power[]);
void find_pokemon(pokemon pok_chosen, int area[COLUMNS][ROWS], int user_poks_on_area[COLUMNS][ROWS], int* chosen_pokemon_column, int* chosen_pokemon_row);
void attack(pokemon pokemon_ruler, int pokemon_ruler_column, int pokemon_ruler_row, int range[], attack_type type[], int attack_power[], int area[COLUMNS][ROWS],int pokemon_staminas[COLUMNS][ROWS], int enemy_poks_on_area[COLUMNS][ROWS], pokemon enemy_pokemons[]);
void remove_pokemon(int pok_column, int pok_row, int area[COLUMNS][ROWS], int enemy_poks_on_area[COLUMNS][ROWS], int pokemon_staminas[COLUMNS][ROWS], pokemon enemy_pokemons[]);
void menu(pokemon Pokemons[POKEMONTYPES], pokemon my_Pokemons[PLAYERMAX], char Pokemon_name[POKEMONTYPES][MAXNAME], int range[POKEMONTYPES], int attack_power[POKEMONTYPES], int stamina[POKEMONTYPES],attack_type type[POKEMONTYPES]);
int absolute(int number);
int compare_strings( char* first_name, char* second_name);
int get_lenght(char* string);

int main()
{
	int choice;
	srand(time(NULL));
	/*	
	printf("%c%c",0,10);
	it doesn't print anything for a null caracter
	prints new line for 10
	*/
	pokemon Pokemons[POKEMONTYPES] = {Charmander, Pikachu, Squirtle, Bulbasaur, Pidgeotto, Ratata, Mug, Caterpie, Zubat, Krabby};
	pokemon my_Pokemons[PLAYERMAX] = {EMPTY, EMPTY, EMPTY, EMPTY};/*-1 intialization helps knowing how many pokemons are chosen*/
	int i;
	/*char *Pokemon_name[POKEMONTYPES]*/
	char Pokemon_name[POKEMONTYPES][MAXNAME] ={
	"Charmander",
	"Pikachu",
	"Squirtle",
	"Bulbasaur",
	"Pidgeotto" ,
	"Ratata",
	"Mug",
	"Caterpie",
	"Zubat",
	"Krabby"
	};
	/*
	printf("%c",Pokemon_name[Charmander][10]);
	it doesn't print anything for a null caracter
	prints new line for 10
	*/
	/*
	printf("%d\n\n",Pokemon_name[Charmander][MAXNAME-1]);
	terminating caracter for strings is \0 or zero, this is a NEWLINE caracter in ascii!!!!
	*/
	/*
	printf("%c\n\n",Pokemon_name[Charmander][MAXNAME]);
	this would print P, which is the first letter of the next string in the consecutive memory
	so charmander would be missing a 0 terminating caracter!!!!!
	*/
	int range[POKEMONTYPES]={1,3,4,2,2,2,1,2,3,2};
	int attack_power[POKEMONTYPES] = {500,350,300,400,250,250,350,200,350,300};
	attack_type type[POKEMONTYPES] = {quadratic, linear, linear, linear, quadratic, linear, quadratic, quadratic, linear, linear};
	int stamina[POKEMONTYPES] = {2200, 1500, 1700, 2500, 1900, 2500, 3000, 1200, 1250, 2600};
	
	
	/*scanf("%s", Pokemon_name);*/
	/*range[MAXNUMOFPOKEMONS-1]= {0};*/	
	/*printf("%d\n",range[0]);*/

	/*
	char sth[5] = "what";
	printf("%d", sth[5]);
	printf("%s",sth);
	it works, it prints zero somehow	
	*/

	menu(Pokemons, my_Pokemons, Pokemon_name, range, attack_power, stamina, type);
	/*
	printf("%p\n",Pokemon_name);
	printf("%p\n", &Pokemon_name);
	these will print out the same thing showing that the array is actually an address no these notations are the same
	*/		
/*pokedex( Pokemon_name, 
	printf("%s\n", &Pokemon_name);
	this makes a problem saying warning format %s expects argument of type char*, but argument 2 has type char(*)[20]
	*/
	/*printf("%d\n", *Pokemon_name);
	this would actually print a value of 112 for pikachy input
	*/
	/*
	gives an error
	printf("%s\n", Pokemon_name[]);
	*/
	/*printf("%s", Pokemon_name);*/

}
void pokedex(char Pokemon_name[POKEMONTYPES][MAXNAME], int range[], attack_type type[], int attack_power[], int stamina[])
{
	int i;
	int chosen_pokemon_index=-2;/*setting to some value that can't be neither index nor exit condition so that it would execute bcs otherwise C might assign it 0*/
	char Pokemon_chosen[MAXNAME];
	do{
		printf("Please type name of the Pokémon (type exit to close Pokedex): \n");
		scanf("%s", Pokemon_chosen);
		for( i=0; i<POKEMONTYPES; i++)
		{
			if( compare_strings(Pokemon_chosen, Pokemon_name[i]) == 0)
			{
				chosen_pokemon_index = i;
				printf("%-7s: %-4s\n","Name",Pokemon_name[i]);
				switch(type[i])
				{
					case linear:
					printf("%-7s: %-4s\n","A. Type","Linear");
					break;
					case quadratic:
					printf("%-7s: %-4s\n","A. Type","Quadratic");
					break;
				}
				printf("%-7s: %d block\n","Range", range[i]);
				printf("%-7s: %-4d\n", "A.power",attack_power[i]);
				printf("%-7s: %-4d\n","Stamina", stamina[i]);
				break;
			}
			else if( compare_strings(Pokemon_chosen, "exit") == 0){ chosen_pokemon_index = -1; break;}
		}
	}while(chosen_pokemon_index != -1);
}
void oaks_laboratory(char Pokemon_name[POKEMONTYPES][MAXNAME], pokemon Pokemons[], pokemon *my_Pokemons)
{
/*initializing my_pocket array so that I could pass my_pocket to another functions within this one*/
	
	/*
	for(i=0; i<PLAYERMAX; i++) my_Pocket[i] = my_Pokemons[i];
	my_Pocket[0] = Charmander;
	my_Pocket[1] = Ratata;
	my_Pocket[2] = Squirtle;
	my_Pocket[3] = Zubat;
	show_Pokemons(Pokemon_name, my_Pocket, 4);
	*/
	int choice;
	do{
		printf("Welcome to Laboratory of Professor Oak. How can I help you?\n   1) Show Pokémons\n   2) Catch a Pokémon\n   3) Release a Pokémon\n   4) Show my pocket\n   5) Back\n");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:
				show_Pokemons(Pokemon_name, Pokemons, POKEMONTYPES);
				break;
			case 2:
				catch_a_pokemon(Pokemon_name, Pokemons, my_Pokemons); 
				break;
			case 3:
				release_a_pokemon(Pokemon_name, Pokemons, my_Pokemons);
				break;
			case 4:
				show_Pokemons(Pokemon_name,my_Pokemons, PLAYERMAX);
				break;
			case 5:
				printf("Going back to main menu....\n");
			break;
			default:
				printf("Invalid option, please try again\n");
		}
	}while(choice !=5);
	/*updating My_pokemons*/
	return;
}

void show_Pokemons(char Pokemon_name[POKEMONTYPES][MAXNAME], pokemon Pokemons[], int pokemon_count)
{
	int i;
		for(i=0; i<pokemon_count; i++)
		{
			/*
			printf("%d - %s\n", Pokemons[i], Pokemon_name[Pokemons[i]]);
			*/
			if( Pokemons[i] == EMPTY)
			{
				if( i==0 ) printf("You don't have any pokemons\n");			
				break;/*exiting if it came to the last element*/
			}
			else	printf("%d - %s\n", i, Pokemon_name[Pokemons[i]]);
		}
	return;
}

void catch_a_pokemon(char Pokemon_name[POKEMONTYPES][MAXNAME], pokemon Pokemons[], pokemon *my_pocket)
{
	int choice;
	int free_index=0;
	/*checking for space where a pokemon can be added if it can be added at all meaning not all elements are different than -1*/
	for(free_index=0; my_pocket[free_index] != EMPTY; free_index++)/**/
		{
			if(free_index == PLAYERMAX) break;/*if it is full I don't want it keep increasing by running through garbage vales of memory*/
		}
	/*printf("%d",free_index); it prints 4*/
		do{
			if(free_index==PLAYERMAX) /*if player has 4 pokemons*/
			{
				printf("Your pocket is full!\nYou are ready to battle!\n");
				break;
			}
			show_Pokemons(Pokemon_name, Pokemons, POKEMONTYPES);
			/*here it is going into pokemons[-1] so idk what happens than, I should maybe just remove this*/
			scanf("%d",&choice);
			if(choice >= 0 && choice <POKEMONTYPES)
			{
				if(is_in_pocket(my_pocket, Pokemons[choice]) == TRUE)
					{
						printf("You already posses this pokemon.\nPlease try to select another one\n");
						choice = -1;/*setting choice to some value so that it gets into the loop again reporting this error at the same time*/
					}
				else 
				{		
				 	my_pocket[free_index] = Pokemons[choice];
					free_index++;
				}
			}
			else
			{
				printf("Error. Invalid option.\n");
			}
		}while(choice<0 || choice>=POKEMONTYPES);
		/*while(choice != -1 || free_index!=PLAYERMAX);*/
	return;
}
int is_in_pocket( pokemon *my_pocket, pokemon chosen_pokemon)
{
	int i;
	int res=FALSE;
	for(i=0; my_pocket[i] != EMPTY; i++)
	{
		if(my_pocket[i] == chosen_pokemon)
		{
			res = TRUE;
			break;
		}
	}
	return res;
}
void release_a_pokemon(char Pokemon_name[POKEMONTYPES][MAXNAME], pokemon Pokemons[],
pokemon *my_pocket)
{
	int choice;
	int free_index=0;
	int i;
	/*put this free_index finding in a seperate function if you have time*/
	/*checking for space where a pokemon can be added if it can be added at all meaning not all elements are different than -1*/
	for(free_index=0; my_pocket[free_index] != EMPTY; free_index++)
		{
			if(free_index == PLAYERMAX) break;/*if it is full I don't want it keep increasing by running through garbage vales of memory*/
		}
	
	if(free_index==0) printf("Before being able to release a pokemon, you need to first catch it!\n");
	else {
			show_Pokemons(Pokemon_name, my_pocket, PLAYERMAX);
			printf("Which pokemon do you want to release:\n");
			scanf("%d",&choice);
			for(i=choice; i<free_index; i++)
			{
				my_pocket[i] = my_pocket[i+1];
				/*this is setting new arangement of the pokemons beacuse it changes when user removes one of them, so it is just iterating until the last free space */
			}
			if(free_index == PLAYERMAX) my_pocket[free_index-1]=EMPTY;
			/*if my pocket is full and then one pokemon is removed last entry should be -1, if this condition if not put it would take some garbage value from memory*/
	}
	return;
}
int get_length(char* string)
{
	int i;
	for(i=0; string[i]!=0;i++);
	return i;
}
int compare_strings( char* first_name, char* second_name)
{
	int res=0;/*if it stays 0 when returning, it will mean that they are the same*/
	int i;
	int length_f_name = get_length(first_name);
	int length_s_name = get_length(second_name);
 	if(length_f_name != length_s_name) res = -1;
	else{
		for(i = 0; i< length_f_name; i++)
		{
			if(first_name[i] != second_name[i])
			{
				res = -1;
				break;
			}
		}
	}
	return res;
}
void battle(char Pokemon_name[POKEMONTYPES][MAXNAME], int range[], attack_type type[], int attack_power[], int stamina[], pokemon user_Pokemons[])
{
	int i,j;
	int area[ROWS][COLUMNS];
	pokemon bot_Pokemons[PLAYERMAX] = {EMPTY,EMPTY,EMPTY,EMPTY};
	int pokemon_staminas[COLUMNS][ROWS];
	int user_poks_on_area[COLUMNS][ROWS], bot_poks_on_area[COLUMNS][ROWS];

	for(i=0; i<COLUMNS; i++)
	{
		for(j=0; j<ROWS; j++)
			{
				area[i][j] = EMPTY;
				pokemon_staminas[i][j] = EMPTY;
				user_poks_on_area[i][j] = FALSE;
				bot_poks_on_area[i][j] = FALSE;
			}
		/*initializing the area to check if it is filled, put -1 means empty, anything else full let's say*/
	}
	/*
	area[4][5] = Pikachu;
	printf("%d\n", check_if_area_space_full(area, 4, 5));
	*/
	/*making a bot*/
	printf("passed\n");
	generate_bot(bot_Pokemons, area, pokemon_staminas, stamina, bot_poks_on_area);
	printf("passed\n");
	player_setup(Pokemon_name, area, user_Pokemons, pokemon_staminas, stamina, user_poks_on_area);
	printf("no  stop\n");
	for(i=0; i<8; i++){
	for(j=0;j<8;j++){
	if(user_poks_on_area[i][j]==TRUE) printf("users::: column: %d, row: %d\n",i,j);
	}}
	for(i=0; i<8; i++){
	for(j=0;j<8;j++){
	if(bot_poks_on_area[i][j]==TRUE) printf("bots::: column: %d, row: %d\n",i,j);
	}}
	do
	{
	show_area (Pokemon_name, area, pokemon_staminas);
	/*exit 2 1 2 3 4 5 0 1 0 2 1 5 1 7*/
	player_move(Pokemon_name, user_Pokemons, bot_Pokemons, area, pokemon_staminas, user_poks_on_area, bot_poks_on_area, range, type, attack_power);/*bot poks not needed*/
	for(i=0; i<8; i++){
	for(j=0;j<8;j++){
	if(user_poks_on_area[i][j]==TRUE) printf("users::: column: %d, row: %d\n",i,j);
	}}
	show_area (Pokemon_name, area, pokemon_staminas);
	if(bot_Pokemons[0]!=EMPTY) bot_move(bot_Pokemons, user_Pokemons, area, pokemon_staminas, bot_poks_on_area, user_poks_on_area, range, type, attack_power);/*bot poks not needed*/
	for(i=0; i<8; i++){
	for(j=0;j<8;j++){
	if(bot_poks_on_area[i][j]==TRUE) printf("bots::: column: %d, row: %d\n",i,j);
	}}
	}while(bot_Pokemons[0]!=EMPTY && user_Pokemons[0] != EMPTY);
	if(user_Pokemons[0] == EMPTY) printf("You have lost! :(\n");
	else printf("Congratulations! You have won");
}
void generate_bot(pokemon enemy_Pokemon[PLAYERMAX], int area[COLUMNS][ROWS], int
pokemon_staminas[COLUMNS][ROWS], int stamina[], int bot_poks_on_area[COLUMNS][ROWS])
{
	pokemon temp;
	int player_column_limit = 2;
	int random_row;
	int random_column;
	int i;
	printf("In\n");
	for(i=0; i<PLAYERMAX; i++) 
	{
		do{
			temp = rand()%POKEMONTYPES;
	printf("In\n");
		}while(is_in_pocket(enemy_Pokemon, temp)==TRUE);
	printf("In\n");
		/*finding a pokemon*/
		enemy_Pokemon[i] = temp;
		do{
			random_row = rand()%ROWS;/*anywhere in the row*/
			random_column=COLUMNS - 1 - rand()%player_column_limit;/*can put it in first two columns in the beginning only*/
		}while(check_if_area_space_full(area, random_column, random_row)==TRUE);
		/*putting a pokemon in an empty space*/

		bot_poks_on_area[random_column][random_row] = TRUE;/*1 or TRUE indicating there is user's pokemon in this array*/
		area[random_column][random_row] = enemy_Pokemon[i];
		pokemon_staminas[random_column][random_row] = stamina[enemy_Pokemon[i]];
	}
}
void player_setup(char Pokemon_name[POKEMONTYPES][MAXNAME], int area[COLUMNS][ROWS], pokemon* my_Pokemons, int pokemon_staminas[COLUMNS][ROWS], int stamina[], int user_poks_on_area[COLUMNS][ROWS])
{
	int i;
	int user_row, user_column;
	int column_limit=2;

	printf("Welcome to the POKE Tournament!\n");
	for(i=0; i<PLAYERMAX; i++) 
	{
		do{
			printf("Plese enter column and row coordinates respectively for your %s:\n", Pokemon_name[my_Pokemons[i]]);
			scanf("%d %d", &user_column, &user_row);
			if( (user_column < 0 || user_column>=column_limit) || (user_row<0 || user_row>=ROWS) )
			{
				printf("Out of bounds. Try again.\n");
			}
			/*if already put a pokemon there*/
			else if(check_if_area_space_full(area, user_column, user_row)==TRUE) printf("You have already put a pokemon here.\n");
			else
			{
				user_poks_on_area[user_column][user_row] = TRUE;/*1 or TRUE indicating there is user's pokemon in this array*/
				area[user_column][user_row] = my_Pokemons[i];
				pokemon_staminas[user_column][user_row] = stamina[my_Pokemons[i]];
				break;
				/*if it comes to this point just break because it passed through all conditions*/
			}
		}while(TRUE);
	}
}
int check_if_area_space_full(int area[COLUMNS][ROWS], int coln_index, int row_index)
{
	int res;
	if(area[coln_index][row_index] != EMPTY ) res = TRUE;
	else res = FALSE;
	return res;
	/*
	for(i=0; i<COLUMNS && res==FALSE ;i++)
	{
		for(j=0; j<ROWS; j++)
		{
			if(area[i][j] != -1)
			{
				res = TRUE;
				break;
			}
		}
	}
	return res;
	*/
}
void player_move(char Pokemon_name[POKEMONTYPES][MAXNAME], pokemon my_Pokemons[], pokemon enemy_pokemons[], int area[COLUMNS][ROWS],int pokemon_staminas[COLUMNS][ROWS], int user_poks_on_area[COLUMNS][ROWS], int bot_poks_on_area[COLUMNS][ROWS], int range[], attack_type type[], int attack_power[])/*bot poks not needed*/
{
	int i;
	int new_column, new_row;
	int chosen_pokemon_column, chosen_pokemon_row;
	int pokemon_ruler_choice, num_of_pok_available=0;
	int max_distance = 2;
	pokemon temp_pokemon;
	int temp_stamina;
		printf("no  stop\n");
		for(i=0; i<4;i++) printf("%d\n", my_Pokemons[i]);/*DELETE*/
	while( my_Pokemons[num_of_pok_available+1] != EMPTY && num_of_pok_available!=PLAYERMAX-1) num_of_pok_available++;/*num_of_pok_available used for checking the condition statement when choosing a poke for ruling*/















	printf("NUMBER OF POKEMONS AVAILABLE: %d\n", num_of_pok_available);
		printf("no  stop\n");
	printf("Choose a pokemon to rule: \n");
	do{
		show_Pokemons(Pokemon_name, my_Pokemons, PLAYERMAX);
		scanf("%d", &pokemon_ruler_choice);
		if(pokemon_ruler_choice<0 || pokemon_ruler_choice>num_of_pok_available) printf("Invalid option. Try again.\n");/*info for user only*/
	}while(pokemon_ruler_choice<0 || pokemon_ruler_choice>num_of_pok_available);/*POK CHOOSE BUG MIGHT SOLVED BY THIS = SIGN FOR NUM_OF_POK*/
	
	find_pokemon( my_Pokemons[pokemon_ruler_choice], area, user_poks_on_area, &chosen_pokemon_column, &chosen_pokemon_row);
	printf("FOUND THE POKEMONS AT COl: %d RoW: %d\n", chosen_pokemon_column, chosen_pokemon_row);
	printf("Please enter column and row respectively: ");
	while(TRUE)/*breaks only when it goes through all conditions, doesn't make sense to put anything here because it would be too complicated*/
	{
		scanf("%d %d", &new_column, &new_row);
		if(absolute(new_column-chosen_pokemon_column)>max_distance || absolute(new_row-chosen_pokemon_row)>max_distance) printf("Your range is not bigger than 2\n");
		else if(new_column != chosen_pokemon_column && new_row != chosen_pokemon_row) printf("You can't move diagonaly!\n");
		else if( (new_column >= COLUMNS || new_column<0) || (new_row>=ROWS || new_row<0) ) printf("Out of bounds!\n");
		else if( check_if_area_space_full(area, new_column, new_row)==TRUE && (new_row != chosen_pokemon_row || new_column != chosen_pokemon_column) ) printf("There is already a pokemon there!\n");/*second condition if player chooses to not move*/
		else
		{
			/*temp_pokemon = my_Pokemons[pokemon_ruler_choice];*/
			temp_stamina = pokemon_staminas[chosen_pokemon_column][chosen_pokemon_row];

			area[chosen_pokemon_column][chosen_pokemon_row] = EMPTY;
			area[new_column][new_row] =  my_Pokemons[pokemon_ruler_choice];/*moving pokemon on the table*/
			pokemon_staminas[chosen_pokemon_column][chosen_pokemon_row] = EMPTY;
			pokemon_staminas[new_column][new_row] = temp_stamina;
			user_poks_on_area[chosen_pokemon_column][chosen_pokemon_row] = FALSE;
			user_poks_on_area[new_column][new_row] = TRUE;
			break;
		}
	}

	attack(my_Pokemons[pokemon_ruler_choice], new_column, new_row, range, type, attack_power, area, pokemon_staminas, bot_poks_on_area, enemy_pokemons);
	return;
}
void bot_move(pokemon bot_Pokemons[], pokemon user_Pokemons[], int area[COLUMNS][ROWS], int pokemon_staminas[COLUMNS][ROWS], int bot_poks_on_area[COLUMNS][ROWS], int user_poks_on_area[COLUMNS][ROWS], int range[],attack_type type[],int attack_power[])
{
	int i;
	int new_column, new_row;/*literally spent 5 hours on debugging this because I didn't initialize these two assholes*/
	int chosen_pokemon_column, chosen_pokemon_row;
	int pokemon_ruler_choice, num_of_pok_available=0;
	int max_distance = 2;
	int temp_stamina;
	printf("entered\n");
	while( bot_Pokemons[num_of_pok_available] != EMPTY && num_of_pok_available!=PLAYERMAX) num_of_pok_available++;/*num_of_pok_available used for generating a new random pokemon*/
	printf("Modulus taken from NUM OF BOT POKS %d\n", num_of_pok_available);
	printf("num of pokemons is %d\n", num_of_pok_available);
	pokemon_ruler_choice = rand()%(num_of_pok_available);/*choosing from 0 to 3 by finding modulus of number of them available but excluding 0 here*/
	printf("pokemin chosen by bot %d\n", bot_Pokemons[pokemon_ruler_choice]);
	printf("entered\n");
	find_pokemon( bot_Pokemons[pokemon_ruler_choice], area, bot_poks_on_area, &chosen_pokemon_column, &chosen_pokemon_row);
	printf("Pokemon's coordinates are %d and %d\n", chosen_pokemon_column,chosen_pokemon_row);

		do{
			printf("entered random num making again\n");
			new_column=chosen_pokemon_column;
			new_row=chosen_pokemon_row;
			if(rand()%2==0)
			{
				if(rand()%2==0) new_column = chosen_pokemon_column + rand()%3;
				else new_column = chosen_pokemon_column - rand()%3;
			}
			else
			{
				if(rand()%2==0) new_row = chosen_pokemon_row + rand()%3;
				else new_row = chosen_pokemon_row - rand()%3;
			} 
			printf("new row:%d new column:%d\n", new_row, new_column);
			if(new_row>10 || new_column >10)
			{
				getchar();
				getchar();
				getchar();
				getchar();
				printf("ERROR BIGGER THAN 10, AGAIN MEMORY PROBLEM\n");
			}
		}while( ((new_column >= COLUMNS || new_column<0) || (new_row>=ROWS || new_row<0)) || check_if_area_space_full(area, new_column, new_row ) );
	/*	I DIDN'T PUT THIS CONDITION BECAUSE POKEMON STAYS AT THE SAME PLACE MOST OF THE TIME, AND I WANT IT TO BE DYNAMIC
		}while( ((new_column >= COLUMNS || new_column<0) || (new_row>=ROWS || new_row<0)) || (check_if_area_space_full(area, new_column, new_row) && (new_column != chosen_pokemon_column && new_row !=chosen_pokemon_row)) );
*/
		/*one of maximum number of possible moves is this*/
		if(rand()%9==0)
		{
			new_row = chosen_pokemon_row;
			new_column = chosen_pokemon_column;
			printf("modulus 9 was 0 so IT DIDN't CHANGE coln: %d row: %d\n",new_column, new_row);
		}
	/*return the old condition*/
	printf("out of random num making\n");
	temp_stamina = pokemon_staminas[chosen_pokemon_column][chosen_pokemon_row];
	area[chosen_pokemon_column][chosen_pokemon_row] = EMPTY;
	area[new_column][new_row] =  bot_Pokemons[pokemon_ruler_choice];/*moving pokemon on the table*/
	pokemon_staminas[chosen_pokemon_column][chosen_pokemon_row] = EMPTY;
	pokemon_staminas[new_column][new_row] = temp_stamina;
	bot_poks_on_area[chosen_pokemon_column][chosen_pokemon_row] = FALSE;
	bot_poks_on_area[new_column][new_row] = TRUE;
	printf("ATTACK FUNC CALL AFTER THIS, it should print sth \n");
	attack(bot_Pokemons[pokemon_ruler_choice], new_column, new_row, range, type, attack_power, area, pokemon_staminas, user_poks_on_area, user_Pokemons);
	return;
}
void attack(pokemon pokemon_ruler, int pokemon_ruler_column, int pokemon_ruler_row, int range[], attack_type type[], int attack_power[], int area[COLUMNS][ROWS],int pokemon_staminas[COLUMNS][ROWS], int enemy_poks_on_area[COLUMNS][ROWS], pokemon enemy_pokemons[])
{
	int i,j,k;
	int flag = FALSE;
	int free_index, length;/*rearranging because that is the way this works when program asks to choose a pokemon, in show function it is necessery*/
	if(type[pokemon_ruler] == linear) printf("LINEAR\n");
	else if(type[pokemon_ruler] == quadratic) printf(" QUADRATIC\n");

	printf("Pok's RANGE is %d\n",range[pokemon_ruler]);
	if(type[pokemon_ruler] == linear)
	{
		printf("in linear\n");
		for(i=1; i<=range[pokemon_ruler] && flag == FALSE; i++)/*attacking in linear range from closest going further away, breaking if at least once attack is executed so that the ruler pokemon only attacks the closest pokemon*/
		{
		printf("in linear range Col%d\n",i);
			for(j = pokemon_ruler_column-i; j<=pokemon_ruler_column+i; j++)
			{
		printf("in linear ROW\n");
				if(j>=0 && j<COLUMNS)
				{
					if(enemy_poks_on_area[j][pokemon_ruler_row] == TRUE)
					{                                                                                                                                 
			printf("in linear column checks\n");
						flag=TRUE;
						pokemon_staminas[j][pokemon_ruler_row] = pokemon_staminas[j][pokemon_ruler_row] - attack_power[pokemon_ruler];
						if(pokemon_staminas[j][pokemon_ruler_row] <= 0)
						{
		/*
							/*area[j][pokemon_ruler_row] = EMPTY;
							pokemon_staminas[j][pokemon_ruler_row] = EMPTY;
							enemy_poks_on_area[j][pokemon_ruler_row] = FALSE;
							for(length=0; enemy_pokemons[length] != EMPTY && length != PLAYERMAX-1; length++);/*getting the length to be able to rearrange it with respect to pokemon with 0 or less stamina and the last one contained within the array the one with less than 0 is also part of*
							for(free_index=0; enemy_pokemons[free_index] != area[j][pokemon_ruler_row]; free_index++);
							enemy_pokemons[free_index] = EMPTY;
							area[j][pokemon_ruler_row] = EMPTY;
							for(free_index; free_index<=length; free_index++)/*got free_index in the previous loop*
							{
								enemy_pokemons[free_index] = enemy_pokemons[free_index+1];
								/*this is setting new arangement of the pokemons beacuse it changes when user removes one of them, so it is just iterating until the last free space 			if(free_index == length) enemy_pokemon[free_index] = EMPTY;
							}*/
							remove_pokemon(j, pokemon_ruler_row, area, enemy_poks_on_area, pokemon_staminas, enemy_pokemons);
						}
					}
				}
			}
			for(j = pokemon_ruler_row-i; j<=pokemon_ruler_row+i; j++)
			{
		printf("in linear row\n");
				if(j>=0 && j<ROWS)
				{

					if(enemy_poks_on_area[pokemon_ruler_column][j] == TRUE)
					{
printf("in linear row checks\n");
						flag=TRUE;
						pokemon_staminas[pokemon_ruler_column][j] = pokemon_staminas[pokemon_ruler_column][j] - attack_power[pokemon_ruler];
						if(pokemon_staminas[pokemon_ruler_column][j] <= 0)
						{
					/*
							pokemon_staminas[pokemon_ruler_column][j] = EMPTY;
							enemy_poks_on_area[pokemon_ruler_column][j] = FALSE;
							for(length=0; enemy_pokemons[length] != EMPTY && length != PLAYERMAX; length++);
							for(free_index=0; enemy_pokemons[free_index] != area[pokemon_ruler_column][j]; free_index++);
							enemy_pokemons[free_index] = EMPTY;
							area[pokemon_ruler_column][j] = EMPTY;
							for(free_index; free_index<length; free_index++)/*got free_index in the previous loop
							{
								enemy_pokemons[free_index] = enemy_pokemons[free_index+1];
								/*this is setting new arangement of the pokemons beacuse it changes when user removes one of them, so it is just iterating until the last free space 
							}
		*/
							remove_pokemon(pokemon_ruler_column, j, area, enemy_poks_on_area, pokemon_staminas, enemy_pokemons);
						}
					}
				}
			}
		/*range loop area*/
		}
	/*if statement*/
	}
	else if(type[pokemon_ruler] == quadratic) /*NO NEED FOR A FLAG HERE*/
	{
	printf("in quadratic\n");
		for(i = pokemon_ruler_column-range[pokemon_ruler]; (i <= pokemon_ruler_column + range[pokemon_ruler]) ; i++)
		{
			if(i>=0 && i< COLUMNS)
			{
				for(j=pokemon_ruler_row-range[pokemon_ruler]; j<= pokemon_ruler_row + range[pokemon_ruler]; j++)
				{
					if(j>=0 && j< ROWS && enemy_poks_on_area[i][j])/*put true also maybe but not needed*/
					{
						pokemon_staminas[i][j] = pokemon_staminas[i][j] - attack_power[pokemon_ruler];
						printf("POKEMON ATTACKED!\n");
						if(pokemon_staminas[i][j] <= 0) remove_pokemon(i, j, area, enemy_poks_on_area, pokemon_staminas, enemy_pokemons);
					}
				}
			}
		}
		/*
		for(i=1; i<=range[pokemon_ruler] && flag == FALSE; i++)
		{printf("in quadratic range of range %d and %d is the increment for range\n", range[pokemon_ruler], i);
		
		
			for(j=pokemon_ruler_column-i; j<=pokemon_ruler_column+i;j++)
			{
printf("in quadratic before column\n");
		
				if(j>=0 && j<COLUMNS)
				{
printf("in quadratic after column\n");
		
					for(k=pokemon_ruler_row-i; k<=pokemon_ruler_column+i; k++)
					{
printf("in quadratic before row\n");
		
						if(k>=0 && k<ROWS)
						{

							if(enemy_poks_on_area[j][k] == TRUE)
							{
printf("in quadratic after row\n");
								flag=TRUE;
								pokemon_staminas[j][k] = pokemon_staminas[j][k] - attack_power[pokemon_ruler];
								if(pokemon_staminas[j][k] <= 0)
								{
									/*
									area[j][k] = EMPTY;
									pokemon_staminas[j][k] = EMPTY;
									enemy_poks_on_area[j][k] = FALSE;
									for(length=0; enemy_pokemons[length] != EMPTY && length != PLAYERMAX; length++);
									for(free_index=0; enemy_pokemons[free_index] != area[j][k]; free_index++);
									enemy_pokemons[free_index] = EMPTY;
									area[j][k] = EMPTY;
									for(free_index; free_index<length; free_index++)/*got free_index in the previous loop
									{
										enemy_pokemons[free_index] = enemy_pokemons[free_index+1];
										/*this is setting new arangement of the pokemons beacuse it changes when user removes one of them, so it is just iterating until the last free space 
									}
									*
									remove_pokemon(j, k, area, enemy_poks_on_area, pokemon_staminas, enemy_pokemons);
								}
							}
						}
					}
				}/*j column area here*
			}
		}
		*/
	}
	return;
}
void remove_pokemon(int pok_column, int pok_row, int area[COLUMNS][ROWS], int enemy_poks_on_area[COLUMNS][ROWS], int pokemon_staminas[COLUMNS][ROWS], pokemon enemy_pokemons[])
{
	int i;
	enemy_poks_on_area[pok_column][pok_row] = FALSE;
	pokemon_staminas[pok_column][pok_row] = EMPTY;
	for(i=0; enemy_pokemons[i] != area[pok_column][pok_row]; i++);
	area[pok_column][pok_row] = EMPTY;
	for(i; i!=PLAYERMAX-1 && enemy_pokemons[i+1] != EMPTY; i++) enemy_pokemons[i] = enemy_pokemons[i+1];	
	enemy_pokemons[i] = EMPTY;/*updating the last element*/
	for(i=0; enemy_pokemons[i]!=EMPTY; i++) 
		{

			printf("NEW ORDER POKS: ------%d ", enemy_pokemons[i]);
		
			if(i>5) printf("\nERROR");
		}
	printf("\n");
}
void find_pokemon(pokemon pok_chosen, int area[COLUMNS][ROWS], int user_poks_on_area[COLUMNS][ROWS], int* chosen_pokemon_column, int* chosen_pokemon_row)
{
	int i,j;
	for(i=0; i<COLUMNS; i++)
	{
		for(j=0; j<ROWS; j++)
		{
			if(area[i][j] == pok_chosen && user_poks_on_area[i][j] == TRUE) /*make them separate to make program faster maybe, but i am not sure*/
			{
				*chosen_pokemon_column = i;
				*chosen_pokemon_row = j;
				return;/*see to maybe remove this from here because hoca says this is not a good practice, but
						if i don't do this i would have to put another condition making this heavier process
						*/
			}
		}
	}
}
void show_area (char Pokemon_name[POKEMONTYPES][MAXNAME],int area[COLUMNS][ROWS],int
pokemon_staminas[COLUMNS][ROWS])
{
	int i,j;
	for(i=0; i<COLUMNS*2; i++)
	{
		if(i%2==0) 
			{
				for(j=0; j<=9*ROWS;j++) printf("%c",'-');/*- there are 9 caracters per row entry and the last | in the next so = included*/ 
				printf("\n");
			}
		for(j=0; j<ROWS; j++)
		{
			if(area[i/2][j] == EMPTY)
			{	
				if(j!=ROWS-1) printf("|%*s",8,"");/*why do I need space here what is happening why is tab not 8 characters*/
				else printf("|%*s|",8,"");
				
			}
			else
			{
				if(j!=ROWS-1)
				{
					if(i%2==0) printf("|%*.2s%*s", (8/2+2/2), Pokemon_name[area[i/2][j]], (8/2-2/2), "" );
					else printf("|%*d%*s", 8/2+4/2, pokemon_staminas[i/2][j], 8/2 - 4/2, "");
				}
				else
				{
					if(i%2==0) printf("|%*.2s%*s|", (8/2+2/2), Pokemon_name[area[i/2][j]], (8/2-2/2), "" );
					else printf("|%*d%*s|", 8/2+4/2, pokemon_staminas[i/2][j], 8/2 - 4/2, "");
				}
			}
		}
		printf("\n");
		if(i==COLUMNS*2 - 1) 
		{
			for(j=0; j<=9*ROWS;j++) printf("%c",'-');
			printf("\n");
		}
	}
}     
int absolute(int number)
{
	int res=number;
	if(number<0) res *= -1;
	return res;
}

void menu(pokemon Pokemons[POKEMONTYPES], pokemon my_Pokemons[PLAYERMAX], char Pokemon_name[POKEMONTYPES][MAXNAME], int range[POKEMONTYPES], int attack_power[POKEMONTYPES], int stamina[POKEMONTYPES], attack_type type[POKEMONTYPES])
{
	int choice;
	int i;
	do{
		printf("\nSelect an option to continue:\n\n");
		printf("1) Open Pokedex\n2) Go to Oak's Labarotary\n3) Enter the tournament\n0) Exit\n");
		scanf("%d", &choice);
		switch(choice)
		{
			case 1:
				pokedex( Pokemon_name, range, type, attack_power, stamina);
				break;
			case 2:
				oaks_laboratory(Pokemon_name, Pokemons, my_Pokemons);
				break;
			case 3:
				for(i=0;my_Pokemons[i+1]!=EMPTY && i!=PLAYERMAX-1;i++);/*just finding maximum index*/
				if(i!=PLAYERMAX-1) printf("You need to catch more pokemons!\n");
				else battle(Pokemon_name, range, type, attack_power, stamina, my_Pokemons);
				break;
			case 0:
				printf("Exiting ....\n");
				break;
			default:
				printf("Invalid option. Please try again\n");
		}
	}while(choice!=0);
}




