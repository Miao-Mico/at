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

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_device_package_s {
	/* @brief This variables will point to the device. */
	void *device_ptr;

	struct {
		/* @brief This function will mount the device. */
		errno_t(*mount)(struct at_device_package_s *package,
						void *arg_list);

		 /* @brief This function will demount the device to be default. */
		errno_t(*demount)(struct at_device_package_s *package);
	}configuration;

	struct {
		/* @brief This function will initialize the device. */
		errno_t(*send)(void *device,
					   void *data, size_t len);

		/* @brief This function will initialize the device to be default. */
		void *(*receive)(void *device, size_t len);
	}transmit;

	struct {
		/* @brief This function will verify the device if valid. */
		bool (*device)(void *device);

		/* @brief This function will verify the device if valid. */
		bool (*package)(struct at_device_package_s *package);
	}verify;
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

/**
 * @brief This struct will contain all the universal vector functions address.
 */

struct at_device_control_s at_device_ctrl = {
	.configuration.init.package = at_device_control_configuration_init_package,
	.configuration.deinit_package = at_device_control_configuration_deinit_package,
	.configuration.destroy_package = at_device_control_configuration_destroy_package,

	.inquire.package_device = at_device_control_inquire_package_device,
	.inquire.package_size = at_device_control_inquire_package_size,

	.package_operator.configuration.mount = at_device_control_package_operator_configuration_mount,
	.package_operator.configuration.demount = at_device_control_package_operator_configuration_demount,
	.package_operator.transmit.send = at_device_control_package_operator_transmit_send,
	.package_operator.transmit.receive = at_device_control_package_operator_transmit_receive,
	.package_operator.verify.device = at_device_control_package_operator_verify_device,
	.package_operator.verify.package = at_device_control_package_operator_verify_package,

	#ifdef _WIN32

	.configuration.init.windows_package = at_device_control_windows_package_init,

	#endif // _WIN32
};

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#ifdef _WIN32

/**
 * @brief This function will initialize the i/o stream of the windows.
 *
 * @param void
 *
 * @return void
 */

errno_t at_windows_control_mount(struct at_device_package_s *package,
								 void *arg_list);

 /**
  * @brief This function will deinitialize the i/o stream of the windows.
  *
  * @param void
  *
  * @return void
  */

errno_t at_windows_control_demount(struct at_device_package_s *package);

/**
 * @brief This function will control the send/output of the windows i/o.
 *
 * @param void
 *
 * @return void
 */

errno_t at_windows_control_send(void *device, void *data, size_t len);

/**
 * @brief This function will control the receive/input of the windows i/o.
 *
 * @param void
 *
 * @return void
 */

void *at_windows_control_receive(void *device, size_t len);

/**
 * @brief This function will verify if the windows i/o device is valid.
 *
 * @param void
 *
 * @return void
 */

static inline bool
at_windows_control_verify_device(void *device);

/**
 * @brief This function will verify if the windows i/o package is valid.
 *
 * @param void
 *
 * @return void
 */

bool at_windows_control_verify_package(struct at_device_package_s *package);

#endif // _WIN32

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

#ifdef _WIN32

/**
 * @brief This function will initialize the i/o stream of the windows.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_device_control_windows_package_init(struct at_device_package_s **package)
{
	assert(package);

	if (at_device_control_configuration_init_package(package)) {
		return -1;
	}

	(*package)->configuration.mount = at_windows_control_mount;
	(*package)->configuration.demount = at_windows_control_demount;

	(*package)->transmit.send = at_windows_control_send;
	(*package)->transmit.receive = at_windows_control_receive;

	(*package)->verify.device = at_windows_control_verify_package;
	(*package)->verify.package = at_windows_control_verify_package;

	if (!at_windows_control_verify_package((*package))) {
        free((*package));

        (*package) = NULL;
		return 1;
	}

	return 0;
}

/**
 * @brief This function will initialize the i/o stream of the windows.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_windows_control_mount(struct at_device_package_s *package,
						 void *arg_list)
{
	assert(package);
	assert(arg_list);

	FILE *file = calloc(2, sizeof(FILE));

    if (NULL == file) {
        return -1;
	}

	fopen_s(((FILE **)file + 0), *((char **)arg_list + 0), "a+");
	fopen_s(((FILE **)file + 1), *((char **)arg_list + 1), "a+");

	if (!at_windows_control_verify_device(file)) {
		return 1;
    }

	package->device_ptr = file;

	return 0;
}

/**
 * @brief This function will deinitialize the i/o stream of the windows.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_windows_control_demount(struct at_device_package_s *package)
{
	assert(package);

	fclose(*((void **)package->device_ptr + 0));
	fclose(*((void **)package->device_ptr + 1));

	package->device_ptr = NULL;

	return 0;
}

/**
 * @brief This function will control the send/output of the windows i/o.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_windows_control_send(void *device, void *data, size_t len)
{
	fwrite(data, sizeof(char), len, *((void **)device + 0));

	return 0;
}

/**
 * @brief This function will control the receive/input of the windows i/o.
 *
 * @param void
 *
 * @return void
 */

static inline void
*at_windows_control_receive(void *device, size_t len)
{
	char *string = calloc(1, sizeof(char) * (len + 1));

    if (NULL == string) {
        return NULL;
    }

	fgets(string, len, *((void **)device + 1));

	return string;
}

/**
 * @brief This function will verify if the windows i/o device is valid.
 *
 * @param void
 *
 * @return void
 */

static inline bool
at_windows_control_verify_device(void *device)
{
	assert(device);

	if (NULL == *((void **)device + 0) ||       /* Verify if the transmit part of the windows i/o package is valid */
		NULL == *((void **)device + 1)) {
		goto FAIL;
	}

	return true;

FAIL:
	return false;
}

/**
 * @brief This function will verify if the windows i/o package is valid.
 *
 * @param void
 *
 * @return void
 */

static inline bool
at_windows_control_verify_package(struct at_device_package_s *package)
{
	assert(package);

	if (NULL == package->transmit.send ||       /* Verify if the transmit part of the windows i/o package is valid */
		NULL == package->transmit.receive) {
		goto FAIL;
	}

	return true;

FAIL:
	return false;
}

#endif // _WIN32