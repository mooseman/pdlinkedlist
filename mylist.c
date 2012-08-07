

/*  mylist.c                                                                 */  
/*  A singly-linked list.                                                    */ 
/*  This code is released to the public domain.                              */  
/*  "Share and enjoy....." ;)                                                */    


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>  


struct node { 
	void  *data;    
    struct node *next;   
} ;



/* Create a new list and set it to NULL. */ 
struct node *mylist = NULL; 



/* Function to add data to end of a list  */ 
void list_append(void *data)
{  
   
   /* A pointer to iterate along the list */ 
   struct node *ptr = NULL; 
   
   /* Now, create a new node using the data. */ 
   struct node *newnode = NULL; 
   newnode = malloc(sizeof(struct node));  
   
   newnode->data = data; 
   newnode->next = NULL;  
   
  
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
		     /* Iterate through the list until we find the last node */   
	       } 	    
	  
	   /* Add the new node to the end of the list. */ 
	   /* In other words: the last ptr->next WAS null, but it is */ 
	   /* now pointing to newnode. */  
	   ptr->next = newnode; 
	
	 }     
	   
}  /* End of list_append  */  

	
	
/* Free the list memory. */ 
void free_list(void) 
{ 
  /* We create TWO pointers. */ 
  /* ptr is used to iterate through the list. */ 
  /* tmp (which points to the same place) is used to */ 
  /* actually free the memory. */    
   struct node *ptr, *tmp = NULL;  	  
  		
  /* Check that the list is non-null. */   
   if(mylist != NULL) 
     {  
		          		
	     for( ptr = mylist; ptr->next != NULL; ptr = ptr->next ) 
	       { 
			 /* Iterate through the list */   
			  printf( "Freeing %s\n",  (char *) (ptr->data) );  
			  tmp = ptr; 			 
		      free(tmp); 			      
	       } 	    
	  
	   /* Free the last element of the list */ 
	   /* (where ptr->next IS null). */ 	   
	    printf( "Freeing %s\n",  (char *) (ptr->data) ); 
	    free(ptr);   
	
	 }     
	
} 	
		
	    
	    

void print_list(void)
{ 
   /* A pointer for iterating. */ 
   struct node *ptr	= NULL;  
	
   /* Iterate along the list.  */ 
    for( ptr = mylist; ptr->next != NULL; ptr = ptr->next ) 	
	   { 
		  printf( "mylist->data = %s\n",  (char *) (ptr->data) );   		 		
	   } 	    
	
	/* For the last nore (where ->next IS NULL), we need to print */ 
	/* it outside the loop. */ 
	printf( "mylist->data = %s\n",  (char *) (ptr->data) ); 
    
}



int main() 
{ 

list_append("foo");  
list_append("bar");  
list_append("baz");   
list_append("one");   
list_append("two");   
list_append("three");   

print_list(); 

free_list();  

return 0;  
 
} 








