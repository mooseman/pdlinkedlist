

/*  tuple.c                                                                 */  
/*  A tuple implemented as a singly-linked list.                                                    */ 
/*  This code is released to the public domain.                              */  
/*  "Share and enjoy....." ;)                                                */    


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>  

/* We will store the data in a union. */  
struct node { 
	union {
		    int i; 
		    float f; 
		    char *s; 		
	      } data ; 
    struct node *next;   
} ;



/* Create a new tuple and set it to NULL. */ 
struct node *mytuple = NULL; 



/* Function to add data to end of a tuple */ 
void tuple_append(void *data)
{  
   
   /* A pointer to iterate along the tuple */ 
   struct node *ptr = NULL; 
   
   /* Now, create a new node using the data. */ 
   struct node *newnode = NULL; 
   newnode = malloc(sizeof(struct node));  
   
   newnode->data = data; 
   newnode->next = NULL;  
   
  
  /* Is our existing tuple null? */ 
  /* If so, our tuple is just the newly-created node. */ 
   if(mytuple == NULL) 
     {  
		mytuple = newnode; 		
     }  		   
		   
   else 
     {  
		
	     for( ptr = mytuple; ptr->next != NULL; ptr = ptr->next ) 
	       { 
		     /* Iterate through the tuple until we find the last node */   
	       } 	    
	  
	   /* Add the new node to the end of the tuple. */ 
	   /* In other words: the last ptr->next WAS null, but it is */ 
	   /* now pointing to newnode. */  
	   ptr->next = newnode; 
	
	 }     
	   
}  /* End of tuple_append  */  

	
	
/* Free the tuple memory. */ 
void free_tuple(void) 
{ 
  /* We create TWO pointers. */ 
  /* ptr is used to iterate through the tuple. */ 
  /* tmp (which points to the same place) is used to */ 
  /* actually free the memory. */    
   struct node *ptr, *tmp = NULL;  	  
  		
  /* Check that the tuple is non-null. */   
   if(mytuple != NULL) 
     {  
		          		
	     for( ptr = mytuple; ptr->next != NULL; ptr = ptr->next ) 
	       { 
			 /* Iterate through the tuple */   
			  printf( "Freeing %s\n",  (char *) (ptr->data) );  
			  tmp = ptr; 			 
		      free(tmp); 			      
	       } 	    
	  
	   /* Free the last element of the tuple */ 
	   /* (where ptr->next IS null). */ 	   
	    printf( "Freeing %s\n",  (char *) (ptr->data) ); 
	    free(ptr);   
	
	 }     
	
} 	
		
	    
	    

void print_tuple(void)
{ 
   /* A pointer for iterating. */ 
   struct node *ptr	= NULL;  
	
   /* Iterate along the tuple.  */ 
    for( ptr = mytuple; ptr->next != NULL; ptr = ptr->next ) 	
	   { 
		  printf( "mytuple->data = %s\n",  (char *) (ptr->data) );   		 		
	   } 	    
	
	/* For the last node (where ->next IS NULL), we need to print */ 
	/* it outside the loop. */ 
	printf( "mytuple->data = %s\n",  (char *) (ptr->data) ); 
    
}



int main() 
{ 

int a = 123; 
float b = 42.42; 
char *c = "Just a test" ; 

void ptr *p;

p = &a; 
tuple_append(p);

p = &b; 
tuple_append(p);  

p = &c; 
tuple_append(p);

print_tuple(); 

free_tuple();  

return 0;  
 
} 








