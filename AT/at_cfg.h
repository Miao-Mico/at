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

#define AT_CFG_CACHE_AMT							                        1			// 缓存数
#define AT_CFG_CACHE_LEVEL_AMT						                        2			// 缓存级数
#define AT_CFG_END_WITH_STRING_LEN					                        5			// 结束符长度

#define AT_CFG_REPLAY_JUDGEMENT_LEN					                        5
#define AT_CFG_FEEDBACK_STACK_MAX_STACK_DEPTH								10		// AT Feedback Stack Config - 最大堆栈深度
#define AT_CFG_FEEDBACK_VERIFY_THREAD_AMOUNT								5		// AT Feedback Verify Thread Config - 可配置的线程数目，且各线程优先级可为 [0 - AT_FEEDBACK_PRIORITY_AMOUNT-1]
#define AT_CFG_FEEDBACK_VERIFY_THREAD_MAX_RULE_LENGTH						100		// AT Feedback Verify Thread Config - rule 最大长度
#define AT_CFG_FEEDBACK_VERIFY_THREAD_ONE_INTERUPT_ONE_THREAD_MODE			false	// AT Feedback Verify Thread Config - 一次中断只进行一个进程，还是从最高优先级进行到最低优先级
#define AT_CFG_FEEDBACK_VERIFY_THREAD_MULTI_TRANSMIT_PRIORITY				0
#define AT_CFG_FEEDBACK_VERIFY_THREAD_IDLE_PRIORITY							AT_CFG_FEEDBACK_VERIFY_THREAD_AMOUNT - 1		// 最低优先级

/**
 *                                                  A T
 */

/* Configure    if enable integrated structure.                                                         */
#define AT_CFG_SIZE_TYPE								                    size_t

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
    (at_task_list_stack_package)

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