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
// Vector API
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
// void vec_init(Vector *vector, const size_t initial_capacity, const size_t el_size);
//     Example:
//         Vector v;
//         vec_init(&v, 8, sizeof(int *));
//
// size_t vec_capacity(const Vector *vector);
//     Example:
//         size_t cap = vec_capacity(&v);
//
// size_t vec_len(const Vector *vector);
//     Example:
//         size_t len = vec_len(&v);
//
// void vec_resize(Vector *vector, const size_t new_capacity);
//     Example:
//         vec_resize(&v, 32);
//
// void vec_ensure(Vector *vector, const size_t n);
//     Example:
//         vec_ensure(&v, 10);
//
// void vec_push(Vector *vector, void *value);
//     Example:
//         int *x = malloc(sizeof(int));
//         *x = 42;
//         vec_push(&v, x);
//
// void vec_set(const Vector *vector, const size_t index, void *value);
//     Example:
//         int *y = malloc(sizeof(int));
//         *y = 99;
//         vec_set(&v, 0, y);
//
// void *vec_get(const Vector *vector, const size_t index);
//     Example:
//         int *z = (int *)vec_get(&v, 0);
//
// void vec_for_each(const Vector *vector, void (*func)(void *));
//     Example:
//         void print_int(void *value)
//         {
//             printf("%d\n", *(int *)value);
//         }
//         vec_for_each(&v, print_int);
//
// void vec_clear(Vector *vector);
//     Example:
//         vec_clear(&v);
//
// void vec_destroy(Vector *vector);
//     Example:
//         vec_destroy(&v, NULL);
// ----------------------------------------
// Initial revision: 2025-05-24
// ----------------------------------------
// Depends on: stdlib.h
// ----------------------------------------

#ifndef VECTOR_LIBRARY_H
#define VECTOR_LIBRARY_H

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

// == MACROS ==
/**
 * @brief Macro to initialize a vector of a given type, name, and capacity.
 *
 * This macro declares a Vector variable with the specified name and
 * initializes it with the given capacity and element type size.
 * The growth factor is set to 2.0 by default.
 *
 * @param type The data type of the elements to be stored in the vector.
 * @param name The name of the Vector variable to declare.
 * @param cap  The initial capacity of the vector.
 *
 * Example:
 *     VEC_INIT(int *, my_vector, 16);
 */
#define VEC_INIT(type, name, cap) \
    Vector name; \
    vec_init(&name, cap, sizeof(type), 2.0)

/**
 * @brief Dynamic array structure for generic pointers.
 *
 * Represents a resizable array that stores pointers to elements.
 * - length:   Current number of elements in the vector.
 * - el_size:  Size of each element in bytes.
 * - capacity: Allocated capacity of the vector.
 * - growth_factor: Factor by which the vector grows when resized.
 * - data:     Pointer to the array of element pointers.
 */
typedef struct
{
    size_t length;
    size_t el_size;
    size_t capacity;
    double growth_factor;
    void **data; // WARNING: Do not manipulate directly, use vec_set and vec_get instead
} Vector;

/**
 * @brief Initializes a dynamic vector with a specified initial capacity.
 *
 * Allocates memory for the vector's internal data array and sets the initial
 * length to zero. If memory allocation fails, prints an error message and exits.
 *
 * @param vector Pointer to the Vector structure to initialize.
 * @param el_size Size of each element in bytes.
 * @param growth_factor The factor by which the vector grows when resized.
 * @param initial_capacity The initial number of elements the vector can hold.
 */
inline void vec_init(
    Vector *vector,
    const size_t initial_capacity,
    const size_t el_size,
    const double growth_factor
)
{
    // Make sure the initial capacity is not 0
    if (initial_capacity == 0)
    {
        fprintf(stderr, "Error: Initial capacity cannot be 0 (fluent_libc: Vector)\n");
        exit(1);
    }

    // Initialize the vector with the given capacity
    vector->length = 0;
    vector->capacity = initial_capacity;
    vector->el_size = el_size;
    vector->growth_factor = growth_factor;

    // Allocate the data array
    vector->data = (void **)malloc(sizeof(void *) * initial_capacity);

    // Handle failure
    if (vector->data == NULL)
    {
        perror("malloc");
        exit(1);
    }
}

/**
 * @brief Returns the current capacity of the vector.
 *
 * @param vector Pointer to the Vector structure.
 * @return The number of elements the vector can currently hold.
 */
inline size_t vec_capacity(const Vector *vector)
{
    return vector->capacity;
}

/**
 * @brief Returns the current number of elements in the vector.
 *
 * @param vector Pointer to the Vector structure.
 * @return The number of elements currently stored in the vector.
 */
inline size_t vec_len(const Vector *vector)
{
    return vector->length;
}

/**
 * @brief Resizes the vector to a new capacity.
 *
 * Reallocates the internal data array to the specified new capacity.
 * Does not perform bounds checking; shrinking the capacity to zero may
 * result in undefined behavior. If memory allocation fails, frees the
 * original data, prints an error message, and exits the program.
 *
 * @param vector Pointer to the Vector structure to resize.
 * @param new_capacity The new capacity for the vector.
 */
inline void vec_resize(Vector *vector, const size_t new_capacity)
{
    // WARNING: Does not do bound-checking, shrinking memory to 0
    // might result in undefined behavior.
    // -----------------------------------------------------------
    // Reallocate the buffer to the new capacity
    // Cast to (void **) to avoid C++ warnings/errors
    void **new_data = (void **)realloc(vector->data, vector->el_size * new_capacity);

    // Handle failure
    if (new_data == NULL)
    {
        free(vector->data);
        perror("realloc");
        exit(1);
    }

    // Set the new data pointer
    vector->data = new_data;
}

/**
 * @brief Ensures the vector has enough capacity for additional elements.
 *
 * Checks if the vector can accommodate `n` more elements. If not, resizes
 * the vector by doubling its current capacity.
 *
 * @param vector Pointer to the Vector structure.
 * @param n The number of additional elements to ensure capacity for.
 */
inline void vec_ensure(Vector *vector, const size_t n)
{
    // Ensure the vector has enough capacity for n elements
    if (vector->length + n > vector->capacity)
    {
        vec_resize(vector, vector->capacity * vector->growth_factor);
    }
}

/**
 * @brief Appends an element to the end of the vector.
 *
 * Ensures there is enough capacity for the new element, then adds the given value
 * to the end of the vector and increments the length.
 *
 * @param vector Pointer to the Vector structure.
 * @param value Pointer to the element to be added.
 */
inline void vec_push(Vector *vector, void *value)
{
    // Ensure we have enough capacity
    vec_ensure(vector, 1);

    // Set the value at the end of the vector
    vector->data[vector->length] = value;
    vector->length++;
}

/**
 * @brief Sets the value at a specific index in the vector.
 *
 * Replaces the element at the given index with the provided value.
 * Exits the program if the index is out of bounds.
 *
 * @param vector Pointer to the Vector structure.
 * @param index The index at which to set the value.
 * @param value Pointer to the new value to set.
 */
inline void vec_set(const Vector *vector, const size_t index, void *value)
{
    // Make sure the index is valid
    if (index >= vector->length)
    {
        fprintf(stderr, "Error: Index out of bounds (fluent_libc: Vector)\n");
        exit(1);
    }

    // Set the value at the given index
    vector->data[index] = value;
}

/**
 * @brief Gets the value at a specific index in the vector.
 *
 * Returns a pointer to the element at the given index.
 * Exits the program if the index is out of bounds.
 *
 * @param vector Pointer to the Vector structure.
 * @param index The index from which to get the value.
 * @return Pointer to the value at the specified index.
 */
inline void *vec_get(const Vector *vector, const size_t index)
{
    // Make sure the index is valid
    if (index >= vector->length)
    {
        fprintf(stderr, "Error: Index out of bounds (fluent_libc: Vector)\n");
        exit(1);
    }

    // Return the value at the given index
    return vector->data[index];
}

/**
 * @brief Iterates over each element in the vector and applies a callback.
 *
 * Calls the provided callback function for every element in the vector,
 * passing the element pointer and its index as arguments.
 *
 * @param vector Pointer to the Vector structure to iterate over.
 * @param callback Function pointer to call for each element. The function
 *        should accept a void pointer to the element and its index.
 */
    inline void vec_for_each(
        const Vector *vector,
        void (*callback)(const void *value, size_t index)
    )
{
    // Iterate over each element in the vector
    for (size_t i = 0; i < vector->length; i++)
    {
        // Call the callback function with the value and index
        callback(vector->data[i], i);
    }
}

/**
 * @brief Clears the vector by resetting its length to zero.
 *
 * This does not free any memory or deallocate the internal data array.
 * The vector remains allocated with the same capacity, but all elements
 * are considered removed.
 *
 * @param vector Pointer to the Vector structure to clear.
 */
inline void vec_clear(Vector *vector) {
    vector->length = 0;
}

/**
 * @brief Frees all memory used by the vector and resets its fields.
 *
 * Releases the memory allocated for the vector's internal data array,
 * sets the data pointer to NULL to avoid dangling references, and
 * resets the length and capacity to zero.
 *
 * @param vector Pointer to the Vector structure to destroy.
 * @param free_fn Optional function to free each element in the vector.
 */
inline void vec_destroy(
    Vector *vector,
    void (*free_fn)(const void *, size_t)
)
{
    // Call the free function for each element if it exists
    if (free_fn != NULL)
    {
        for (size_t i = 0; i < vector->length; i++)
        {
            free_fn(vector->data[i], i);
        }
    }

    // Free the data array
    free(vector->data);
    vector->data = NULL; // Set to NULL to avoid dangling pointer

    // Reset the vector
    vector->length = 0;
    vector->capacity = 0;
}

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
}
#endif

#endif //VECTOR_LIBRARY_H