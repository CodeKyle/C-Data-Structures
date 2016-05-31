/*  queue.c
    Implementation of a queue. The first element of a queue
    contains a sentinel value that cannot be changed or deleted until 
    program termination. This causes additional memory to be used but allowed
    me to more easily insert and remove nodes since there is always guaranteed
    to be at least one element in a list. 

    enqueue()  - O(1)
    dequeue()  - O(1)
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

/*  Creates an empty queue. Returns a pointer to a node element whose 'value'
    is equal to SENTINEL and whose 'next' pointer is equal to NULL. SENTINEL is
    used as a sentinel value and is treated as if it were a NULL element. */
struct node* create_queue()
{
    struct node* new_queue = malloc(sizeof(struct node));
    if (new_queue == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a new queue.\n");
        return NULL;
    }
    
    *new_queue = (struct node){SENTINEL, NULL, NULL};
    return new_queue;
}

/*  Inserts a new node whose value is 'value' to the end of the queue. This
    function runs in O(1) time. */
bool enqueue(struct node* queue_head, int value)
{
    if (value == SENTINEL)
    {
        printf("Cannot manually insert sentinel value into queue.\n");
        return false;
    }
    
    struct node *new_entry = malloc(sizeof(struct node));
    if (new_entry == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a new node.\n");
        return false;
    }
    
    *new_entry = (struct node){value, NULL, NULL};
    
    /*  If there are no nodes in the queue (excluding the node with the
        sentinel value) then the 'next' pointer of 'queue_head' should point
        to this new entry. */
    if (queue_head->next == NULL)
    {
        queue_head->next = new_entry;
        queue_head->prev = new_entry;
        new_entry->prev = queue_head;
    }
    /*  If there are other nodes in the queue, get the current tail, then
        point 'next' of 'tail' to the new entry and update other pointers
        accordingly as well. */
    else
    {
        struct node *tail = queue_head->prev;
        
        tail->next = new_entry;
        new_entry->prev = tail;
        
        queue_head->prev = new_entry;
    }
    
    return true;
}

/*  Returns the 'value' of the node that has been in queue the longest.
    This function runs in O(1) time. */
int dequeue(struct node* queue_head)
{
    struct node *dequeue_entry = queue_head->next;
    
    if (dequeue_entry == NULL)
    {
        printf("No values to dequeue from queue.\n");
        return SENTINEL;
    }
    
    int value = dequeue_entry->value;
    
    queue_head->next = dequeue_entry->next;
    dequeue_entry->prev = queue_head;
    free(dequeue_entry);

    return value;
}

void print_queue(struct node *queue_head)
{
    /*  If queue only contains the default sentinel value, the queue is
        considered to be empty. */
    if (queue_head->next == NULL)
    {
        printf("Empty queue.\n");
        return;
    }
    
    while (queue_head != NULL)
    {
        //  If value is not the sentinel value, print it out.
        if (queue_head->value != SENTINEL)
            printf("%i ", queue_head->value);
            
        //  Fetch the next node in the queue.
        queue_head = queue_head->next;
    }
    
    printf("\n");
}

/*  Recursively frees all nodes connected to a queue starting with
    'queue_head'. */
void delete_queue(struct node* queue_head)
{
    if (queue_head != NULL)
    {
        delete_queue(queue_head->next);
        free(queue_head);
    }
}

int main(void)
{
    struct node *queue_head = create_queue();

    int value = 0;
    while (value >= 0)
    {
        printf("Add a positive number to the queue or a negative number to quit: ");
        if ( (scanf("%i", &value) == 1) && value >= 0)
             enqueue(queue_head, value);
        clear_input(stdin);
        //print_queue(queue_head);
    }    
    
    value = 0;
    
    char input = 0;
    while (input != 'q')
    {
        printf("Type anything to dequeue or 'q' to quit: ");
        if ( (scanf("%c", &input) == 1) && input != 'q')
        {
            int dequeued = dequeue(queue_head);
            if (dequeued != SENTINEL)
                printf("Dequeued a %i!\n", dequeued);
        }
             
        clear_input(stdin);
        //print_queue(queue_head);
    }

    delete_queue(queue_head);
    
    return 0;
}