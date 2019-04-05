#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));
  ht->storage = calloc(capacity, sizeof(LinkedPair *));
  ht->capacity = capacity;

  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  unsigned int ht_index = hash(key, ht->capacity);
  LinkedPair *new_item = create_pair(key, value);
  // make a pointer node to keep track of where we are
  LinkedPair *list_item = ht->storage[ht_index];

  if (ht->storage[ht_index] == NULL) {
    ht->storage[ht_index] = new_item;
  } else {
    // replace the value if the keys match
    while (list_item->next != NULL) {
      if (strcmp(list_item->key, new_item->key) == 0) {
        printf("overwriting value\n");
        strcpy(list_item->value, new_item->value);
        return;
      }
      list_item = list_item->next;
    }
    // check the last item key in the list
    if (strcmp(list_item->key, new_item->key) == 0) {
      printf("overwriting value\n");
      strcpy(list_item->value, new_item->value);
      return;
    }
    list_item->next = new_item;
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int ht_index = hash(key, ht->capacity);
  // make a pointer node
  LinkedPair *hash_list = ht->storage[ht_index];

  // if there no list at that index
  if (ht->storage[ht_index] == NULL) {
    printf("Entry not found.");
  }
  // if the first item matches but has a next node
  if (strcmp(hash_list->key, key) == 0) {
    LinkedPair *temp = hash_list->next;
    ht->storage[ht_index] = hash_list->next;
    // destroy node
    destroy_pair(temp);
  }
  // otherwise we have to search the list
  while (hash_list->next != NULL) {
    // since we already checked the first item
    // we can consider the current item as 'prev item'
    if (strcmp(hash_list->next->key, key) == 0) {
      LinkedPair *temp = hash_list->next;
      hash_list->next = hash_list->next->next;
      // destroy node
      destroy_pair(temp);
      return;
    } // should have checked every item in the list
    printf("Entry not found.");
    hash_list = hash_list->next;
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  unsigned int ht_index = hash(key, ht->capacity);

  // make a pointer node
  LinkedPair *hash_list = ht->storage[ht_index];

  // if there is no list at that index
  if (hash_list == NULL) {
    printf("Entry not found.\n");
    return NULL;
  }

  while (hash_list->next != NULL) {
    printf("e\n");

    if (strcmp(hash_list->key, key) == 0) {
      printf("matched keys\n");
      return hash_list->value;
    }
    printf("f\n");

    hash_list = hash_list->next;
  } // should have checked every item but the last in the list

  if (strcmp(hash_list->key, key) == 0) {
    printf("last item matched keys\n");
    return hash_list->value;
  } // should have checked every item in the list
  printf("no matching items\n");
  return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
  // go through every index of ht
  for (int i = 0; i < ht->capacity; i++) {
    LinkedPair *temp = ht->storage[i];
    // check if index is empty
    if (ht->storage[i] != NULL) {
      // remove all items of linked list
      while (temp->next != NULL) {
        ht->storage[i] = temp->next;
        destroy_pair(temp);
        temp = temp->next;
      }
      destroy_pair(ht->storage[i]);
    }
  }

  free(ht);
}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  printf("resizing\n");

  HashTable *new_ht = create_hash_table(ht->capacity * 2);

  // we have to remove every item from the old table
  // rehash the keys for the new size
  // and destroy the old table
  // go through every index of ht
  printf("a\n");

  for (int i = 0; i < ht->capacity; i++) {
    // check if index is empty
    printf("looping through capacity\n");
    LinkedPair *item = ht->storage[i];

    printf("there is a linked list\n");

    // remove all items of linked list
    while (item != NULL) {
      printf("looping through the items\n");
      LinkedPair *temp = item;

      hash_table_insert(new_ht, strdup(item->key), strdup(item->value));
      ht->storage[i] = item->next;
      item = item->next;
      destroy_pair(temp);
    }
    destroy_pair(item);
  }
  printf("destroying old table");
  destroy_hash_table(ht);
  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
