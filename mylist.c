

/*  mylist.c                                                                 */  
/*  A singly-linked list.                                                    */ 
/*  Acknowledgement - This code is largely based on                          */ 
/*  Adam Cooper's code from here -                                           */ 
/*  http://adamhooper.com/code?path=mcgill-se%2FCOMP206%2Fass2%2Fproducts.c  */   
/*  Many thanks, Adam!                                                       */   
/*  This code is released to the public domain.                              */  
/*  "Share and enjoy....." ;)                                                */    


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>  


typedef struct LIST {
    struct LIST *next;
    void *data;
} List;


static List *list_malloc(void)
{ 
	return (List *) malloc(sizeof (List) );
}


List *list_create(List *list, void *data) 
{       
  	List *ret = list_malloc(); 
	ret->data = data;
	ret->next = list;     
	return ret;   
}     


List *list_append(List *list, void *data)
{  
   List *ret;
  
  /* Is our list null? */ 
  /* If so, just create one element at the head. */ 
   if(list == NULL) return list_create(list, data); 
     
   /*  Otherwise, append to the existing list  */       
    ret = list;  

	for (; list->next; list = list->next);
	list->next = list_malloc();
	list->next->data = data;     
	list->next->next = NULL;          
	return ret;                  
}  


void print_list(List *list)
{ 
   while (list != NULL) 
   {  
      printf( "list->data = %s\n", (char *)(list->data) );
      list = list->next;   
   } 
}


void list_free (List *list)
{
	List *next;  
	while (list)
	{
		next = list->next;        
		free (list);
		list = next;
	}     
} 



int main() 
{ 

List *mylist = NULL; 

mylist = list_append(mylist, "foo");  
mylist = list_append(mylist, "bar");  
mylist = list_append(mylist, "baz");   
mylist = list_append(mylist, "one");   
mylist = list_append(mylist, "two");   
mylist = list_append(mylist, "three");   

print_list(mylist); 
list_free(mylist);  

return 0;  
 
} 








