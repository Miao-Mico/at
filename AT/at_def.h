/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __AT_DEFINITION_H
#define __AT_DEFINITION_H

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "at_cfg.h"

/*
*********************************************************************************************************
*									            DEFINES
*********************************************************************************************************
*/

/**
 * @brief This typedef will extern the type of at.
 */

typedef AT_CFG_SIZE_TYPE at_size_t;

/**
 * @brief This typedef will extern the type of at.
 */

typedef AT_TASK_CFG_SIZE_TYPE at_task_size_t;

/**
 * @brief This typedef will extern the type of at.
 */

typedef void *(*at_import_func_t)(void* arg_list,...);

/**
 * @brief This typedef will extern the type of at.
 */

typedef struct at_s
*at_stp,
**at_stpp;

/**
 * @brief This typedef will extern the type of at feedback thread.
 */

typedef struct at_feedback_thread_s
*at_feedback_thread_stp,
**at_feedback_thread_stpp;

/**
 * @brief This struct will contain all the at task control functions.
 */

struct at_task_data_structure_package_s {
    struct {
        at_import_func_t init;
        at_import_func_t destroy;
    }configuration;

    struct {
        at_import_func_t top;
    }access;

    struct {
        at_import_func_t push;
        at_import_func_t pop;
    }modifiers;
};

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

#endif // !__AT_DEFINITION_H