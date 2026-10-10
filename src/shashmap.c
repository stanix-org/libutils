#include <libutils/shashmap.h>
#include <string.h>

utils_hash_t utils_shashmap_hash(const void *data) {
	const char *string = data;
    const utils_hash_t p = 31;
    const utils_hash_t m = 1e9 + 9;
    utils_hash_t hash_value = 0;
    utils_hash_t p_pow = 1;
    while (*string) {
        hash_value = (hash_value + *string * p_pow) % m;
        p_pow = (p_pow * p) % m;
		string++;
    }
	return hash_value;
}

int utils_shashmap_cmp(const void *a, const void *b) {
	return strcmp(a, b);
}
