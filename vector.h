/*
 * This code is distributed under the terms of the GNU General Public License.
 * For more information, please refer to the LICENSE file in the root directory.
 * -------------------------------------------------
 * Copyright (C) 2025 Rodrigo R.
 * This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions; type `show c' for details.
*/

// ============= FLUENT LIB C =============
// vector_t API
// ----------------------------------------
// A dynamic array implementation for C.
// This library provides a simple and efficient
// way to manage dynamic arrays without memory
// leaks or manual memory management.
// ----------------------------------------
// While it is not the most efficient
// implementation, it is simple and easy to
// use.
//
// Features:
// - vec_init:      Initialize a new vector.
// - vec_capacity:  Get the current capacity of the vector.
// - vec_len:       Get the current length of the vector.
// - vec_resize:    Resize the vector to a new capacity.
// - vec_ensure:    Ensure the vector has enough capacity for n elements.
// - vec_push:      Append an element to the end of the vector.
// - vec_set:       Set the value at a specific index.
// - vec_get:       Get the value at a specific index.
// - vec_for_each:  Iterate over each element in the vector.
// - vec_clear:     Clear the vector without allocating new memory.
// - vec_destroy:   Free all memory used by the vector.
// Function Signatures:
// ----------------------------------------
// void vec_init(vector_t *vector, const size_t initial_capacity, const size_t el_size);
//     Example:
//         vector_t v;
//         vec_init(&v, 8, sizeof(int *));
//
// size_t vec_capacity(const vector_t *vector);
//     Example:
//         size_t cap = vec_capacity(&v);
//
// size_t vec_len(const vector_t *vector);
//     Example:
//         size_t len = vec_len(&v);
//
// void vec_resize(vector_t *vector, const size_t new_capacity);
//     Example:
//         vec_resize(&v, 32);
//
// void vec_ensure(vector_t *vector, const size_t n);
//     Example:
//         vec_ensure(&v, 10);
//
// void vec_push(vector_t *vector, void *value);
//     Example:
//         int *x = malloc(sizeof(int));
//         *x = 42;
//         vec_push(&v, x);
//
// void vec_set(const vector_t *vector, const size_t index, void *value);
//     Example:
//         int *y = malloc(sizeof(int));
//         *y = 99;
//         vec_set(&v, 0, y);
//
// void *vec_get(const vector_t *vector, const size_t index);
//     Example:
//         int *z = (int *)vec_get(&v, 0);
//
// void vec_for_each(const vector_t *vector, void (*func)(void *));
//     Example:
//         void print_int(void *value)
//         {
//             printf("%d\n", *(int *)value);
//         }
//         vec_for_each(&v, print_int);
//
// void vec_clear(vector_t *vector);
//     Example:
//         vec_clear(&v);
//
// void vec_destroy(vector_t *vector);
//     Example:
//         vec_destroy(&v, NULL);
// ----------------------------------------
// Initial revision: 2025-05-24
// ----------------------------------------
// Depends on: stdlib.h
// ----------------------------------------

#ifndef FLUENT_LIBC_VECTOR_LIBRARY_H
#define FLUENT_LIBC_VECTOR_LIBRARY_H

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
extern "C" // C++ linkage
{
#endif

#include <stdio.h>

// == DEFINITIONS ==
typedef unsigned long int size_t; // size_type

// == IMPORTS ==
#include <stdlib.h> // For malloc and free

#define DEFINE_VECTOR(V, NAME)                             \
    typedef struct                                         \
    {                                                      \
        size_t length;                                     \
        size_t capacity;                                   \
        double growth_factor;                              \
        V *data;                                           \
    } vector_##NAME##_t;                                   \
                                                           \
    static inline void vec_##NAME##_init(                  \
        vector_##NAME##_t *vector,                         \
        const size_t initial_capacity,                      \
        const double growth_factor                         \
    )                                                      \
    {                                                      \
        if (initial_capacity == 0)                         \
        {                                                  \
            fprintf(stderr, "Error: Initial capacity cannot be 0 (fluent_libc: vector_t)\n"); \
            exit(1);                                       \
        }                                                  \
                                                           \
        vector->length = 0;                                \
        vector->capacity = initial_capacity;               \
        vector->growth_factor = growth_factor;             \
        vector->data = (V *)malloc(sizeof(V) * initial_capacity); \
                                                           \
        if (vector->data == NULL)                          \
        {                                                  \
            fprintf(stderr, "Error: Out of memory (fluent_libc: vector_t)\n"); \
            exit(1);                                       \
        }                                                  \
    }                                                      \
                                                           \
    static inline size_t vec_##NAME##_capacity(            \
        vector_##NAME##_t *vector                          \
    )                                                      \
    {                                                      \
        return vector->capacity;                           \
    }                                                      \
                                                           \
    static inline size_t vec_##NAME##_len(                 \
        vector_##NAME##_t *vector                          \
    )                                                      \
    {                                                      \
        return vector->length;                             \
    }                                                      \
                                                           \
    static inline void vec_##NAME##_resize(                \
        vector_##NAME##_t *vector,                         \
        const size_t new_capacity                          \
    )                                                      \
    {                                                      \
        V* new_data = (V *)realloc(vector->data, sizeof(V) * new_capacity); \
                                                           \
        if (new_data == NULL)                              \
        {                                                  \
            free(vector->data);                            \
            fprintf(stderr, "Error: Out of memory (fluent_libc: vector_t)\n"); \
            exit(1);                                       \
        }                                                  \
                                                           \
        vector->data = new_data;                           \
    }                                                      \
                                                           \
    static inline void vec_##NAME##_ensure(                \
        vector_##NAME##_t *vector,                         \
        const size_t n                                     \
    )                                                      \
    {                                                      \
        if (vector->length + n > vector->capacity)         \
        {                                                  \
            vec_##NAME##_resize(vector, vector->capacity * vector->growth_factor); \
        }                                                  \
    }                                                      \
                                                           \
    static inline void vec_##NAME##_push(                  \
        vector_##NAME##_t *vector,                         \
        V value                                            \
    )                                                      \
    {                                                      \
        vec_##NAME##_ensure(vector, 1);                    \
        vector->data[vector->length] = value;              \
        vector->length++;                                  \
    }                                                      \
                                                           \
    static inline void vec_##NAME##_set(                   \
        vector_##NAME##_t *vector,                         \
        const size_t index,                                \
        V value                                            \
    )                                                      \
    {                                                      \
        if (index >= vector->length)                       \
        {                                                  \
            fprintf(stderr, "Error: Index out of bounds (fluent_libc: vector_t)\n"); \
            exit(1);                                       \
        }                                                  \
                                                           \
        vector->data[index] = value;                       \
    }                                                      \
                                                           \
    static inline V vec_##NAME##_get(                      \
        vector_##NAME##_t *vector,                         \
        const size_t index                                 \
    )                                                      \
    {                                                      \
        if (index >= vector->length)                       \
        {                                                  \
            fprintf(stderr, "Error: Index out of bounds (fluent_libc: vector_t)\n"); \
            exit(1);                                       \
        }                                                  \
                                                           \
        return vector->data[index];                        \
    }                                                      \
                                                           \
    static inline void vec_##NAME##_for_each(              \
        vector_##NAME##_t *vector,                         \
        void (*callback)(const V value, size_t index)      \
    )                                                      \
    {                                                      \
        for (size_t i = 0; i < vector->length; i++)        \
        {                                                  \
            callback(vector->data[i], i);                  \
        }                                                  \
    }                                                      \
                                                           \
    static inline void vec_##NAME##_clear(                 \
        vector_##NAME##_t *vector,                         \
        void (*callback)(const V value, size_t index)      \
    )                                                      \
    {                                                      \
        vector->length = 0;                                \
    }                                                      \
                                                           \
    static inline void vec_##NAME##_destroy(               \
        vector_##NAME##_t *vector,                         \
        void (*free_fn)(const V, size_t)                   \
    )                                                      \
    {                                                      \
        if (free_fn != NULL)                               \
        {                                                  \
            for (size_t i = 0; i < vector->length; i++)    \
            {                                              \
                free_fn(vector->data[i], i);               \
            }                                              \
        }                                                  \
                                                           \
        free(vector->data);                                \
        vector->data = NULL;                               \
        vector->data = NULL;                               \
                                                           \
        vector->length = 0;                                \
        vector->capacity = 0;                               \
    }

#ifndef FLUENT_LIBC_VECTOR_GENERIC_DEFINED
#   define FLUENT_LIBC_VECTOR_GENERIC_DEFINED 1
    DEFINE_VECTOR(void *, generic);
#endif

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
}
#endif

#endif //FLUENT_LIBC_VECTOR_LIBRARY_H