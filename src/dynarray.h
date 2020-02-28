#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stdlib.h>
#include <string.h>
#include <memory.h>

#define DEF_CAP 100

#define DEF_DYN_ARRAY(T) \
typedef struct T##_Array_t { \
	T* data; \
	int len, cap; \
} T##Array; \
void T##Array_new(T##Array* arr) { \
	arr->data = (T*) malloc(sizeof(T) * DEF_CAP); \
	arr->cap = DEF_CAP; \
	arr->len = 0; \
} \
void T##Array_free(T##Array* arr) { \
	free(arr->data); \
	arr->data = NULL; \
} \
void T##Array_push(T##Array* arr, T val) { \
	if (arr->len >= arr->cap) { \
		arr->cap *= 2; \
		arr->data = (T*) realloc(arr->data, arr->cap * sizeof(T)); \
	} \
	arr->data[arr->len++] = val; \
}

#endif // DYNARRAY_H