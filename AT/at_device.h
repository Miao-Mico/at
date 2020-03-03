/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This definition header file is protected from multiple pre-processor inclusion
*               through use of the definition module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __AT_DEVICE_H
#define __AT_DEVICE_H

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "at_def.h"

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
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_device_control_s {
	struct {
		errno_t(*init_package)(struct at_device_package_s **package);

		errno_t(*deinit_package)(struct at_device_package_s *package);

		errno_t(*destroy_package)(struct at_device_package_s **package);
	}configuration;
};

/*
*********************************************************************************************************
*								            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/**
 * @brief This function will deinitialize the i/o stream of the windows.
 *
 * @param void
 *
 * @return void
 */

errno_t at_device_control_configuration_deinit_package(struct at_device_package_s *package);

/**
 * @brief This function will destroy the i/o stream of the windows.
 *
 * @param void
 *
 * @return void
 */

errno_t at_device_control_configuration_destroy_package(struct at_device_package_s **package);

/*
*********************************************************************************************************
*                                       EXTERN GLOBAL VARIABLES
*********************************************************************************************************
*/

/**
 * @brief This variables will obtain the operator of the at device.
 */

struct at_device_control_s at_device_ctrl;

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif // !__AT_DEVICE_H