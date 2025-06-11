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
// A lightweight and type-safe dynamic array implementation for C.
// This version uses macros to generate type-specific vectors, so
// you get better type safety without losing simplicity.
//
// Features:
// - vec_<T>_init:      Initialize a new vector.
// - vec_<T>_capacity:  Get current capacity.
// - vec_<T>_len:       Get current number of elements.
// - vec_<T>_resize:    Resize internal buffer.
// - vec_<T>_ensure:    Ensure capacity for N extra items.
// - vec_<T>_push:      Push a value to the end.
// - vec_<T>_set:       Set value at index.
// - vec_<T>_get:       Get value at index.
// - vec_<T>_for_each:  Iterate with a callback.
// - vec_<T>_clear:     Reset length to 0.
// - vec_<T>_destroy:   Free memory (+ optional cleanup).
//
// Usage:
// ----------------------------------------
// DEFINE_VECTOR(int, int); // Defines `vector_int_t`
// vector_int_t v;
// vec_int_init(&v, 16, 2.0); // growth_factor = 2.0
// vec_int_push(&v, 42);
// int val = vec_int_get(&v, 0);
// vec_int_destroy(&v, NULL);
//
// Pro tip: Use `generic` for `void*` style:
//     vec_generic_push(&v, ptr); // Works with malloc-ed ptrs
//
// All vectors follow the same pattern, just swap `int`, `float`, etc.
// ----------------------------------------
// Initial revision: 2025-05-24
// Last updated:     2025-06-05
// ----------------------------------------
// Depends on: stdlib.h, stdio.h
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
        vector->capacity = new_capacity;                   \
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
                                                           \
        vector->length = 0;                                \
        vector->capacity = 0;                              \
    }                                                      \
                                                           \
    static inline V vec_##NAME##_pop(                      \
        vector_##NAME##_t *vector                          \
    )                                                      \
    {                                                      \
        if (vector->length == 0)                           \
        {                                                  \
            fprintf(stderr, "Error: Cannot pop from empty vector (fluent_libc: vector_t)\n"); \
            exit(1);                                       \
        }                                                  \
                                                           \
        vector->length--;                                  \
        return vector->data[vector->length];               \
    }                                                      \

#ifndef FLUENT_LIBC_VECTOR_GENERIC_DEFINED
#   define FLUENT_LIBC_VECTOR_GENERIC_DEFINED 1
    DEFINE_VECTOR(void *, generic);
#endif

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
}
#endif

#endif //FLUENT_LIBC_VECTOR_LIBRARY_H