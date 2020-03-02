/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "at.h"

#include "at_device.h"
#include "at_message.h"
#include "at_task.h"

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

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_information_s {
	bool switch_status;
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_exception_s {
	void (*error)(void);
};

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_s {
	struct at_information_s info;

	struct at_device_package_s *device_package;

	struct at_exception_s exception;
};

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

struct at_control_s at_ctrl = {
	.configuration.init = at_control_configuration_init,
	.configuration.destroy = at_control_configuration_destroy,
	.configuration.retarget = at_control_configuration_retarget
};

struct at_task_s *at_task_os_multi_level_transmit_task = NULL;

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/**
 * @brief This function will be called when the multi level transmit task run.
 *
 * @param void
 *
 * @return void
 */

void at_control_at_task_os_multi_level_transmit_task_function(void);

/*
*********************************************************************************************************
*                                            FUNCTIONS
*********************************************************************************************************
*/

/**
 * @brief This function will initialize the at struct.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_configuration_init(struct at_s **at,
									  struct at_device_package_s *device_package)
{
	assert(at);
	assert(device_package);

	if (NULL == device_package ||																				/* If the device_package is NULL */
		NULL == ((*at) = calloc(1, sizeof(struct at_s)))) {
		//(*at)->exception.hardware_not_ready();

		return 1;
	}

	at_message_ctrl.configuration.init();																		/* Initialize the at message */

	at_task_ctrl.os.configuration.init();																		/* Initialize the at task os */

	at_task_ctrl.task.configuration.init(&at_task_os_multi_level_transmit_task,									/* Initialize the multi level transmit task */
										 "at task os.multi level transmit task",
										 at_control_at_task_os_multi_level_transmit_task_function,
										 0,
										 NULL,
										 NULL,
										 0);

	if (at_control_configuration_retarget(*at, NULL, NULL, device_package)) {									/* Target the at to the device_package */
		return 2;
	}

	(*at)->info.switch_status = true;

	return 0;
}

/**
 * @brief This function will destroy the at struct.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_configuration_destroy(struct at_s **at)
{
	assert(at);
	assert(*at);

	if (at_message_ctrl.configuration.destroy()) {											/* Destroy the at message */
		return 1;
	}

	if (at_task_ctrl.os.configuration.destroy()) {											/* Destroy the at tack os */
		return 2;
	}

	free(*at);

	(*at) = NULL;

	return 0;
}

/**
 * @brief This function will retarget the device_package of at struct to the device_package specified.
 *
 * @param void
 *
 * @return void
 */

errno_t at_control_configuration_retarget(struct at_s *at,
										  void *device,
										  void *arg_list,
										  struct at_device_package_s *device_package)
{
	assert(at);

	void *devide_package_cpy = NULL;

	if (NULL == device_package &&
		NULL == (device_package = at->device_package)) {			/* If not appoint the device_package pack */
		return 1;
	}

	if (!device_package->verify.package(device_package)) {			/* If device package is not valid */
		return 2;
	}

	if (NULL != at->device_package &&
		device_package->device_pack_id ==
		at->device_package->device_pack_id) {						/* If device_pack is the same as the device_pack of at */
		goto CONFIG_DEVICE_PART;
	} else {
		free(at->device_package);									/* Deallocate the outdated device package */
	}

	if (NULL == (devide_package_cpy =
				 calloc(1, sizeof(struct at_device_package_s))))	/* Allocate the space for device package copy */
	{
		return -1;
	}

	memcpy(devide_package_cpy, device_package,
		   sizeof(struct at_device_package_s));						/* Copy a new device package */

	at->device_package = devide_package_cpy;

CONFIG_DEVICE_PART:

	if (NULL != device &&
		NULL != arg_list) {
		errno_t err = 0;

		if (err = (errno_t)device_package->configuration.
			mount(device_package, arg_list)) {
			return err;
		}
	}

	return 0;
}

/**
 * @brief This function will be called when the multi level transmit task run.
 *
 * @param void
 *
 * @return void
 */

void at_control_at_task_os_multi_level_transmit_task_function(void)
{
	printf("at task os.multi level transmit task\r\n");
}