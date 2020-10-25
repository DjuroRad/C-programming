#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 20
typedef struct node{
  int data;
  struct node* next;
}node;
typedef struct queue{
  struct node* head;
  struct node* tail;
  int size;
}queue;
typedef struct stack{
  struct node* top;
  int size;
}stack;
typedef struct bst{
  int data;
  struct bst* left;
  struct bst* right;
}bst;
void fill_structures(stack ** stack_, queue ** queue_, bst ** bst_, int data[MAX]);
bst* insert_tree(bst* tree, int data);
void insert_node_stack(stack* my_stack, int data);
void insert_node_queue(queue* queue_, int data);
void del_q(queue* queue_);
void del_s(stack* stack_);
bst* del_t(bst* tree,int to_del);
bst* find_max_subtree(bst* tree);
bst* find_min_subtree(bst* tree);
void init_queue(queue** queue_);
void init_tree(bst** tree);
void init_stack(stack** stack_);
void ordered_print(stack * stack_, queue * queue_, bst * bst_);
void special_traverse(stack * stack_, queue * queue_, bst * bst_);
void special_traverse_t(bst* bst_);
void special_traverse_q(queue* queue_);
void special_traverse_s(stack* stack_);
void special_traverse_help_qs(node* head, node* tail, int size, int i);
void copy_tree(bst* tree_source, bst** tree_destination);
void ordered_print(stack * stack_, queue * queue_, bst * bst_);
void ordered_print_tree(bst* tree);
queue* sort_queue(queue* queue_);
void print_stack(stack* stack_);
void print_queue(queue* queue_);
stack* sort_stack(stack* stack_);
void remove_nodes_sq(node *head);
void remove_queue(queue** queue_);
void remove_stack(stack** stack_);
int find_next(queue* queue_, int largest);
void search(stack * stack_, queue * queue_, bst * bst_, int val_to_search);
void search_t(bst* tree, int val_to_search);
void search_t_help(bst* tree, int* i,int* num_of_occ, queue* steps, int val_to_search);
void search_q(queue* queue_, int val_to_search);
void search_s(stack* stack_, int val_to_search);
void print_steps(int num_of_occ, queue* steps);
void remove_tree(bst* tree);
/*Note that every data structure is working separately, meaning I am not combining them together. Sorting stacks is sorted using only stacks, queues only queues. Special traverse is done in the scanf("
in the same manner. Also original queue and stack are never changed meaning sorting for them is done separately(2 times, once for sorted printing the other for special traverse)*/
/*I was thinking about using double linked list as a structure but since we didn't really do it but just mentioned it during the class i decided to go with standard linked list, otherwise they would be more appropriate for special traverse*/
int main()
{
  int data[20]={5, 2, 7, 8, 11, 3, 21, 7, 6, 15, 19, 35, 24, 1, 8, 12, 17,
  8, 23, 4};
  bst * bst_;
  queue * queue_;
  stack * stack_;
  fill_structures(&stack_, &queue_, &bst_, data);
  ordered_print(stack_, queue_, bst_);
  search(stack_, queue_, bst_, 8);
  special_traverse(stack_, queue_, bst_);
  return 0;
}
stack* sort_stack(stack* stack_)
{
  stack* stack1, *stack2;
  node* curr_node;
  int data;
  init_stack(&stack1);
  init_stack(&stack2);
  for(curr_node = stack_->top; curr_node!=NULL; curr_node=curr_node->next) insert_node_stack(stack1, curr_node->data);
  insert_node_stack(stack2,stack1->top->data);
  del_s(stack1);/*60 and 61 -> starting with the first element as an assumption of the max element*/
  while(stack1->top != NULL)
  {
    data = stack1->top->data;
    del_s(stack1);
    if(data<stack2->top->data)
    {
      while(data<stack2->top->data)
      {
        insert_node_stack(stack1, stack2->top->data);
        del_s(stack2);
        if(stack2->top == NULL) break;
      }
    }
    insert_node_stack(stack2, data);
  }
  remove_stack(&stack1);
  return stack2;
}
void print_stack(stack* stack_)
{
  node* head;
  for(head = stack_->top; head!=NULL; head = head->next) printf("%d ", head->data);
}
void remove_stack(stack** stack_)/*reomves the whole previously declared stack*/
{
  node* head=(*stack_)->top;
  if(head!=NULL) remove_nodes_sq(head);
  free(*stack_);
}
void remove_queue(queue** queue_)/*reomves the whole previously declared queue*/
{
  node* head = (*queue_)->head;
  if(head != NULL) remove_nodes_sq(head);
  free(*queue_);
}
void remove_nodes_sq(node *head)/*reomves nodes from stacks and from queue*/
{
  if(head->next != NULL) remove_nodes_sq(head->next);
  free(head);
}
void del_s(stack* stack_)/*deletes only one node from the stack*/
{
  if(stack_->top != NULL)
  {
    node* to_free = stack_->top;
    stack_->top = stack_->top->next;
    free(to_free);
    (stack_->size)--;
  }
  else printf("Stack is empty");
}
void del_q(queue* queue_)/*deletes only one node from the queue*/
{
  if(queue_->size != 0)
  {
    node* to_free = queue_->head;
    queue_->head = queue_->head->next;
    free(to_free);
    if(queue_->size == 1) queue_->tail = queue_->head;
    queue_->size --;
  }
  else printf("Queue is empty\n");
}
bst* find_max_subtree(bst* tree)/*finds biggest element of a tree and returns tree it is in*/
{
  bst* max;
  if(tree->right == NULL) max = tree;
  else max = find_max_subtree(tree->right);
  return max;
}
bst* find_min_subtree(bst* tree)/*finds smallest element of a tree and returns tree it is in*/
{
  bst* min;
  if(tree->left == NULL) min = tree;
  else min = find_min_subtree(tree->left);
  return min;
}
bst* del_t(bst* tree, int to_del)/*delets one element of a given tree*/
{
  if(tree != NULL)
  {
    if(tree->data == to_del)
    {
      bst* to_free;
      if(tree->right == NULL && tree->left == NULL)
      {
        free(tree);
        tree = NULL;
      }
      else if(tree->right != NULL && tree->left == NULL)
      {
        to_free = tree;
        tree = tree->right;
        free(to_free);
      }
      else if(tree->right == NULL && tree->left != NULL)
      {
        to_free = tree;
        tree = tree->left;
        free(to_free);
      }
      else
      {
        bst* max_sub_tree = find_max_subtree(tree->left);
        tree->data = max_sub_tree->data;
        tree->left = del_t(tree->left, max_sub_tree->data);/*will just proceed to delete this duplicate*/
      }
    }
    else
    {
      if(to_del < tree->data) tree->left = del_t(tree->left, to_del);
      else tree->right = del_t(tree->right, to_del);
    }
  }
  return tree;
}
queue* sort_queue(queue* queue_)/*sorts a queue*/
{
  queue *queue1, *queue2;
  node* curr_node, *max_node;
  init_queue(&queue1);
  init_queue(&queue2);
  /*queue1 used for copying original queue since I don not want to change the original one, queue2 used for sorting*/
  for(curr_node = queue_->head; curr_node!=NULL; curr_node = curr_node->next) insert_node_queue(queue1, curr_node->data);
  if(queue_->head != NULL)/*just making sure it has some elements inside*/
  {
    while(queue1->head->next!=NULL)/*finding min elements, putting it as the first one, than removing it from the front until i totally empty out this queue*/
    {/*queue1->next!=null since i am taking current node to be the next of it*/
      max_node = queue1->head;
      for(curr_node = queue1->head->next; curr_node!=NULL; curr_node = curr_node->next)
      {
        if(curr_node->data>max_node->data)/*moving max value to be in the front*/
        {
          while(max_node->next!=curr_node->next)
          {
            insert_node_queue(queue1, max_node->data);
            del_q(queue1);
            max_node = max_node->next;
          }
        }
      }
      insert_node_queue(queue2, max_node->data);/*putting critical value found into the queue that will be sorted*/
      del_q(queue1);/*getting rid of the min value in current queue of size SIZE and than finding next min value of queue of size SIZE-1*/
    }
    insert_node_queue(queue2, queue1->head->data);/*putting the last element from the queue*/
    del_q(queue1);
  }else printf("Queue is empty\n");
  remove_queue(&queue1);
  return queue2;
}
void print_queue(queue* queue_)
{
  node* head;
  for(head = queue_->head; head!=NULL; head = head->next) printf("%d ",head->data);
}
void ordered_print_tree(bst* tree)
{
  if(tree!=NULL)
  {
  ordered_print_tree(tree->right);
  printf("%d ",tree->data);
  ordered_print_tree(tree->left);
  }
  else return;
}
void init_queue(queue** queue_)
{
  *queue_ = (queue*)malloc(sizeof(queue));
  (*queue_)->head = (*queue_)->tail = NULL;
}
void init_tree(bst** tree)
{
  *tree = NULL;
}
void init_stack(stack** stack_)
{
  *stack_ = (stack*)malloc(sizeof(stack));
  (*stack_)->top = NULL;
  (*stack_)->size = 0;
}
void insert_node_queue(queue* queue_, int data)
{
  node* new_node = (node*)malloc(sizeof(node));
  new_node->data = data;
  new_node->next = NULL;
  if(queue_->tail!=NULL) queue_->tail->next = new_node;
  queue_->tail = new_node;
  if(queue_->head == NULL) queue_->head = new_node;
  queue_->size ++;
}
void insert_node_stack(stack* my_stack, int data)
{
  node* new_node = (node*)malloc(sizeof(node));
  new_node->data = data;
  new_node->next = my_stack->top;
  my_stack->top = new_node;
  (my_stack->size) ++;
}
bst* insert_tree(bst* tree, int data)
{
  if(tree==NULL)
  {
    tree = (bst*)malloc(sizeof(bst));
    tree->data = data;
    tree->left = tree->right = NULL;
  }
  else if(tree->data > data) tree->left = insert_tree(tree->left, data);
  else tree->right = insert_tree(tree->right,data);
  return tree;
}
void ordered_print(stack * stack_, queue * queue_, bst * bst_)
{
  printf("\nDescending order\n");
  clock_t start_execution, end_execution;
  queue* sorted_queue;/*these two new queues and stacks are created and than removed in the end because i don't want to change the original ones so that i would give real world results for the following function that also need to execute sorting in order to work*/
  stack* sorted_stack;
  double stack_time, queue_time, tree_time;
  start_execution=clock();
  sorted_stack = sort_stack(stack_);
  printf("%-10s","Stack: ");
  print_stack(sorted_stack);
  printf("\n");
  end_execution=clock();
  stack_time = ((double)(end_execution-start_execution)/CLOCKS_PER_SEC)*1000;

  start_execution=clock();
  sorted_queue = sort_queue(queue_);
  printf("%-10s","Queue: ");
  print_queue(sorted_queue);
  printf("\n");
  end_execution=clock();
  queue_time = ((double)(end_execution-start_execution)/CLOCKS_PER_SEC)*1000;

  start_execution=clock();
  printf("%-10s","Tree: ");
  ordered_print_tree(bst_);
  printf("\n");
  end_execution=clock();
  tree_time = ((double)(end_execution-start_execution)/CLOCKS_PER_SEC)*1000;

  printf("%-20s%-20s%-20s%-20s\n", "Structures", "Stack", "Queue", "BST");
  printf("%-20s%-20.3lf%-20.3lf%-20.3lf\n", "Exec. time(ms)", stack_time, queue_time, tree_time);
  /*avoiding zombie memory allocation*/
  remove_stack(&sorted_stack);
  remove_queue(&sorted_queue);
}
void special_traverse(stack * stack_, queue * queue_, bst * bst_)
{
  printf("\nSpecial traverse\n");
  clock_t start_execution, end_execution;
  queue* sorted_queue;
  stack* sorted_stack;
  double stack_time, queue_time, tree_time;

  start_execution=clock();
  sorted_stack = sort_stack(stack_);
  printf("%-10s", "Stack: ");
  special_traverse_s(sorted_stack);
  printf("\n");
  end_execution = clock();
  stack_time = ((double)(end_execution-start_execution)/CLOCKS_PER_SEC)*1000;

  start_execution=clock();
  sorted_queue = sort_queue(queue_);
  printf("%-10s", "Queue: ");
  special_traverse_q(sorted_queue);
  printf("\n");
  end_execution = clock();
  queue_time = ((double)(end_execution-start_execution)/CLOCKS_PER_SEC)*1000;

  start_execution=clock();
  printf("%-10s","Tree: ");
  special_traverse_t(bst_);
  printf("\n");
  end_execution = clock();
  tree_time = ((double)(end_execution-start_execution)/CLOCKS_PER_SEC)*1000;

  printf("%-20s%-20s%-20s%-20s\n", "Structures", "Stack", "Queue", "BST");
  printf("%-20s%-20.3lf%-20.3lf%-20.3lf\n", "Exec. time(ms)", stack_time, queue_time, tree_time);

  remove_stack(&sorted_stack);
  remove_queue(&sorted_queue);
}
void special_traverse_t(bst* bst_)/*this function will find min and max elements of a binary tree and than remove them so that it could find next min and max element*/
{
  bst* bst_new=NULL;
  bst *min, *max;
  copy_tree(bst_, &bst_new);
  if(bst_new != NULL)
  {
    do{
      max = find_max_subtree(bst_new);
      min = find_min_subtree(bst_new);
      if(max!=min)printf("%d %d ", max->data, min->data);/*if statement so that it works when there is an odd number of elements*/
      else printf("%d", max->data);
      bst_new = del_t(bst_new, min->data);
      bst_new = del_t(bst_new, max->data);
    } while(bst_new!=NULL);
  }
  remove_tree(bst_new);
}
void remove_tree(bst* tree)/*removes the whole binary tree*/
{
  if(tree!=NULL)
  {
    remove_tree(tree->left);
    remove_tree(tree->right);
    free(tree);
  }
}
void special_traverse_q(queue* queue_)
{/*data structure provided needs to be sorted for this algorithm to work*/
  int i;
  node* tail=queue_->head;
  for(i=0; i<queue_->size/2;i++) tail = tail->next;/*setting the tail to be exactly to the half so that i could recursively go till the end of the queue and print until the half*/
  if(queue_->size % 2 != 0) special_traverse_help_qs(queue_->head, tail, queue_->size/2, 0);
  else special_traverse_help_qs(queue_->head, tail, queue_->size/2, 1);
}
void special_traverse_s(stack* stack_)
{/*data structure provided needs to be sorted for this algorithm to work*/
  int i;
  node* tail=stack_->top;
  for(i=0; i<stack_->size/2;i++) tail = tail->next;/*setting the tail to be exactly to the half so that i could recursively go till the end of the queue and print until the half*/
  if(stack_->size % 2 != 0) special_traverse_help_qs(stack_->top, tail, stack_->size/2, 0);
  else special_traverse_help_qs(stack_->top, tail, stack_->size/2, 1);
}
void special_traverse_help_qs(node* head, node* tail, int size, int i)
{/*works the same for both stack and queue*/
  if(tail->next!=NULL) special_traverse_help_qs(head, tail->next, size, i+1);
  while(i!=size)
  {
    head = head->next;
    i++;
  }
  if(head!=tail)printf("%d %d ", head->data, tail->data);
  else printf("%d",head->data);
}
void copy_tree(bst* tree_source, bst** tree_destination)
{/*copies one tree into another*/
  if(tree_source!=NULL)
  {
    *tree_destination = insert_tree(*tree_destination, tree_source->data);
    copy_tree(tree_source->left,tree_destination);
    copy_tree(tree_source->right,tree_destination);
  }
  else return;
}
void fill_structures(stack ** stack_, queue ** queue_, bst ** bst_, int data[MAX] )
{
  int i;
  clock_t start_execution, end_execution;
  double stack_time, queue_time, tree_time;
  printf("Fill structures\n");
  start_execution=clock();
  init_stack(stack_);
  for(i=0; i<MAX; i++)
  {
    insert_node_stack(*stack_, data[i]);
  }
  end_execution=clock();
  stack_time = ((double)(end_execution-start_execution)/CLOCKS_PER_SEC)*1000;
  start_execution=clock();

  init_queue(queue_);
  for(i=0; i<MAX; i++)
  {
    insert_node_queue(*queue_, data[i]);
  }
  end_execution=clock();
  queue_time = ((double)(end_execution-start_execution)/CLOCKS_PER_SEC)*1000;

  start_execution=clock();
  init_tree(bst_);
  for(i=0; i<MAX; i++)
  {
    *bst_ = insert_tree(*bst_, data[i]);
  }
  end_execution=clock();
  tree_time = ((double)(end_execution-start_execution)/CLOCKS_PER_SEC)*1000;

  printf("%-20s%-20s%-20s%-20s\n", "Structures", "Stack", "Queue", "BST");
  printf("%-20s%-20.3lf%-20.3lf%-20.3lf\n", "Exec. time(ms)", stack_time, queue_time, tree_time);

}
void search(stack * stack_, queue * queue_, bst * bst_, int val_to_search)
{
  clock_t start_execution, end_execution;
  double stack_time, queue_time, tree_time;
  printf("\nSearch\n");

  start_execution=clock();
  printf("Stack: ");
  search_s(stack_, val_to_search);
  end_execution=clock();

  stack_time = ((double)(end_execution-start_execution)/CLOCKS_PER_SEC)*1000;
  start_execution=clock();
  printf("Queue: ");
  search_q(queue_, val_to_search);
  end_execution=clock();
  queue_time = ((double)(end_execution-start_execution)/CLOCKS_PER_SEC)*1000;

  start_execution=clock();
  printf("Tree: ");
  search_t(bst_, val_to_search);
  end_execution=clock();
  tree_time = ((double)(end_execution-start_execution)/CLOCKS_PER_SEC)*1000;

  printf("%-20s%-20s%-20s%-20s\n", "Structures", "Stack", "Queue", "BST");
  printf("%-20s%-20.3lf%-20.3lf%-20.3lf\n", "Exec. time(ms)", stack_time, queue_time, tree_time);
}
void search_q(queue* queue_, int val_to_search)
{/*searches through the queue, finds steps and puts them in a queue named steps, also find the number of occurences at the same time*/
  node* head;
  int num_of_occ;
  int i;
  queue* steps;
  init_queue(&steps);
  for(num_of_occ=0, head = queue_->head; head!=NULL; head=head->next)
  {
    i++;
    if(head->data == val_to_search)
    {
      num_of_occ++;
      insert_node_queue(steps, i);
    }
  }
  print_steps(num_of_occ, steps);
  remove_queue(&steps);
}
void search_s(stack* stack_, int val_to_search)
{/*searches through the stack, finds steps and puts them in a queue named steps, also find the number of occurences at the same time*/
  node* head;
  int num_of_occ;
  int i;
  queue* steps;
  init_queue(&steps);
  for(num_of_occ=0, head = stack_->top; head!=NULL; head=head->next)
  {
    i++;
    if(head->data == val_to_search)
    {
      num_of_occ++;
      insert_node_queue(steps, i);
    }
  }
  print_steps(num_of_occ, steps);
  remove_queue(&steps);
}
void search_t(bst* tree,int val_to_search)
{
  int i=1, num_of_occ;
  num_of_occ=0;
  queue* steps;
  init_queue(&steps);
  search_t_help(tree, &num_of_occ, &i, steps, val_to_search);
  print_steps(num_of_occ, steps);
  remove_queue(&steps);
}
void search_t_help(bst* tree, int* num_of_occ, int* i, queue* steps, int val_to_search)
{
  if(tree!=NULL)
  {
    if(tree->data == val_to_search)
    {
      (*num_of_occ)++;
      insert_node_queue(steps, *i);
      if(tree->right != NULL)
      {/*all the duplicates are placed in the right subtree when inserting into tree*/
          ++(*i);
         search_t_help(tree->right, num_of_occ, i, steps, val_to_search);
      }
    }
    else if(tree->data > val_to_search)
    {
      ++(*i);
      search_t_help(tree->left, num_of_occ, i, steps, val_to_search);
    }
    else
    {
      ++(*i);
      search_t_help(tree->right, num_of_occ, i, steps, val_to_search);
    }
  }
}
void print_steps(int num_of_occ, queue* steps)/*the same is done for all three data strucures so just avoiding the repetition of the same code*/
{/*this function just prints the results provided in a nice user-friendly manner*/
  node* head;
  if(steps->head != NULL)
  {
    printf("Found %d result(s) on: ", num_of_occ);
    if(steps->head->next !=NULL)
    {
      for(head = steps->head; head!=NULL; head = head->next)
      {
        if(head->next != NULL)
        {
          if(head->next->next != NULL) printf("%d., ", head->data);
          else printf("%d. ", head->data);
        }
        else printf("and %d. step\n", head->data);
      }
    }
    else printf("%d. step\n", steps->head->data);
  }
  else printf("No such element present\n");
}
