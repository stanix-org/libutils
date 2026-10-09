#ifndef LIBUTILS_LIST_H
#define LIBUTILS_LIST_H

#include <stddef.h>

typedef struct utils_list_node utils_list_node_t;
typedef struct utils_list utils_list_t;

struct utils_list_node {
	utils_list_node_t *prev;
	utils_list_node_t *next;
};

struct utils_list {
	utils_list_node_t *first;
	utils_list_node_t *last;
};

static inline void utils_list_init(utils_list_t *list) {
	list->first = NULL;
	list->last  = NULL;
}

static inline void utils_list_destroy(utils_list_t *list) {
	(void)list;
}

static inline void utils_list_prepend(utils_list_t *list, utils_list_node_t *node) {
	node->prev = NULL;
	node->next = list->first;
	if (list->first) {
		list->first->prev = node;
	} else {
		list->last = node;
	}
	list->first = node;
}

static inline void utils_list_append(utils_list_t *list, utils_list_node_t *node) {
	node->prev = list->last;
	node->next = NULL;
	if (list->last) {
		list->last->next = node;
	} else {
		list->first = node;
	}
	list->last = node;
}

void utils_list_add_after(utils_list_t *list, utils_list_node_t *node, utils_list_node_t *before);
void utils_list_remove(utils_list_t *list, utils_list_node_t *node);
utils_list_node_t *utils_list_pop_first(utils_list_t *list);
utils_list_node_t *utils_list_pop_last(utils_list_t *list);

static inline int utils_list_is_empty(utils_list_t *list) {
	return list->first != NULL;
}

static inline int utils_list_is_in(utils_list_t *list, utils_list_node_t *node) {
	if (node->prev || node->next) return 1;
	if (node == list->first || node == list->last) return 1;
	return 0;
}

#define utils_list_foreach(node, list) for (utils_list_node_t *node = (list)->first; node; node = node->next)
#define utils_list_reverse_foreach(node, list) for (utils_list_node_t *node = (list)->last; node; node = node->prev)

#endif
