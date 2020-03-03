/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "at_device.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_device_control_s at_device_ctrl = {
	.configuration.init_package = at_device_control_configuration_init_package,
	.configuration.deinit_package = at_device_control_configuration_deinit_package,
	.configuration.destroy_package = at_device_control_configuration_destroy_package,
};

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*					LOCAL GLOBAL VARIABLES & LOCAL FUNCTION PROTOTYPES INTERSECTION
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            FUNCTIONS
*********************************************************************************************************
*/

/**
 * @brief This function will initialize the device package.
 *
 * @param void
 *
 * @return void
 */

extern inline errno_t
at_device_control_configuration_init_package(struct at_device_package_s **package)
{
	assert(package);

	void *block = calloc(1, sizeof(struct at_device_package_s));

	if (NULL == block) {
		return 1;
	}

	*package = block;

	return 0;
}

/**
 * @brief This function will deinitialize the device package.
 *
 * @param void
 *
 * @return void
 */

extern inline errno_t
at_device_control_configuration_deinit_package(struct at_device_package_s *package)
{
	assert(package);

	memset(package, '0', sizeof(struct at_device_package_s));

	return 0;
}

/**
 * @brief This function will destroy the i/o stream of the windows.
 *
 * @param void
 *
 * @return void
 */

extern inline errno_t
at_device_control_configuration_destroy_package(struct at_device_package_s **package)
{
	assert(package);
	assert(*package);

	free((*package));

	(*package) = NULL;

	return 0;
}