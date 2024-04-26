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
  if (map == NULL || key == NULL)
    return;
  if (map->size == map->capacity)
    enlarge(map);
  long posicion = hash(key, map->capacity);
  while (map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL)
    posicion = (posicion + 1) % map->capacity;

  if (map->buckets[posicion] == NULL || map->buckets[posicion]->key == NULL) {
    map->buckets[posicion] = createPair(key, value);
    map->size++;
  } 
    
}

void enlarge(HashMap *map) { //buckets = arreglo de pares
  long old_capacity = map->capacity;
  Pair **old_buckets = map->buckets; //un puntero a un puntero

  map->capacity *= 2;
  map->size = 0;
  
  map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));

  for(long i = 0; i < old_capacity; i++)
    {
      Pair *currentpair = old_buckets[i];
      while(currentpair != NULL)
        {
          insertMap(map, currentpair->key, currentpair->value);
          Pair *temp = currentpair;
          currentpair = currentpair->next;
          free(temp);
        }
    }
  free(old_buckets);
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
  if (map == NULL || key == NULL)
    return;

  long posicion = hash(key, map->capacity);
  Pair *currentpair = map->buckets[posicion];
  Pair *previo = NULL;

  while (currentpair != NULL && strcmp(currentpair->key, key) != 0)
  {
    previo = currentpair;
    currentpair = currentpair->next;
  }

  if (currentpair != NULL && strcmp(currentpair->key, key) == 0){
  if(previo == NULL)
  {
    map->buckets[posicion] = currentpair->next;
  }
  else{
    previo->next = currentpair->next;
  }

    free(currentpair->next);
    free(currentpair->value);
    free(currentpair);
  }
  
}

Pair *searchMap(HashMap *map, char *key) { 
  if (map == NULL || key == NULL)
    return NULL;
  
  long posicion = hash(key, map->capacity);
  Pair *currentpair = map->buckets[posicion];
  
  while (currentpair != NULL && strcmp(currentpair->key, key) != 0)
    {
      currentpair = currentpair->next;
    }

  return currentpair;
}

Pair *firstMap(HashMap *map) { 
  if (map == NULL)
    return NULL;

  for(long i = 0; i < map->capacity; i++){
    if(map->buckets[i] != NULL)
      return map->buckets[i];
  }
  return NULL; 
}

Pair *nextMap(HashMap *map) { 
  if (map == NULL)
    return NULL;

  for(long i = map->current + 1; i < map->capacity; i++){
    if(map->buckets[i] != NULL){
      map->current = i;
      return map->buckets[i];
    }
  }

  map->current = -1;
  return NULL; 
}
