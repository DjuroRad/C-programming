#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define BOARD_SIZE 20
#define MAX_OWNED 11
#define EMPTY -1
#define HOUSE_LIMIT 3
typedef enum{noone, cap, car}player_type;
typedef enum{start, property, tax, punish}block_type;
typedef enum{FALSE, TRUE}bool;
typedef struct{
  player_type type;/* Holds type of the player. The player_type must be defined as
  enumerated type which consist of noone, cap and car values.*/
  int current_block_id;/* Holds player location as block id.*/
  int owned_block_ids[MAX_OWNED];/*Holds property block ids that are owned by the user.*/
  int account;/* Holds amount of the current money of the player.*/
  int turn_to_wait;/* Holds number of turns to wait if the player got a punishment.*/
  char * name;/*Holds the name of the player.*/
}player;

typedef struct{
  int block_id;/* Holds id of the block.*/
  char * name;/* Holds text of the block that is shown on the top of the block.*/
  int price;/*If the block is property, holds the price of property. If the block is a tax
block than holds the tax fee and if the block is punishment block, then holds the number of
turns to wait.*/
  int rent;/* Holds the default rent of a property (no house)*/
  int rent_1;/*Holds the rent of a property with one house.*/
  int rent_2;/*Holds the rent of a property with two houses.*/
  int rent_3;/*Holds the rent of a property with three houses.*/
  int house_price;/*Holds price of building a house on the block.*/
  int house_count;/*Holds the number of the houses on the block that are already built.*/
  player owner;/*Holds the owner of the block.*/
  block_type type;/*Holds type of the block. The block_type must be defined as
enumerated type which consist of start, property, tax, punish values.*/
}block;
void init_the_board(block board[BOARD_SIZE]);
int find_longest(block board[BOARD_SIZE]);
int find_num_of_digits(int num);
void show_board( block board[BOARD_SIZE], player player_one, player player_two);
void show_properties( block board[20]);
bool is_not_choice(int possible_choices[BOARD_SIZE], int n, int user_choice);
void show_chosen_property(block chosen_block, int longest_word, int user_choice);
void buy_property(block* current_block, player* current_player);
void buy_house(block* current_block,player* current_player);
void sell_property(block board[20], player* current_player);
void remove_property(int* arr, int n, int user_choice);
void init_players(player* player_one, player* player_two);
void gameplay (block board[20], player player_one, player player_two);
void roll_the_dice(player* current_player);
void play_menu();
void pay_rent(player* current_player,player* next_player, block current_block);
void show_my_properties(int owned_blocks[MAX_OWNED], block board[BOARD_SIZE]);
int check_account(player current_player, block board[BOARD_SIZE]);

int main()
{
  srand(time(NULL));
  block board[BOARD_SIZE];
  init_the_board(board);

  player  player_one;
  player  player_two;
  init_players(&player_one, &player_two);
  gameplay(board, player_one, player_two);
  return 0;
}
void gameplay (block board[20], player player_one, player player_two)
{
  int choice, buy_choice;
  int rent_to_pay;
  player* current_player = &player_one;
  player* next_player = &player_two;
  player* temp;
  show_board(board, player_one, player_two);
  do {
    do{
      play_menu();
      printf("%s's turn\n", current_player->name);
      printf("Please select an option to continue:\n");
      scanf("%d",&choice);
      switch(choice)
      {
        case 1:
          roll_the_dice(current_player);
          show_board(board, player_one, player_two);
            switch(board[current_player->current_block_id].type)
            {
              case punish:
                printf("You just got punished, you won't be able to move during the next %d turn(s)\n", board[current_player->current_block_id].rent);
                current_player->turn_to_wait = board[current_player->current_block_id].rent;
                break;
              case tax:
                printf("You have to pay the tax\n");
                if(rent_to_pay=check_account(*current_player,board))/*would assign 0 to retn to pay if user has lost the game or doesn't have enough funds to continue, otherwise any kind of a positive value which satisfies the condition of if*/
                {
                  while(current_player->account<=rent_to_pay)
                  {
                    printf("Your funds are isufficient, you need to sell some property(s)\n");
                    sell_property(board, current_player);
                  }
                  current_player->account -= board[current_player->current_block_id].rent;
                  printf("After paying you have %d$ left on your account\n", current_player->account);
                }
                else
                {
                  printf("Oh, %s just went bankrupt.\nGame over\n", current_player->name);
                  current_player->account = EMPTY;
                }
                break;
              case property:/*checking for all types of owners*/
                if(board[current_player->current_block_id].owner.type == noone)
                {
                  printf("This property is not owned by anyone. Press 1 to buy it, 0 to exit (Selling price:%d$): ", board[current_player->current_block_id].price);
                  do{
                    scanf("%d", &buy_choice);
                    if(buy_choice!=1 && buy_choice!=0) printf("Please enter a valid choice.\n");
                  }while(buy_choice!=1 && buy_choice!=0);
                  if(buy_choice==1) buy_property(&board[current_player->current_block_id], current_player);
                }
                else if(board[current_player->current_block_id].owner.type != current_player->type)
                {
                  printf("You have arrived on someone else's land and you will have to pay the rent.\n");
                  if(rent_to_pay=check_account(*current_player, board))/*would assign 0 to retn to pay if user has lost the game or doesn't have enough funds to continue, otherwise any kind of a positive value which satisfies the condition of if*/
                  {
                    while(current_player->account<=rent_to_pay)
                    {
                      printf("Your funds are isufficient, you need to sell some property(s)\n");
                      sell_property(board, current_player);
                    }
                    pay_rent(current_player, next_player, board[current_player->current_block_id]);
                    printf("Your account after paying the rent: %d$\n", current_player->account);
                  }
                  else
                  {
                    printf("Oh, %s just went bankrupt.\nGame over\n", current_player->name);
                    current_player->account = EMPTY;
                  }
                }
                else printf("You just arrived on your property.\n");
            }
            break;
        case 2:
          printf("Your account is %d\n", current_player->account);
          break;
        case 3:
          show_my_properties(current_player->owned_block_ids, board);/*player's properties*/
          break;
        case 4:
          show_properties(board);
          break;
        case 5:
          buy_property(&board[current_player->current_block_id], current_player);
          break;
        case 6:
          buy_house(&board[current_player->current_block_id], current_player);
          break;
        case 7:
          sell_property(board, current_player);
          break;
        default:
          printf("Invalid choice. Please try again.\n");
      }
    }while(choice!=1);
    /*changing player's turn*/

    if(next_player->turn_to_wait!=0 && current_player->turn_to_wait!=0)/*when both of them are punished. Here we'll just have to make it 0 for one of them so that the game continues correctly*/
    {
      if(next_player->turn_to_wait > current_player->turn_to_wait)
      {
        next_player->turn_to_wait -= current_player->turn_to_wait;
        current_player->turn_to_wait = 0;/*it will again be current player's turn*/
      }
      else
      {
        current_player->turn_to_wait -= next_player->turn_to_wait ;
        next_player->turn_to_wait  = 0;/*it will now be next player's turn*/
      }
    }
    if(next_player->turn_to_wait==0)/*now after making sure one of the player's turn_to_wait is 0 we can proceed*/
    {
      temp = current_player;
      current_player = next_player;
      next_player = temp;
    }
    else
    {
      (next_player->turn_to_wait)--;
      if(player_one.account>0 && player_two.account>0) printf("%s will have to wait %d more turn(s)\n", next_player->name, (next_player->turn_to_wait)+1);/*if it is still different than zero inform on how much it should wait*/
      /*this condition is put because i don't want it to print this if the game is already finished. Other preceding steps will be executet but it doesn't matter since it is exiting after that*/
    }
  }while(player_one.account>0 && player_two.account>0);
  if(player_one.account>0) printf("Congrats %s. You have won the game\n", player_one.name);
  else printf("Congrats %s. You have won the game\n", player_two.name);
}
int check_account(player current_player, block board[BOARD_SIZE])
{
  bool answ;
  int rent_to_pay;
  int total_money=current_player.account;
  int i;
  switch(board[current_player.current_block_id].house_count)
  {
    case 0:
      rent_to_pay=board[current_player.current_block_id].rent;
      break;
    case 1:
      rent_to_pay=board[current_player.current_block_id].rent_1;
      break;
    case 2:
      rent_to_pay=board[current_player.current_block_id].rent_2;
      break;
    case 3:
      rent_to_pay = board[current_player.current_block_id].rent_3;
      break;
  }
  for(i=0; current_player.owned_block_ids[i]!=EMPTY && i < MAX_OWNED; i++)
  {
    total_money += (board[current_player.owned_block_ids[i]].price)/2 + board[current_player.owned_block_ids[i]].house_count*(board[current_player.owned_block_ids[i]].house_price/2);/*when selling it is worth only half of the price paid originally for it*/
  }
  printf("Rent/Tax to pay: %d$\n", rent_to_pay);
  if(rent_to_pay>=total_money) answ = FALSE;
  else answ = rent_to_pay;/*so that i could control num of blocks to be sold in playgame function*/

  return answ;
}
void show_my_properties(int owned_blocks[MAX_OWNED], block board[BOARD_SIZE])
{
  int i;
  for(i=0; i<MAX_OWNED && owned_blocks[i]!=EMPTY;i++)
  {
    printf("%d - %*s%s (Selling price: %d)\n", owned_blocks[i], 5-find_num_of_digits(owned_blocks[i]), "", board[owned_blocks[i]].name, board[owned_blocks[i]].price/2 + board[owned_blocks[i]].house_count*(board[owned_blocks[i]].house_price/2));
  }
  if(owned_blocks[0]==EMPTY) printf("You don't own any properties\n");
}
void pay_rent(player* current_player,player* next_player, block current_block)
{
    switch (current_block.house_count)
    {
      case 0:
        current_player->account -= current_block.rent;
        next_player->account += current_block.rent;
        break;
      case 1:
        current_player->account -= current_block.rent_1;
        next_player->account += current_block.rent_1;
        break;
      case 2:
        current_player->account -= current_block.rent_2;
        next_player->account += current_block.rent_2;
        break;
      case 3:
        current_player->account -= current_block.rent_3;
        next_player->account += current_block.rent_3;
        break;
    }

    return;
}
void play_menu()
{
  printf("\n");
  printf("1 - Roll the dice\n");
  printf("2 - Show my account\n");
  printf("3 - Show my properties\n");
  printf("4 - Show property deeds\n");
  printf("5 - Buy Property\n");
  printf("6 - Buy house\n");
  printf("7 - Sell property\n");
}
void roll_the_dice(player* current_player)
{
  int max = 7;/*can move maximum 7 points at a time*/
  int dice=0;
  while(dice==0) dice = rand()%max;/*removing 0 from possible outcomes of a roll dice*/
  if(dice+current_player->current_block_id>=BOARD_SIZE)
  {
    current_player->account += 10000;/*adding 100000 to the account after going over start*/
    printf("You have earned 10000$ by crossing the start line!\n");
  }
  current_player->current_block_id = (current_player->current_block_id+dice)%BOARD_SIZE;/*updating players position*/
  printf("You just moved %d blocks\n",dice);
}
void init_players(player* player_one, player* player_two)
{
  int i;
  for(i=0;i<MAX_OWNED; i++)
  {
    player_one->owned_block_ids[i]=EMPTY;
    player_two->owned_block_ids[i]=EMPTY;
  }
  player_one->name = "Car";
  player_two->name = "Cap";
  player_one->type = car;
  player_two->type = cap;
  player_one->account=100000;
  player_two->account=100000;
  player_one->current_block_id = 0;
  player_two->current_block_id = 0;
  player_one->turn_to_wait = 0;
  player_two->turn_to_wait = 0;
}
void sell_property(block board[BOARD_SIZE], player* current_player)
{
  int user_choice;
  player bank;
  bank.type = noone;

  if(current_player->owned_block_ids[0]!=EMPTY)
  {
    do
    {
      show_my_properties(current_player->owned_block_ids, board);
      printf("Your choice(0 to exit): ");
      scanf("%d", &user_choice);
      if(!is_not_choice(current_player->owned_block_ids, MAX_OWNED, user_choice))/*not is not choice meaning it is a possible choice*/
      {
        current_player->account += board[user_choice].price/2 + board[user_choice].house_count *(board[user_choice].house_price/2);
        remove_property(current_player->owned_block_ids, MAX_OWNED, user_choice);
        board[user_choice].house_count=0;/*not sure if I should remove the houses built after selling, ask hoca*/
        board[user_choice].owner=bank;
      }
      else if(user_choice!=0) printf("Invalid choice. Please try again\n");
      if(current_player->owned_block_ids[0]==EMPTY && user_choice!=0) printf("You have sold all your properties.\n");
      if(user_choice==0) printf("Returning to main menu.\n");
    }while(user_choice!=0 && is_not_choice(current_player->owned_block_ids, MAX_OWNED, user_choice) && current_player->owned_block_ids[0]!=EMPTY);
  }
  else printf("You need to be an owner of at least 1 property in order to be able to sell\n");
}
void remove_property(int* arr, int n, int user_choice)
{
  int i;
  for(i=0; arr[i] != user_choice; i++);/*finding index of the element to be removed*/
  for(; arr[i+1]!=EMPTY && i+1<n; i++) arr[i] = arr[i+1];
  arr[i] = EMPTY;/*setting the last element whose duplicate is in the index before him to EMPTY(terminating value)*/
}

void buy_house(block* current_block,player* current_player)
{
  int user_choice;
  int max_choice;
    if(current_block->owner.type == current_player->type)
    {
      if(current_player->account >= current_block->house_price)
      {
        if(current_block->house_count < HOUSE_LIMIT)/*3 is the maximum number of houses on a property*/
        {
          printf("Price of one house: %d$\n", current_block->house_price);
          max_choice = HOUSE_LIMIT - current_block->house_count;
          printf("You can construct %d houses at most here.\n", max_choice);
          printf("How many of them do you want to build?\n");
          do{
            scanf("%d", &user_choice);/*User is able to buy 1 or more houses as stated in homework's pdf, according to its funds of course, once it came to here it will buy at least one house(or more) since its funds are sufficient*/
            if(user_choice>max_choice) printf("This exceeds the maximum number of houses that can be constructed on this property\n");
            else if(current_player->account<user_choice*(current_block->house_price)) printf("Your funds are not sufficient for building this many houses. Please try again\n");
            else if(user_choice<=0) printf("Please enter a positive valid number\n");
          }while(user_choice>max_choice || current_player->account<user_choice*(current_block->house_price) || user_choice<=0);
          current_block->house_count += user_choice;
          current_player->account -= user_choice*(current_block->house_price);
          printf("Congrats, you have just built %d house(s)\n", user_choice);
        }
        else printf("This property is full. Three houses have already been built here\n");
      }
      else printf("Your funds are not sufficient to construct a house here\n");
    }
    else printf("You need to own a property in order to buy a house on it\n");/*will work for car park, start and so on since they don't have owners*/
  return;
}
void buy_property(block* current_block, player* current_player)
{
  int i;
  int choice;
  if(current_player->owned_block_ids[MAX_OWNED-1]==EMPTY)
  {
    for(i=0; current_player->owned_block_ids[i]!=EMPTY; i++);/*iterating until first empty space is reached for property to be put into*/
    if(current_player->current_block_id == current_block->block_id)
    {
      if(current_block->type==property)
      {
        if(current_block->owner.type == noone)
        {
          if(current_player->account >= current_block->price)
          {
            current_block->owner = *current_player;
            current_player->account -= current_block->price;
            current_player->owned_block_ids[i] = current_block->block_id;
            do
            {
              printf("Press 1 if you want to built a house here at price %d$, or 0 to exit: ", current_block->house_price);
              scanf("%d",&choice);
              if(choice!=1 && choice!=0) printf("Please enter a valid option.\n");
            }while(choice!=1 && choice!=0);
            if(choice == 1)buy_house(current_block, current_player);
          }
          else printf("You don't have enough funds on your account to make this purchase.\n");
        }
        else printf("Invalid purchase. This property has already been sold.\n");
      }
      else printf("This block is not a property meaning it is not for sale.\n");
    }
    else printf("Invalid purchase. You need to be standing on the block you are trying to buy.\n");
  }
  else printf("You have already acquired maximum number of properties\n");
  return;
}
void show_properties(block board[20])
{
  int i,j;
  int longest = find_longest(board);
  int user_choice;
  int possible_choices[BOARD_SIZE];
  for(i=0,j=0;i<BOARD_SIZE;i++)/*finding possible choices for user*/
  {
   if(board[i].type==property)
    {
      possible_choices[j] = board[i].block_id;
      j++;
    }
  }
  for(; j<BOARD_SIZE; j++) possible_choices[j]=EMPTY;/*putting a terminating condition, at least one EMPTY will be present due to the start field which has to be present in every game no matter the number of blocks*/

  do{
      printf("Please select a property to see details:\n");
      for(i=0; i<BOARD_SIZE;i++)
      {
        if(board[i].type==property) printf("%d - %*s%s\n", board[i].block_id, longest/2-find_num_of_digits(board[i].block_id), "", board[i].name);
      }
      printf("0 - %*s%s\n", longest/2-1, "", "Exit");/*-1 since zero is a single digit num*/
      scanf("%d", &user_choice);
      if(user_choice!= 0 && is_not_choice(possible_choices, BOARD_SIZE, user_choice)) printf("Please enter a valid choice\n");
      else if(user_choice!=0) show_chosen_property(board[user_choice], longest, user_choice);
      else printf("Returning to main menu\n");
    }while(is_not_choice(possible_choices, BOARD_SIZE, user_choice) && user_choice!=0);
}
void show_chosen_property(block chosen_block, int longest_word, int user_choice)
{
  printf("\n\n");
  int i;
  /*----*/
  for(i=0; i<longest_word*3+1; i++) printf("-");
  printf("\n");
  /*Name of the neighbourhood*/
  printf("|%*s%*s|", longest_word+longest_word/2+(int)strlen(chosen_block.name)/2, chosen_block.name, longest_word+longest_word/2-(int)strlen(chosen_block.name)/2,"");
  printf("\n");
  /*----*/
  for(i=0; i<longest_word*3+1; i++) printf("-");
  printf("\n");
  /*Rent*/
  printf("|%*s%*s", (longest_word+longest_word/2)/2 + (int)strlen("Rent")/2, "Rent", (longest_word+longest_word/2)/2 - (int)strlen("Rent")/2,"");
  printf("%*d$%*s|", (longest_word+longest_word/2)/2 + find_num_of_digits(chosen_block.rent)/2, chosen_block.rent, (longest_word+longest_word/2)/2 - find_num_of_digits(chosen_block.rent)/2-1,"");/*-1 in the last one because of the dollar caracter*/
  printf("\n");
  /*Rent 1 H*/
  printf("|%*s%*s", (longest_word+longest_word/2)/2 + (int)strlen("Rent 1 H")/2, "Rent 1 H", (longest_word+longest_word/2)/2 - (int)strlen("Rent 1 H")/2,"");
  printf("%*d$%*s|", (longest_word+longest_word/2)/2 + find_num_of_digits(chosen_block.rent_1)/2, chosen_block.rent_1, (longest_word+longest_word/2)/2 - find_num_of_digits(chosen_block.rent_1)/2-1,"");/*-1 in the last one because of the dollar caracter*/
  printf("\n");
  /*Rent 2 H*/
  printf("|%*s%*s", (longest_word+longest_word/2)/2 + (int)strlen("Rent 2 H")/2, "Rent 2 H", (longest_word+longest_word/2)/2 - (int)strlen("Rent 2 H")/2,"");
  printf("%*d$%*s|", (longest_word+longest_word/2)/2 + find_num_of_digits(chosen_block.rent_2)/2, chosen_block.rent_2, (longest_word+longest_word/2)/2 - find_num_of_digits(chosen_block.rent_2)/2-1,"");/*-1 in the last one because of the dollar caracter*/
  printf("\n");
  /*Rent 3 H*/
  printf("|%*s%*s", (longest_word+longest_word/2)/2 + (int)strlen("Rent 3 H")/2, "Rent 3 H", (longest_word+longest_word/2)/2 - (int)strlen("Rent 3 H")/2,"");
  printf("%*d$%*s|", (longest_word+longest_word/2)/2 + find_num_of_digits(chosen_block.rent_3)/2, chosen_block.rent_3, (longest_word+longest_word/2)/2 - find_num_of_digits(chosen_block.rent_3)/2-1,"");/*-1 in the last one because of the dollar caracter*/
  printf("\n");
  /*----*/
  for(i=0; i<longest_word*3+1; i++) printf("-");
  printf("\n");
  /*House price*/
  printf("|%*s%*s", (longest_word+longest_word/2)/2 + (int)strlen("House Price")/2, "House Price", (longest_word+longest_word/2)/2 - (int)strlen("House Price")/2,"");
  printf("%*d$%*s|", (longest_word+longest_word/2)/2 + find_num_of_digits(chosen_block.house_price)/2, chosen_block.house_price, (longest_word+longest_word/2)/2 - find_num_of_digits(chosen_block.house_price)/2-1,"");/*-1 in the last one because of the dollar caracter*/
  printf("\n");
  /*----*/
  for(i=0; i<longest_word*3+1; i++) printf("-");
  printf("\n");
  printf("\n");
}
bool is_not_choice(int possible_choices[], int n, int user_choice)
{
  int i=0;
  bool answ=TRUE;
  while(possible_choices[i]!=EMPTY && i<n)/*Terminating caracter is put in callee function*/
  {
    if(possible_choices[i]==user_choice)
    {
      answ = FALSE;
      break;
    }
    i++;
  }
  return answ;
}
void show_board(block board[BOARD_SIZE], player player_one, player player_two)
{
  int longest = find_longest(board);
  int i,j,k,h,f;
  int num_of_info=3;
  int horizontal_display=6;
  if(longest%2!=0)longest++;/*to be able to make proper identation if printing more than one word in the same block(two players at the same block)*/

  /*PRINTING THE FIRST ROW, 6 BLOCKS*/
  for(i=0;i<longest*2*horizontal_display+horizontal_display+1;i++) printf("-");/*printing the border*/
  printf("\n");
  {
    for(i=0;i<num_of_info;i++)/*Number 3 is representing maximum number of things being displayed in each block*/
    {
      for(j=0;j<horizontal_display;j++)
      {
        if(j==0) printf("|");
        switch(i)
        {
          case 0:
            printf("%*s%*s|", longest+(int)strlen(board[j].name)/2,board[j].name,longest-(int)strlen(board[j].name)/2,"");

            break;
          case 1:
            if(board[j].type == property) printf("%*d$%*s|", longest + find_num_of_digits(board[j].price)/2,board[j].price, longest-find_num_of_digits(board[j].price)/2-1,"");
            else if(board[j].type == tax)  printf("%*d$%*s|", longest + find_num_of_digits(board[j].rent)/2,board[j].rent, longest-find_num_of_digits(board[j].rent)/2-1,"");
            else printf("%*s|", longest*2,"");

            break;
          case 2:/*Current player should be here*/
            if(board[j].block_id!=player_one.current_block_id && board[j].block_id!=player_two.current_block_id) printf("%*s|", longest*2,"");
            else/*if one or more players are present on the block*/
            {
              if(board[j].block_id==player_one.current_block_id && board[j].block_id!=player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_one.name)/2, player_one.name,longest-(int)strlen(player_one.name)/2,"");
                else if(board[j].block_id!=player_one.current_block_id && board[j].block_id==player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_two.name)/2, player_two.name,longest-(int)strlen(player_two.name)/2,"");
                else/*if they are on the same block*/
                {
                  printf("%*s%*s", longest/2+(int)strlen(player_one.name)/2, player_one.name,longest/2-(int)strlen(player_one.name)/2,"");
                  printf("%*s%*s|", longest/2+(int)strlen(player_two.name)/2, player_two.name,longest/2-(int)strlen(player_two.name)/2,"");
                }
            }
            break;
        }
      }
      printf("\n");
    }
    for(i=0;i<longest*2*horizontal_display+horizontal_display+1;i++) printf("-");/*bottom border*/
    printf("\n");

    /*PRINTING MIDDLE PART*/
    for(h=1, k=horizontal_display; h<=(BOARD_SIZE-2*horizontal_display)/2; k++, h++)/*-2 because of the first and the last row which are printted out differently*/
    {
      for(i=0;i<num_of_info;i++)/*Number 3 is representing maximum number of things being displayed in each block*/
      {
        for(j=0;j<horizontal_display;j++)
        {
          if(j==0 || j==horizontal_display-1) printf("|");
          if(j==0 || j==horizontal_display-1)
          {
            if(j==0)
            {
              switch(i)
              {
                case 0:
                  printf("%*s%*s|", longest+(int)strlen(board[BOARD_SIZE-h].name)/2,board[BOARD_SIZE-h].name,longest-(int)strlen(board[BOARD_SIZE-h].name)/2,"");
                  break;
                case 1:
                  if(find_num_of_digits(board[BOARD_SIZE-h].price!=0)) printf("%*d$%*s|", longest + find_num_of_digits(board[BOARD_SIZE-h].price)/2,board[BOARD_SIZE-h].price, longest-find_num_of_digits(board[BOARD_SIZE-h].price)/2-1,"");
                  else if(board[BOARD_SIZE-h].type == tax)  printf("%*d$%*s|", longest + find_num_of_digits(board[BOARD_SIZE-h].rent)/2,board[BOARD_SIZE-h].rent, longest-find_num_of_digits(board[BOARD_SIZE-h].rent)/2-1,"");
                  else printf("%*s|", longest*2,"");

                  break;
                case 2:/*Current player should be here*/
                  if(board[BOARD_SIZE-h].block_id!=player_one.current_block_id && board[BOARD_SIZE-h].block_id!=player_two.current_block_id) printf("%*s|", longest*2,"");
                  else/*if one or more players are present on the block*/
                  {
                    if(board[BOARD_SIZE-h].block_id==player_one.current_block_id && board[BOARD_SIZE-h].block_id!=player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_one.name)/2, player_one.name,longest-(int)strlen(player_one.name)/2,"");
                      else if(board[BOARD_SIZE-h].block_id!=player_one.current_block_id && board[BOARD_SIZE-h].block_id==player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_two.name)/2, player_two.name,longest-(int)strlen(player_two.name)/2,"");
                      else/*if they are on the same block*/
                      {
                        printf("%*s%*s", longest/2+(int)strlen(player_one.name)/2, player_one.name,longest/2-(int)strlen(player_one.name)/2,"");
                        printf("%*s%*s|", longest/2+(int)strlen(player_two.name)/2, player_two.name,longest/2-(int)strlen(player_two.name)/2,"");
                      }
                  }
                  break;
              }
            }
            else
            {
              switch(i)
              {
                case 0:
                  printf("%*s%*s|", longest+(int)strlen(board[k].name)/2,board[k].name,longest-(int)strlen(board[k].name)/2,"");
                  break;
                case 1:
                  if(find_num_of_digits(board[k].price!=0)) printf("%*d$%*s|", longest + find_num_of_digits(board[k].price)/2,board[k].price, longest-find_num_of_digits(board[k].price)/2-1,"");
                  else if(board[k].type == tax)  printf("%*d$%*s|", longest + find_num_of_digits(board[k].rent)/2,board[k].rent, longest-find_num_of_digits(board[k].rent)/2-1,"");
                  else printf("%*s|", longest*2,"");

                  break;
                case 2:/*Current player should be here*/
                  if(board[k].block_id!=player_one.current_block_id && board[k].block_id!=player_two.current_block_id) printf("%*s|", longest*2,"");
                  else/*if one or more players are present on the block*/
                  {
                    if(board[k].block_id==player_one.current_block_id && board[k].block_id!=player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_one.name)/2, player_one.name,longest-(int)strlen(player_one.name)/2,"");
                      else if(board[k].block_id!=player_one.current_block_id && board[k].block_id==player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_two.name)/2, player_two.name,longest-(int)strlen(player_two.name)/2,"");
                      else/*if they are on the same block*/
                      {
                        printf("%*s%*s", longest/2+(int)strlen(player_one.name)/2, player_one.name,longest/2-(int)strlen(player_one.name)/2,"");
                        printf("%*s%*s|", longest/2+(int)strlen(player_two.name)/2, player_two.name,longest/2-(int)strlen(player_two.name)/2,"");
                      }
                  }
                  break;
              }
            }
          }
          else
          {/*The same as in part one just more spaces printed*/
              if(j!=horizontal_display-2) for(f=0;f<2*longest+1;f++) printf(" ");
              else for(f=0;f<2*longest;f++) printf(" ");/*for indentation not printing the last one since it is place of | caracter fora new block*/
          }
        }
        printf("\n");
      }
      /*to be printed after every line of blocks is printed*/
      if(h+1<=(BOARD_SIZE-2*horizontal_display)/2)/*only until +1 or one time less than block columns since the last line is different, it is consisted just of ----- caracters*/
      {
        for(i=0;i<2*longest+2;i++)printf("-");/*+2 because no | caracters*/
        for(i=0;i<(longest*2+1)*(horizontal_display-2)-1;i++) printf(" ");/*+1 because only 1 | caracter missing insetad of 2 like previously and -1 because last place is reserved for - caracter*/
        for(i=0;i<2*longest+2;i++)printf("-");
        printf("\n");
      }
    }
    /*MIDDLE PART PRINTTED*/

    for(i=0;i<longest*2*horizontal_display+horizontal_display+1;i++) printf("-");/*printing the border*/

    /*PRINTING BOTTOM PART, THE SAME LOGIC LIKE FOR FIRST ROW JUST IN REVERSE PRINTTED*/
    printf("\n");
    {
      for(i=0;i<num_of_info;i++)/*Number 3 is representing maximum number of things being displayed in each block*/
      {
        for(k=BOARD_SIZE-h, j=0;j<horizontal_display;j++,k--)/*using h because I have to continue from where it have stopped printing, that is the last element on the left side just befor the last row*/
        {
          if(j==0) printf("|");
          switch(i)
          {
            case 0:
              printf("%*s%*s|", longest+(int)strlen(board[k].name)/2,board[k].name,longest-(int)strlen(board[k].name)/2,"");

              break;
            case 1:
              if(find_num_of_digits(board[k].price!=0)) printf("%*d$%*s|", longest + find_num_of_digits(board[k].price)/2,board[k].price, longest-find_num_of_digits(board[k].price)/2-1,"");
              else if(board[k].type == tax)  printf("%*d$%*s|", longest + find_num_of_digits(board[k].rent)/2,board[k].rent, longest-find_num_of_digits(board[k].rent)/2-1,"");

              else printf("%*s|", longest*2,"");

              break;
            case 2:/*Current player should be here*/
              if(board[k].block_id!=player_one.current_block_id && board[k].block_id!=player_two.current_block_id) printf("%*s|", longest*2,"");
              else/*if one or more players are present on the block*/
              {
                if(board[k].block_id==player_one.current_block_id && board[k].block_id!=player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_one.name)/2, player_one.name,longest-(int)strlen(player_one.name)/2,"");
                  else if(board[k].block_id!=player_one.current_block_id && board[k].block_id==player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_two.name)/2, player_two.name,longest-(int)strlen(player_two.name)/2,"");
                  else/*if they are on the same block*/
                  {
                    printf("%*s%*s", longest/2+(int)strlen(player_one.name)/2, player_one.name,longest/2-(int)strlen(player_one.name)/2,"");
                    printf("%*s%*s|", longest/2+(int)strlen(player_two.name)/2, player_two.name,longest/2-(int)strlen(player_two.name)/2,"");
                  }
              }
              break;
          }
        }
        printf("\n");
      }
      for(i=0;i<longest*2*horizontal_display+horizontal_display+1;i++) printf("-");/*bottom border*/
      printf("\n");
    }
  }
}
int find_num_of_digits(int num)
{
  int i;
  for(i=0; num!=0; i++,num/=10);
  return i;
}
int find_longest(block board[BOARD_SIZE])/*return lenght of the longest board name*/
{
  int i;
  int longest = strlen(board[0].name);
  for(i=1; i<BOARD_SIZE; i++)
  {
    if(longest<strlen(board[i].name)) longest = strlen(board[i].name);
  }
  return longest;
}

void init_the_board(block board[BOARD_SIZE])
{
  player bank;
  bank.type = noone;
  /*first block*/
  board[0].block_id=0;
  board[0].name = "Start";
  board[0].price=0;
  board[0].rent=0;
  board[0].rent_1=0;
  board[0].rent_2=0;
  board[0].rent_3=0;
  board[0].house_price=0;
  board[0].house_count=0;
  board[0].owner=bank;
  board[0].type=start;
  /*Esenyurt*/
  board[1].block_id=1;
  board[1].name = "Esenyurt";
  board[1].price=16000;
  board[1].rent=800;
  board[1].rent_1=4000;
  board[1].rent_2=9000;
  board[1].rent_3=25000;
  board[1].house_price=10000;
  board[1].owner=bank;
  board[1].house_count=0;
  board[1].type=property;
  /*first block*/
  board[2].block_id=2;
  board[2].name = "Car Park";
  board[2].price=0;
  board[2].rent=1500;
  board[2].rent_1=0;
  board[2].rent_2=0;
  board[2].rent_3=0;
  board[2].house_price=0;
  board[2].owner=bank;
  board[2].house_count=0;
  board[2].type=tax;
  /*first block*/
  board[3].block_id=3;
  board[3].name = "Tuzla";
  board[3].price=16500;
  board[3].rent=850;
  board[3].rent_1=4250;
  board[3].rent_2=9500;
  board[3].rent_3=26000;
  board[3].house_price=12000;
  board[3].owner=bank;
  board[3].house_count=0;
  board[3].type=property;
  /*kurackoy*/
  board[4].block_id=4;
  board[4].name = "Arnavutkoy";
  board[4].price=17000;
  board[4].rent=875;
  board[4].rent_1=4500;
  board[4].rent_2=10000;
  board[4].rent_3=28000;
  board[4].house_price=12000;
  board[4].owner=bank;
  board[4].house_count=0;
  board[4].type=property;
  /*Wait 2 Turn*/
  board[5].block_id=5;
  board[5].name = "Wait 2 Turn";
  board[5].price=0;
  board[5].rent=2;
  board[5].rent_1=0;
  board[5].rent_2=0;
  board[5].rent_3=0;
  board[5].house_price=0;
  board[5].owner=bank;
  board[5].house_count=0;
  board[5].type=punish;
  /*Catalca*/
  board[6].block_id=6;
  board[6].name = "Catalca";
  board[6].price=20000;
  board[6].rent=1000;
  board[6].rent_1=5000;
  board[6].rent_2=12000;
  board[6].rent_3=30000;
  board[6].house_price=13000;
  board[6].owner=bank;
  board[6].house_count=0;
  board[6].type=property;
  /*Beykoz*/
  board[7].block_id=7;
  board[7].name = "Beykoz";
  board[7].price=23000;
  board[7].rent=1100;
  board[7].rent_1=5500;
  board[7].rent_2=12500;
  board[7].rent_3=33000;
  board[7].house_price=13000;
  board[7].owner=bank;
  board[7].house_count=0;
  board[7].type=property;
  /*Car fix*/
  board[8].block_id=8;
  board[8].name = "Car Fix";
  board[8].price=0;
  board[8].rent=1750;
  board[8].rent_1=0;
  board[8].rent_2=0;
  board[8].rent_3=0;
  board[8].house_price=0;
  board[8].owner=bank;
  board[8].house_count=0;
  board[8].type=tax;
  /*Maltepe*/
  board[9].block_id=9;
  board[9].name = "Maltepe";
  board[9].price=30000;
  board[9].rent=1350;
  board[9].rent_1=7000;
  board[9].rent_2=15000;
  board[9].rent_3=40000;
  board[9].house_price=15000;
  board[9].owner=bank;
  board[9].house_count=0;
  board[9].type=property;
  /*Bills*/
  board[10].block_id=10;
  board[10].name = "Bills";
  board[10].price=0;
  board[10].rent=2000;
  board[10].rent_1=0;
  board[10].rent_2=0;
  board[10].rent_3=0;
  board[10].house_price=0;
  board[10].owner=bank;
  board[10].house_count=0;
  board[10].type=tax;
  /*Bills*/
  board[11].block_id=11;
  board[11].name = "Sisli";
  board[11].price=33000;
  board[11].rent=1500;
  board[11].rent_1=8000;
  board[11].rent_2=16000;
  board[11].rent_3=42000;
  board[11].house_price=16000;
  board[11].owner=bank;
  board[11].house_count=0;
  board[11].type=property;
  /*Bills*/
  board[12].block_id=12;
  board[12].name = "Oil";
  board[12].price=0;
  board[12].rent=2250;
  board[12].rent_1=0;
  board[12].rent_2=0;
  board[12].rent_3=0;
  board[12].house_price=0;
  board[12].owner=bank;
  board[12].house_count=0;
  board[12].type=tax;
  /*Atasehir*/
  board[13].block_id=13;
  board[13].name = "Atasehir";
  board[13].price=35000;
  board[13].rent=1600;
  board[13].rent_1=8500;
  board[13].rent_2=17000;
  board[13].rent_3=45000;
  board[13].house_price=17000;
  board[13].owner=bank;
  board[13].house_count=0;
  board[13].type=property;
  /*Sariyer*/
  board[14].block_id=14;
  board[14].name = "Sariyer";
  board[14].price=40000;
  board[14].rent=1750;
  board[14].rent_1=9500;
  board[14].rent_2=19000;
  board[14].rent_3=48000;
  board[14].house_price=19000;
  board[14].owner=bank;
  board[14].house_count=0;
  board[14].type=property;
  /*Wait 1 Turn*/
  board[15].block_id=15;
  board[15].name = "Wait 1 Turn";
  board[15].price=0;
  board[15].rent=1;
  board[15].rent_1=0;
  board[15].rent_2=0;
  board[15].rent_3=0;
  board[15].house_price=0;
  board[15].owner=bank;
  board[15].house_count=0;
  board[15].type=punish;
  /*Kadikoy*/
  board[16].block_id=16;
  board[16].name = "Kadikoy";
  board[16].price=43000;
  board[16].rent=1900;
  board[16].rent_1=11000;
  board[16].rent_2=21500;
  board[16].rent_3=55000;
  board[16].house_price=23000;
  board[16].owner=bank;
  board[16].house_count=0;
  board[16].type=property;
  /*Besiktas*/
  board[17].block_id=17;
  board[17].name = "Besiktas";
  board[17].price=60000;
  board[17].rent=2500;
  board[17].rent_1=15000;
  board[17].rent_2=28000;
  board[17].rent_3=60000;
  board[17].house_price=30000;
  board[17].owner=bank;
  board[17].house_count=0;
  board[17].type=property;
  /*Vacation*/
  board[18].block_id=18;
  board[18].name = "Vacation";
  board[18].price=0;
  board[18].rent=5000;
  board[18].rent_1=0;
  board[18].rent_2=0;
  board[18].rent_3=0;
  board[18].house_price=0;
  board[18].owner=bank;
  board[18].house_count=0;
  board[18].type=tax;
  /*Bebek*/
  board[19].block_id=19;
  board[19].name = "Bebek";
  board[19].price=70000;
  board[19].rent=3500;
  board[19].rent_1=20000;
  board[19].rent_2=35500;
  board[19].rent_3=65000;
  board[19].house_price=35000;
  board[19].owner=bank;
  board[19].house_count=0;
  board[19].type=property;
}
