/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __AT_DATA_STRUCTURE_TEMPLATE_H
#define __AT_DATA_STRUCTURE_TEMPLATE_H

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "at_def.h"

/* Data Structure Include file																		    */
#include "stack.h"
#include "queue.h"
#include "priority_queue.h"
#include "red_black_tree.h"

/*
*********************************************************************************************************
*									            DEFINES
*********************************************************************************************************
*/

/* Configure    if enable debug.                                                                        */
#define AT_TASK_DATA_STRUCTURE_CFG_DEBUG_EN										    0u

/*
*********************************************************************************************************
*									           DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*								            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       EXTERN GLOBAL VARIABLES
*********************************************************************************************************
*/

#ifdef __STACK_H

/**
 * @brief This struct will contain all the at task list stack control functions.
 */

extern struct at_data_structure_control_package_s at_list_stack_control_package;

#else

#error  "Please transplant a good data structure package fit the data structure that at needs \
            They are the stack!"

#endif // __STACK_H

#ifdef __QUEUE_H

/**
 * @brief This struct will contain all the at task list stack control functions.
 */

extern struct at_data_structure_control_package_s at_list_queue_control_package;

#else

#error  "Please transplant a good data structure package fit the data structure that at needs \
            They are the queue!"

#endif // __QUEUE_H

#ifdef __PRIORITY_QUEUE_H

/**
 * @brief This struct will contain all the at task list stack control functions.
 */

extern struct at_data_structure_control_package_s at_list_priority_queue_control_package;

#else

#error  "Please transplant a good data structure package fit the data structure that at needs \
            They are the priority queue!"

#endif // __PRIORITY_QUEUE_H

#ifdef __RED_BLACK_TREE_H

/**
 * @brief This struct will contain all the at task list stack control functions.
 */

extern struct at_data_structure_control_package_s at_red_black_tree_control_package;

#else

#error  "Please transplant a good data structure package fit the data structure that at needs \
            They are the red black tree!"

#endif // __RED_BLACK_TREE_H

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif // !__AT_DATA_STRUCTURE_TEMPLATE_H