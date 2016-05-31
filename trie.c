/*  trie.c
    Implementation of a trie.
    
    insert_entry()  - O(1)
    find_entry()    - O(1)
    remove_entry()  - O(1)
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRIE_PATHS 26
#define INPUT_LENGTH 80

struct trie
{
	bool is_data;
	struct trie *paths[TRIE_PATHS];
};

/*  Returns a pointer to an empty trie or NULL if trie was unable to be
    created. */
struct trie* create_trie()
{
    struct trie *new_trie = malloc(sizeof(struct trie));
    if (new_trie == NULL)
    {
        printf("Unable to allocate memory for a new trie.\n");
        return NULL;
    }
    
    new_trie->is_data = false;
    
    for (size_t i = 0; i < TRIE_PATHS; ++i)
        new_trie->paths[i] = NULL;
    
    return new_trie;
}

/*  Returns the alphabetical index of a character if character is indeed
    alphabetical. For example, a = 0, b = 1, ... z = 25. Otherwise, returns -1.
    to indicate a non-alphabetic character was passed. */
int get_alphabetical_index(char c)
{
    if (isalpha(c))
    {
        if (isupper(c))
            c = tolower(c);
            
        return c % 'a';
    }
    else
        return -1;
}

/*  Inserts a new entry into 'trie_root'. Returns true if successful, otherwise
    false. This function runs in O(1) time, as the amount of steps to insert
    an entry is not dependent on the amount of data already present in the
    root of the trie, but rather the size of 'value'. */
bool insert_entry(struct trie *trie_root, char *value)
{
    /*  Check if string 'value' now empty. If so, the end of a path has been
        reached so the current 'trie_root' can be marked as now containing
        data. */
    if (strlen(value) == 0)
    {
        trie_root->is_data = true;
        return true;
    }
    
    /*  Get corresponding index based on alphabetical character. Return false
        if the index is not successfully calculated. */
    int index = get_alphabetical_index(*value);
    if (index < 0)
        return false;
    
    /*  If a trie does not already exist at the next necessary path, attempt to
        create it. Return false if a new trie is not successfully created. */
    if (trie_root->paths[index] == NULL)
        if ((trie_root->paths[index] = create_trie()) == NULL)
            return false;
    
    /*  If the trie was created successfully, call the function again using the
        newly created path trie as the new root and the next character appearing
        in the string. */
    return insert_entry(trie_root->paths[index], value+1);
}

/*  Checks to see if a given 'value' is present within 'trie_root'. If it does,
    returns a pointer to the trie element that marks the data as valid. 
    Otherwise, returns NULL. This function runs in O(1) time, as the amount of
    steps to find an entry is not dependent on the amount of data already present 
    in the root of the trie, but rather the size of 'value'. */
struct trie* find_entry(struct trie *trie_root, char *value)
{
    /*  Check if string 'value' is now empty. If it is, the end of a path has
        been found and 'trie_root->is_data' would be true if 'value' was
        previously inserted. */
    if (strlen(value) == 0)
    {
        if (trie_root->is_data)
            return trie_root;
        else
            return NULL;
    }
    
    /*  Get corresponding index based on alphabetical character. Return NULL
        if the index is not successfully calculated. */
    int index = get_alphabetical_index(*value);
    if (index < 0)
        return NULL;
    
    /*  Check if a trie exists at that index in 'trie_root'. If it doesn't,
        return false. Otherwise, enter it and continue searching with the
        next character in 'value'. */
    if (trie_root->paths[index] == NULL)
        return NULL;
    else
        return find_entry(trie_root->paths[index], value+1);
}

/*  Removes the corresponding 'value' from 'trie_root' if it is present. 
    Returns true if successful, otherwise false which indicates 
    non-existing entry. This function runs in O(1) time, as the amount of
    steps to remove an entry is not dependent on the amount of data already 
    present in the root of the trie, but rather the size of 'value'.
    
    An important thing to note is this function merely removes the flag
    indicating if a trie has valid data. It does not actually deallocate
    any memory that would no longer be in use. */
bool remove_entry(struct trie *trie_root, char *value)
{
    if ((trie_root = find_entry(trie_root, value)) != NULL)
    {
        trie_root->is_data = false;
        return true;
    }
    else
        return false;
}

/*  Recursively frees all tries connected to a 'trie_root'. */
void delete_trie(struct trie *trie_root)
{
    if (trie_root != NULL)
    {
        for (size_t i = 0; i < TRIE_PATHS; ++i)
        {
            if (trie_root->paths[i] != NULL)
                delete_trie(trie_root->paths[i]);
        }

        free(trie_root);
        trie_root = NULL;
    }
}

int main(void)
{
    struct trie *trie_root = create_trie();
    char input[INPUT_LENGTH];
    
    printf("Add a word to the trie or Ctrl+D to quit: ");
    while (scanf("%79s", input) != EOF)
    {
        if (insert_entry(trie_root, input) == false)
            printf("Failed to add %s to trie. Make sure string only contains alphabetic characters.\n", input);
        printf("Add a word to the trie or Ctrl+D to quit: ");
    } 

    printf("\nSearch for a word in the trie or Ctrl+D to quit: ");
    while (scanf("%79s", input) != EOF)
    {
        if (find_entry(trie_root, input) == false)
            printf("Can't find %s in trie.\n", input);
        printf("Search for a word in the trie or Ctrl+D to quit: ");
    } 
    
    printf("\nRemove a word from the trie or Ctrl+D to quit: ");
    while (scanf("%79s", input) != EOF)
    {
        if (remove_entry(trie_root, input) == false)
            printf("Can't remove %s from trie.\n", input);
        printf("Remove a word from the trie or Ctrl+D to quit: ");
    } 
    
    printf("\nSearch for a word in the trie or Ctrl+D to quit: ");
    while (scanf("%79s", input) != EOF)
    {
        if (find_entry(trie_root, input) == false)
            printf("Can't find %s in trie.\n", input);
        printf("Search for a word in the trie or Ctrl+D to quit: ");
    } 
    
    printf("\n");
    
    delete_trie(trie_root);
    return 0;
}