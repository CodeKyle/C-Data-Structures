/*  stack.c
    Implementation of a stack. The first element of a stack
    contains a sentinel value that cannot be changed or deleted until 
    program termination. This causes additional memory to be used but allowed
    me to more easily insert and remove nodes since there is always guaranteed
    to be at least one element in a list. 

    push() - O(1)
    pop()  - O(1)
*/

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

const int SENTINEL = INT_MAX;

struct node
{
    int value;
    struct node *prev;
    struct node *next;
};

/*  Clears out any remaining characters left in the stream. */
void clear_input(FILE *stream)
{
    int ch;
    while ((ch = fgetc(stream)) != '\n' && ch != EOF);
}

/*  Creates an empty stack. Returns a pointer to a node element whose 'value'
    is equal to SENTINEL and whose 'next' pointer is equal to NULL. SENTINEL is
    used as a sentinel value and is treated as if it were a NULL element. */
struct node* create_stack()
{
    struct node* new_stack = malloc(sizeof(struct node));
    if (new_stack == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a new stack.\n");
        return NULL;
    }
    
    *new_stack = (struct node){SENTINEL, NULL, NULL};
    return new_stack;
}

/*  Pushes an entry to the front of the stack denoted by 'stack_head'.
    Returns true if successful, otherwise false. Based on the implementation,
    the first entry in a stack will always technically be the default created
    node with value 'SENTINEL', though this will never be displayed. This 
    function runs in O(1) time. */
bool push(struct node *stack_head, int value)
{
    if (value == SENTINEL)
    {
        printf("Cannot manually insert sentinel value into stack.\n");
        return false;
    }
    
    struct node *new_entry = malloc(sizeof(struct node));
    if (new_entry == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a new node.\n");
        return false;
    }
    
    *new_entry = (struct node){value, stack_head, stack_head->next};
    
    /*  If 'new_entry' is being inserted to a stack that has at least one other
        node, (exluding the node with the sentinel value), set that other 
        node's 'prev' pointer to refer to 'new_entry'. */
    if (new_entry->next != NULL)
        new_entry->next->prev = new_entry;
        
    stack_head->next = new_entry;
    
    return true;
}

/*  Returns the 'value' of most recently pushed node and removes it from the 
    stack. This function runs in O(1) time. */
int pop(struct node *stack_head)
{
    struct node *pop_entry = stack_head->next;
    
    if (pop_entry == NULL)
    {
        printf("No values to pop from stack.\n");
        return SENTINEL;
    }
    
    int value = pop_entry->value;
    stack_head->next = pop_entry->next;
    
    /*  If we're not removing the last node of the stack, set the 'prev'
        pointer of the next node to refer to the 'prev' node of 'stack_head'. */
    if (pop_entry->next != NULL)
        pop_entry->next->prev = pop_entry->prev;
        
    pop_entry = NULL;
    free(pop_entry);
    
    return value;
}

/*  Prints out 'value' of 'stack_head' and all connected nodes until
    either a NULL node is encountered or a node with 'value' SENTINEL
    is encountered. This function runs in O(n) time. */
void print_stack(struct node *stack_head)
{
    /*  If stack only contains the default sentinel value, the stack is
        considered to be empty. */
    if (stack_head->next == NULL)
    {
        printf("Empty stack.\n");
        return;
    }
    
    while (stack_head != NULL)
    {
        //  If value is not the sentinel value, print it out.
        if (stack_head->value != SENTINEL)
            printf("%i ", stack_head->value);
            
        //  Fetch the next node in the stack.
        stack_head = stack_head->next;
    }
    
    printf("\n");
}

/*  Recursively frees all nodes connected to a stack starting with
    'stack_head'. */
void delete_stack(struct node* stack_head)
{
    if (stack_head != NULL)
    {
        delete_stack(stack_head->next);
        free(stack_head);
    }
}

int main(void)
{
    struct node* stack_head = create_stack(); 
    
    int value = 0;
    while (value >= 0)
    {
        printf("Add a positive number to the stack or a negative number to quit: ");
        if ( (scanf("%i", &value) == 1) && value >= 0)
             push(stack_head, value);
        clear_input(stdin);
        //print_stack(stack_head);
    }    
    
    value = 0;
    
    char input = 0;
    while (input != 'q')
    {
        printf("Type anything to pop or 'q' to quit: ");
        if ( (scanf("%c", &input) == 1) && input != 'q')
        {
            int popped = pop(stack_head);
            if (popped != SENTINEL)
                printf("Popped a %i!\n", popped);
        }
             
        clear_input(stdin);
        //print_stack(stack_head);
    }

    delete_stack(stack_head);
    
    return 0;
}