#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

struct Node {                 //Basic component of the list each list has: a name value, a unique ID, a pointer to the next and previous node
  int id;
  struct Node *next;
  struct Node *prev;
};

struct List {               //A list contains pointers to the head and tail nodes
  struct Node *head;
  struct Node *tail;
  int size;
};

struct Node *create_node(int id) {        //Takes a pointer to a character string, and an id
  struct Node *node = malloc(sizeof(struct Node));    //node is a pointer to the memory space allocated by malloc which allocates the enough memory space for the node component
  if (node == NULL) {
    fprintf (stderr, "%s: Couldn't create memory for the node; %s\n", "linkedlist", strerror(errno));
    exit(-1);
  }
  node->id = id;                  //node id points to the id input
  node->next = NULL;              //
  node->prev = NULL;              //Initialize the rear and front pointers of the node
  return node;        //Returns a pointer to the created node
}

struct List *create_list() {      
  struct List *list = malloc(sizeof(struct List));    //Creates a pointer to a List where the memory space is dynamically created to contain two pointers
  if (list == NULL) {
    fprintf (stderr, "%s: Couldn't create memory for the list; %s\n", "linkedlist", strerror (errno));
    exit(-1);
  }
  list->head = NULL;  //Initialize the rear and front pointers of the node
  list->tail = NULL;  //
  list->size = 0;
  return list;        //Returns a pointer to the created list
}

void insert_tail(struct Node *node, struct List *list) {    //Takes a pointer to a node, and a pointer to a list
  if (list->head == NULL && list->tail == NULL) { //If the list contains no elements, set both pointers to the node
    list->head = node;
    list->tail = node;
    list->size++;
  } else {                    //If the list contains at least one other element, do the following:
    list->tail->next = node;        //Set the next pointer of previous tail node to the next node
    node->prev = list->tail;        //Set the previous pointer of the current node to the previous tail node
    list->tail = node;              //Set the tail pointer to the current node
    list->size++;
  }
}

void insert_head(struct Node *node, struct List *list){
  if (list->head == NULL && list->tail == NULL){
    list->head = node;
    list->tail = node;
    list->size++;
  } else {
    list->head->prev = node;
    node->next = list->head;
    node->prev = NULL;
    list->head = node;
    list->size++;
  }
}

void print_list(struct List *list) {    //Takes a pointer to the list to print
  struct Node *ptr = list->head;      //creates a pointer to a node starting at the head of the list
  while (ptr != NULL) {         //While there are nodes to step through
    if (ptr != list->head) {      //If the node is not the head node, print "->"
      printf("->");
    }
    printf("(%d)", ptr->id);   //Print the name and id of the node
    ptr = ptr->next;      //Navigate to the next node
  }
  printf("\n");
  printf("The list now has %d elements.\n", list->size);

}

void destroy_list(struct List *list) {    //Takes a pointer to a list
  struct Node *ptr = list->head;      //Pointer to the head node
  struct Node *tmp;             //Temporary node pointer
  while (ptr != NULL) {       //While nodes exist
    tmp = ptr;            //Set the temporary node to the current node
    ptr = ptr->next;      //Point to the next node
    free(tmp);          //Free the temporary node
  }
  free(list);           //After all nodes are freed, free the space for the list
}


struct Node *find_by_id(int id, struct List *list) {    //Takes the target id and a pointer to the list

     struct Node *ptr = list->head;   //create a pointer to the first node in the list
     int found = 0;   //Loop break logic
     while (found != 1) {
        if(ptr->id == id){    //if the target id is the current id break the loop
            //printf("(%s,%d)", ptr->name, ptr->id);
            found = 1;
        }
        else{
            ptr = ptr->next;  //if target id not found, traverse the list
        }
     }

     return ptr;  //return the target node
}

struct Node *insertSort_remove(struct Node* ptr, struct List *list){    //removes a node from the list without freeing the memory, but still retaining the pointer
    
    if (list->head == NULL && list->tail == NULL) {
      printf("List is empty"); 
      return(NULL);} //If the list is empty
    else if (list->tail == ptr) {
      list->tail = ptr->prev;
      ptr->prev->next = NULL;
      ptr->prev = NULL;
      ptr->next = NULL;
      list->size--;} //If the node is the tail
    else {
      ptr->prev->next = ptr->next;
      ptr->next->prev = ptr->prev;
      ptr->next = NULL;
      ptr->prev = NULL;
      list->size--;} //Removing a middle node
    
    return(ptr);
}

void remove_by_id(int id, struct List *list) {
          /*
                Pseudocode:
                      1. Find the node by ID, call find_by_id
                      2. Update pointer from tmp->previous->next to tmp->next
                      3. Update pointer from tmp->next->previous to tmp->previous
                      4. Free memory of node
          */

    struct Node *ptr = find_by_id(id,list);   //create a pointer to the id node
    
    if (list->head == NULL && list->tail == NULL) {
      //printf("List is empty"); 
      return;} //If the list is empty
    else if(list->head == ptr && list->tail == ptr) {
      list->head = NULL, list->tail = NULL; 
      list->size--;
      free(ptr);} //If the list only has 1 node
    else if (list->head == ptr) {
      list->head = ptr->next;
      ptr->next->prev = NULL;
      list->size--;
      free(ptr);} //If the node is the head
    else if (list->tail == ptr) {
      list->tail = ptr->prev;
      ptr->prev->next = NULL;
      list->size--;
      free(ptr);} //If the node is the tail
    else {
      ptr->prev->next = ptr->next;
      ptr->next->prev = ptr->prev;
      list->size--;
      free(ptr);} //Removing a middle node

    //printf("Removing node with id %d, the list now has %d elements.", id, list->size);
    //printf("\n");
}

void insert_after_id(int id, struct Node *node, struct List *list) {
    struct Node *ptr = find_by_id(id,list);     //Create a pointer to the node we want to insert after
    
    if(ptr->next != NULL){            //If this is not the last node in the list
      struct Node *tmp = ptr->next;   //temporary node pointer to the next node
      ptr->next = node;     //id node now points to node
      tmp->prev = node;     //next node points back to node

      node->prev = ptr;     //node points back to id node
      node->next = tmp;     //node points forward to tmp node
      list->size++;   //increment the list size

      }

    else{               //if id node is the last in the list
      insert_tail(node, list);
    }

    //printf("Inserted (%d) after node with id %d. The list now has %d elements.\n", node->id, ptr->id, list->size);
}

void insert_before_id(int id, struct Node *node, struct List *list){
  struct Node *ptr = find_by_id(id,list);

  if(ptr->prev == NULL){
    insert_head(node, list);
  } //If the node is the first node in the list  
  else{
    node->prev = ptr->prev;
    node->next = ptr;
    ptr->prev->next = node;
    ptr->prev = node;
    list->size++;
  }//If the node is not the first node in the list
    //printf("Inserted (%d) before node with id %d. The list now has %d elements.\n", node->id, ptr->id, list->size);
}

void insertion_sort_by_ID_increasing(struct List *list) {
      struct Node *ptr = NULL;
      struct Node *tmp = NULL;
      struct Node *key = NULL;
      int swap;

      if (list->head == list->tail) {                    //If list is of size 1, it is already sorted
        //printf("The list is already sorted");
        return;
        }

      for (int i = 1; i < list->size; i++){    
        key = list->head;                       //Start the key at the head of the list
        swap = 0;                               //No swap needed
        tmp = NULL;
        for(int j = i; j > 0; j--){             //Loop to bring key value forward through list
          key = key->next;                      //Bring key to point to next node
        }
        ptr = key->prev;                        //ptr points to previous node of key
        while (ptr != NULL){                    //loop while there are values to compare
          if(key->id < ptr->id){                //if key id is less than the previous id
            tmp = ptr;                          //set tmp to ptr
            ptr = ptr->prev;                    //move ptr backwards
            swap = 1;                           //indicate a swap needs to take place
          }
          else{                                 //If there is no switch needed
            if(tmp == NULL) {swap = 0;}
            //printf("(%d) does not need to switch position with (%d)\n", key->id, ptr->id);
            ptr = NULL;          
          }
        }

        if(swap == 1){                                          //Moving pointers of nodes
          if(tmp->prev == NULL && list->head == tmp){
            insertSort_remove(key,list);                //Removing the node from the list without freeing the memory
            insert_before_id(tmp->id,key,list);         //Inserting the node back into the list in the proper position
          }//If putting the key as the first element
          else {
            insertSort_remove(key,list);                //Removing the node from the list without freeing the memory
            insert_before_id(tmp->id,key,list);         //Inserting the node back into the list in the proper position
          }//Else putting it behind any other node

          //print_list(list);
        }
      }
}

void busNumbers_print(struct List *list){
  struct Node *ptr = NULL;
  struct Node *tmp = NULL;

  ptr = list->head;
  tmp = ptr->next;
  //Why this isn't working: only print the sequence with a dash, if there are two or more consecutive
  int count = 0;

  if(list->size == 1) {printf("%d", list->head->id);}

  else{
    while (ptr != NULL){
      if(tmp != NULL && (ptr->id == tmp->id - 1)) {
        count++;
        while (tmp->next != NULL){  
          if (tmp->id == tmp->next->id - 1) {
            tmp = tmp->next;
            count++; 
          }
        }
        if(count >= 2) {printf("%d-%d", ptr->id, tmp->id);}
        else {printf ("%d %d", ptr->id, tmp->id);}
        
        ptr = tmp->next;
        tmp = ptr->next;
        count = 0;
      }

      else {
        printf("%d", ptr->id);
        ptr = ptr->next;
        if(tmp != NULL) {tmp = tmp->next;}
      }
      if (ptr != NULL) {printf(" ");}
    }
  }
}

int main(int argc, char *argv[]) {              //Main function takes command line arguments
    struct List *list = create_list();        //create a list pointer and create a list
    struct Node *tmp = NULL;                  //create a temporary node pointer and set to 

    int cases = 0;
    int busNumber = 0;

    scanf("%d", &cases);

    for (int i = 0; i < cases; i++){
      scanf("%d", &busNumber);
      tmp = create_node(busNumber);
      insert_tail(tmp, list);
      busNumber = 0;
    }

    insertion_sort_by_ID_increasing(list);
    
    print_list(list);

    busNumbers_print(list);

    free(tmp);
    destroy_list(list);                         //Frees the memory that was dynamically located during program execution

    return 0;
}
