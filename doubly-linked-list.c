/*  doubly-linked-list.c
    Implementation of a doubly-linked list. The first element of a list
    contains a sentinel value that cannot be changed or deleted until 
    program termination. This causes additional memory to be used but allows
    me to easily insert and remove nodes since there is always guaranteed
    to be at least one element present.

    insert_entry()  - O(1)
    find_entry()    - O(n)
    remove_entry()  - O(1)
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

/*  Creates an empty list. Returns a pointer to a node element whose 'value'
    is equal to SENTINEL and whose 'next' pointer is equal to NULL. SENTINEL is
    used as a sentinel value and is treated as if it were a NULL element. */
struct node* create_list()
{
    struct node* new_list = malloc(sizeof(struct node));
    if (new_list == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a new list.\n");
        return NULL;
    }
    
    *new_list = (struct node){SENTINEL, NULL, NULL};
    return new_list;
}

/*  Inserts an entry at the beginning of the list denoted by 'list_head'.
    Returns true if successful, otherwise false. Based on the implementation,
    the first entry in a list will always technically be the default created
    node with value 'SENTINEL', though this will never be displayed. This 
    function runs in O(1) time. */
bool insert_entry(struct node *list_head, int value)
{
    if (value == SENTINEL)
    {
        printf("Cannot manually insert sentinel value into list.\n");
        return false;
    }
    
    struct node *new_entry = malloc(sizeof(struct node));
    if (new_entry == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a new node.\n");
        return false;
    }
    
    *new_entry = (struct node){value, list_head, list_head->next};
    
    /*  If 'new_entry' is being inserted to a list that has at least one other
        node, (exluding the node with the sentinel value), set that other 
        node's 'prev' pointer to refer to 'new_entry'. */
    if (new_entry->next != NULL)
        new_entry->next->prev = new_entry;
        
    list_head->next = new_entry;
    
    return true;
}

/*  Returns the address of the first node found in the list denoted by
    'list_head' whose 'value' is equal to the argument supplied. Function
    runs in O(n) time. */
struct node* find_entry(struct node *list_head, int value)
{
    if (value == SENTINEL)
    {
        printf("Cannot manually locate sentinel value in list.\n");
        return NULL;
    }
    
    while (list_head != NULL)
    {
        //  If the matching value was found, return address of the node.
        if (list_head->value == value)
            return list_head;
        else
            list_head = list_head->next;
    }
    
    return NULL;
}

/*  Removes specified 'entry' in list pointed to by 'list_head'. Runs in
    O(1) time. */
bool remove_entry(struct node *entry)
{
    if (entry->value == SENTINEL)
    {
        printf("Cannot manually remove sentinel value in list.\n");
        return false;
    }
    
    entry->prev->next = entry->next;
    
    /*  If we're not removing the last node of the list, set the 'prev'
        pointer of the next node to refer to the 'prev' node of 'entry'. */
    if (entry->next != NULL)
        entry->next->prev = entry->prev;
        
    entry = NULL;
    free(entry);
    
    return true;
}

/*  Prints out value of 'list_head' and all connected nodes until
    either a NULL node is encountered or a node with 'value' SENTINEL
    is encountered. This function runs in O(n) time. */
void print_list(struct node *list_head)
{
    /*  If list only contains the default sentinel value, the list is
        considered to be empty. */
    if (list_head->next == NULL)
    {
        printf("Empty list.\n");
        return;
    }
    
    while (list_head != NULL)
    {
        //  If value is not the sentinel value, print it out.
        if (list_head->value != SENTINEL)
            printf("%i ", list_head->value);
            
        //  Fetch the next node in the list.
        list_head = list_head->next;
    }
    
    printf("\n");
}

/*  Recursively frees all nodes connected to a list starting with
    'list_head'. */
void delete_list(struct node* list_head)
{
    if (list_head != NULL)
    {
        delete_list(list_head->next);
        free(list_head);
    }
}

int main(void)
{
    struct node* list_head = create_list(); 
    
    int value = 0;
    while (value >= 0)
    {
        printf("Add a positive number to the list or a negative number to quit: ");
        if ( (scanf("%i", &value) == 1) && value >= 0)
             insert_entry(list_head, value);
        clear_input(stdin);
        print_list(list_head);
    }
    
    value = 0;
    struct node *entry = NULL;
    while (value >= 0)
    {
        printf("Remove a positive value from the list or a negative number to quit: ");
        if ( (scanf("%i", &value) == 1) && value >= 0)
        {
            entry = find_entry(list_head, value);
            if (entry != NULL)
            {
                if (!remove_entry(entry))
                    printf("Could not remove entry from list.\n");
            }
            else
                printf("Entry not found in list.\n");
        }
             
        clear_input(stdin);
        print_list(list_head);
    }

    delete_list(list_head);
    
    return 0;
}