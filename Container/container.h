/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __CONTAINER_H
#define __CONTAINER_H

/*
*********************************************************************************************************
*                                          INCLUDE FILES
*********************************************************************************************************
*/

#include "container_cfg.h"

#if (CONTAINER_FAMILY_LEVEL_CFG_BINARY_TREE_EN)

#include "binary_search_tree.h"

#endif // (CONTAINER_FAMILY_LEVEL_CFG_BINARY_TREE_EN)

#if (CONTAINER_FAMILY_LEVEL_CFG_RED_BLACK_TREE_EN)

#include "red_black_tree.h"

#endif // (CONTAINER_FAMILY_LEVEL_CFG_RED_BLACK_TREE_EN)

#if (CONTAINER_FAMILY_LEVEL_CFG_B_TREE_EN)

#include "b_tree.h"

#endif // (CONTAINER_FAMILY_LEVEL_CFG_B_TREE_EN)

#if (CONTAINER_FAMILY_LEVEL_CFG_ARRAY_EN)

#include "array.h"

#endif // (CONTAINER_FAMILY_LEVEL_CFG_ARRAY_EN)

#if (CONTAINER_FAMILY_LEVEL_CFG_VECTOR_EN)

#include "vector.h"

#endif // (CONTAINER_FAMILY_LEVEL_CFG_VECTOR_EN)

#if (CONTAINER_FAMILY_LEVEL_CFG_FORWARD_LIST_EN)

#include "forward_list.h"

#endif // (CONTAINER_FAMILY_LEVEL_CFG_FORWARD_LIST_EN)

#if (CONTAINER_FAMILY_LEVEL_CFG_LIST_EN)

#include "list.h"

#endif // (CONTAINER_FAMILY_LEVEL_CFG_LIST_EN)

#if (CONTAINER_GLOBAL_TYPE_CFG_STACK_EN)

#include "stack.h"

#endif // (CONTAINER_GLOBAL_TYPE_CFG_STACK_EN)

#if (CONTAINER_GLOBAL_TYPE_CFG_QUEUE_EN)

#include "queue.h"

#include "priority_queue.h"

#endif // (CONTAINER_GLOBAL_TYPE_CFG_QUEUE_EN)

/*
*********************************************************************************************************
*									      CONFIG DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*									           DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*								         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       EXTERN GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif // !__CONTAINER_H