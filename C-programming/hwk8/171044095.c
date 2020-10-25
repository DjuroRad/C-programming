#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define BOARD_SIZE 24
#define MAX_OWNED 11
#define EMPTY -1
#define HOUSE_LIMIT 3
typedef enum{noone, cap, car}player_type;
typedef enum{start, property, tax, punish, fortune}block_type;
typedef enum{FALSE, TRUE}bool;
typedef enum{free_house=1, time_travel, garnishment, generosity, treasure_hunter}card_type;
typedef struct{
  player_type type;/* Holds type of the player. The player_type must be defined as
  enumerated type which consist of noone, cap and car values.*/
  int current_block_id;/* Holds player location as block id.*/
  int owned_block_ids[MAX_OWNED];/*Holds property block ids that are owned by the user.*/
  int account;/* Holds amount of the current money of the player.*/
  int turn_to_wait;/* Holds number of turns to wait if the player got a punishment.*/
  char * name;/*Holds the name of the player.*/
}player;

typedef struct block_node{
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
  struct block_node* next_block;/*stores the address of the next node*/
}block_node;

typedef struct
{
  char* name;/*holds the name of the fortune card,Free House, Time travel, etc*/
  card_type type;/*card's number is also contained within this data_type*/
}fortune_card;

void init_the_board(block_node* head_node);
int find_longest(block_node* head_block);
int find_num_of_digits(int num);
void show_board( block_node* head_block, player player_one, player player_two);
void init_players(player* player_one, player* player_two, int against_computer);
block_node* find_block_by_index(block_node* head_block, int j);
bool is_not_choice(int possible_choices[], int n, int user_choice);
void show_chosen_property(block_node chosen_block, int longest_word, int user_choice);
void show_properties(block_node* head_block);
void buy_property(block_node* current_block, player* current_player);
void buy_house(block_node* current_block,player* current_player);
void remove_property(int* arr, int n, int user_choice);
void sell_property(block_node* head_block, player* current_player);
void show_my_properties(int owned_blocks[MAX_OWNED], block_node* head_block);
void move_current_player(player* current_player,int dice);
void play_menu();
void pay_rent(player* current_player,player* next_player, block_node current_block);
int check_account(player current_player, block_node* head_block, int rent_check, int bill_check);
void gameplay (block_node* head_node, player player_one, player player_two, int against_computer);
fortune_card get_card();
void execute_card(player* current_player,player*  next_player,block_node* head_block, fortune_card pulled_card, int against_computer);
void fortune_free_house(player* current_player,block_node* head_block, int against_computer);
int roll_the_dice();
void after_move( player* current_player, player* next_player, block_node* head_block, int against_computer);
int average_price(block_node* block_head);/*finds average price of all properties(note that int is used on purpose since everything in the code is regarded as int up to this point)*/
void computer_buy_property(block_node* block_head, player* computer);
void computer_buy_house(block_node* block_head, player* computer);
void computer_sells_property(player* computer, block_node* block_head, int payment);/*first checking if there is enough money on the account and than if there is not, coming here*/

int main()
{
  player  player_one;
  player  player_two;
  int user_wants_to_play_against_comp;
  block_node* block_head = (block_node*)malloc(sizeof(block_node));
  do{/*seeing if game will be against computer or another user*/
  printf("%d - Play against computer\n%d - Play against another user\n", TRUE, FALSE);
  scanf("%d", &user_wants_to_play_against_comp);
  if(user_wants_to_play_against_comp!=FALSE && user_wants_to_play_against_comp!=TRUE) printf("Please enter a valid choice\n");
  }while(user_wants_to_play_against_comp!=FALSE && user_wants_to_play_against_comp!=TRUE);/*1 and 0 could be also used*/
  /*Since it is stated in homework features are added, now user can choose if it is to play against computer or someone */
  init_players(&player_one, &player_two, user_wants_to_play_against_comp);
  init_the_board(block_head);
  gameplay(block_head, player_one, player_two, user_wants_to_play_against_comp);
}
void gameplay (block_node* head_block, player player_one, player player_two, int against_computer)
{
  int choice;
  int dice;
  player* current_player = &player_one;
  player* next_player = &player_two;
  player* temp;
  show_board(head_block, player_one, player_two);
  do {
    do{
      if(against_computer && current_player->type == cap)
      {
        choice = 1;
        printf("\n%s's turn\n", current_player->name);
      }
      else
      {
        play_menu();
        printf("%s's turn\n", current_player->name);
        printf("Please select an option to continue:\n");
        scanf("%d",&choice);
      }
      switch(choice)
      {
        case 1:
          dice = roll_the_dice();
          move_current_player(current_player, dice);
          show_board(head_block, player_one, player_two);
          after_move(current_player, next_player, head_block, against_computer);
          break;
        case 2:
          printf("Your account is %d$\n", current_player->account);
          break;
        case 3:
          show_my_properties(current_player->owned_block_ids, head_block);/*player's properties*/
          break;
        case 4:
          show_properties(head_block);
          break;
        case 5:
          buy_property(find_block_by_index(head_block,current_player->current_block_id), current_player);
          break;
        case 6:
          buy_house(find_block_by_index(head_block,current_player->current_block_id), current_player);
          break;
        case 7:
          sell_property(head_block, current_player);
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
  }while(player_one.account>0 && player_two.account>0);/*0 or less is bankruptcy*/
  if(player_one.account>0) printf("Congrats %s. You have won the game\n", player_one.name);
  else printf("Congrats %s. You have won the game\n", player_two.name);
}
void computer_sells_property(player* computer, block_node* block_head, int payment)/*first checking if there is enough money on the account and than if there is not, coming here*/
{
  block_node* current_block;
  block_node* block_to_sell = find_block_by_index(block_head, computer->owned_block_ids[0]);/*we know that comp must have a property to sell here because we executed check_account before calling thsi function*/
  int current_block_selling_price;
  int computer_choice;
  player bank;
  bank.type = noone;
  int max=block_to_sell->price/2 + (block_to_sell->house_count)*(block_to_sell->house_price/2);
  computer_choice = computer->owned_block_ids[0];
  int i;
  for(i=1; i<MAX_OWNED && computer->owned_block_ids[i]!=EMPTY; i++)/*max is alraedy the first elemnt so starts from secoond, i=1*/
  {
    current_block = find_block_by_index(block_head, computer->owned_block_ids[i]);
    current_block_selling_price = current_block->price/2 + (current_block->house_count)*(current_block->house_price/2);
    if(max<current_block_selling_price)
    {
    max = current_block_selling_price;
    block_to_sell = current_block;
    computer_choice = computer->owned_block_ids[i];
    }
  }
  computer->account += max;
  remove_property(computer->owned_block_ids, MAX_OWNED, computer_choice);
  block_to_sell->owner = bank;
  block_to_sell->house_count = 0;
  printf("%s just sold %s at price %d$\n", computer->name, block_to_sell->name, max);
  if(computer->account<=payment) computer_sells_property(computer, block_head, payment);/*sells until it has enough money, note that it is checked before that computer will have enough properties if necessery(in check_account function) */
}
void computer_buy_house(block_node* block_head, player* computer)
{
  int dice;
  int num_of_prop_owned;
  int num_of_prop_board=0;
  int max;
  block_node* current_block = find_block_by_index(block_head, computer->current_block_id);
  block_node* temp_block = block_head;
  while(temp_block!=NULL)/*calculating number of properties*/
  {
    if(temp_block->type == property) num_of_prop_board++;
    temp_block = temp_block->next_block;
  }
  for(num_of_prop_owned=0; computer->owned_block_ids[num_of_prop_owned]!=EMPTY && num_of_prop_owned<MAX_OWNED; num_of_prop_owned++);/*calculating the number of owned properties, ; in the end*/
  if(current_block->owner.type == computer->type)/*otherwise it is not on the property it owns and would skip the turn so no need for an else condition*/
  {
    if(num_of_prop_owned>=num_of_prop_board/3)
    {
      max = HOUSE_LIMIT - current_block->house_count;
      dice = roll_the_dice();/*if dice is not <= max than it will skip the turn, also it skips if it doesn't have enough money*/
      if(dice>=1 && dice<=3 && dice<=max && (computer->account > dice*current_block->house_price) && (current_block->house_count<HOUSE_LIMIT))
      {
      (current_block->house_count) += dice;
      computer->account -= current_block->house_price;
      printf("%s just purchased %d house(s) on %s\n", computer->name, dice, current_block->name);
      }
    }
  }
}
void computer_buy_property(block_node* block_head, player* computer)
{
  int dice;
  int i;
  int avrg = average_price(block_head);
  block_node* block_to_buy = find_block_by_index(block_head, computer->current_block_id);
  if(computer->owned_block_ids[MAX_OWNED-1]==EMPTY)/*if there is a possibility for a property to be bought*/
  {
    if(block_to_buy->price <= avrg)
    {
      if(computer->account>block_to_buy->price)
      {
        computer->account -= block_to_buy->price;
        for(i=0; computer->owned_block_ids[i]!=EMPTY && i<MAX_OWNED; i++);
        computer->owned_block_ids[i] = block_to_buy->block_id;
        block_to_buy->owner = *computer;
        printf("Computer just puchased %s\n", block_to_buy->name);
      }
    }
    else
    {/*throw dice*/
      dice = roll_the_dice();
      if(dice>=1 && dice<=3 && (computer->account > block_to_buy->price))
      {
        computer->account -= block_to_buy->price;
        for(i=0; computer->owned_block_ids[i]!=EMPTY && i<MAX_OWNED; i++);
        computer->owned_block_ids[i] = block_to_buy->block_id;
        block_to_buy->owner = *computer;
        printf("Computer just puchased %s\n", block_to_buy->name);
      }
    }
  }
  else printf("%s posseses maximum number of properties\n", computer->name);
}
int average_price(block_node* block_head)
{
  double avrg_price=0;
  int num_of_prop=0;
  while(block_head!=NULL)
  {
    if(block_head->type == property)
    {
      num_of_prop++;
      avrg_price += block_head->price;
    }
    block_head = block_head->next_block;
  }
  avrg_price = avrg_price/num_of_prop;
  return (int)avrg_price;
}
void after_move( player* current_player, player* next_player, block_node* head_block, int against_computer)
{
  int buy_choice;
  fortune_card pulled_card;
  int rent_to_pay;
  switch((*find_block_by_index(head_block,current_player->current_block_id)).type)
  {
    case fortune:
      pulled_card = get_card();
      printf("%s just stepped on Fortune Card block!\n%s pulled out \"%s\" fortune card!\n", current_player->name, current_player->name, pulled_card.name);
      execute_card(current_player, next_player, head_block, pulled_card, against_computer);
      if(pulled_card.type == time_travel) after_move(current_player, next_player, head_block, against_computer);
      break;
    case punish:/*same when both playing with another user or a computer*/
      printf("%s just got punished, %s won't be able to move during the next %d turn(s)\n", current_player->name, current_player->name, (*find_block_by_index(head_block,current_player->current_block_id)).rent);
      current_player->turn_to_wait = (*find_block_by_index(head_block,current_player->current_block_id)).rent;
      break;
    case tax:
      printf("%s has to pay the tax\n", current_player->name);
      if( (rent_to_pay=check_account(*current_player,head_block, TRUE, FALSE)) )/*would assign 0 to retn to pay if user has lost the game or doesn't have enough funds to continue, otherwise any kind of a positive value which satisfies the condition of if*/
      {
        if(current_player->account<=rent_to_pay && against_computer && current_player->type == cap)
        {
          computer_sells_property(current_player, head_block, rent_to_pay);
        }
        else
        {
          while(current_player->account<=rent_to_pay)
          {
            printf("Your funds are isufficient, you need to sell some property(s)\n");
            sell_property(head_block, current_player);
          }
        }
        current_player->account -= rent_to_pay;
        printf("After paying %s has %d$ left on its account\n", current_player->name, current_player->account);
      }
      else
      {
        printf("Oh, %s just went bankrupt.\nGame over\n", current_player->name);
        current_player->account = EMPTY;
      }
      break;
    case property:/*checking for all types of owners*/
      if((*find_block_by_index(head_block,current_player->current_block_id)).owner.type == noone)
      {
        if(against_computer && current_player->type == cap)
        {/*computer buying*/
          computer_buy_property(head_block, current_player);
          computer_buy_house(head_block, current_player);
        }
        else/*user buying*/
        {
          printf("This property is not owned by anyone. Press 1 to buy it, 0 to exit (Selling price:%d$): ", (*find_block_by_index(head_block,current_player->current_block_id)).price);
          do{
            scanf("%d", &buy_choice);
            if(buy_choice!=1 && buy_choice!=0) printf("Please enter a valid choice.\n");
          }while(buy_choice!=1 && buy_choice!=0);
          if(buy_choice==1) buy_property(find_block_by_index(head_block,current_player->current_block_id), current_player);
        }
      }
      else if((*find_block_by_index(head_block,current_player->current_block_id)).owner.type != current_player->type)/*arriving on enemy's property*/
      {
        printf("%s has arrived on someone else's land and will have to pay the rent.\n", current_player->name);
        if( (rent_to_pay=check_account(*current_player, head_block, TRUE, FALSE)) )/*would assign 0 to retn to pay if user has lost the game or doesn't have enough funds to continue, otherwise any kind of a positive value which satisfies the condition of if*/
        {
          if(against_computer && current_player->type == cap && current_player->account<=rent_to_pay)
          {/*the same condition as for when paying the tax*/
            computer_sells_property(current_player, head_block, rent_to_pay);
          }
          else
          {
            while(current_player->account<=rent_to_pay)
            {
              printf("Your funds are isufficient, you need to sell some property(s)\n");
              sell_property(head_block, current_player);
            }
          }
          pay_rent(current_player, next_player, *find_block_by_index(head_block,current_player->current_block_id));
          printf("Your account after paying the rent: %d$\n", current_player->account);
        }
        else
        {
          printf("Oh, %s just went bankrupt.\nGame over\n", current_player->name);
          current_player->account = EMPTY;
        }
      }
      else/*arriving on owned property*/
      {
      printf("%s just arrived on its property.\n", current_player->name);
      if(against_computer && current_player->type == cap) computer_buy_house(head_block, current_player);/*computer buys a house if possible*/
      }
      break;
  }
}
void execute_card(player* current_player,player*  next_player,block_node* head_block, fortune_card pulled_card, int against_computer)
{
  int dice;
  int garnishment_amount = 5000;
  int generosity_amount = 10000;
  switch (pulled_card.type) {
    case free_house:
      fortune_free_house(current_player, head_block, against_computer);
      break;
    case time_travel:
      dice = roll_the_dice();
      if(dice>=1 && dice<=3) move_current_player(current_player, 2);
      else move_current_player(current_player, -2);
      show_board(head_block, *current_player, *next_player);
      break;
    case garnishment:
      if(check_account(*current_player, head_block, FALSE, garnishment_amount))/*0(false) indicates it is not a rent/tax but but a bank payment*/
      {/*5000 represents the amount of garnishment to be paid*/
        if(current_player->account > garnishment_amount)
        {
        current_player->account -= garnishment_amount;
        }
        else
        {
          if(against_computer && current_player->type == cap)
          {/*buy checking account we already know that it would have enough money after selling some of the properties*/
            computer_sells_property(current_player, head_block, garnishment_amount);/*this function assures that after selling some properties computer will have more than the given amount(garnishment_amount) on its account*/
          }
          else
          {/*for user, when it is not computer's turn*/
            while(current_player->account <= garnishment_amount)
            {
              printf("You don't have enough money to pay the garnishment.\nPlease sell some of your properties before you countinue with payment\n");
              sell_property(head_block, current_player);
            }
          }
          current_player->account -= garnishment_amount;
        }
        printf("%s just paid the garnishment(%d$)\n", current_player->name, garnishment_amount);
      }
      else
      {
        printf("%s's funds are not sufficient to pay the garnishment\n", current_player->name);
        current_player->account = EMPTY;/*Game over condition*/
      }
      break;
    case generosity:
      if(check_account(*current_player, head_block, FALSE, generosity_amount))
      {
        if(current_player->account>generosity_amount)
        {
        current_player->account -= generosity_amount;
        next_player->account += generosity_amount;
        }
        else
        {
          if(against_computer && current_player->type == cap)
          {
            computer_sells_property(current_player, head_block, generosity_amount);/*this function assures that after selling some properties computer will have more than the given amount(garnishment_amount) on its account*/
          }
          else
          {
            while(current_player->account<=generosity_amount)
            {
              printf("Isufficient funds.\nSell some properties in order to be able to give money to your opponent\n");
              sell_property(head_block, current_player);
            }
          }
          current_player->account -= generosity_amount;
          next_player->account += generosity_amount;
        }
        printf("%s just gave %d$ to %s\n", current_player->name, generosity_amount, next_player->name);
      }
      else
      {
        printf("%s doesn't posses enough funds to give its opponent its well deserved money\n", current_player->name);
        current_player->account = EMPTY;
      }
      break;
    case treasure_hunter:
      printf("%s just got 20000$ from the bank!\n", current_player->name);
      current_player->account += 20000;
      break;
  }
}
void fortune_free_house(player* current_player,block_node* head_block, int against_computer)
{
  block_node* current_node;
  int user_choice, max_price;
  int house_space[MAX_OWNED];
  int i,j;
  for(i=0; i<MAX_OWNED; i++) house_space[i] = EMPTY; /*initializing it with empty elements at first*/
  if(current_player->owned_block_ids[0]!=EMPTY)
  {
    for(i=0,j=0; i<MAX_OWNED && current_player->owned_block_ids[i]!=EMPTY; i++)/*checking if it is possible to build a house on any of the properties*/
    {
      current_node = find_block_by_index(head_block, current_player->owned_block_ids[i]);
      if(current_node->house_count<HOUSE_LIMIT)
      {
        house_space[j] = current_node->block_id;
        j++;
      }
    }
    if(house_space[0]!=EMPTY)/*meaning there is at least one property house could be built on*/
    {
      if(current_player->type == cap && against_computer)/*if user is playing against computer and it is computer's turn*/
      {/*Computer buying house on the most expensive property*/
        max_price = find_block_by_index(head_block, house_space[0])->price;
        user_choice = house_space[0];
        for(i=1; house_space[i]!=EMPTY && i<MAX_OWNED; i++)/*starting from 1 bcs 0 already consumed*/
        {
          if(max_price<find_block_by_index(head_block, house_space[i])->price)
          {
            max_price = find_block_by_index(head_block, house_space[i])->price;
            user_choice = house_space[i];
          }
        }
      }
      else
      {
        do{
          show_my_properties(house_space, head_block);
          printf("On which property of yours do you want to build a free house?");
          scanf("%d", &user_choice);
          if(is_not_choice(house_space, MAX_OWNED, user_choice)) printf("Please enter a valid option\n");
        }while(is_not_choice(current_player->owned_block_ids, MAX_OWNED, user_choice));
      }
      /*building a free house on the chosen property, if it is computer it would build on the first available property*/
      current_node = find_block_by_index(head_block, user_choice);
      (current_node->house_count)++;
      printf("%s just built a free house on %s\n", current_player->name, current_node->name);
    }
    else printf("There is no place at any of %s's properties to build a house.\n", current_player->name);
  }
  else printf("Unfortunately %s does not own any properties, meaning %s can't build a free house.\n", current_player->name, current_player->name);
}
fortune_card get_card()
{
  fortune_card random_card;
  int deck_size = 5;
  int card_number;/*it is between 1 and deck_size*/
  card_number = rand()%deck_size + 1;
  random_card.type = card_number;/*type and card_number are equvivalent(by predefined enum)*/
  switch(card_number)
  {
    case free_house:
      random_card.name = "Free House";
      break;
    case time_travel:
      random_card.name = "Time Travel";
      break;
    case garnishment:
      random_card.name = "Garnishment";
      break;
    case generosity:
      random_card.name = "Generosity";
      break;
    case treasure_hunter:
      random_card.name = "Treasure Hunter";
      break;
  }
  return random_card;
}
int check_account(player current_player, block_node* head_block, int rent_check, int bill_check)/*note that for this function to work either bill_check or rent_check needs to be FALSE*/
{
  bool answ;
  int rent_to_pay;
  int total_money=current_player.account;
  int i;
  block_node* current_block = find_block_by_index(head_block, current_player.current_block_id);
  if(rent_check)
  {
    switch(current_block->house_count)
    {
      case 0:
        rent_to_pay=current_block->rent;
        break;
      case 1:
        rent_to_pay=current_block->rent_1;
        break;
      case 2:
        rent_to_pay=current_block->rent_2;
        break;
      case 3:
        rent_to_pay = current_block->rent_3;
        break;
    }
  }
  else rent_to_pay = bill_check;
  for(i=0; current_player.owned_block_ids[i]!=EMPTY && i < MAX_OWNED; i++)
  {
    current_block = find_block_by_index(head_block, current_player.owned_block_ids[i]);
    total_money += (current_block->price)/2 + current_block->house_count*(current_block->house_price/2);/*when selling it is worth only half of the price paid originally for it*/
  }
  printf("Rent/Tax/Amount to pay: %d$\n", rent_to_pay);
  if(rent_to_pay>=total_money) answ = FALSE;/*amount of money can not be 0*/
  else answ = rent_to_pay;/*so that i could control num of blocks to be sold in playgame function*/

  return answ;
}
void pay_rent(player* current_player,player* next_player, block_node current_block)
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
int roll_the_dice()
{
  int max = 6;/*can move maximum 6 fields at a time*/
  int dice;
  dice = (rand()%max)+1;/*removing 0 from possible outcomes of a roll dice*/
  return dice;
}
void move_current_player(player* current_player, int dice)
{
  if(dice>0)
  {
    if(dice+current_player->current_block_id>=BOARD_SIZE)
    {
      current_player->account += 10000;/*adding 100000 to the account after going over start*/
      printf("%s has earned 10000$ by crossing the start line!\n", current_player->name);
    }
    current_player->current_block_id = (current_player->current_block_id+dice)%BOARD_SIZE;/*updating players position*/
    printf("%s rolled the dice :%d\n",current_player->name, dice);
  }
  else/*when the player is going backwards*/
  {
    dice = dice*(-1);/*for printing*/
    if((current_player->current_block_id-dice)>=0) current_player->current_block_id -= dice;
    else current_player->current_block_id = BOARD_SIZE+current_player->current_block_id - dice;/*just getting the index of the elemnts player is now on*/
    printf("%s just moved %d steps backwards\n", current_player->name, dice);
  }
}
void show_my_properties(int owned_blocks[MAX_OWNED], block_node* head_block)
{
  int i;
  block_node* current_block;
  for(i=0; i<MAX_OWNED && owned_blocks[i]!=EMPTY;i++)
  {
    current_block = find_block_by_index(head_block, owned_blocks[i]);
    printf("%d - %*s%s (Selling price: %d)\n", owned_blocks[i], 5-find_num_of_digits(owned_blocks[i]), "", current_block->name, current_block->price/2 + current_block->house_count*(current_block->house_price/2));
  }
  if(owned_blocks[0]==EMPTY) printf("You don't own any properties\n");
}

void sell_property(block_node* head_block, player* current_player)
{
  int user_choice;
  player bank;
  bank.type = noone;
  block_node* current_block;
  if(current_player->owned_block_ids[0]!=EMPTY)
  {
    do
    {
      show_my_properties(current_player->owned_block_ids, head_block);
      printf("Your choice(0 to exit): ");
      scanf("%d", &user_choice);
      if(!is_not_choice(current_player->owned_block_ids, MAX_OWNED, user_choice))/*not is not choice meaning it is a possible choice*/
      {
        current_block = find_block_by_index(head_block, user_choice);
        current_player->account += current_block->price/2 + current_block->house_count *(current_block->house_price/2);
        remove_property(current_player->owned_block_ids, MAX_OWNED, user_choice);
        current_block->house_count=0;/*not sure if I should remove the houses built after selling, ask hoca*/
        current_block->owner=bank;
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
void buy_house(block_node* current_block,player* current_player)
{
  int user_choice;
  int max_choice;
    if(current_block->owner.type == current_player->type)
    {
      if(current_player->account > current_block->house_price)
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
            else if(current_player->account<=user_choice*(current_block->house_price)) printf("Your funds are not sufficient for building this many houses. Please try again\n");
            else if(user_choice<=0) printf("Please enter a positive valid number\n");
          }while(user_choice>max_choice || current_player->account<=user_choice*(current_block->house_price) || user_choice<=0);
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
void buy_property(block_node* current_block, player* current_player)
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
          if(current_player->account > current_block->price)
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
void show_properties(block_node* head_block)
{
  int i,j;
  int longest = find_longest(head_block);
  int user_choice;
  block_node* current_block;
  int possible_choices[BOARD_SIZE];
  for(i=0,j=0;i<BOARD_SIZE;i++)/*finding possible choices for user*/
  {
    current_block = find_block_by_index(head_block, i);
    if(current_block->type==property)
    {
      possible_choices[j] = current_block->block_id;
      j++;
    }
  }
  for(; j<BOARD_SIZE; j++) possible_choices[j]=EMPTY;/*putting a terminating condition, at least one EMPTY will be present due to the start field which has to be present in every game no matter the number of blocks*/
  do{
      printf("Please select a property to see details:\n");
      for(i=0; i<BOARD_SIZE;i++)
      {
        current_block = find_block_by_index(head_block, i);
        if(current_block->type==property) printf("%d - %*s%s\n", current_block->block_id, longest/2-find_num_of_digits(current_block->block_id), "", current_block->name);
      }
      printf("0 - %*s%s\n", longest/2-1, "", "Exit");/*-1 since zero is a single digit num*/
      scanf("%d", &user_choice);
      if(user_choice!= 0 && is_not_choice(possible_choices, BOARD_SIZE, user_choice)) printf("Please enter a valid choice\n");
      else if(user_choice!=0) show_chosen_property(*find_block_by_index(head_block, user_choice), longest, user_choice);
      else printf("Returning to main menu\n");
    }while(is_not_choice(possible_choices, BOARD_SIZE, user_choice) && user_choice!=0);
}
void show_chosen_property(block_node chosen_block, int longest_word, int user_choice)
{
  printf("\n\n");
  int i;
  /*----*/
  if(longest_word%2==0) for(i=0; i<=longest_word*3+1; i++) printf("-");
  else{
  for(i=0; i<=longest_word*3; i++) printf("-");
  }
  printf("\n");
  /*Name of the neighbourhood*/
  printf("|%*s%*s|", longest_word+longest_word/2+(int)strlen(chosen_block.name)/2, chosen_block.name, longest_word+longest_word/2-(int)strlen(chosen_block.name)/2,"");
  printf("\n");
  /*----*/
  if(longest_word%2==0) for(i=0; i<=longest_word*3+1; i++) printf("-");
  else {
  for(i=0; i<=longest_word*3; i++) printf("-");
  }
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
  if(longest_word%2==0) for(i=0; i<=longest_word*3+1; i++) printf("-");
  else{
  for(i=0; i<=longest_word*3; i++) printf("-");
  }
  printf("\n");
  /*House price*/
  printf("|%*s%*s", (longest_word+longest_word/2)/2 + (int)strlen("House Price")/2, "House Price", (longest_word+longest_word/2)/2 - (int)strlen("House Price")/2,"");
  printf("%*d$%*s|", (longest_word+longest_word/2)/2 + find_num_of_digits(chosen_block.house_price)/2, chosen_block.house_price, (longest_word+longest_word/2)/2 - find_num_of_digits(chosen_block.house_price)/2-1,"");/*-1 in the last one because of the dollar caracter*/
  printf("\n");
  /*----*/
  if(longest_word%2==0) for(i=0; i<=longest_word*3+1; i++) printf("-");
  else{
  for(i=0; i<=longest_word*3; i++) printf("-");
  }
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
void init_players(player* player_one, player* player_two, int against_computer)
{
  int i;
  for(i=0;i<MAX_OWNED; i++)
  {
    player_one->owned_block_ids[i]=EMPTY;
    player_two->owned_block_ids[i]=EMPTY;
  }
  player_one->name = "Car";
  if(against_computer)player_two->name = "Computer";
  else player_two->name = "Cap";
  player_one->type = car;
  player_two->type = cap;
  player_one->account=100000;
  player_two->account=100000;
  player_one->current_block_id = 0;
  player_two->current_block_id = 0;
  player_one->turn_to_wait = 0;
  player_two->turn_to_wait = 0;
}
block_node* find_block_by_index(block_node* head_block, int j)
{
  int i=0;
  while(i<j)
  {
    head_block=head_block->next_block;
    i++;
  }
  return head_block;
}
void show_board(block_node* head_block, player player_one, player player_two)
{
  int longest = find_longest(head_block);
  int i,j,k,h,f;
  int num_of_info=3;
  int horizontal_display=7;
  if(longest%2!=0)longest++;/*to be able to make proper identation if printing more than one word in the same block(two players at the same block)*/
  block_node* current_block;/*used for printing beacuse i don't want to modify head_block since it shows where the begning of the linked list is*/
  /*PRINTING THE FIRST ROW, 6 BLOCKS*/
  for(i=0;i<longest*2*horizontal_display+horizontal_display+1;i++) printf("-");/*printing the border*/
  printf("\n");
  {
    for(i=0;i<num_of_info;i++)/*Number 3 is representing maximum number of things being displayed in each block*/
    {
      for(j=0;j<horizontal_display;j++)
      {
        current_block = find_block_by_index(head_block, j);
        if(j==0) printf("|");
        switch(i)
        {
          case 0:
            printf("%*s%*s|", longest+(int)strlen(current_block->name)/2,current_block->name,longest-(int)strlen(current_block->name)/2,"");

            break;
          case 1:
            if(current_block->type == property) printf("%*d$%*s|", longest + find_num_of_digits(current_block->price)/2,current_block->price, longest-find_num_of_digits(current_block->price)/2-1,"");
            else if(current_block->type == tax)  printf("%*d$%*s|", longest + find_num_of_digits(current_block->rent)/2,current_block->rent, longest-find_num_of_digits(current_block->rent)/2-1,"");
            else printf("%*s|", longest*2,"");

            break;
          case 2:/*Current player should be here*/
            if(current_block->block_id!=player_one.current_block_id && current_block->block_id!=player_two.current_block_id) printf("%*s|", longest*2,"");
            else/*if one or more players are present on the block*/
            {
              if(current_block->block_id==player_one.current_block_id && current_block->block_id!=player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_one.name)/2, player_one.name,longest-(int)strlen(player_one.name)/2,"");
                else if(current_block->block_id!=player_one.current_block_id && current_block->block_id==player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_two.name)/2, player_two.name,longest-(int)strlen(player_two.name)/2,"");
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
          current_block = find_block_by_index(head_block, BOARD_SIZE-h);
          if(j==0 || j==horizontal_display-1) printf("|");
          if(j==0 || j==horizontal_display-1)
          {
            if(j==0)
            {
              switch(i)
              {
                case 0:
                  printf("%*s%*s|", longest+(int)strlen(current_block->name)/2,current_block->name,longest-(int)strlen(current_block->name)/2,"");
                  break;
                case 1:
                  if(find_num_of_digits(current_block->price!=0)) printf("%*d$%*s|", longest + find_num_of_digits(current_block->price)/2,current_block->price, longest-find_num_of_digits(current_block->price)/2-1,"");
                  else if(current_block->type == tax)  printf("%*d$%*s|", longest + find_num_of_digits(current_block->rent)/2,current_block->rent, longest-find_num_of_digits(current_block->rent)/2-1,"");
                  else printf("%*s|", longest*2,"");

                  break;
                case 2:/*Current player should be here*/
                  if(current_block->block_id!=player_one.current_block_id && current_block->block_id!=player_two.current_block_id) printf("%*s|", longest*2,"");
                  else/*if one or more players are present on the block*/
                  {
                    if(current_block->block_id==player_one.current_block_id && current_block->block_id!=player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_one.name)/2, player_one.name,longest-(int)strlen(player_one.name)/2,"");
                      else if(current_block->block_id!=player_one.current_block_id && current_block->block_id==player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_two.name)/2, player_two.name,longest-(int)strlen(player_two.name)/2,"");
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
              current_block = find_block_by_index(head_block, k);
              switch(i)
              {
                case 0:
                  printf("%*s%*s|", longest+(int)strlen(current_block->name)/2,current_block->name,longest-(int)strlen(current_block->name)/2,"");
                  break;
                case 1:
                  if(find_num_of_digits(current_block->price!=0)) printf("%*d$%*s|", longest + find_num_of_digits(current_block->price)/2,current_block->price, longest-find_num_of_digits(current_block->price)/2-1,"");
                  else if(current_block->type == tax)  printf("%*d$%*s|", longest + find_num_of_digits(current_block->rent)/2,current_block->rent, longest-find_num_of_digits(current_block->rent)/2-1,"");
                  else printf("%*s|", longest*2,"");

                  break;
                case 2:/*Current player should be here*/
                  if(current_block->block_id!=player_one.current_block_id && current_block->block_id!=player_two.current_block_id) printf("%*s|", longest*2,"");
                  else/*if one or more players are present on the block*/
                  {
                    if(current_block->block_id==player_one.current_block_id && current_block->block_id!=player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_one.name)/2, player_one.name,longest-(int)strlen(player_one.name)/2,"");
                      else if(current_block->block_id!=player_one.current_block_id && current_block->block_id==player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_two.name)/2, player_two.name,longest-(int)strlen(player_two.name)/2,"");
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
          current_block = find_block_by_index(head_block, k);
          if(j==0) printf("|");
          switch(i)
          {
            case 0:
              printf("%*s%*s|", longest+(int)strlen(current_block->name)/2,current_block->name,longest-(int)strlen(current_block->name)/2,"");

              break;
            case 1:
              if(find_num_of_digits(current_block->price!=0)) printf("%*d$%*s|", longest + find_num_of_digits(current_block->price)/2,current_block->price, longest-find_num_of_digits(current_block->price)/2-1,"");
              else if(current_block->type == tax)  printf("%*d$%*s|", longest + find_num_of_digits(current_block->rent)/2,current_block->rent, longest-find_num_of_digits(current_block->rent)/2-1,"");

              else printf("%*s|", longest*2,"");

              break;
            case 2:/*Current player should be here*/
              if(current_block->block_id!=player_one.current_block_id && current_block->block_id!=player_two.current_block_id) printf("%*s|", longest*2,"");
              else/*if one or more players are present on the block*/
              {
                if(current_block->block_id==player_one.current_block_id && current_block->block_id!=player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_one.name)/2, player_one.name,longest-(int)strlen(player_one.name)/2,"");
                  else if(current_block->block_id!=player_one.current_block_id && current_block->block_id==player_two.current_block_id) printf("%*s%*s|", longest+(int)strlen(player_two.name)/2, player_two.name,longest-(int)strlen(player_two.name)/2,"");
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
int find_longest(block_node* head_block)/*return lenght of the longest board name*/
{
  int longest = strlen(head_block->name);
  while(head_block!=NULL)
  {
    if(longest<strlen(head_block->name)) longest = strlen(head_block->name);
    head_block = head_block->next_block;
  }
  return longest;
}

void init_the_board(block_node* head_node)
{
  player bank;
  bank.type = noone;

  block_node* current_node = head_node;
  block_node* next_node;

  current_node->block_id=0;
  current_node->name = "Start";
  current_node->price=0;
  current_node->rent=0;
  current_node->rent_1=0;
  current_node->rent_2=0;
  current_node->rent_3=0;
  current_node->house_price=0;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=start;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=1;
  current_node->name = "Esenyurt";
  current_node->price=16000;
  current_node->rent=800;
  current_node->rent_1=4000;
  current_node->rent_2=9000;
  current_node->rent_3=25000;
  current_node->house_price=10000;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=property;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=2;
  current_node->name = "Car Park";
  current_node->price=0;
  current_node->rent=1500;
  current_node->rent_1=0;
  current_node->rent_2=0;
  current_node->rent_3=0;
  current_node->house_price=0;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=tax;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=3;
  current_node->name = "Fortune Card";
  current_node->price=0;
  current_node->rent=0;
  current_node->rent_1=0;
  current_node->rent_2=0;
  current_node->rent_3=0;
  current_node->house_price=0;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=fortune;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=4;
  current_node->name = "Tuzla";
  current_node->price=16500;
  current_node->rent=850;
  current_node->rent_1=4250;
  current_node->rent_2=9500;
  current_node->rent_3=26000;
  current_node->house_price=12000;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=property;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=5;
  current_node->name = "Arnavutkoy";
  current_node->price=17000;
  current_node->rent=875;
  current_node->rent_1=4500;
  current_node->rent_2=10000;
  current_node->rent_3=28000;
  current_node->house_price=12000;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=property;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=6;
  current_node->name = "Wait 2 Turn";
  current_node->price=0;
  current_node->rent=2;
  current_node->rent_1=0;
  current_node->rent_2=0;
  current_node->rent_3=0;
  current_node->house_price=0;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=punish;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=7;
  current_node->name = "Catalca";
  current_node->price=20000;
  current_node->rent=1000;
  current_node->rent_1=5000;
  current_node->rent_2=12000;
  current_node->rent_3=30000;
  current_node->house_price=13000;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=property;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=8;
  current_node->name = "Beykoz";
  current_node->price=23000;
  current_node->rent=1100;
  current_node->rent_1=5500;
  current_node->rent_2=12500;
  current_node->rent_3=33000;
  current_node->house_price=13000;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=property;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=9;
  current_node->name = "Fortune Card";
  current_node->price=0;
  current_node->rent=0;
  current_node->rent_1=0;
  current_node->rent_2=0;
  current_node->rent_3=0;
  current_node->house_price=0;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=fortune;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=10;
  current_node->name = "Car Fix";
  current_node->price=0;
  current_node->rent=1750;
  current_node->rent_1=0;
  current_node->rent_2=0;
  current_node->rent_3=0;
  current_node->house_price=0;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=tax;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=11;
  current_node->name = "Maltepe";
  current_node->price=30000;
  current_node->rent=1350;
  current_node->rent_1=7000;
  current_node->rent_2=15000;
  current_node->rent_3=40000;
  current_node->house_price=15000;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=property;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=12;
  current_node->name = "Bills";
  current_node->price=0;
  current_node->rent=2000;
  current_node->rent_1=0;
  current_node->rent_2=0;
  current_node->rent_3=0;
  current_node->house_price=0;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=tax;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=13;
  current_node->name = "Sisli";
  current_node->price=33000;
  current_node->rent=1500;
  current_node->rent_1=8000;
  current_node->rent_2=16000;
  current_node->rent_3=42000;
  current_node->house_price=16000;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=property;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=14;
  current_node->name = "Oil";
  current_node->price=0;
  current_node->rent=2250;
  current_node->rent_1=0;
  current_node->rent_2=0;
  current_node->rent_3=0;
  current_node->house_price=0;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=tax;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=15;
  current_node->name = "Fortune Card";
  current_node->price=0;
  current_node->rent=0;
  current_node->rent_1=0;
  current_node->rent_2=0;
  current_node->rent_3=0;
  current_node->house_price=0;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=fortune;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=16;
  current_node->name = "Atasehir";
  current_node->price=35000;
  current_node->rent=1600;
  current_node->rent_1=8500;
  current_node->rent_2=17000;
  current_node->rent_3=45000;
  current_node->house_price=17000;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=property;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=17;
  current_node->name = "Sariyer";
  current_node->price=40000;
  current_node->rent=1750;
  current_node->rent_1=9500;
  current_node->rent_2=19000;
  current_node->rent_3=48000;
  current_node->house_price=19000;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=property;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=18;
  current_node->name = "Wait 1 Turn";
  current_node->price=0;
  current_node->rent=1;
  current_node->rent_1=0;
  current_node->rent_2=0;
  current_node->rent_3=0;
  current_node->house_price=0;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=punish;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=19;
  current_node->name = "Kadikoy";
  current_node->price=43000;
  current_node->rent=1900;
  current_node->rent_1=11000;
  current_node->rent_2=21500;
  current_node->rent_3=55000;
  current_node->house_price=23000;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=property;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=20;
  current_node->name = "Besiktas";
  current_node->price=60000;
  current_node->rent=2500;
  current_node->rent_1=15000;
  current_node->rent_2=28000;
  current_node->rent_3=60000;
  current_node->house_price=30000;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=property;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;


  current_node->block_id=21;
  current_node->name = "Fortune Card";
  current_node->price=0;
  current_node->rent=0;
  current_node->rent_1=0;
  current_node->rent_2=0;
  current_node->rent_3=0;
  current_node->house_price=0;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=fortune;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=22;
  current_node->name = "Vacation";
  current_node->price=0;
  current_node->rent=5000;
  current_node->rent_1=0;
  current_node->rent_2=0;
  current_node->rent_3=0;
  current_node->house_price=0;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=tax;
  next_node = (block_node*)malloc(sizeof(block_node));
  current_node->next_block = next_node;
  current_node = next_node;

  current_node->block_id=23;
  current_node->name = "Bebek";
  current_node->price=70000;
  current_node->rent=3500;
  current_node->rent_1=20000;
  current_node->rent_2=35500;
  current_node->rent_3=65000;
  current_node->house_price=35000;
  current_node->owner=bank;
  current_node->house_count=0;
  current_node->type=property;
  current_node->next_block = NULL;/*last linked list's element*/

}
