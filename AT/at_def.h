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
#include <stdarg.h>
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
 * @brief This type is the typedef of the at transmit message group.
 */

struct at_message_transmit_group_s {
	at_size_t count;

	at_size_t len[AT_CFG_TRANSMIT_LEVEL_MAX];
	char *pool[AT_CFG_TRANSMIT_LEVEL_MAX];
};

/**
 * @brief This typedef will extern the type of at.
 */

typedef struct at_s
*at_stp,
**at_stpp;

/**
 * @brief This struct will contain all the universal device functions.
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

	at_import_func_t interrupt;
};

/**
 * @brief This typedef will extern the type of at.
 */

struct at_device_package_interrupt_return_s {
	size_t count;

	char *string;
};

/**
 * @brief This typedef will extern the type of at.
 */

typedef errno_t(*at_device_package_packer_func_t)(struct at_device_package_s **package,
												  void *arg_list);

/**
 * @brief This struct will contain all the at task control functions.
 */

struct at_data_structure_control_package_s {
	struct {
		errno_t(*init)(void **data_structure);
		errno_t(*destroy)(void **data_structure);
	}configuration;

	struct {
		at_size_t(*size)(void *data_structure);
	}capacity;

	union {
		struct {
			void *(*search)(void *data_structure,
							at_size_t key);
		}lookup;

		struct {
			void *(*at)(void *data_structure,
						...);
		}element_access;
	};

	struct {
		errno_t(*insert)(void *data_structure,
						 void *element,
						 ...);
		errno_t(*delete)(void *data_structure,
						 ...);
	}modifiers;
};

/**
 * @brief This struct will contain all the at task control functions.
 */

struct at_data_structure_package_s {
	struct at_data_structure_control_package_s *control_ptr;

	void *data_structure_ptr;
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