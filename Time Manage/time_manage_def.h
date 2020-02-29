/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __TIME_MANAGE_DEFINITION_H
#define __TIME_MANAGE_DEFINITION_H

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include "time_manage_cfg.h"

/*
*********************************************************************************************************
*									            DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*									           DATA TYPES
*********************************************************************************************************
*/

/**
 * @brief This type is typedef of the time manage size type definition.
 */

typedef TIME_MANAGE_CFG_SIZE_TYPE time_manage_size_t;

/**
 * @brief This type is typedef of the time manage float type definition.
 */

typedef TIME_MANAGE_CFG_FLOAT_TYPE time_manage_float_t;

/**
 * @brief This typedef is the time manage hardware counter function.
 */

typedef void (*time_manage_timer_counter_t)(void *operator);

/**
 * @brief This type is the body of the time frame struct.
 */

typedef struct time_manage_visual_time_frame_s {
    time_manage_size_t year;
    time_manage_size_t month;
    time_manage_size_t week;
    time_manage_size_t day;
    time_manage_size_t hour;
    time_manage_size_t minue;
    time_manage_size_t second;
    time_manage_size_t millisec;
    time_manage_size_t microsec;
}time_manage_time_frame_st;

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

#endif // !__TIME_MANAGE_DEFINITION_H