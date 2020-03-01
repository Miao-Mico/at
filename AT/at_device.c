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

	//.inquire.package_device = at_device_control_inquire_package_device,
	//.inquire.package_size = at_device_control_inquire_package_size,
    //
	//.package_operator.configuration.mount = at_device_control_package_operator_configuration_mount,
	//.package_operator.configuration.demount = at_device_control_package_operator_configuration_demount,
	//.package_operator.transmit.send = at_device_control_package_operator_transmit_send,
	//.package_operator.transmit.receive = at_device_control_package_operator_transmit_receive,
	//.package_operator.verify.device = at_device_control_package_operator_verify_device,
	//.package_operator.verify.package = at_device_control_package_operator_verify_package,
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
 * @brief This function will mount the device package.
 *
 * @param void
 *
 * @return void
 */

extern inline errno_t
at_device_control_configuration_mount_package(struct at_device_package_s *package,
											  void *arg_list)
{
	assert(package);

	//package->configuration.init(package, arg_list);

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
at_device_control_configuration_unmount_package(struct at_device_package_s *package)
{
	assert(package);

	free(package);

	package = NULL;

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

/**
 * @brief This function will inquire the device of the device package.
 *
 * @param void
 *
 * @return void
 */

extern inline void
*at_device_control_inquire_package_device(struct at_device_package_s *package)
{
	return package->device_ptr;
}

/**
 * @brief This function will inquire the size of the data package.
 *
 * @param void
 *
 * @return void
 */

extern inline at_size_t
at_device_control_inquire_package_size(void)
{
	return sizeof(struct at_device_package_s);
}

/**
 * @brief This function will mount the device to the package.
 *
 * @param void
 *
 * @return void
 */

extern inline errno_t
at_device_control_package_operator_configuration_mount(struct at_device_package_s *package,
													   void *arg_list)
{
	assert(package);

	package->configuration.mount(package, arg_list);

	return 0;
}

/**
 * @brief This function will demount the device from the package.
 *
 * @param void
 *
 * @return void
 */

extern inline errno_t
at_device_control_package_operator_configuration_demount(struct at_device_package_s *package)
{
	assert(package);

	package->configuration.demount(package);

	return 0;
}

/**
 * @brief This function will control the send/output of the device i/o.
 *
 * @param void
 *
 * @return void
 */

extern inline errno_t
at_device_control_package_operator_transmit_send(struct at_device_package_s *package,
												 void *data, size_t len)
{
	assert(package);
	assert(data);

	package->transmit.send(package->device_ptr,
						   data, len);

	return 0;
}

/**
 * @brief This function will control the receive/input of the device i/o.
 *
 * @param void
 *
 * @return void
 */

extern inline void
*at_device_control_package_operator_transmit_receive(struct at_device_package_s *package,
													 size_t len)
{
	assert(package);

	return package->transmit.receive(package->device_ptr, len);
}

/**
 * @brief This function will verify if the device is valid.
 *
 * @param void
 *
 * @return void
 */

extern inline bool
at_device_control_package_operator_verify_device(struct at_device_package_s *package,
												 void *device)
{
	assert(package);
	assert(device);

	return package->verify.device(package->device_ptr);
}

/**
 * @brief This function will verify if the device package is valid.
 *
 * @param void
 *
 * @return void
 */

extern inline bool
at_device_control_package_operator_verify_package(struct at_device_package_s *package)
{
	assert(package);

	return package->verify.package(package);
}