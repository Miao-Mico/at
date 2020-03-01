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

typedef void *(*at_import_func_t)(void *arg_list, ...);

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
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_device_package_s {
	/* @brief This variables will point to the device. */
	at_size_t device_pack_id;

	/* @brief This variables will point to the device. */
	void *device_ptr;

	struct {
		/* @brief This function will mount the device. */
		at_import_func_t mount;

		 /* @brief This function will demount the device to be default. */
		at_import_func_t demount;
	}configuration;

	struct {
		/* @brief This function will initialize the device. */
		at_import_func_t send;

		/* @brief This function will initialize the device to be default. */
		at_import_func_t receive;
	}transmit;

	struct {
		/* @brief This function will verify the device if valid. */
		at_import_func_t device;

		/* @brief This function will verify the device if valid. */
		at_import_func_t package;
	}verify;
};

/**
 * @brief This typedef will extern the type of at.
 */

typedef errno_t(*at_device_package_packer_func_t)(struct at_device_package_s **package,
												  void *arg_list);

/**
 * @brief This struct will contain all the at task control functions.
 */

struct at_task_data_structure_package_s {
	struct {
		at_import_func_t init;
		at_import_func_t destroy;
	}configuration;

	struct {
		at_import_func_t size;
	}capacity;

	struct {
		at_import_func_t top;
	}element_access;

	struct {
		at_import_func_t push;
		at_import_func_t pop;
	}modifiers;
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_timeout_package_s {
	struct {
		at_import_func_t init;

		at_import_func_t destroy;
	}configuration;

	at_import_func_t set;

	at_import_func_t inquire;
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

/**
 * @brief This function will initialize the device package.
 *
 * @param void
 *
 * @return void
 */

errno_t at_device_control_configuration_init_package(struct at_device_package_s **package);

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