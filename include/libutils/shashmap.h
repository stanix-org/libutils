#ifndef LIBUTILS_SHASHMAP_H
#define LIBUTILS_SHASHMAP_H

#include "hashmap.h"
#include <string.h>

typedef utils_hashmap_t utils_shashmap_t;

utils_hash_t utils_shashmap_hash(const void *data);
int utils_shashmap_cmp(const void *a, const void *b);

static inline int utils_shashmap_init(utils_shashmap_t *shashmap, size_t capacity, utils_destroy_func_t destroy_element) {
	return utils_hashmap_init(shashmap, capacity, utils_shashmap_hash, utils_shashmap_cmp, free, destroy_element);
}

static inline void utils_shashmap_destroy(utils_shashmap_t *shashmap) {
	utils_hashmap_destroy(shashmap);
}

static inline void *utils_shashmap_get(utils_shashmap_t *shashmap, const char *key) {
	return utils_hashmap_get(shashmap, key);
}

static inline void utils_shashmap_add(utils_shashmap_t *shashmap, const char *key, const void *element) {
	char *dup_key = strdup(key);
	if (!dup_key) return; // TODO : report error??
	utils_hashmap_add(shashmap, dup_key, element);
}

static inline int utils_shashmap_remove(utils_shashmap_t *shashmap, const char *key) {
	return utils_hashmap_remove(shashmap, key);
}

#endif
