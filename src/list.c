#include <libutils/list.h>

void utils_list_add_after(utils_list_t *list, utils_list_node_t *node, utils_list_node_t *before) {
	if (!before) {
		utils_list_prepend(list, node);
		return;
	}

	node->prev = before;
	node->next = before->next;
	if (before->next) {
		before->next->prev = node;
	} else {
		list->last = node;
	}
	before->next = node;
}

void utils_list_remove(utils_list_t *list, utils_list_node_t *node) {
	if (node->prev) {
		node->prev->next = node->next;
	} else {
		list->first = node->next;
	}
	if (node->next) {
		node->next->prev = node->prev;
	} else {
		list->last = node->prev;
	}
	node->prev = NULL;
	node->next = NULL;
}

utils_list_node_t *utils_list_pop_first(utils_list_t *list) {
	utils_list_node_t *node = list->first;
	if (!node) return NULL;
	list->first = node->next;
	if (list->first) {
		list->first->prev = NULL;
	} else {
		list->last = NULL;
	}
	return node;
}

utils_list_node_t *utils_list_pop_last(utils_list_t *list) {
	utils_list_node_t *node = list->last;
	if (!node) return NULL;
	list->last = node->prev;
	if (list->last) {
		list->last->next = NULL;
	} else {
		list->first = NULL;
	}
	return node;
}
