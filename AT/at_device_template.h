/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __AT_DEVICE_TEMPLATE_H
#define __AT_DEVICE_TEMPLATE_H

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "at_def.h"

#include "windows.h"

/*
*********************************************************************************************************
*									            DEFINES
*********************************************************************************************************
*/

/* Configure    if enable integrated structure.                                                         */
#define AT_DEVICE_TEMPLATE_CFG_WINDOWS_FILE_STREAM_ID								         1

/*
*********************************************************************************************************
*									           DATA TYPES
*********************************************************************************************************
*/

/**
 * @brief This struct will pack the device package to manage input/output in different platform.
 */

struct at_device_package_packer_s {
	#ifdef _WIN32

	/* @brief This function will initialize the i/o stream of the windows.                              */
	at_device_package_packer_func_t windows_file_stream;

	#else

	#error "Please transplant a good device package fit your platform!"

	#endif // __PLATFORM
};

/*
*********************************************************************************************************
*								            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#ifdef _WIN32

/**
 * @brief This function will package the operator of the windows file i/o stream into the package.
 *
 * @param void
 *
 * @return void
 */

errno_t at_device_package_packer_windows_file_stream(struct at_device_package_s **package,
													 void *arg_list);

#endif // _WIN32

/*
*********************************************************************************************************
*                                       EXTERN GLOBAL VARIABLES
*********************************************************************************************************
*/

/**
 * @brief This struct will pack the device package to manage input/output in different platform.
 */

extern struct at_device_package_packer_s at_device_package_packer;

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif // !__AT_DEVICE_TEMPLATE_H