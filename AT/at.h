/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __AT_H
#define __AT_H

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include <stdio.h>

#include <stdbool.h>

#include "stack.h"

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
#define AT_CFG_FEEDBACK_VERIFY_THREAD_IDLE_PRIORITY							AT_FEEDBACK_VERIFY_THREAD_CFG_AMOUNT - 1		// 最低优先级

/*
*********************************************************************************************************
*									           DATA TYPES
*********************************************************************************************************
*/

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_control_s {

};

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

#endif // !__AT_H