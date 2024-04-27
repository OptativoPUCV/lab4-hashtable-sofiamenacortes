#include "hashmap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
  Pair **buckets;
  long size;     // cantidad de datos/pairs en la tabla
  long capacity; // capacidad de la tabla
  long current;  // indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value) {
  Pair *new = (Pair *)malloc(sizeof(Pair));
  new->key = key;
  new->value = value;
  return new;
}

long hash(char *key, long capacity) {
  unsigned long hash = 0;
  char *ptr;
  for (ptr = key; *ptr != '\0'; ptr++) {
    hash += hash * 32 + tolower(*ptr);
  }
  return hash % capacity;
}

int is_equal(void *key1, void *key2) {
  if (key1 == NULL || key2 == NULL)
    return 0;
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

void insertMap(HashMap *map, char *key, void *value) {
  int pos = hash(key, map->capacity);
  while(map->buckets[pos] != NULL){
    if(is_equal(map->buckets[pos]->key, key)){
      map->buckets[pos]->value = value;
      return;
    }
    pos++;
    if(pos == map->capacity){
      pos = 0;
    }
  }

  if(map->size == map->capacity){
    enlarge(map);
  }
  
  map->buckets[pos] = createPair(key, value);
  map->size++;
  
}

void enlarge(HashMap *map) { 
  
  enlarge_called = 1; // no borrar (testing purposes)
}

HashMap *createMap(long capacity) {
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
    map->size = 0;
    map->capacity = capacity;
    map->current = -1; 

    return map;
}

void eraseMap(HashMap *map, char *key) {
    
}

Pair *searchMap(HashMap *map, char *key) { 
    int pos = hash(key, map->capacity);
  while(map->buckets[pos] != NULL){
    if(is_equal(map->buckets[pos]->key, key)){
      map->current = pos;
      return map->buckets[pos];
    }
    pos++;
    if(pos == map->capacity){
      pos = 0;
    }
  }

  return NULL;
}

Pair *firstMap(HashMap *map) { 
    int i;
  for(i = 0; i < map->capacity; i++){
    if(map->buckets[i] != NULL){
      map->current = i;
      return map->buckets[i];
    }
  }
  return NULL;
}


Pair *nextMap(HashMap *map) { 
 int i;
  for(i = map->current + 1; i < map->capacity; i++)
    if(map->buckets[i] != NULL){
      map->current = i;
      return map->buckets[i];
    }

    for(i = 0; i < map->current; i++){
      if(map->buckets[i] != NULL){
        map->current = i;
        return map->buckets[i];
      }
    }

  return NULL;
}
