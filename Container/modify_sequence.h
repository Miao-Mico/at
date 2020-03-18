/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __MODIFY_SEQUENCE_H
#define __MODIFY_SEQUENCE_H

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "algorithm_def.h"

/*
*********************************************************************************************************
*									            DEFINES
*********************************************************************************************************
*/

/* Configure        if enable modify sequence debug.													*/
#define MODIFY_SEQUENCE_CFG_DEBUG_EN										1u

/* Define			the definition of swap algorithm.													*/
#define MODIFY_SEQUENCE_CONTROL_SWAP(x,y)		(x) = (x) ^ (y),(y) = (x) ^ (y),(x) = (x) ^ (y)

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

/**
 * @brief This function will swap the left-hand-side with the right-hand-side.
 *
 * @param lhs the pointer to the left-hand-side item
 * @param rhs the pointer to the right-hand-side item
 *
 * @return the error code
 */

errno_t modify_sequence_control_swap(void *lhs, void *rhs, size_t len);

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

#endif // !__MODIFY_SEQUENCE_H