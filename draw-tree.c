/* This code comes from here -  */ 
/* ftp://ftp.cs.cmu.edu/usr/sleator/splaying/draw-tree.c  */ 

/* Draws ascii picture of a tree.  Allows the user to splay */
/* any node, and then redraws the tree.                     */
/* written by Daniel Sleator <sleator@cs.cmu.edu>           */
/* The following program is in the public domain.           */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>


#define TRUE 1
#define FALSE 0
int size;


/* 
 * This version of top-down-simple splay maintains parent pointers.
 */
typedef struct tree_node {
    struct tree_node * left, * right, * parent;
    int key;
} Tree;

#define set_parent_left(t) {if((t)->left != NULL) (t)->left->parent = (t);}
#define set_parent_right(t) {if((t)->right != NULL) (t)->right->parent = (t);}

Tree * splay (int i, Tree * t) {
    Tree N, *l, *r, *y;
    if (t == NULL) return t;
    N.left = N.right = NULL;
    l = r = &N;

    for (;;) {
	if (i < t->key) {
	    if (t->left == NULL) break;
	    if (i < t->left->key) {
		y = t->left;                           /* rotate right */
		t->left = y->right;
		if (t->left != NULL) t->left->parent = t;
		y->right = t;
		t->parent = y;
		t = y;
		if (t->left == NULL) break;
	    }
	    r->left = t;                               /* link right */
	    t->parent = r;
	    r = t;
	    t = t->left;
	} else if (i > t->key) {
	    if (t->right == NULL) break;
	    if (i > t->right->key) {
		y = t->right;                          /* rotate left */
		t->right = y->left;
		if (t->right != NULL) t->right->parent = t;
		y->left = t;
		t->parent = y;
		t = y;
		if (t->right == NULL) break;
	    }
	    l->right = t;                              /* link left */
	    t->parent = l;
	    l = t;
	    t = t->right;
	} else {
	    break;
	}
    }
    l->right = t->left;                                /* assemble */
    if (l->right != NULL) l->right->parent = l;
    r->left = t->right;
    if (r->left != NULL) r->left->parent = r;
    t->left = N.right;
    if (t->left != NULL) t->left->parent = t;
    t->right = N.left;
    if (t->right != NULL) t->right->parent = t;
    t->parent = NULL;
    return t;
}

Tree * move_to_root (int i, Tree * t) {
    Tree N, *l, *r, *y;
    if (t == NULL) return t;
    N.left = N.right = NULL;
    l = r = &N;

    for (;;) {
	if (i < t->key) {
	    if (t->left == NULL) break;
	    r->left = t;                               /* link right */
	    t->parent = r;
	    r = t;
	    t = t->left;
	} else if (i > t->key) {
	    if (t->right == NULL) break;
	    l->right = t;                              /* link left */
	    t->parent = l;
	    l = t;
	    t = t->right;
	} else {
	    break;
	}
    }
    l->right = t->left;                                /* assemble */
    if (l->right != NULL) l->right->parent = l;
    r->left = t->right;
    if (r->left != NULL) r->left->parent = r;
    t->left = N.right;
    if (t->left != NULL) t->left->parent = t;
    t->right = N.left;
    if (t->right != NULL) t->right->parent = t;
    t->parent = NULL;
    return t;
}

int check_tree(Tree * t) {
    if (t == NULL) return TRUE;
    if (t->left != NULL &&(t->left->parent != t || !check_tree(t->left))) {
	return FALSE;
    }
    if (t->right != NULL && (t->right->parent != t || !check_tree(t->right))) {
	return FALSE;
    }
    return TRUE;
}

#define INFINITY (1<<20)
#define LABLEN 20

int min (int X, int Y)  {return ((X) < (Y)) ? (X) : (Y);}
int max (int X, int Y)  {return ((X) > (Y)) ? (X) : (Y);}

typedef struct pnode_struct Pnode;

struct pnode_struct {
    Pnode * left, * right;
    int edge_length; /* length of the edge from this node to its children */
                     /* number of "\" or "/".  so it's at least 1         */
                     /* unless both children are null.  Then it's 0       */
    
    int height;      /* The number of rows required to print this tree */
    int lablen;
    int parent_dir;       /* -1=I am left, 0=I am root, 1=right       */
                          /* this is used to decide how to break ties */
                          /* when the label is of even length         */
    char label[LABLEN+1];
};

int allocs_in_use;
void my_free(void * p) {
    allocs_in_use--;
    free(p);
}
void * my_alloc(int size) {
    void * p = malloc(size);
    allocs_in_use++;
    if (p == NULL) {
        fprintf(stderr, "Ran out of space.  Requested size=%d.\n", size);
        exit(1);
    }
    return p;
}

/* Free all the nodes of the given tree */
void free_ptree(Pnode *pn) {
    if (pn == NULL) return;
    free_ptree(pn->left);
    free_ptree(pn->right);
    my_free(pn);
}

Pnode * build_ptree_rec(Tree * t) {
    Pnode * pn;
    if (t == NULL) return NULL;
    pn = my_alloc(sizeof(Pnode));
    pn->left = build_ptree_rec(t->left);
    pn->right = build_ptree_rec(t->right);
    if (pn->left != NULL) pn->left->parent_dir = -1;
    if (pn->right != NULL) pn->right->parent_dir = 1;

    sprintf(pn->label, "%d", t->key);
    pn->lablen = strlen(pn->label);
    return pn;
}

/*
 * Copy the tree from the original structure into the Pnode structure
 * fill in the parent_dir, label, and labelen fields, but not the    
 * edge_length or height fields.                                     
 */
Pnode * build_ptree(Tree * t) {
    Pnode *pn;
    if (t == NULL) return NULL;
    pn = build_ptree_rec(t);
    pn->parent_dir = 0;
    return pn;
}

/* 
 * The lprofile array is description of the left profile of a tree.
 * Assuming the root is located at (0,0), lprofile[i] is the leftmost
 * point used on row i of the tree.  rprofile is similarly defined.
 */
#define MAX_HEIGHT 1000
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];

/*
 * The following function fills in the lprofile array for the given tree.
 * It assumes that the center of the label of the root of this tree
 * is located at a position (x,y).  It assumes that the edge_length
 * fields have been computed for this tree.
 */
void compute_lprofile(Pnode *pn, int x, int y) {
    int i, isleft;
    if (pn == NULL) return;
    isleft = (pn->parent_dir == -1);
    lprofile[y] = min(lprofile[y], x-((pn->lablen-isleft)/2));
    if (pn->left != NULL) {
	for (i=1; i <= pn->edge_length && y+i < MAX_HEIGHT; i++) {
	    lprofile[y+i] = min(lprofile[y+i], x-i);
	}
    }
    compute_lprofile(pn->left, x-pn->edge_length-1, y+pn->edge_length+1);
    compute_lprofile(pn->right, x+pn->edge_length+1, y+pn->edge_length+1);
}
void compute_rprofile(Pnode *pn, int x, int y) {
    int i, notleft;
    if (pn == NULL) return;
    notleft = (pn->parent_dir != -1);
    rprofile[y] = max(rprofile[y], x+((pn->lablen-notleft)/2));
    if (pn->right != NULL) {
	for (i=1; i <= pn->edge_length && y+i < MAX_HEIGHT; i++) {
	    rprofile[y+i] = max(rprofile[y+i], x+i);
	}
    }
    compute_rprofile(pn->left, x-pn->edge_length-1, y+pn->edge_length+1);
    compute_rprofile(pn->right, x+pn->edge_length+1, y+pn->edge_length+1);
}

/* 
 * This function fills in the edge_length and height fields of the
 * specified tree.
 */
void compute_edge_lengths(Pnode *pn) {
    int h, hmin, i, delta;
    if (pn == NULL) return;
    compute_edge_lengths(pn->left);
    compute_edge_lengths(pn->right);

    /* first fill in the edge_length of pn */
    if (pn->right == NULL && pn->left == NULL) {
	pn->edge_length = 0;
    } else {
	if (pn->left != NULL) {
	    for (i=0; i<pn->left->height && i < MAX_HEIGHT; i++) {
		rprofile[i] = -INFINITY;
	    }
	    compute_rprofile(pn->left, 0, 0);
	    hmin = pn->left->height;
	} else {
	    hmin = 0;
	}
	if (pn->right != NULL) {
	    for (i=0; i<pn->right->height && i < MAX_HEIGHT; i++) {
		lprofile[i] = INFINITY;
	    }
	    compute_lprofile(pn->right, 0, 0);
	    hmin = min(pn->right->height, hmin);
	} else {
	    hmin = 0;
	}
	delta = 4;
	for (i=0; i<hmin; i++) {
	    delta = max(delta, 2 + 1 + rprofile[i] - lprofile[i]);
       /* the "2" guarantees a gap of 2 between different parts of the tree */
	}
	/* If the node has two children of height 1, then we allow the
           two leaves to be within 1, instead of 2 */
	if (((pn->left != NULL && pn->left->height == 1) ||
	    (pn->right != NULL && pn->right->height == 1))&&delta>4) delta--;
	pn->edge_length = ((delta+1)/2) - 1;
    }

    /* now fill in the height of pn */
    h = 1;
    if (pn->left != NULL) {
	h = max(pn->left->height + pn->edge_length + 1, h);
    }
    if (pn->right != NULL) {
	h = max(pn->right->height + pn->edge_length + 1, h);
    }
    pn->height = h;
}

int print_next;  /* used by print_level.  If you call "printf()" at   */
                 /* any point, this is the x coordinate of the next   */
                 /* char printed.                                     */

/*
 * This function prints the given level of the given tree, assuming
 * that the node pn has the given x cordinate.
 */
void print_level(Pnode *pn, int x, int level) {
    int i, isleft;
    if (pn == NULL) return;
    isleft = (pn->parent_dir == -1);
    if (level == 0) {
	for (i=0; i<(x-print_next-((pn->lablen-isleft)/2)); i++) {
	    printf(" ");
	}
	print_next += i;
	printf("%s", pn->label);
	print_next += pn->lablen;
    } else if (pn->edge_length >= level) {
	if (pn->left != NULL) {
	    for (i=0; i<(x-print_next-(level)); i++) {
		printf(" ");
	    }
	    print_next += i;
	    printf("/");
	    print_next++;
	}
	if (pn->right != NULL) {
	    for (i=0; i<(x-print_next+(level)); i++) {
		printf(" ");
	    }
	    print_next += i;
	    printf("\\");
	    print_next++;
	}
    } else {
	print_level(pn->left, x-pn->edge_length-1, level-pn->edge_length-1);
	print_level(pn->right, x+pn->edge_length+1, level-pn->edge_length-1);
    }
}

/* 
 * This pretty-prints the given tree, left-justified.
 * The tree is drawn in such a way that both of the edges down from
 * a node are the same length.  This length is the minimum such that
 * the two subtrees are separated by at least two blanks.
 */
void pretty_print_tree(Tree * t) {
    Pnode *proot;
    int xmin, i;
    if (t == NULL) return;
    proot = build_ptree(t);
    compute_edge_lengths(proot);
    for (i=0; i<proot->height && i < MAX_HEIGHT; i++) {
	lprofile[i] = INFINITY;
    }
    compute_lprofile(proot, 0, 0);
    xmin = 0;
    for (i = 0; i < proot->height && i < MAX_HEIGHT; i++) {
	xmin = min(xmin, lprofile[i]);
    }
    for (i = 0; i < proot->height; i++) {
	print_next = 0;
	print_level(proot, -xmin, i);
	printf("\n");
    }
    if (proot->height >= MAX_HEIGHT) {
	printf("(This tree is taller than %d, and may be drawn incorrectly.)\n", MAX_HEIGHT);
    }
    free_ptree(proot); 
}

Tree * insert(int i, Tree * t) {
/* Insert i into the tree t, unless it's already there.    */
/* Return a pointer to the resulting tree.                 */
    Tree * new;
    
    new = (Tree *) my_alloc (sizeof (Tree));
    new->key = i;
    if (t == NULL) {
	new->left = new->right = new->parent = NULL;
	size = 1;
	return new;
    }
    t = splay(i,t);
    if (i < t->key) {
	new->left = t->left;
	new->right = t;
	t->left = NULL;
	set_parent_left(new);
	set_parent_right(new);
	size ++;
	return new;
    } else if (i > t->key) {
	new->right = t->right;
	new->left = t;
	t->right = NULL;
	set_parent_left(new);
	set_parent_right(new);
	size++;
	return new;
    } else { /* We get here if it's already in the tree */
             /* Don't add it again                      */
	my_free(new);
	return t;
    }
}

Tree * delete(int i, Tree * t) {
/* Deletes i from the tree if it's there.               */
/* Return a pointer to the resulting tree.              */
    Tree * x;
    if (t==NULL) return NULL;
    t = splay(i,t);
    if (i == t->key) {               /* found it */
	if (t->left == NULL) {
	    x = t->right;
	} else {
	    x = splay(i, t->left);
	    x->right = t->right;
	    set_parent_right(x);
	}
	size--;
	my_free(t);
	return x;
    }
    return t;                         /* It wasn't there */
}

void main() {
    Tree * root;
    char line[100];
    int i, N;
    root = NULL;              /* the empty tree */
    size = 0;
    while(TRUE) {
	printf("Enter the number of nodes in the tree: ");
	N = -1;
	if (fgets(line, sizeof(line), stdin) == NULL) exit(1);
	sscanf(line,"%d", &N);
	if ((N<1) || (N > 200)) {
	    printf("Choose a number between 1 and 200.\n");
	    continue;
	}
	break;
    }

    for (i = 0; i < N; i++) {
	root = insert(i, root);
	if (!check_tree(root)) printf("error\n");;
    }
    pretty_print_tree(root);
    for (;;) {
	printf("Select a node to splay: ");
	if (fgets(line, sizeof(line), stdin) == NULL) break;
	if ((sscanf(line, "%d", &i) == 1) && (i >= 0 && i < N)) {
	    /*	root = move_to_root(i, root);  */
	    root = splay(i, root);
	} else {
	    if (strncasecmp(line, "quit", strlen("quit")) == 0) {
		break;
	    } else {
		printf("Choose a number in [%d, %d] or type \"quit\".\n", 0, N-1);
		continue;
	    }
	}
	pretty_print_tree(root);
    }
/*
    for (i = 0; i < N; i++) {
	root = delete((541*i) & (N-1), root);
	if (!check_tree(root)) printf("error\n");;
    }
    printf("size = %d\n", size);
*/
}
