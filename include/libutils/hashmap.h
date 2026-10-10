#ifndef LIBUTILS_HASHMAP_H
#define LIBUTILS_HASHMAP_H

#include "vector.h"
#include "macro.h"
#include <stdlib.h>
#include <stdint.h>

typedef unsigned int utils_hash_t;
typedef utils_hash_t (*utils_hash_func_t)(const void *data);
typedef int (*utils_cmp_func_t)(const void *a, const void *b);
typedef void (*utils_destroy_func_t)(void *data);

typedef struct utils_hashmap_entry {
	void *key;
	void *element;
} utils_hashmap_entry_t;

typedef struct utils_hashmap {
	utils_vector_t *vectors;
	size_t capacity;
	utils_hash_func_t hash;
	utils_cmp_func_t cmp;
	utils_destroy_func_t destroy_key;
	utils_destroy_func_t destroy_element;
} utils_hashmap_t;

static inline size_t utils_hashmap_hash(utils_hashmap_t *hashmap, const void *key) {
	return (size_t)hashmap->hash(key) % hashmap->capacity;
}

static inline size_t utils_hashmap_equal(utils_hashmap_t *hashmap, const void *a, const void *b) {
	return hashmap->cmp(a, b) == 0;
}

static inline void utils_hashmap_destroy_key(utils_hashmap_t *hashmap, void *key) {
	if (hashmap->destroy_key) {
		hashmap->destroy_key(key);
	}
}

static inline void utils_hashmap_destroy_element(utils_hashmap_t *hashmap, void *element) {
	if (hashmap->destroy_element) {
		hashmap->destroy_element(element);
	}
}

static inline int utils_hashmap_init(utils_hashmap_t *hashmap, size_t capacity, utils_hash_func_t hash, utils_cmp_func_t cmp, utils_destroy_func_t destroy_key, utils_destroy_func_t destroy_element) {
	hashmap->vectors = malloc(sizeof(utils_vector_t) * capacity);
	if (!hashmap->vectors) return -1;
	memset(hashmap->vectors, 0, sizeof(utils_vector_t) * capacity);
	hashmap->capacity = capacity;
	hashmap->hash = hash;
	hashmap->cmp  = cmp;
	hashmap->destroy_key     = destroy_key;
	hashmap->destroy_element = destroy_element;
	return 0;
}

static inline void utils_hashmap_destroy(utils_hashmap_t *hashmap) {
	if (!hashmap->vectors) return;
	for (size_t i = 0; i < hashmap->capacity; i++) {
		if (hashmap->vectors[i].count == 0) continue;
		utils_vector_typed_foreach (utils_hashmap_entry_t, entry, &hashmap->vectors[i]) {
			utils_hashmap_destroy_key(hashmap, entry->key);
			utils_hashmap_destroy_element(hashmap, entry->element);
		}
		utils_vector_destroy(&hashmap->vectors[i]);
	}
	free(hashmap->vectors);
	hashmap->vectors = NULL;
	hashmap->capacity = 0;
}

static inline void *utils_hashmap_get(utils_hashmap_t *hashmap, const void *key) {
	utils_vector_t *vector = &hashmap->vectors[utils_hashmap_hash(hashmap, key)];
	utils_vector_typed_foreach (utils_hashmap_entry_t, entry, vector) {
		if (utils_hashmap_equal(hashmap, entry->key, key)) {
			return entry->element;
		}
	}
	return NULL;
}

static inline void utils_hashmap_add(utils_hashmap_t *hashmap, const void *key, const void *element) {
	utils_vector_t *vector = &hashmap->vectors[utils_hashmap_hash(hashmap, key)];
	if (vector->count == 0) {
		utils_vector_init(vector, sizeof(utils_hashmap_entry_t));
	}
	utils_vector_typed_foreach (utils_hashmap_entry_t, entry, vector) {
		if (utils_hashmap_equal(hashmap, entry->key, key)) {
			utils_hashmap_destroy_element(hashmap, entry->element);
			entry->element = (void*)element;
			return;
		}
	}
	utils_hashmap_entry_t entry = {
		.key = (void*)key,
		.element = (void*)element,
	};
	utils_vector_push_back(vector, &entry);
}


static inline int utils_hashmap_remove(utils_hashmap_t *hashmap, const void *key) {
	utils_vector_t *vector = &hashmap->vectors[utils_hashmap_hash(hashmap, key)];
	utils_vector_typed_indexed_foreach (utils_hashmap_entry_t, entry, i, vector) {
		if (utils_hashmap_equal(hashmap, entry->key, key)) {
			utils_hashmap_destroy_key(hashmap, entry->key);
			utils_hashmap_destroy_element(hashmap, entry->element);
			if (i != vector->count - 1) memcpy(entry, utils_vector_at(vector, vector->count-1), sizeof(utils_hashmap_entry_t));
			utils_vector_pop_back(vector, NULL);
			if (vector->count == 0) utils_vector_destroy(vector);
			return 1;
		}
	}
	return 0;
}


// foreach macro from hell
#define utils_hashmap_foreach(_key, _element, _hashmap) for(size_t i=0; i<(_hashmap)->capacity; i++) \
for (size_t j=0; j<(_hashmap)->vectors[i].count; j++) \
for (long _key      = ((utils_hashmap_entry_t*)((_hashmap)->vectors[i].data))[j].key    , _1=1; _1; _1=0)\
for (void *_element = ((utils_hashmap_entry_t*)((_hashmap)->vectors[i].data))[j].element, *_2=(void*)1; _2; _2=NULL)

#endif
