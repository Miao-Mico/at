/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __CONTAINER_DEFINATION_H
#define __CONTAINER_DEFINATION_H

/*
*********************************************************************************************************
*                                          INCLUDE FILES
*********************************************************************************************************
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>
#include <malloc.h>

#include "allocator.h"

#include "algorithm.h"

/*
*********************************************************************************************************
*												DEFINES
*********************************************************************************************************
*/

/* Configure    container global size type.																*/
#define CONTAINER_GLOBAL_CFG_SIZE_TYPE															size_t

#pragma warning( disable : 4996)
#pragma warning( disable : 26812)

/*
*********************************************************************************************************
*									         DATA TYPES
*********************************************************************************************************
*/

/**
 * @brief This typedef is the container size type
 */

typedef CONTAINER_GLOBAL_CFG_SIZE_TYPE container_size_t;

/**
 * @brief This enum is the container's category enums
 */

enum container_category_e {
	SEQUENCE_CONTAINERS = 0x10,

	ASSOCIATIVE_CONTAINERS = 0x20,

	UNORDERED_ASSOCIATIVE_CONTAINERS = 0x30,

	CONTAINERS_ADAPTORS = 0x40,

	SPAN = 0x50,
};

/**
 * @brief This enum is the container's type enums
 */

enum container_type_e {
	ARRAY = SEQUENCE_CONTAINERS,
	VECTOR,
	DEQUE,
	FORWARD_LIST,
	LIST,

	SET = ASSOCIATIVE_CONTAINERS,
	MAP,

	UNORDERED_SET = UNORDERED_ASSOCIATIVE_CONTAINERS,

	STACK = CONTAINERS_ADAPTORS,
	QUEUE,
	PRIORITY_QUEUE,
};

/**
 * @brief This struct is the container's common information
 */

struct container_common_information_s {
	/* @brief This variables will record the size that each element will take up.						*/
	container_size_t mem_size;

	/* @brief This variables will record the maximum number of elements.								*/
	container_size_t max_size;

	/* @brief This variables will record the number of elements that
			  the container has currently allocated space for.											*/
	container_size_t size;
};

/**
 * @brief This struct is the container's generic element handler
 */

struct container_element_handler_s {
	/* @brief This variables will point to the address of the vector element assign handler.			*/
	void (*assign)(void *dst, void *src);

	/* @brief This variables will point to the address of the vector element free handler.				*/
	void (*free)(void *dst);
};

/**
 * @brief This struct is the container's common exception
 */

struct container_common_exception_s {
	/* @brief This variables will point to the address of the list empty exception handler.				*/
	void (*empty)(void);

	/* @brief This variables will point to the address of the list full exception handler.				*/
	void (*full)(void);
};

/**
 * @brief This struct is the container's common control
 */

struct container_control_s {
	struct {
		/* @brief This function will initialize the container struct and the specified container.		*/
		void (*init)(void **container,
					 container_size_t element_size,
					 void (*assign)(void *dst, void *src), void (*free)(void *dst));

		/* @brief This function will destroy the container struct. */
		void (*destroy)(void **container);
	}configuration;

	struct {
		/* @brief This function will returns a reference to the element
					at specified location position, with bounds checking.								*/
		void *(*at)(void *container, container_size_t position);
	}element_access;

	struct {
		/* @brief This function will check if the underlying container has no elements.					*/
		bool(*empty)(void *container);

		/* @brief This function will returns the number of elements in the container.					*/
		size_t(*size)(void *container);

		/* @brief This function will returns the maximum number of elements
					the container is able to hold due to system or library implementation limitations.	*/
		size_t(*max_size)(void *container);
	}capacity;

	struct {
		/* @brief This function will inserts elements at the specified location in the container.		*/
		void (*insert)(void *container,
					   container_size_t position,
					   container_size_t amount, void **source);

		/* @brief This function will erases the specified elements from the container.                  */
		void (*earse)(void *container,
					  container_size_t position);

	   /* @brief This function will exchange the contents of the container adaptor with those of other. */
		void (*swap)(void **container,
					 void **other);

		/* @brief This function will erase the specified elements from the container.					*/
		void (*copy)(void **destination,
					 void *source);
	}modifiers;
};

/*
*********************************************************************************************************
*								        FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/**
 * @brief This function will initialize the container struct and attach to the specified container.
 *
 * @param container the container adapter struct
 *
 * @return the pointer to the specified container function address table.
 */

inline void *container_control_convert_type_to_func_addr_table(enum container_type_e type);

/*
*********************************************************************************************************
*                                       EXTERN GLOBAL VARIABLES
*********************************************************************************************************
*/

#ifdef __ARRAY_H

/**
 * @brief This array will contain all the universal array functions address.
 */

extern void *array_function_address_tables[];

#endif // __ARRAY_H

#ifdef __VECTOR_H

/**
 * @brief This array will contain all the universal vector functions address.
 */

extern void *vector_function_address_tables[];

#endif // __VECTOR_H

#ifdef __FORWARD_LIST_H

/**
 * @brief This array will contain all the universal forward_list functions address.
 */

extern void *forward_list_function_address_tables[];

#endif // __FORWARD_LIST_H

#ifdef __LIST_H

/**
 * @brief This array will contain all the universal list functions address.
 */

extern void *list_function_address_tables[];

#endif // __LIST_H

/*
*********************************************************************************************************
*                                            FUNCTIONS
*********************************************************************************************************
*/

/**
 * @brief This function will initialize the container struct and attach to the specified container.
 *
 * @param container the container adapter struct
 *
 * @return the pointer to the specified container function address table.
 */

inline void *container_control_convert_type_to_func_addr_table(enum container_type_e type)
{
	void
		*func_addr_table = NULL;

	switch (type) {
		case ARRAY:
			#ifdef __ARRAY_H

			func_addr_table = &array_function_address_tables;

			#endif // __ARRAY_H
			break;
		case VECTOR:
			#ifdef __VECTOR_H

			func_addr_table = &vector_function_address_tables;

			#endif // __VECTOR_H
			break;
		case DEQUE:
			break;
		case FORWARD_LIST:
			#ifdef __FORWARD_LIST_H

			func_addr_table = &forward_list_function_address_tables;

			#endif // __FORWARD_LIST_H
			break;
		case LIST:
			#ifdef __LIST_H

			func_addr_table = &list_function_address_tables;

			#endif // __LIST_H
			break;
		case SET:
			break;
		case MAP:
			break;
		case UNORDERED_SET:
			break;
		default:
			break;
	}

	return func_addr_table;
}

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif // !__CONTAINER_DEFINATION_H