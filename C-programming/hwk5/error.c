#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define DICT_SIZE 15
#define WORD_LEN 10
#define LINE_LEN 18
#define ROWS 15
#define COLUMNS 15
#define TRUE 1
#define FALSE 0
#define start_column 0
#define start_row 1
#define end_column 2
#define end_row 3
int get_line_size(char *line) {
	char *ch_iter = line; // so as not to lose beginning of line
	int counter = 0;
	// go until you see new line or null char
	while(*ch_iter != '\n' && *ch_iter != '\0') {
		ch_iter++; // next char
		counter++; // increment counter
	}

	return counter;
}

void copy_string(char *source, char *destination) {
	// get iterators over original pointers
	char *src_iter = source;
	char *dst_iter = destination;
	// until null char
	while (*src_iter != '\0') {
		// copy pointers
		*dst_iter = *src_iter;
		// advance to next char
		src_iter++;
		dst_iter++;
   }
   // terminate string
   *dst_iter = '\0';
}

void remove_newline(char *line) {
	char *ch_iter = line;
	// go until you see new line
	while(*ch_iter != '\n') {
		ch_iter++; // next char
	}
	*ch_iter = '\0'; // overwrite new line
}

void print_dictionary(char *dict[]) {
	int i;
	for(i = 0 ; i < DICT_SIZE ; i++) {
		printf("%s\n", dict[i]);
	}
}

void print_coord(int coord[DICT_SIZE][4]) {
	int i, j;
	for(i = 0 ; i < DICT_SIZE ; i++) {
		for(j = 0 ; j < 4 ; j++) {
			printf("%d ", coord[i][j]);
		}
		printf("\n");
	}
}


/*AFTER HOCAS FUNCTIONS*/
char get_letter();
void fill_puzzle(char puzzle[COLUMNS][ROWS]);
void print_puzzle(char puzzle[COLUMNS][ROWS]);
void setup(char puzzle[COLUMNS][ROWS], char *dict[DICT_SIZE], int coord[DICT_SIZE][4]);
int string_compare(char* string1, char* string2);
int string_length(char* string);
int guess_check(char* guess, int column_u, int row_u, char puzzle[COLUMNS][ROWS]);
void make_uppercase(char* guess, char *dict[DICT_SIZE], int coord[DICT_SIZE][4], int num_of_guessed_ones);/*this function also ensures user from guessing the same word twice*/
void reverse_string(char* string);
void swap_for_upper_case( char *dict[DICT_SIZE], int coord[DICT_SIZE][4] , int index, int num_of_guessed_ones);
int already_guessed( char arr_guessed[DICT_SIZE][10], char* word);
int is_lowercase_word(char* word);
/*MAIN---MAIN---MAIN---MAIN---MAIN---MAIN---MAIN---MAIN---MAIN---MAIN---MAIN---MAIN---MAIN*/
int main(){

	char guess[COLUMNS];
	char arr_guessed[DICT_SIZE][10];
	int column_u, row_u;
	char game[15];
	char check=0;/*checking a win condition*/
	int counter=0;
	srand(time(NULL));
	int i;
	char puzzle[COLUMNS][ROWS];
	char *dict[DICT_SIZE];
	int coord[DICT_SIZE][4];
	char line[LINE_LEN];
	FILE *fp = fopen("word_hunter.dat", "r");
	int line_counter = 0;
	int dict_counter = 0;
	for(i=0; i<DICT_SIZE; i++) arr_guessed[i][0]=0;
	while(fgets(line, LINE_LEN, fp) != NULL) {
		if(line_counter%5 == 0) {
			dict[dict_counter] = (char*) malloc(sizeof(char) * get_line_size(line));
			remove_newline(line);

			copy_string(line, dict[dict_counter]);
		} else if (line_counter%5 == 1){
			coord[dict_counter][0] = atoi(line);
		} else if (line_counter%5 == 2){
			coord[dict_counter][1] = atoi(line);
		} else if (line_counter%5 == 3){
			coord[dict_counter][2] = atoi(line);
		} else if (line_counter%5 == 4){
			coord[dict_counter][3] = atoi(line);
			dict_counter++;
		}
		line_counter++;
	}

	fclose(fp);
	/*print_dictionary(dict);
	print_coord(coord);*/
	fill_puzzle(puzzle);/*filling it in with random values*/
	setup(puzzle,dict,coord);
	do{
		print_puzzle(puzzle);
		printf("Enter your guess (exit game to exit): ");
		scanf("%s%c",guess,&check);/*putting space here makes problems*/
		if( check != '\n')
		{
			for(i=0; check!='\n';i++)
			{
			game[i]=check;
			check = getchar();
			}
			if(string_compare(guess,"exit") && string_compare(game," game")) break;
			else 	printf("Invalid input. Please enter only 1 word with no spaces at a time\n");
			for(i=0; game[i]!=0; i++) game[i]=0;/*if user inputs word longer than game accidentaly last letter stays there for comparison so just removing to be able to do correct comparison*/
		}
		else
		{
			if(!already_guessed(arr_guessed, guess) && is_lowercase_word(guess))
			{
					do{
					printf("\nEnter coordinates\n");
					printf("Column and row respectively:");
					scanf("%d %d", &column_u, &row_u);
					if( (column_u<0 && column_u>=COLUMNS) || (row_u<0 && row_u>=ROWS) ) printf("Out of bounds! Enter a valid choice.\n");
				}while( (column_u<0 && column_u>=COLUMNS) || (row_u<0 && row_u>=ROWS) );
			}
			if( guess_check(guess, column_u, row_u, puzzle) && !already_guessed(arr_guessed, guess) && is_lowercase_word(guess))
			{
				printf("Your guess is right!\n");
				copy_string(guess, arr_guessed[counter]);
				make_uppercase(guess,dict, coord,counter);
				setup(puzzle, dict, coord);
				counter++;
				printf("Correctly guessed words %d/%d \n", counter, DICT_SIZE);
			}
			else
			{
				if(!is_lowercase_word(guess)) printf("Please enter a word whose all letters are lowercase\n");
				else if(already_guessed(arr_guessed, guess)) printf("You alraedy guessed this word. Try again\n");
				else printf("Incorrect guess. Try again\n");
			}
			printf("\n");
		}
	}while(counter<DICT_SIZE);
	if( string_compare(guess,"exit") && string_compare(game," game") ) printf("Exiting....\n");
	else printf("Congratulations! You have finished the WordHunter game!\n");
	return 0;
}
char get_letter()
{
	int range = ('z' - 'a') +1;/*+1 for modulus*/
	char new_rand_char = 'a' + random()%range;
	return new_rand_char;
}
void fill_puzzle(char puzzle[COLUMNS][ROWS])/*this function is just filling in the puzzle with random letters*/
{
	int i,j;
	for(i=0;i<COLUMNS;i++)
	{
		for(j=0;j<ROWS;j++)
		{
			puzzle[i][j] = get_letter();
		}
	}
}
void print_puzzle(char puzzle[COLUMNS][ROWS])
{
	int i,j;
	for(i=0;i<COLUMNS;i++)
	{
		for(j=0;j<ROWS;j++)
		{
			printf("%c ", puzzle[i][j]);
		}
	printf("\n");
	}
}
void setup(char puzzle[COLUMNS][ROWS], char *dict[DICT_SIZE], int coord[DICT_SIZE][4])
{
	int i,j,k,word_index = 0;
	for(i=0;i<DICT_SIZE;i++)
	{
		if( coord[i][start_column] == coord[i][end_column] )/*horizontal*/
		{
			if(coord[i][start_row] < coord[i][end_row])/*left_to_right*/
			{
				for(j=coord[i][start_row]; j<=coord[i][end_row]; j++)
				{
					puzzle[coord[i][start_column]][j] = dict[i][word_index];
					word_index++;
				}
			}
			else/*right_to_left*/
			{
				/*when start row is smaller than end row*/
				for(j=coord[i][start_row]; j>=coord[i][end_row]; j--)
				{
					puzzle[coord[i][start_column]][j] = dict[i][word_index];
					word_index++;
				}
			}
			word_index=0;
		}
		else if(coord[i][start_row] == coord[i][end_row])/*vertical*/
		{
			if(coord[i][start_column] < coord[i][end_column])/*up-down*/
			{
				for(j=coord[i][start_column]; j<=coord[i][end_column]; j++)
				{
					puzzle[j][coord[i][start_row]] = dict[i][word_index];
					word_index++;
				}
			}
			else/*down-up*/
			{
				for(j=coord[i][start_column]; j>=coord[i][end_column]; j--)
				{
					puzzle[j][coord[i][start_row]] = dict[i][word_index];
					word_index++;
				}
			}
			word_index=0;
		}
		else/*diagonal ones*/
		{
			if(coord[i][start_column]<coord[i][end_column])/*up-down*/
			{
				if(coord[i][start_row]<coord[i][end_row])/*left_to_right*/
				{
					k=coord[i][start_row];
					for(j=coord[i][start_column];j<=coord[i][end_column];j++)
					{
						puzzle[j][k]=dict[i][word_index];
						word_index++;
						k++;
					}
				}
				else/*right_to_left*/
				{
					k=coord[i][start_row];
					for(j=coord[i][start_column];j<=coord[i][end_column];j++)
					{
						puzzle[j][k]=dict[i][word_index];
						word_index++;
						k--;
					}
				}
				word_index=0;
			}
			else/*down-up*/
			{
				if(coord[i][start_row]<coord[i][end_row])/*left_to_right*/
				{
					k=coord[i][start_row];
					for(j=coord[i][start_column];j>=coord[i][end_column];j--)
					{
						puzzle[j][k]=dict[i][word_index];
						word_index++;
						k++;
					}
				}
				else/*right_to_left*/
				{
					k=coord[i][start_row];
					for(j=coord[i][start_column];j>=coord[i][end_column];j--)
					{
						puzzle[j][k]=dict[i][word_index];
						word_index++;
						k--;
					}
				}
				word_index=0;
			}
		}
	}
}

int string_compare(char* string1, char* string2)
{
	int res=FALSE;
	int i;
	int string1_length = string_length(string1);
	int string2_length = string_length(string2);

	if(string1_length == string2_length)
	{
		res = TRUE;
		for(i=0; i<string1_length; i++)
		{
			if(string1[i]!=string2[i])/*when at least one char of strings with the same length is different make result FALSE*/
			{
				res = FALSE;
				break;
			}
		}
	}
	return res;
}
int string_length(char* string)
{
	int i;
	for(i = 0; string[i]!= 0 && string[i]!='\r' ; i++);/*second condition used because carriage return left in every word of the dictionary*/
	return i;
}
int guess_check(char* guess, int column_u, int row_u, char puzzle[COLUMNS][ROWS])
{
	int i,j;
	int word_index=0;
	int res = FALSE;
	int flag=0, reversed_flag=0;
	int guess_length = string_length(guess);
	if( (guess[0]==puzzle[column_u][row_u] || guess[guess_length-1]==puzzle[column_u][row_u]) || (guess[0]-32==puzzle[column_u][row_u] || guess[guess_length-1]-32==puzzle[column_u][row_u]) )/*in regular order*/
	{
		if( (guess[guess_length-1]==puzzle[column_u][row_u]) || (guess[guess_length-1]-32==puzzle[column_u][row_u]) )
		{
			reverse_string(guess);/*setting it up for checking*/
			reversed_flag=1;
		}
		if ( (row_u+guess_length-1 < ROWS) && res==FALSE)/*east*/
		{
			for(i=row_u, word_index=0; i<row_u+guess_length;i++, word_index++)
			{
				if( (puzzle[column_u][i] != guess[word_index]) && (puzzle[column_u][i] != guess[word_index]-32) )
				{
				flag = 1;
				break;
				}
			}
			if(flag==0) res = TRUE;
			flag = 0;/*updating the flag if it changed*/
		}
		if( (row_u-guess_length+1 >=  0) && res==FALSE )/*west*/
		{
			for(i=row_u, word_index=0; i>=row_u-guess_length+1; i--, word_index++)
			{
				if( (puzzle[column_u][i] != guess[word_index]) && (puzzle[column_u][i] != guess[word_index]-32) )
				{
					flag = 1;
					break;
				}
			}
			if(flag==0)res = TRUE;
			flag = 0;
		}
		if( (column_u+guess_length-1 <COLUMNS) && res==FALSE )/*south*/
		{
			for(i=column_u, word_index=0; i<column_u+guess_length; i++, word_index++)
			{
				if( (puzzle[i][row_u]!=guess[word_index]) && (puzzle[i][row_u]!=guess[word_index]-32) )
				{
					flag = 1;
					break;
				}
			}
			if(flag==0) res = TRUE;
			flag = 0;
		}
		if( (column_u-guess_length+1>=0) && res==FALSE )/*north*/
		{
			for(i=column_u, word_index=0; i>=column_u-guess_length+1;i--, word_index++)
			{
				if( (puzzle[i][row_u] != guess[word_index]) && (puzzle[i][row_u] != guess[word_index]-32) )
				{
					flag = 1;
					break;
				}
			}
			if(flag==0) res=TRUE;
			flag = 0;
		}
		/*diagonal ones left*/
		if(row_u+guess_length-1<ROWS && column_u+guess_length-1<COLUMNS && res==FALSE)/*south_east*/
		{
			for(i=column_u,j=row_u,word_index=0;i<column_u+guess_length;i++,j++,word_index++)
			{
				if( (puzzle[i][j]!=guess[word_index]) && (puzzle[i][j]!=guess[word_index]-32) )
				{
					flag = 1;
					break;
				}
			}
			if(flag==0) res = TRUE;
			flag = 0;
		}
		if(row_u-guess_length+1>=0 && column_u+guess_length-1<COLUMNS && res==FALSE)/*south_west*/
		{
			for(i=column_u,j=row_u,word_index=0;i<column_u+guess_length;i++,j--,word_index++)
			{
				if( (puzzle[i][j]!=guess[word_index]) && (puzzle[i][j]!=guess[word_index]-32))
				{
					flag = 1;
					break;
				}
			}
			if(flag==0) res = TRUE;
			flag = 0;
		}
		if(row_u-guess_length+1>=0 && column_u-guess_length+1>=0 && res == FALSE)/*north_west*/
		{
			for( i=column_u, j=row_u, word_index=0;i>=column_u-guess_length+1;i--,j--,word_index++)
			{
				if( (puzzle[i][j]!=guess[word_index]) && (puzzle[i][j]!=guess[word_index]-32) )
				{
					flag = 1;
					break;
				}
			}
			if(flag==0) res = TRUE;
			flag = 0;
		}
		if(row_u+guess_length-1 < ROWS && column_u-guess_length+1>=0 && res==FALSE)/*north_east*/
		{
			for( i=column_u, j=row_u, word_index=0; i>=column_u-guess_length+1; i--,j++,word_index++)
			{
				if( (puzzle[i][j]!=guess[word_index]) && (puzzle[i][j]!=guess[word_index]-32) )
				{
					flag = 1;
					break;
				}
			}
			if(flag==0) res = TRUE;
			flag = 0;
		}

		if(guess[0]==guess[guess_length-1] && res == FALSE)
		{
			res = guess_check( guess, column_u, row_u, puzzle );/*if first and last letter the same guess already reversed so now just checking for the other way around because I can't know if user meant from the starting or ending corrdinate so I will have to check for both of them, first time it checks as if it was the last coordinate and the next time as if it was the first*/
			/*this function will again reverse this string to original state and than because of the reverse_string in the 439th line reverse it again in the state it was when it was sent to this function meaning that after this function is executed guess will still be reversed because it was sent reversed*/
		}
	}
	if(reversed_flag==1) reverse_string(guess);/*putting it back in its original state*/
	/*putting it back in its original state is important since the puzzle needs to be updated according to it*/
	return res;
}
void make_uppercase(char* guess, char *dict[DICT_SIZE], int coord[DICT_SIZE][4], int num_of_guessed_ones)/*this function ensures puzzle to be printed correctly*/
{
	int index,i;
	for(index=0;index<DICT_SIZE;index++)
	{
		if( string_compare(guess, dict[index])==TRUE )
		{
			for(i=0; dict[index][i]!=0 && dict[index][i]!='\r'; i++) dict[index][i] -= 32;/*from lowercase to uppercase*/
			swap_for_upper_case( dict,  coord, index, num_of_guessed_ones);
		}
	}
}
void reverse_string(char* string)
{
	int i;
	int length = string_length(string);
	char temp;
	for(i=0; i<length/2; i++)
	{
		temp = string[i];
		string[i]=string[length-i-1];
		string[length-i-1]=temp;
	}
	return;
}
void swap_for_upper_case( char *dict[DICT_SIZE], int coord[DICT_SIZE][4] , int index, int num_of_guessed_ones)/*this function is used because when a word is guessed correctly its letters become big after this and sometimes overlap if it is not the last one */
{/*need counter or num_of_guessed_ones to update the letters in the dictionary correctly*/
	int i;
	int temp_arr[4];/*storing two points*/
	char temp[10];/*biggesst word from the dictionary*/
	copy_string(dict[DICT_SIZE-1-num_of_guessed_ones], temp);
	copy_string(dict[index],dict[DICT_SIZE-1-num_of_guessed_ones]);
	copy_string(temp, dict[index]);/*now big letters are at the end*/
	for(i=0; i<4; i++) temp_arr[i] = coord[index][i];/*updating its coordinates also so that the words are printted correctly*/
	for(i=0; i<4; i++) coord[index][i] = coord[DICT_SIZE-1-num_of_guessed_ones][i];
	for(i=0; i<4; i++) coord[DICT_SIZE-1-num_of_guessed_ones][i] = temp_arr[i];
}
int already_guessed( char arr_guessed[DICT_SIZE][10], char* word)/*keeping track of guessed words and than updating them in guessed_words array*/
{
	int res=FALSE;
	int i;
	if(arr_guessed[0][0]!=0)/*initialized them with 0s at the beginning*/
	{
		for(i=0; arr_guessed[i][0]!=0; i++)
		{
			if(string_compare(word, arr_guessed[i])==TRUE)
			{
				res = TRUE;
				break;
			}
		}
	}
	return res;
}
int is_lowercase_word(char* word)
{
	int i;
	int res = TRUE;
	for (i=0; word[i]!= 0; i++)
	{
		if(word[i]<'a' || word[i]>'z')
		{
			res = FALSE;
			break;
		}
	}
	return res;
}
