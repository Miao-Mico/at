/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __AT_CONFIGURATION_H
#define __AT_CONFIGURATION_H

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

/* Data Structure Include file																		    */
#include "at_data_structure_template.h"

/*
*********************************************************************************************************
*									            DEFINES
*********************************************************************************************************
*/

/**
 *                                                  A T
 */

/* Configure    if enable integrated structure.                                                         */
#define AT_CFG_SIZE_TYPE								                    size_t

/* Configure    if enable integrated structure.                                                         */
#define AT_CFG_TRANSMIT_LEVEL_MAX							                3u

/* Configure    if enable integrated structure.                                                         */
#define AT_CFG_INTERGRATED_STRUCTURE_MODE_EN								1u

/* Configure    if enable integrated structure.                                                         */
#define AT_CFG_DEBUG_EN														0u

/**
 *                                          A T     D E V I C E
 */

/* Configure    if enable integrated structure.                                                         */
#define AT_DEVICE_CFG_INTERGRATED_STRUCTURE_MODE_EN							1u

/**
 *                                          A T     M E S S A G E
 */

/* Configure    if enable integrated structure.                                                         */
#define AT_MESSAGE_CFG_INTERGRATED_STRUCTURE_MODE_EN					    1u

/* Configure    if enable integrated structure.                                                         */
#define AT_MESSAGE_CFG_USER_DEFINED_MESSAGE_POOL_DATA_STRUCTURE_EN	        1u

#if (AT_MESSAGE_CFG_USER_DEFINED_MESSAGE_POOL_DATA_STRUCTURE_EN)

#define AT_MESSAGE_CFG_USER_DEFINED_TRANSMIT_MESSAGE_POOL_DATA_STRUCTURE                                \
    (at_list_queue_package)

#define AT_MESSAGE_CFG_USER_DEFINED_FEEDBACK_MESSAGE_POOL_DATA_STRUCTURE                                \
    (at_list_stack_package)

#endif // (AT_MESSAGE_CFG_USER_DEFINED_MESSAGE_POOL_DATA_STRUCTURE_EN)

/**
 *                                          A T     T A S K
 */

/* Configure    if enable integrated structure.                                                         */
#define AT_TASK_CFG_SIZE_TYPE								                unsigned short

/* Configure    if enable integrated structure.                                                         */
#define AT_TASK_CFG_INTERGRATED_STRUCTURE_MODE_EN							1u

/* Configure    if enable integrated structure.                                                         */
#define AT_TASK_CFG_PRIORITY_MAX											5u

/* Configure    if enable integrated structure.                                                         */
#define AT_TASK_CFG_IDLE_TASK_EN											1u

/* Configure    if enable at task os user-defined ready list data structure.                            */
#define AT_TASK_CFG_USER_DEFINED_READY_LIST_DATA_STRUCTURE_EN               1u

#if (AT_TASK_CFG_USER_DEFINED_READY_LIST_DATA_STRUCTURE_EN)

#define AT_TASK_CFG_USER_DEFINED_READY_LIST_DATA_STRUCTURE                                              \
    (at_list_stack_package)

#endif // (AT_TASK_CFG_USER_DEFINED_READY_LIST_DATA_STRUCTURE_EN)

/* Configure    if enable at task os user-defined message queue data structure.                         */
#define AT_TASK_CFG_USER_DEFINED_MESSAGE_QUEUE_DATA_STRUCTURE_EN            0u

#if (AT_TASK_CFG_USER_DEFINED_MESSAGE_QUEUE_DATA_STRUCTURE_EN)

#define AT_TASK_CFG_UD_MSG_QUEUE_DS_CONFIGRATION_INIT_FUNC_ADDR                                         \
    (NULL)

#define AT_TASK_CFG_UD_MSG_QUEUE_DS_CONFIGRATION_DESTROY_FUNC_ADDR                                      \
    (NULL)

#define AT_TASK_CFG_UD_MSG_QUEUE_DS_MODIFIERS_PUSH_FUNC_ADDR                                            \
    (NULL)

#define AT_TASK_CFG_UD_MSG_QUEUE_DS_MODIFIERS_POP_FUNC_ADDR                                             \
    (NULL)

#endif // (AT_TASK_CFG_USER_DEFINED_MESSAGE_QUEUE_DATA_STRUCTURE_EN)

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

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif // !__AT_CONFIGURATION_H