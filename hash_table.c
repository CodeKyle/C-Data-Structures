/*  hash_table.c
    Implementation of a hash table using doubly-linked lists.The first element
    of a list contains a node whose 'value' points to a sentinel value. This 
    node cannot be changed or deleted until program termination. This causes 
    additional memory to be used but allowed me to more easily insert and 
    remove nodes since there is always guaranteed to be at least one element 
    present.

    insert_entry()  - O(1)
    find_entry()    - O(n)
    remove_entry()  - O(1)
    
    Even though find_entry() is running in O(n), using the hash function to
    determine the index of the array 'tables' where the potential value
    would be stored technically reduces lookup time by (n/TABLE_SIZE) in
    the long run.
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 5
#define NAME_LENGTH 80

char *SENTINEL = "SENTINEL";

struct node
{
    char *value;
    struct node *prev;
    struct node *next;
};

/*  Clears out any remaining characters left in the stream. */
void clearInput(FILE *stream)
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
bool insert_entry(struct node *list_head, const char *value)
{
    if (strcmp(value, SENTINEL) == 0)
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
    
    char *new_string = malloc(sizeof(char) * (strlen(value) + 1));
    strcpy((char*)new_string, (char*)value);
    *new_entry = (struct node){new_string, list_head, list_head->next};
    
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
struct node* find_entry(struct node *list_head, const char *value)
{
    if (strcmp(value, SENTINEL) == 0)
    {
        printf("Cannot manually locate sentinel value in list.\n");
        return NULL;
    }
    
    while (list_head != NULL)
    {
        //  If the matching value was found, return address of the node.
        if (strcmp(list_head->value, value) == 0)
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
    if (strcmp(entry->value, SENTINEL) == 0)
    {
        printf("Cannot manually remove sentinel value in list.\n");
        return false;
    }
    
    entry->prev->next = entry->next;
    
    /*  If we're not removing the last node of the list, set the 'prev'
        pointer of the next node to refer to the 'prev' node of 'entry'. */
    if (entry->next != NULL)
        entry->next->prev = entry->prev;
        
    free(entry->value);  
    entry->value = NULL;
    
    free(entry);
    entry = NULL;
    
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
        if (strcmp(list_head->value, SENTINEL) != 0)
            printf("%s ", list_head->value);
            
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
        /*  If the 'value' is not pointing to the same chunk of memory
            as the sentinel value, there was memory allocated there so
            it should be freed. */
        if (list_head->value != SENTINEL)
            free(list_head->value);
            
        delete_list(list_head->next);
        free(list_head);
    }
}

/*  Hash function I found on the internet. Lost the original source but it is
    not mine. I changed "unsigned ch*/
unsigned long hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ( (c = (*str++)))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return (hash % TABLE_SIZE);
}

int main(void)
{
    struct node *table[TABLE_SIZE];
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table[i] = create_list();
        if (table[i] == NULL)
        {
            fprintf(stderr, "Failed to create list element at %d.\n", i);
            return 1;
        }
    }
    
    char name[NAME_LENGTH];
    printf("Add a name to the list or Ctrl+D to quit: ");
    while (scanf("%79s", name) != EOF)
    {
        int index = hash(name);
        insert_entry(table[index], name);
        printf("Add a name to the list or Ctrl+D to quit: ");
    } 
    
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        printf ("%d = ", i);
        print_list(table[i]);
    }
    
    printf("Remove a name from the list or Ctrl+D to quit: ");
    while (scanf("%79s", name) != EOF)
    {
        int index = hash(name);
        struct node *element = find_entry(table[index], name);
        if (element != NULL)
        {
            if (!remove_entry(element))
                fprintf(stderr, "Could not remove entry.\n");
        }
        else
            printf("There is no entry containing that value.\n");
        printf("Remove a name from the list or  Ctrl+D to quit: ");
    } 
    
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        printf ("%d = ", i);
        print_list(table[i]);
    }
    
    for (int i = 0; i < TABLE_SIZE; ++i)
        delete_list(table[i]);
    
    return 0;
}