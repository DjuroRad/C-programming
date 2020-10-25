#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct person{
int age;
char* name;
char* surname;
char* creation;/*Creation is musical work*/
struct person *next;
} *top;
void addNode( char name[], char surname [], char creation [], int age);
void print_person(top element);
void print_stack();
void deleteNode();
void popNode();
void Sort_Alphabetically();
void Sort_Increasingly();
int num_of_elem();
void menu();
void display_menu();
void get_string(char** string);
top head;
int main()
{
  menu();
}
void menu()
{
  int user_choice;
  char junk;
  char* name;
  char* surname;
  char* creation;
  int age;
  do{
    display_menu();
    scanf("%d", &user_choice);
    junk = getchar();/*geting rid of the new line*/
    switch (user_choice) {
      case 1:
      printf("Name:"); get_string(&name);
      printf("Surname:"); get_string(&surname);
      printf("Creation:"); get_string(&creation);
      printf("Age:"); scanf("%d", &age); junk = getchar();
      printf("-----------------\n");
        addNode(name, surname, creation, age);
        print_stack();
        break;
      case 2:
        deleteNode();
        print_stack();
        break;
      case 3:
        Sort_Alphabetically();
        print_stack();
        break;
      case 4:
        Sort_Increasingly();
        print_stack();
        break;
      case 5:
        printf("Exiting....\n");
        break;
      default:
        printf("Please enter a valid choice\n");
    }
  }while(user_choice!=5);
}
void get_string(char** string)/*getting a string dynamically since I don't know how long user wants it*/
{
  *string = (char*)malloc(sizeof(char));/*initializing to be able to use realloc*/
  int size=1;
  char ch_in;
  do{
    ch_in = getchar();
    if(ch_in!='\n')/*getting everything user typed until he presses enter*/
    {
      *string = (char*)realloc(*string, (size+1)*sizeof(char));/*+1 for 0 terminating caracter*/
      (*string)[size-1] = ch_in;
      (*string)[size] = '\0';
      size++;
    }
  }while(ch_in!='\n');
}
void display_menu()
{
  int i;
  printf("****MENU****\n");
  printf("%*s1- Add a Person to the Stack\n", 4," ");
  printf("%*s2- Pop a Person from the Stack\n",4," ");
  printf("%*s3- Sort Alphabetical Order\n",4," ");
  printf("%*s4- Sort Age in Increasing Order\n",4," ");
  printf("%*s5- Exit menu\n",4," ");
  for(i=0;i<4*2 + (int)strlen("MENU");i++)printf("*");
  printf("\n");
  printf("Select your choice: ");
}
void Sort_Increasingly()
{
  int i,j;
  int n = num_of_elem();/*finding the num of queue elements to be able to make an array of that size*/
  top temp = head;
  struct person temp_elem;
  struct person stack_array[n];
  for(i=0; i<n; i++, temp = temp->next)/*filling in the array with stack elements*/
  {/**/
    stack_array[i] = *temp;
  }
  for(i=0;i<n-1;i++)/*sorting an array with the most basic sorting algorithm*/
  {
    for(j=i+1; j<n;j++)
    {
      if(stack_array[i].age>stack_array[j].age)
      {
        temp_elem = stack_array[i];
        stack_array[i] = stack_array[j];
        stack_array[j] = temp_elem;
      }
    }
  }
  while(head!=NULL)/*emptying the stack, but only the structure, not deallocating the dynamic string contained within*/
  {
    popNode(head);
  }
  for(i=n-1; i>=0; i--)/*making the stack from the now sorted array*/
  {
    addNode(stack_array[i].name, stack_array[i].surname, stack_array[i].creation, stack_array[i].age);
  }
}
void Sort_Alphabetically()
{
  int i,j;
  int n = num_of_elem();/*finding the num of queue elements to be able to make an array of that size*/
  top temp = head;
  struct person temp_elem;
  struct person stack_array[n];
  for(i=0; i<n; i++, temp = temp->next)/*filling in the array with stack elements*/
  {/**/
    stack_array[i] = *temp;
  }
  for(i=0;i<n-1;i++)
  {
    for(j=i+1; j<n;j++)
    {
      if(strcmp(stack_array[i].name,stack_array[j].name)>0)/*sorting the stack*/
      {
        temp_elem = stack_array[i];
        stack_array[i] = stack_array[j];
        stack_array[j] = temp_elem;
      }
    }
  }
  while(head!=NULL)/*emptying the stack, but only the structure, not deallocating the dynamic string contained within*/
  {
    popNode(head);
  }
  for(i=n-1; i>=0; i--)/*making the stack from the now sorted array*/
  {
    addNode(stack_array[i].name, stack_array[i].surname, stack_array[i].creation, stack_array[i].age);
  }
}
int num_of_elem()/*gets the number of elements in a stack*/
{
  top temp;
  int res = 0;
  for(temp = head; temp!=NULL; temp = temp->next) res ++;
  return res;
}
void addNode(char name[], char surname [], char creation [], int age)
{/*dynamically alllocating the stack's element*/
  top new_node=(top)malloc(sizeof(struct person));
  new_node->name = (char*)calloc(strlen(name)+1, sizeof(char));
  strcpy(new_node->name, name);
  new_node->surname = (char*)calloc(strlen(surname)+1, sizeof(char));
  strcpy(new_node->surname, surname);
  new_node->creation = (char*)calloc(strlen(creation)+1, sizeof(char));
  strcpy(new_node->creation, creation);
  new_node->age = age;
  if(head!=NULL)  new_node->next = head;
  else new_node->next = NULL;
  head = new_node;
}
void deleteNode()
{
  if(head!=NULL)
  {
    top temp = head->next;
    free(head->name);/*freeing manually since complier wouldnt do it on its own of course, it would just empty out the struct's address*/
    free(head->surname);/*freeing manually since complier wouldnt do it on its own of course, it would just empty out the struct's address*/
    free(head->creation);/*freeing manually since complier wouldnt do it on its own of course, it would just empty out the struct's address*/
    free(head);
    head = temp;
  }
  if(head==NULL)printf("Stack is empty\n");
}
void popNode()/*doesn't deallocate name surname and creating, used for temorary emptying when putting in an array*/
{/*this function is only used when array is being reconstructed*/
  if(head!=NULL)
  {
    top temp = head->next;
    free(head);
    head = temp;
  }
}
void print_stack()
{
  top temp = head;
  int i=1;
  while(temp!=NULL)/*it will print order 1) 2) even after sorting, no response on the mail I sent so I considered that a mistake in the pdf otherwise it would be the same function just not using this counter i*/
  {
    printf("%d)",i++);
    print_person(temp);
    temp = temp->next;
  }
  return;
}
void print_person(top element)
{
  printf("%s\n", element->name);
  printf("%s\n", element->surname);
  printf("%s\n", element->creation);
  printf("%d\n", element->age);
}
