

/*  mylist3.c                                                                 */  
/*  A singly-linked list.                                                    */ 
/*  This code is released to the public domain.                              */  
/*  "Share and enjoy....." ;)                                                */    


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Our list of ints. */ 
struct node { 
	int data;    
    struct node *next;   
} ;


/* Create a new list */ 
struct node *list_new(int i)
{ 
  struct node *mylist = NULL; 
  mylist = malloc(sizeof(struct node));   
  mylist->data = i;  
  mylist->next = NULL;   
  return mylist; 
} 



/* Function to add data to end of a list  */ 
void list_append(struct node *mylist)
{  
      
   /* A pointer to iterate along the list */ 
   struct node *ptr = NULL; 
   
   /* Now, create a new node using the data. */ 
   struct node *newnode = NULL; 
   newnode = malloc(sizeof(struct node));  
       
  
  /* Is our existing list null? */ 
  /* If so, our "list" is just the newly-created node. */ 
   if(mylist == NULL) 
     {  
		mylist = newnode; 		
     }  		   
		   
   else 
     {  
		
	     for( ptr = mylist; ptr->next != NULL; ptr = ptr->next ) 
	       { 
		     /* Iterate through the list until we */ 
		     /* get to the last node */   		  		     
	       } 	    
	  
	   /* Add the new node to the end of the list. */ 
	   /* In other words: the last ptr->next WAS null, but it is */ 
	   /* now pointing to newnode. */  
	   ptr->next = newnode; 
	   /* Make the data member one greater than the previous member. */ 
	   newnode->data = ptr->data+1; 
       newnode->next = NULL;  
	
	 }     
	   
}  /* End of list_append  */  

	
	
/* Free the list memory. */ 
void free_list(struct node *mylist) 
{   
 	  
  struct node *ptr = mylist;
  
  while(ptr != NULL) { 
	 /* Create pointer for next. */   
     struct node *next = ptr->next;
     printf( "Freeing %d\n",  ptr->data );  
     /* Free the node being pointed at. */ 
     free(ptr);
     /* Go to next node. */ 
     ptr = next;
  }
  		      	
} 	
		
	    	    

void print_list(struct node *mylist)
{ 
   /* A pointer for iterating. */ 
   struct node *ptr	= mylist;  
	
   /* Iterate along the list.  */ 
   while (ptr != NULL) { 
	 /* Create pointer for next. */   
     struct node *next = ptr->next;
     printf( "mylist->data = %d\n",  ptr->data );            
     /* Go to next node. */ 
     ptr = next;
  }
       
}



int main() 
{ 

struct node *my_list = list_new(0); 

list_append(my_list);  
list_append(my_list);  
list_append(my_list);  
list_append(my_list);  
list_append(my_list);  
list_append(my_list);  

print_list(my_list); 

free_list(my_list);  

return 0;  
 
} 








